#include<iostream>
#include<vector>
#include<math.h>

#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"../../OpenGL-Mathematics-master/glm/glm.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//matrix_transform.hpp"
#include"../../OpenGL-Mathematics-master/glm/gtc//type_ptr.hpp"

#include"MyObject.h"

using namespace glm;

//将物体存入objects，然后使用divide()划分场景，将各个部分的物体依次放入objectBox，然后对每个objectBox进行makeBox()，得到盒子坐标放入boxes。

struct MyList {

	std::vector<MyObject*>* objects;
	float* box;
	MyList* nextLeftNode;
	MyList* nextRightNode;

};

class Scene {
	
public:
	//std::vector<MyObject*> objects;//所有物体
	//std::vector< std::vector<MyObject*> > objectBox;//划分后存放物体的各个盒子内存放的物体
	//std::vector<float*> boxes;//划分后存各个盒子的坐标
	//int maxLayer;//bvh树的最大层数，用来结束递归
	MyList bvhObject;

	Scene();
	//void sceneComplete();//当所有物体放入场景后调用
	bool judgeDivide(MyList* bvh,int shaft);
	void divide(MyList* bvh);//BVH树
	float* makeBox(std::vector<MyObject*> objects);
};
