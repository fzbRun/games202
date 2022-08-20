#include"MyModel.h"


unsigned int textureFromFile(const char* path, const string& directory) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponent;
	string fileName = directory + "\\" + string(path);
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponent, 0);
	if (data) {
		GLenum format = GL_RGB;
		if (nrComponent == 1) {
			format = GL_RED;
		}
		else if (nrComponent == 2) {
			format = GL_RGB;
		}
		else if (nrComponent == 4) {
			format = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "texture fail to load" << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}

void MyModel::Draw(MyShader shader) {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

MyModel::MyModel(const char* path) {
	loadModel(path);
}

void MyModel::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || (scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
		std::cout << "error::assimp::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0,path.find_last_of('\\'));

	processNode(scene->mRootNode, scene);
}

void MyModel::processNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh MyModel::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		
		if(mesh->HasNormals()){
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (mesh->mTextureCoords[0]) {
			vec2 texcoords;
			texcoords.x = mesh->mTextureCoords[0][i].x;
			texcoords.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = texcoords;
		}
		else {
			vertex.texCoords = vec2(0.0f);
		}
		if (mesh->HasTangentsAndBitangents()) {
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
		}

		vertexs.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> texture = loadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), texture.begin(), texture.end());
		texture = loadTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), texture.begin(), texture.end());
		texture = loadTexture(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), texture.begin(), texture.end());
	}
	return Mesh(vertexs, indices, textures);
}

std::vector<Texture> MyModel::loadTexture(aiMaterial* material, aiTextureType type, string typeName) {
	std::vector<Texture> textures;
	for (int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (int j = 0; j < textureLoaded.size(); j++) {
			if (std::strcmp(textureLoaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textureLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = textureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			textures.push_back(texture);
			textureLoaded.push_back(texture);
		}
	}
	return textures;
}