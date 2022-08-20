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
	float* vertices;//位置,法线，UV信息
	float* box;
	int num;
	vec3 size;
	vec4 rotate;//前三项为旋转轴，第四项为角度
	vec3 albedo;

	MyObject(vec3 position, float* vertices, int num, vec3 size, vec3 albedo, vec4 rotate);

	//应当将物体划分的更小，因为如果有几十万个三角形，但光线没必要与之一一判断
	void makeBox();
};