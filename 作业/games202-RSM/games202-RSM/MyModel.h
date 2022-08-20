#include<iostream>
#include<vector>
#include<string>

#include"glad/glad.h"
#include"GLFW/glfw3.h"

#include"../../OpenGL-Mathematics-master/glm/glm.hpp"

#include"../../stb-image-master/stb_image.h"
#include"../../stb-image-master/stb_image_write.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"

#include"MyShader.h"
#include"Mesh.h"

using namespace glm;

class MyModel {
public:
	MyModel(const char* path);
	void Draw(MyShader shader);
	std::vector<Mesh> meshes;
private:
	string directory;
	std::vector<Texture> textureLoaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scence);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTexture(aiMaterial* mat, aiTextureType type, string typeName);
};