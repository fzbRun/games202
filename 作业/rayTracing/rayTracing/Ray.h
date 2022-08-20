#include<iostream>
#include<vector>
#include<random>
#include<time.h>

#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"../../OpenGL-Mathematics-master/glm/glm.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//matrix_transform.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//type_ptr.hpp"

#include"Scene.h"

using namespace glm;

//����Ļÿ���������η�����ߡ����жϹ�����������������Ƿ��ཻ���õ������ཻ�ĺ��ӣ��˺��ж��Ƿ�������ڵ������ཻ���õ������ཻ�ģ��ж��������������Ƿ��ཻ

class Ray {

public:
	vec3 origin;
	vec3 direction;
	float depth = (float)DBL_MAX;//��¼��ײ�����Ƚ�����Ȳ���
	std::vector<MyObject*>* sceneBox;

	Ray() :origin(vec3(0.0f)), direction(vec3(1.0f)) { sceneBox = new std::vector<MyObject*>(); };
	Ray(vec3 origin, vec3 direction) : origin(origin), direction(direction) { sceneBox = new std::vector<MyObject*>(); };

	void hitScene(MyList* hit);
	bool hitBox(float* objectBox, int k);
	vec3 hitMesh(vec3 p1, vec3 p2, vec3 p3);
	vec4 pointColor(vec3 inRay, vec3 normal);
	vec4 getBandD(Scene* scene,std::vector<vec3*> lights);//������Ⱥ����
};
