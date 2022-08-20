#include"Mesh.h"

using namespace glm;

Mesh::Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indice, std::vector<Texture> textures) {
	this->vertexs = vertexs;
	this->indice = indice;
	this->textures = textures;
	
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(Vertex), &vertexs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(unsigned int), &indice[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);	
	
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);	
	
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);	
	
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_BoneIDs));
	glEnableVertexAttribArray(5);	
	
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	glEnableVertexAttribArray(6);
}

void Mesh::Draw(MyShader shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number = "";
		if (textures[i].type == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (textures[i].type == "texture_specular") {
			number = std::to_string(specularNr++);
		}		
		else if (textures[i].type == "texture_normal") {
			number = std::to_string(normalNr++);
		}		
		else if (textures[i].type == "texture_height") {
			number = std::to_string(heightNr++);
		}
		shader.use();
		glActiveTexture(GL_TEXTURE+i);
		shader.setInt(textures[i].type + number, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);



}