#ifndef SHADER_H
#define SHADER_H


#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../OpenGL-Mathematics-master/glm/glm.hpp"
#include "../../OpenGL-Mathematics-master/glm/gtc/matrix_transform.hpp"
#include "../../OpenGL-Mathematics-master/glm/gtc/type_ptr.hpp"


using namespace std;

class MyShader {
public:
	//����ID
	unsigned int ID;
	//���캯��
	MyShader(const char* vertexPath, const char* fragmentPath);
	//��Ӽ�����ɫ��
	void addGeoShader(const char* geoPath);
	//ʹ�ü���
	void use();
	//uniform ���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setMaxtrix4fv(const std::string& name, glm::mat4 value);
	void setMaxtrix3fv(const std::string& name, glm::mat3 value);
};

#endif