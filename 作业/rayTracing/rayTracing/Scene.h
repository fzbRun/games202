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

//���������objects��Ȼ��ʹ��divide()���ֳ��������������ֵ��������η���objectBox��Ȼ���ÿ��objectBox����makeBox()���õ������������boxes��

struct MyList {

	std::vector<MyObject*>* objects;
	float* box;
	MyList* nextLeftNode;
	MyList* nextRightNode;

};

class Scene {
	
public:
	//std::vector<MyObject*> objects;//��������
	//std::vector< std::vector<MyObject*> > objectBox;//���ֺ�������ĸ��������ڴ�ŵ�����
	//std::vector<float*> boxes;//���ֺ��������ӵ�����
	//int maxLayer;//bvh���������������������ݹ�
	MyList bvhObject;

	Scene();
	//void sceneComplete();//������������볡�������
	bool judgeDivide(MyList* bvh,int shaft);
	void divide(MyList* bvh);//BVH��
	float* makeBox(std::vector<MyObject*> objects);
};
