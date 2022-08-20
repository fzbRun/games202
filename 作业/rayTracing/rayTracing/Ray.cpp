#include"Ray.h"


bool Ray::hitBox(float* objectBox,int k) {

	vec3 min(objectBox[0], objectBox[1], objectBox[2]);
	vec3 max(objectBox[3], objectBox[4], objectBox[5]);

	float maxEnterT;
	float minLeaveT;

	//���ԭ�������������,����ֱ�Ӳ�Ҫ�ˣ����Գ�����Ч
	if (k == 1) {
		if (origin.x >= min.x && origin.x <= max.x && origin.y >= min.y && origin.y <= max.y && origin.z >= min.z && origin.z <= max.z) {
			return false;
		}
	}

	if (direction.x == 0.0f) {
		//������ں���
		if (origin.x < min.x || origin.x > max.x) {
			return false;
		}
	}
	else {
		if (direction.x > 0.0f) {
			if (max.x < origin.x) {
				return false;
			}
			maxEnterT = (min.x - origin.x) / direction.x;
			minLeaveT = (max.x - origin.x) / direction.x;
		}
		else {
			if (min.x > origin.x) {
				return false;
			}
			maxEnterT = (max.x - origin.x) / direction.x;
			minLeaveT = (min.x - origin.x) / direction.x;
		}
	}

	if (direction.y == 0.0f) {
		//������ں���
		if (origin.y < min.y || origin.y > max.y) {
			return false;
		}
	}
	else {
		if (direction.y > 0.0f) {
			if (max.y < origin.y) {
				return false;
			}
			maxEnterT = std::max(maxEnterT,(min.y - origin.y) / direction.y);
			minLeaveT = std::min(minLeaveT,(max.y - origin.y) / direction.y);
		}
		else {
			if (min.y > origin.y) {
				return false;
			}
			maxEnterT = std::max(maxEnterT,(max.y - origin.y) / direction.y);
			minLeaveT = std::min(minLeaveT, (min.y - origin.y) / direction.y);
		}
	}

	if (direction.z == 0.0f) {
		//������ں���
		if (origin.z < min.z || origin.z > max.z) {
			return false;
		}
	}
	else {
		if (direction.z > 0.0f) {
			if (max.z < origin.z) {
				return false;
			}
			maxEnterT = std::max(maxEnterT, (min.z - origin.z) / direction.z);
			minLeaveT = std::min(minLeaveT, (max.z - origin.z) / direction.z);
		}
		else {
			if (min.z > origin.z) {
				return false;
			}
			maxEnterT = std::max(maxEnterT, (max.z - origin.z) / direction.z);
			minLeaveT = std::min(minLeaveT, (min.z - origin.z) / direction.z);
		}
	}

	vec3 point = origin + maxEnterT * direction;
	if (point.z > depth) {
		//����Ҫ�޸����ֵ��
		return false;
	}
	
	return minLeaveT > maxEnterT;
}

vec3 Ray::hitMesh(vec3 p1, vec3 p2, vec3 p3) {

	vec3 tbb;
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;
	vec3 s1 = origin - p1;
	vec3 s2 = cross(direction, e2);
	vec3 s3 = cross(s1, e1);

	tbb = (1.0f / dot(s2, e1)) * vec3(dot(s3, e2), dot(s2, s1), dot(s3, direction));

	if ((1.0f - tbb.y - tbb.z) > 0 && tbb.y > 0 && tbb.z > 0) {

		vec3 point = origin + tbb.x * direction;

		//��Ȳ���
		if (depth > point.z) {

			depth = point.z;
			return point;

		}

	}

	return vec3(0.0f);

}

void Ray::hitScene(MyList* hit) {

	if (this->hitBox(hit->box, 0)) {

		if (hit->objects->size() <= 2 || hit->nextLeftNode == NULL) {
			//�����Ҷ�ӽڵ�
			this->sceneBox->insert(sceneBox->end(), hit->objects->begin(), hit->objects->end());
		}
		else {
			//�������Ҷ�ӽڵ�
			MyList* left = hit->nextLeftNode;
			hitScene(left);

			MyList* right = hit->nextRightNode;
			hitScene(right);
		}

	}

}

