#include"Scene.h"

Scene::Scene() {

	bvhObject.objects = new std::vector<MyObject*>();

}

/*
void Scene::sceneComplete() {

	//objectBox.reserve(2 * (sizeof(objects) / sizeof(objects[0])) - 1);//ԭ���������ܽڵ���ΪҶ��*2-1��������������Ҷ�������������壬�����൱��Ҷ������/2.
	int nodeNum = bvhObject->objects.size();
	//��������Ϊż��,Ҷ�ӽڵ�����������壬��ônode��������������/2*2-1
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
		//������һ��Ϊ�����ٽ��������objectBox��
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

	//�Ƚ������ᣬ�ҳ���ᣬ��֮Ϊ�ָ���
	int maxShaft;
	maxShaft = bvh->box[3] - bvh->box[0] > bvh->box[4] - bvh->box[1] ? 0 : 1;//x����y��Ƚ�
	maxShaft = bvh->box[maxShaft + 3] - bvh->box[maxShaft] > bvh->box[5] - bvh->box[2] ? maxShaft : 2;//x����y��֮��ϳ�������z��Ƚ�

	bool enDivide = judgeDivide(bvh, maxShaft);

	if (!enDivide) {
		//˵�����û�зָ�����������ǻ�һ����
		int firstShaft = maxShaft;
		maxShaft = maxShaft == 1 ? 0 : 2 - maxShaft;
		enDivide = judgeDivide(bvh, maxShaft);

		if (!enDivide) {
			//˵���ڶ���ѡ�����û�зָ�����������ǻ�һ����
			maxShaft = maxShaft == 2 ? 1 : (firstShaft == 1 ? 2 : 1);
			enDivide = judgeDivide(bvh, maxShaft);

			if (!enDivide) {
				//��������ᶼ�޷��ָ������ô�ͷ���
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