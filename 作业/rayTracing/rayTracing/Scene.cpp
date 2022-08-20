#include"Scene.h"

Scene::Scene() {

	bvhObject.objects = new std::vector<MyObject*>();

}

/*
void Scene::sceneComplete() {

	//objectBox.reserve(2 * (sizeof(objects) / sizeof(objects[0])) - 1);//原本二叉树总节点数为叶子*2-1，但是我们最后的叶子中有两个物体，所以相当于叶子数量/2.
	int nodeNum = bvhObject->objects.size();
	//物体数量为偶数,叶子节点包含两个物体，那么node数量就是物体数/2*2-1
	if ( nodeNum % 2 == 0) {

		nodeNum -= 1;

	}
	std::cout << nodeNum << std::endl;
	bvhObject->objects.resize(nodeNum);


	for (int i = 0; i < nodeNum; i++) {
		if (nodeNum <= std::pow(2, i+1)) {
			maxLayer = i;
			break;
		}
	}
}
*/

bool Scene::judgeDivide(MyList* bvh, int maxShaft) {

	std::vector<MyObject*>* leftNode = new std::vector<MyObject*>();
	std::vector<MyObject*>* rightNode = new std::vector<MyObject*>();

	float midLine = (bvh->box[maxShaft + 3] - bvh->box[maxShaft]) / 2.0f + bvh->box[maxShaft];
	for (MyObject* object : *(bvh->objects)) {
		//将场景一分为二，再将物体放入objectBox中
		if (object->box[maxShaft + 3] < midLine) {
			leftNode->push_back(object);
		}
		else {
			rightNode->push_back(object);
		}

	}
	if (leftNode->size() != 0 && rightNode->size() != 0) {

		bvh->nextLeftNode->objects = leftNode;
		bvh->nextRightNode->objects = rightNode;
		return true;

	}

	return false;

}

void Scene::divide(MyList* bvh) {

	bvh->box = makeBox(*(bvh->objects));

	if (bvh->objects->size() <= 2) {
		bvh->nextLeftNode = NULL;
		bvh->nextRightNode = NULL;
		return;
	}

	bvh->nextLeftNode = new MyList();
	bvh->nextLeftNode->objects = new std::vector<MyObject*>();
	bvh->nextRightNode = new MyList();
	bvh->nextRightNode->objects = new std::vector<MyObject*>();

	//比较三个轴，找出最长轴，以之为分割线
	int maxShaft;
	maxShaft = bvh->box[3] - bvh->box[0] > bvh->box[4] - bvh->box[1] ? 0 : 1;//x轴与y轴比较
	maxShaft = bvh->box[maxShaft + 3] - bvh->box[maxShaft] > bvh->box[5] - bvh->box[2] ? maxShaft : 2;//x轴与y轴之间较长的轴与z轴比较

	bool enDivide = judgeDivide(bvh, maxShaft);

	if (!enDivide) {
		//说明最长轴没有分割场景，所以我们换一个轴
		int firstShaft = maxShaft;
		maxShaft = maxShaft == 1 ? 0 : 2 - maxShaft;
		enDivide = judgeDivide(bvh, maxShaft);

		if (!enDivide) {
			//说明第二次选择的轴没有分割场景，所以我们换一个轴
			maxShaft = maxShaft == 2 ? 1 : (firstShaft == 1 ? 2 : 1);
			enDivide = judgeDivide(bvh, maxShaft);

			if (!enDivide) {
				//如果三个轴都无法分割场景，那么就放弃
				bvh->nextLeftNode = NULL;
				bvh->nextRightNode = NULL;
				return;
			}

		}

	}


	divide(bvh->nextLeftNode);
	divide(bvh->nextRightNode);

}

float* Scene::makeBox(std::vector<MyObject*> objects) {

	if (!objects.size()) {
		return NULL;
	}

	float maxX = (float)-DBL_MAX;
	float minX = (float)DBL_MAX;
	float maxY = (float)-DBL_MAX;
	float minY = (float)DBL_MAX;
	float maxZ = (float)-DBL_MAX;
	float minZ = (float)DBL_MAX;

	for (MyObject* object : objects) {

		float* vertices = object->box;

		if (vertices[3] > maxX) {
			maxX = vertices[3];
		}
		if (vertices[0] < minX) {
			minX = vertices[0];
		}

		if (vertices[4] > maxY) {
			maxY = vertices[4];
		}
		if (vertices[1] < minY) {
			minY = vertices[1];
		}

		if (vertices[5] > maxZ) {
			maxZ = vertices[5];
		}
		if (vertices[2] < minZ) {
			minZ = vertices[2];
		}
	}

	return new float[6]{ minX, minY, minZ, maxX, maxY, maxZ };

}