//����ͨ����Ⱦ�������õ��������巴�����Ĺ�͹�Դ�����Ĺ�ķ�����ǿ��
//���������ǲ�֪����εõ��ֲڶ������ֵ��
//�ȷ���
/*
vec4 Ray::pointColor(vec3 inRay, vec3 normal) {



}
*/

vec4 Ray::getBandD(Scene* scene,std::vector<vec3*> lights) {

	vec4 objectColor = vec4(0.0f);//����ķ�����
	vec4 color;//���صĹ��ߵ�ֵ
	float PRR = 0.5;
	vec3 point;
	vec3 normal;
	int k;//��¼��ײ���ڶ��������е�����
	float* vertices;

	//3.0�汾,���жϹ����Ƿ��볡��������ײ�������ײ���볡�������µ������ж�
	this->hitScene( &(scene->bvhObject) );

	if (this->sceneBox->size() == 0) {
		
	}
	else{
		//��ʹ����������ֻ��һ������Ҳ��Ҫ����aabb��ײ��⣬��������������Լ����ڵ�mesh��
		for (MyObject* object : *sceneBox) {

			if (this->hitBox(object->box, 1)) {

				vertices = object->vertices;
				k = 0;
				for (int i = 0; i < (object->num - 2) * 8; i += 8) {

					vec3 p1 = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
					vec3 p2 = vec3(vertices[i + 8], vertices[i + 9], vertices[i + 10]);
					vec3 p3 = vec3(vertices[i + 16], vertices[i + 17], vertices[i + 18]);
					vec3 p;
					if ((p = this->hitMesh(p1, p2, p3)) != vec3(0.0f)) {
						point = p;
						k = i;
						normal = cross(p2 - p1, p3 - p1);//�����ߣ��������淨�ߣ�����ͨ���������������ϵ��ߣ��ٲ�˵õ�����ȻҲ����ʹ�ö��㷨��ƽ���ٵõ�
						objectColor = vec4(object->albedo, 1.0f);
					}

				}
			}
		}
		if (point != vec3(0.0f)) {

			//4.0�汾����ײ�����ٷ�����ߣ��ݹ�����ɫ.���ַ�ģ�͡�����û�й�Դ���������е�����ɫΪ1.0�Ĺ�Ӱ��
			//��������⣬�����й��˷��صı��������ɫ�ģ������ɫֵ���С��
			float attenuation = 1.0f / (1.0f + length(origin - point));//��������ģ��

			//std::srand(time(NULL));
			float prr = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			//ͨ�����ߵ�direction�����ж�normal�ķ��򡣲�˲�֪������.
			//direction���߷����Ƿ���!!!!!!!!!
			normal = dot(normal, direction) < 0 ? normal : -normal;
			normal = normalize(normal);

			if (prr < PRR) {

				vec3 inRay;
				float inX;
				float inY;
				float inZ;
				while (true) {

					inX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					inX = (inX + -0.5) * 2;
					inY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					inY = (inY + -0.5) * 2;
					inZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					inZ = (inZ + -0.5) * 2;

					inRay = vec3(inX, inY, inZ);
					if (dot(normal, inRay) > 0) {	//ʹ�������ʹ�����
						break;
					}

				}
				inRay = normalize(inRay);
				float diffuse = max(dot(normal, inRay), 0.0f);
				Ray ray(point, inRay);
				vec4 returnColor = ray.getBandD(scene, lights);
				if (returnColor != vec4(0.0f)) {
					color += diffuse * returnColor * objectColor * 50.0f;
				}
			}

			//5.0�汾,�����Դ
			for (vec3* light : lights) {

				//�����Դ�������Ĺ��ǿ�ȳ���������뷨�ߵ�cos
				float lightAttenuation = 1.0f / pow(length(*light - point), 2);
				vec3 lightDir = normalize(*light - point);
				float diffuse = max(dot(lightDir, normal), 0.0f);
				color += objectColor * diffuse * lightAttenuation * 400.0f;//��ǿ��Ϊ10.0f;

			}

			color *= attenuation;
		}
			
		
	}

	return color;

}

