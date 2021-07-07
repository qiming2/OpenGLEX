#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "m_Shader.h"
#include "Mesh.h"

// Abstraction for a model consisting of meshes, textures,
// etc imported via assimp
class Model
{
public:
	Model(const char* path);
	Model(const char* path, bool flip);
	~Model();
	void Draw(m_Shader& shader);
	void Delete();
private:
	// model date
	std::vector<Mesh> meshes;
	std::string dir;
	bool gammaCorrection;
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typename);
};

