#include <memory>
#include "TextureScene.h"
#include "Engine/Renderer.h"
#include "Engine/VertexBufferLayout.h"

namespace Scene
{
    TextureScene::TextureScene() :
        m_mix(0.0f),
        m_clear_color(0.45f, 0.55f, 0.60f, 1.00f),
        m_show_another_window(false),
        m_show_demo_window(false)
	{
        std::vector<float> vertices =
        {
            // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };

        std::vector<unsigned int> indices = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };


        m_va = std::make_unique<VertexArray>();
        // Creating buffer and getting an index
        m_vb = std::make_unique<VertexBuffer>();
        m_vb->ReMap(vertices.data(), vertices.size() * sizeof(float));
        // Creating index buffer and getting an index
        m_ib = std::make_unique<IndexBuffer>();
        m_ib->ReMap(indices.data(), indices.size());
        VertexBufferLayout layout;
        // Position Attri
        layout.Push<float>(3);
        // Color Attri
        layout.Push<float>(3);
        // Texture Attri
        layout.Push<float>(2);
        // Add vertexbuffer using layout object functions
        m_va->AddBuffer(*m_vb, layout);

        // Use relative path
        m_shader = std::make_unique<m_Shader>("res/shaders/b_vert.shader", "res/shaders/b_frag.shader");
        m_shader->Bind();
        m_shader->SetInt("texture1", 0);

        m_shader->SetInt("texture2", 1);

        // shader.SetUniform4f("u_Color", 0.3f, 0.7f, 0.8f, 1.0f);
        // shader.SetUniform4f("a_Color", 0.1f, 0.1f, 0.1f, 1.0f);


        m_renderer = std::make_unique<Renderer>();



        m_texture1 = std::make_unique<Texture>("res/Model/simple_model/diffuse1.jpg", GL_TEXTURE0);
        m_texture2 = std::make_unique<Texture>("res/Texture/awesomeface.png", GL_TEXTURE1);
      
        m_texture1->Bind();
        m_texture2->Bind();
	}

	TextureScene::~TextureScene() {
        m_va->Delete();
		m_vb->Delete();
		m_ib->Delete();
		m_shader->Delete();
		m_texture1->Delete();
		m_texture2->Delete();
    }

	void TextureScene::OnUpdate(float deltaTime)
	{
        // Currently do nothing

	}

	void TextureScene::OnRendering()
	{
        // Sending uniform needs to be after the shader program has been bound
        // and it needs to be the right shader program definitely
        m_renderer->Clear();
        m_texture1->Bind();
        m_texture2->Bind();
        m_shader->Bind();
        m_shader->SetFloat("mixVal", m_mix);
        // shader.SetUniform4f("u_Color", r, 0.6f, 0.8f, 1.0f);

        // Draw
        m_renderer->Draw(*m_va, *m_ib);
	}

	void TextureScene::OnImGuiRendering()
	{
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (m_show_demo_window)
            ImGui::ShowDemoWindow(&m_show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static int counter = 0;                       // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &m_show_demo_window); 

            ImGui::SliderFloat("float", &m_mix, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&m_clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
	}
}