/*
//for (std::vector<MyObject*>::iterator it = scene->bvhObject.objects->begin(); it != scene->bvhObject.objects->end(); it++) {
for (MyObject* it : *(scene->bvhObject.objects)) {

	//std::cout << this->hitObject((*it)->box) << std::endl;
	if (this->hitBox((it)->box)) {

		//color = vec4(object->albedo,1.0f);	//1.0�汾��ֱ��������İ�Χ����

		float* vertices = (it)->vertices;
		for(int i = 0;i < ((it)->num - 2) * 8;i+=8){	//2.0�汾,����������Χ���ཻ��ô��������������mesh�Ƿ��ཻ

			vec3 p1 = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			vec3 p2 = vec3(vertices[i+8], vertices[i + 9], vertices[i + 10]);
			vec3 p3 = vec3(vertices[i+16], vertices[i + 17], vertices[i + 18]);
			vec3 point;
			if ((point = this->hitMesh(p1, p2, p3)) != vec3(0.0f)) {

				color = vec4((it)->albedo, 1.0f);

			}

		}

	}

}
*/

/*	else if (hitObjects->size() == 1) {

		//�������������������Ϊ1����ôֱ�Ӻ������mesh��ײ�ж�
		MyObject* object = (*hitObjects)[0];
		vertices = object->vertices;
		k = 0;
		for (int i = 0; i < (object->num - 2) * 8; i += 8) {

			vec3 p1 = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			vec3 p2 = vec3(vertices[i + 8], vertices[i + 9], vertices[i + 10]);
			vec3 p3 = vec3(vertices[i + 16], vertices[i + 17], vertices[i + 18]);
			vec3 p;
			if (( p = this->hitMesh(p1, p2, p3)) != vec3(0.0f)) {
				point = p;
				k = i;
				normal = cross(p2 - p1, p3 - p1);//�����ߣ��������淨�ߣ�����ͨ���������������ϵ��ߣ��ٲ�˵õ�����ȻҲ����ʹ�ö��㷨��ƽ���ٵõ�
			}

		}

		if (point != vec3(0.0f)) {
			objectColor = vec4(object->albedo, 1.0f);
			
			//4.0�汾����ײ�����ٷ�����ߣ��ݹ�����ɫ.���ַ�ģ�͡�����û�й�Դ���������е�����ɫΪ1.0�Ĺ�Ӱ��
			//��������⣬�����й��˷��صı��������ɫ�ģ������ɫֵ���С��
			//���߷����Ƿ���!!!!!!!!!
			float attenuation = 1.0f / length(origin - point);//��������ģ��
			float prr = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			//ͨ�����ߵ�direction�����ж�normal�ķ��򡣲�˲�֪������.
			normal = dot(normal, direction) < 0 ? normal : -normal;
			normal = normalize(normal);

			if (prr < PRR) {

				std::random_device rd;
				std::default_random_engine rng{ rd() };
				std::uniform_real_distribution<float> inX(-1, 1);
				std::uniform_real_distribution<float> inY(-1, 1);
				std::uniform_real_distribution<float> inZ(-1, 1);

				vec3 inRay = normalize(normal + vec3(inX(rng), inY(rng), inZ(rng)));//ʹ�������ʹ�����
				float diffuse = max(dot(normal, inRay), 0.0f);
				Ray ray(point, inRay);
				vec4 returnColor = ray.getBandD(scene,lights);
				if (returnColor != vec4(0.0f)) {
					color += diffuse * returnColor * objectColor * 1000.0f;//�Ŵ�Ч��������1000
				}
			}

			//5.0�汾,�����Դ
			for (vec3* light : lights) {

				//�����Դ�������Ĺ��ǿ�ȳ���������뷨�ߵ�cos
				float lightAttenuation = 1.0f / length(*light - point);
				vec3 lightDir = normalize(*light - point);
				vec3 h = normalize(lightDir - direction);
				float cos = max(dot(h, normal), 0.0f);
				color += objectColor * cos * 10.0f * lightAttenuation;//��ǿ��Ϊ10.0f;

			}

			color *= attenuation;
		}

	}*/