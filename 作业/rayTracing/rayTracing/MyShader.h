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
	//程序ID
	unsigned int ID;
	//构造函数
	MyShader(const char* vertexPath, const char* fragmentPath);
	//添加几何着色器
	void addGeoShader(const char* geoPath);
	//使用激活
	void use();
	//uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setMaxtrix4fv(const std::string& name, glm::mat4 value);
	void setMaxtrix3fv(const std::string& name, glm::mat3 value);
};

#endif