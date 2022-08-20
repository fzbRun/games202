#include<iostream>
#include<vector>

#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"../../OpenGL-Mathematics-master/glm/glm.hpp"

#include"MyShader.h"

using namespace glm;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
	vec3 tangent;
	vec3 bitangent;
	int m_BoneIDs[4];
	float m_Weights[4];
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:

	std::vector<Vertex> vertexs;
	std::vector<unsigned int> indice;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;

	Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> id, std::vector<Texture> textures);
	void Draw(MyShader shader);
private:
	//unsigned int VAO, VBO, EBO;
	void setupMesh();
};
