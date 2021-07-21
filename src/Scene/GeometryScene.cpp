#include <vector>
#include "GeometryScene.h"
#include "Common.h"
#include "Texture.h"

namespace Scene {
	// Position for each point and color for each point
	static std::vector<float> points = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	GeometryScene::GeometryScene():
		normalShader("res/shaders/geometry_vert.shader", "res/shaders/geometry_frag.shader", "res/shaders/geometry_geom.shader"),
		model(1.0f)
	{
		va.Bind();
		vb.ReMap(points.data(), points.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);
		va.Unbind();
		vb.Unbind();
	}

	GeometryScene::~GeometryScene() {
		normalShader.Delete();
	}

	void GeometryScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void GeometryScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
		
	}

	void GeometryScene::OnRendering() {
		// Wireframe mode
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glEnable(GL_PROGRAM_POINT_SIZE);  
		gl_renderer.Clear();
		va.Bind();
		normalShader.Bind();
		glDrawArrays(GL_POINTS, 0, 4);
		glDisable(GL_PROGRAM_POINT_SIZE);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}