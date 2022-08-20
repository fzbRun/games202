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

//在屏幕每个像素依次发射光线。先判断光线与各个场景盒子是否相交，得到最先相交的盒子，人后判断是否与盒子内的物体相交，得到最先相交的，判断与物体三角形是否相交

class Ray {

public:
	vec3 origin;
	vec3 direction;
	float depth = (float)DBL_MAX;//记录碰撞点的深度进行深度测试
	std::vector<MyObject*>* sceneBox;

	Ray() :origin(vec3(0.0f)), direction(vec3(1.0f)) { sceneBox = new std::vector<MyObject*>(); };
	Ray(vec3 origin, vec3 direction) : origin(origin), direction(direction) { sceneBox = new std::vector<MyObject*>(); };

	void hitScene(MyList* hit);
	bool hitBox(float* objectBox, int k);
	vec3 hitMesh(vec3 p1, vec3 p2, vec3 p3);
	vec4 pointColor(vec3 inRay, vec3 normal);
	vec4 getBandD(Scene* scene,std::vector<vec3*> lights);//获得亮度和深度
};
