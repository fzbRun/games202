#include "MyShader.h"

using namespace std;

MyShader::MyShader(const char* vertexPath, const char* fragmentPath) {

	//���ļ�·���л�ö���/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//��֤ifstream��������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ��Ļ������ݵ�������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//�ر��ļ��������
		vShaderFile.close();
		fShaderFile.close();
		//ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOTSUCCESSFULLY_READ:" << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	
	//������ɫ��
	unsigned int vertex, fragment;
	int success;
	char infolog[1024];
	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infolog);
		std::cout << "ERROE::SHADER::VERTEX::COMPILATION_FAILED" << vertexPath << infolog << std::endl;
	};
	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infolog);
		std::cout << "ERROE::SHADER::FRAGMENT::COMPILATION_FAILED" << fragmentPath << infolog << std::endl;
	};
	//��ɫ��������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//��ӡ���Ӵ���
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 1024, NULL, infolog);
		std::cout << "ERROE::SHADER::PROGRAM::LINKING_FAILED:" << infolog << std::endl;
	}
	//ɾ����ɫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void MyShader::addGeoShader(const char* geoPath) {
	string geoCode;
	ifstream gShaderFile;
	gShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		gShaderFile.open(geoPath);
		stringstream gShaderStream;
		gShaderStream << gShaderFile.rdbuf();
		gShaderFile.close();
		geoCode = gShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOTSUCCESSFULLY_READ:" << e.what() << endl;
	}
	const char* gShaderCode = geoCode.c_str();
	unsigned int geometry;
	int success;
	char infolog[1024];
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometry, 1024, NULL, infolog);
		std::cout << "ERROE::SHADER::GEOMETRY::COMPILATION_FAILED" << geoPath << infolog << std::endl;
	};
	glAttachShader(ID, geometry);
	glLinkProgram(ID);
	//��ӡ���Ӵ���
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 1024, NULL, infolog);
		std::cout << "ERROE::SHADER::PROGRAM::LINKING_FAILED:" << infolog << std::endl;
	}
	glDeleteShader(geometry);
}

void MyShader::use() {
	glUseProgram(ID);
}

void MyShader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void MyShader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void MyShader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void MyShader::setMaxtrix4fv(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void MyShader::setMaxtrix3fv(const std::string& name, glm::mat3 value) {
	glUniformMatrix3fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void MyShader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID,name.c_str()), 1, &value[0]);
}

void MyShader::setVec2(const std::string& name, glm::vec2 value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
