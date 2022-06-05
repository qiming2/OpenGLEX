#include "PBRScene.h"
#include "CameraFps.h"
#include "m_Shader.h"

///////////////////////////////////////////////////////////////////////////
// Physically Based Shading: The Meat!
//////////////////////////////////////////////////////////////////////////


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

namespace Scene {

	

	PBRScene::PBRScene():
        pbrShader("res/shaders/PBR_vert.shader", "res/shaders/PBR_frag.shader")
    {
        pbrShader.Bind();
		pbrShader.SetVec3fv("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		pbrShader.SetFloat("ao", 1.0f);

		
	}

	PBRScene::~PBRScene() {

	}

	void PBRScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}
	void PBRScene::OnRendering() {
        /*glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);*/
		gl_renderer.Clear();
		pbrShader.Bind();
		camera.SetViewProjectMat(pbrShader);
        pbrShader.SetVec3fv("camPos", camera.pos);

		// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			pbrShader.SetFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				pbrShader.SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

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
        glDisable(GL_CULL_FACE);
	}
	void PBRScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
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
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

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


        //        // two triangles
        //        indices.push_back(v1);
        //        indices.push_back(v4);
        //        indices.push_back(v2);


        //        indices.push_back(v3);
        //        indices.push_back(v4);
        //        indices.push_back(v1);
        //    }
        //}

            unsigned int v1 = 1 + 1 * (Y_SEGMENTS + 1);
            unsigned int v2 = 1 + 1 + 1 * (Y_SEGMENTS + 1);
            unsigned int v3 = 1 + (1 + 1) * (Y_SEGMENTS + 1);
            unsigned int v4 = 1 + 1 + (1 + 1) * (Y_SEGMENTS + 1);


            // two triangles
            indices.push_back(v1);
            indices.push_back(v2);


            indices.push_back(v3);
            indices.push_back(v4);
            /*indices.push_back(v4);
            indices.push_back(v1);*/
        //bool oddRow = false;
        //for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        //{
        //    if (!oddRow) // even rows: y == 0, y == 2; and so on
        //    {
        //        for (unsigned int x = 0; x <= 1; ++x)
        //        {
        //            indices.push_back(y * (X_SEGMENTS + 1) + x);
        //            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        //        }
        //    }
        //    else
        //    {
        //        for (int x = 1; x >= 0; --x)
        //        {
        //            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        //            indices.push_back(y * (X_SEGMENTS + 1) + x);
        //        }
        //    }
        //    oddRow = !oddRow;
        //}
        /*indices.push_back(0);
        indices.push_back(X_SEGMENTS + 1);
        indices.push_back(1);
        indices.push_back(X_SEGMENTS + 2);
        indices.push_back(2);
        indices.push_back(X_SEGMENTS + 3);*/



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