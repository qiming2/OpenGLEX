#include "PBRScene.h"
#include "CameraFps.h"

///////////////////////////////////////////////////////////////////////////
// Physically Based Shading: The Meat!
//////////////////////////////////////////////////////////////////////////



static CameraFps camera;
namespace Scene {

	

	PBRScene::PBRScene() {
		
	}

	PBRScene::~PBRScene() {

	}

	void PBRScene::OnUpdate(float deltaTime) {
		camera.processInput();
		std::cout << "PBR!" << std::endl;
	}
	void PBRScene::OnRendering() {

	}
	void PBRScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
	}
}