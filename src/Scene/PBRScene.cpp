#include <stb_image.h>
#include "PBRScene.h"
#include "CameraFps.h"
#include "m_Shader.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////
// Physically Based Shading: The Meat!
//////////////////////////////////////////////////////////////////////////


static void renderCube();
static void renderSphere();
static CameraFps camera;
static glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
};
static glm::vec3 lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};

static int nrRows = 7;
static int nrColumns = 7;
static float spacing = 2.5;
static unsigned int albedo;
static unsigned int metallic;
static unsigned int roughness;
static bool use_pbr_tex = true;


static const unsigned int capture_resolution = 2048;
static unsigned int captureFbo, captureRbo;
static unsigned int env_cube_map;

static unsigned int irradiance_map;
static const unsigned int irradiance_resolution = 64;
static bool use_irradiance_map = true;
namespace Scene {

	

    PBRScene::PBRScene() :
        pbrShader("res/shaders/PBR.shader"),
        hdrToMap("res/shaders/hdr_to_cubemap.shader"),
        albedo_tex("res/Material/PBR/pbr_ex/rustediron2_basecolor.png", 0),
        roughness_tex("res/Material/PBR/pbr_ex/rustediron2_roughness.png", 1),
        metallic_tex("res/Material/PBR/pbr_ex/rustediron2_metallic.png", 2),
        diffu_radiance_tex("res\\Texture\\HDR\\Malibu_Overlook\\Malibu_Overlook_3k.hdr", 0, gl_texture_flag::HDR),
        normal("res/Material/PBR/pbr_ex/rustediron2_normal.png", 3),
        cube_shader("res/shaders/cube_map.shader"),
        irradiance_shader("res/shaders/irradiance_map.shader")
    {
        pbrShader.Bind();
		pbrShader.SetFloat("ao", 1.0f);
        pbrShader.SetVec3fv("albedo_test", 0.5f, 0.0f, 0.0f);
        pbrShader.SetInt("albedo_tex", 0);
        pbrShader.SetInt("roughness_tex", 1);
        pbrShader.SetInt("metallic_tex", 2);
        pbrShader.SetInt("normal_tex", 3);
        pbrShader.SetInt("irradiance_map", 4);

        // set up framebuffer and depth render buffer for diffuse radiance map
        glGenFramebuffers(1, &captureFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
        
        glGenRenderbuffers(1, &captureRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, capture_resolution, capture_resolution);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo);
        
        // convert hdr environment map to a cube map for a more performant rendering
        glGenTextures(1, &env_cube_map);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cube_map);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, capture_resolution, capture_resolution, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glm::mat4 capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 capture_view[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        hdrToMap.Bind();
        hdrToMap.SetInt("raw_hdr_tex", 0);
        hdrToMap.SetMat4fv("projection", capture_projection);
        diffu_radiance_tex.Bind();

        // Change viewport to match the resolution of the cubemap
        glViewport(0, 0, capture_resolution, capture_resolution);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
        for (unsigned int i = 0; i < 6; ++i) {
            hdrToMap.SetMat4fv("view", capture_view[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cube_map, 0);
            gl_renderer.Clear();
            renderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Width, Height);


        // pre-compute irradiance map - environment map convolution
        glGenTextures(1, &irradiance_map);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, irradiance_resolution, irradiance_resolution, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // rescale render buffer storage for depth component
        glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradiance_resolution, irradiance_resolution);
        
        irradiance_shader.Bind();
        irradiance_shader.SetMat4fv("projection", capture_projection);
        irradiance_shader.SetInt("env_map", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cube_map);
        glViewport(0, 0, irradiance_resolution, irradiance_resolution);
        for (unsigned int i = 0; i < 6; ++i) {
            irradiance_shader.SetMat4fv("view", capture_view[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map, 0);
            gl_renderer.Clear();
            renderCube();
        }
        glViewport(0, 0, Width, Height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	PBRScene::~PBRScene() {

	}

	void PBRScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}
	void PBRScene::OnRendering() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // blinking screen might be because that we did not 
        // switch renderbuffer to default renderbuffer which 0

        /*glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);*/
        glDepthFunc(GL_LEQUAL);
		gl_renderer.Clear();
		pbrShader.Bind();
		camera.SetViewProjectMat(pbrShader);
        pbrShader.SetVec3fv("camPos", camera.pos);
        
        albedo_tex.Bind();
        roughness_tex.Bind();
        metallic_tex.Bind();
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);

        pbrShader.SetInt("use_tex", use_pbr_tex);
        pbrShader.SetInt("use_irradiance_map", use_irradiance_map);
		// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
            if (!use_pbr_tex)
			    pbrShader.SetFloat("metallic_test", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
                if (!use_pbr_tex)
				    pbrShader.SetFloat("roughness_test", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
                //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				pbrShader.SetMat4fv("model", model);
				renderSphere();
			}
		}


		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
        
        // Can send a bunch of vec3 to the gpu
        //glUniform3fv(glGetUniformLocation(pbrShader.getID(), "lightPositions"), 4, &lightPositions[0][0]);
        pbrShader.Bind();
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShader.SetVec3fv("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader.SetVec3fv("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader.SetMat4fv("model", model);
			renderSphere();
		}

        cube_shader.Bind();
        camera.SetViewProjectMat(cube_shader);
        /*cube_shader.SetMat4fv("projection", camera.getProjection());
        cube_shader.SetMat4fv("view", glm::mat4(glm::mat3(camera.getView())));*/
        cube_shader.SetInt("env_map", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, env_cube_map);
        renderCube();
        //glDisable(GL_CULL_FACE);
	}
	void PBRScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
        ImGui::Checkbox("PBR Texture", &use_pbr_tex);
        ImGui::Checkbox("Irradiance Map", &use_irradiance_map);
	}
}

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
static void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                
                // Important(qiming):
                // Here I made it negative since I want to make the row going counterclock wise instead of clock wise
                float zPos = -std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }


        // My previous method works pretty well
        //    for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
        //for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {

        //        unsigned int v1 = y + x * (Y_SEGMENTS + 1);
        //        unsigned int v2 = y + 1 + x * (Y_SEGMENTS + 1);
        //        unsigned int v3 = y + (x + 1) * (Y_SEGMENTS + 1);
        //        unsigned int v4 = y + 1 + (x + 1) * (Y_SEGMENTS + 1);


        //        // Looking at how the sphere is built xPos, yPos, zPos
        //        // we need to switch order to 1,4,2 to make the triangles on the sphere
        //        // in the order of counterclock wise
        //        indices.push_back(v1);
        //        indices.push_back(v2);
        //        indices.push_back(v4);


        //        indices.push_back(v4);
        //        indices.push_back(v3);
        //        indices.push_back(v1);
        //    }
        //}

            //unsigned int v1 = 1 + 1 * (Y_SEGMENTS + 1);
            //unsigned int v2 = 1 + 1 + 1 * (Y_SEGMENTS + 1);
            //unsigned int v3 = 1 + (1 + 1) * (Y_SEGMENTS + 1);
            //unsigned int v4 = 1 + 1 + (1 + 1) * (Y_SEGMENTS + 1);


            //// two triangles
            //indices.push_back(v1);
            //indices.push_back(v2);


            //indices.push_back(v3);
            //indices.push_back(v4);
            /*indices.push_back(v4);
            indices.push_back(v1);*/
        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}