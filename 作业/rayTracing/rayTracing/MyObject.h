#include<iostream>
#include<float.h>

#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"../../OpenGL-Mathematics-master/glm/glm.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//matrix_transform.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//type_ptr.hpp"

using namespace glm;

class MyObject {

public:
	vec3 position;
	float* vertices;//λ��,���ߣ�UV��Ϣ
	float* box;
	int num;
	vec3 size;
	vec4 rotate;//ǰ����Ϊ��ת�ᣬ������Ϊ�Ƕ�
	vec3 albedo;

	MyObject(vec3 position, float* vertices, int num, vec3 size, vec3 albedo, vec4 rotate);

	//Ӧ�������廮�ֵĸ�С����Ϊ����м�ʮ��������Σ�������û��Ҫ��֮һһ�ж�
	void makeBox();
};