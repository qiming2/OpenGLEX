#include "PointShadowScene.h"


namespace Scene {

PointShadowScene::PointShadowScene() {
	
	// generate point light buffer
	glGenTextures(1, &depthCubeMap);

}

PointShadowScene::~PointShadowScene() {

}

void PointShadowScene::OnUpdate(float deltaTime) {
	camera.processInput();
}

void PointShadowScene::OnImGuiRendering() {
	camera.OnImGuiRendering();
}

void PointShadowScene::OnRendering() {

}

}