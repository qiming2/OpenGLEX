#include <iostream>
#include <unordered_map>
#include "Model.h"

// keep track of loaded texture, so that we don't need to
// reload texture
static std::unordered_map<std::string, Texture> loaded;

Model::Model(const char* path) {
	Texture::SetFlip(true);
	LoadModel(path);
}

Model::Model(const char* path, bool flip) {
	Texture::SetFlip(flip);
	LoadModel(path);
}

Model::~Model() {
	
}

void Model::Draw(m_Shader& shader) {
	// loop over meshes and draw them
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::Delete() {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Delete();
	}
	meshes.clear();
	dir = "";
}

void Model::LoadModel(const std::string& path) {
	Assimp::Importer importer;

	// turn rendering primitives to triangles and flip uvs since
	// opengl use reversed texture coordinates
	// many more flags!
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::IMPORT MODEL FAILED: " << importer.GetErrorString() << std::endl;
		return;
	}
	// store directory for later use
	dir = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);

	// Clean up loaded cache after finish loading
	loaded.clear();
}

// Since model loaded via assimp follows a recursive pattern,
// we recursive process each node and its children retriving all
// meshes
void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	// store all meshes if any
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// recursively process any child
	for (int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

// convert assimp mesh to our own mesh
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Vertex vertex;
	// Process vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		// position
		glm::vec3 temp;
		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;
		vertex.position = temp;

		// normal
		if (mesh->HasNormals()) {
			temp.x = mesh->mNormals[i].x;
			temp.y = mesh->mNormals[i].y;
			temp.z = mesh->mNormals[i].z;
			vertex.normal = temp;
		}
		
		// Texture coordinates
		// Note:: assimp allows up 8 texture coordinates for a single vertex
		// check there is one at set 0
		if (mesh->HasTextureCoords(0)) {
			glm::vec2 tempTex;
			// we don't use other set of texture coordinates
			tempTex.x = mesh->mTextureCoords[0][i].x;
			tempTex.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = tempTex;

			// TODO: Need to learn more about use of tangent and bitangent
			// tangent
			temp.x = mesh->mTangents[i].x;
			temp.y = mesh->mTangents[i].y;
			temp.z = mesh->mTangents[i].z;
			vertex.tangent = temp;

			// bitangent
			temp.x = mesh->mBitangents[i].x;
			temp.y = mesh->mBitangents[i].y;
			temp.z = mesh->mBitangents[i].z;
			vertex.bitangent = temp;

		} else {
			// just set it to 0 if none
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
			//std::cout << "No tex coord?" << std::endl;
		}
		vertices.push_back(vertex);
	}

	// Process indices
	// A face is a primitive rendering type, in our case
	// since we specified (maybe changed) triangulate, thus
	// a face is a triangle, each face has its indices that
	// it needs to be drawn
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	bool skip;
	for (int i = 0; i < mat->GetTextureCount(type); i++) {
		skip = false;
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = dir + '/' + std::string(str.C_Str());
		//std::cout << "path: " << path << std::endl;

		std::unordered_map<std::string, Texture>::const_iterator it = loaded.find(path);
		if (it != loaded.end()) {
			textures.push_back(it->second);
			skip = true;
		}

		if (!skip) {
			// can optimize using emplace back potentially
			Texture tex(path.c_str(), GL_TEXTURE0 + i, typeName);

			// maybe path is not necessary
			tex.path = path;
			textures.push_back(tex);
			loaded.emplace(path, tex);
		}
		
	}
	return textures;
}