#include"Ray.h"


bool Ray::hitBox(float* objectBox,int k) {

	vec3 min(objectBox[0], objectBox[1], objectBox[2]);
	vec3 max(objectBox[3], objectBox[4], objectBox[5]);

	float maxEnterT;
	float minLeaveT;

	//如果原点在物体盒子内,物体直接不要了，而对场景无效
	if (k == 1) {
		if (origin.x >= min.x && origin.x <= max.x && origin.y >= min.y && origin.y <= max.y && origin.z >= min.z && origin.z <= max.z) {
			return false;
		}
	}

	if (direction.x == 0.0f) {
		//如果不在盒内
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
		//如果不在盒内
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
		//如果不在盒内
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
		//不需要修改深度值。
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

		//深度测试
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
			//如果是叶子节点
			this->sceneBox->insert(sceneBox->end(), hit->objects->begin(), hit->objects->end());
		}
		else {
			//如果不是叶子节点
			MyList* left = hit->nextLeftNode;
			hitScene(left);

			MyList* right = hit->nextRightNode;
			hitScene(right);
		}

	}

}

//我想通过渲染方程来得到其他物体反射来的光和光源射来的光的反射光的强度
//但是问题是不知道如何得到粗糙度纹理的值。
//先放着
/*
vec4 Ray::pointColor(vec3 inRay, vec3 normal) {



}
*/

vec4 Ray::getBandD(Scene* scene,std::vector<vec3*> lights) {

	vec4 objectColor = vec4(0.0f);//物体的反射率
	vec4 color;//返回的光线的值
	float PRR = 0.5;
	vec3 point;
	vec3 normal;
	int k;//记录碰撞点在顶点数组中的索引
	float* vertices;

	//3.0版本,先判断光线是否与场景盒子碰撞，如果碰撞再与场景盒子下的物体判断
	this->hitScene( &(scene->bvhObject) );

	if (this->sceneBox->size() == 0) {
		
	}
	else{
		//即使场景盒子内只有一个物体也需要进行aabb碰撞检测，来避免光线碰到自己所在的mesh。
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
						normal = cross(p2 - p1, p3 - p1);//先求法线，这里是面法线，可以通过三点求两条面上的线，再叉乘得到。当然也可以使用顶点法线平均再得到
						objectColor = vec4(object->albedo, 1.0f);
					}

				}
			}
		}
		if (point != vec3(0.0f)) {

			//4.0版本，以撞击点再发射光线，递归求颜色.布林冯模型。但是没有光源，假设所有点受颜色为1.0的光影响
			//因此有问题，代码中光会乘返回的别的物体颜色的，因此颜色值会很小。
			float attenuation = 1.0f / (1.0f + length(origin - point));//采用物理模型

			//std::srand(time(NULL));
			float prr = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			//通过光线的direction可以判断normal的方向。叉乘不知道方向.
			//direction光线方向是反的!!!!!!!!!
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
					if (dot(normal, inRay) > 0) {	//使入射光线使向外的
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

			//5.0版本,加入光源
			for (vec3* light : lights) {

				//这里光源发射来的光的强度乘以入射光与法线的cos
				float lightAttenuation = 1.0f / pow(length(*light - point), 2);
				vec3 lightDir = normalize(*light - point);
				float diffuse = max(dot(lightDir, normal), 0.0f);
				color += objectColor * diffuse * lightAttenuation * 400.0f;//光强度为10.0f;

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

		//color = vec4(object->albedo,1.0f);	//1.0版本，直接与物体的包围盒求交

		float* vertices = (it)->vertices;
		for(int i = 0;i < ((it)->num - 2) * 8;i+=8){	//2.0版本,如果物体与包围盒相交那么计算光线与物体的mesh是否相交

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

		//如果场景盒子物体数量为1，那么直接和物体的mesh碰撞判定
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
				normal = cross(p2 - p1, p3 - p1);//先求法线，这里是面法线，可以通过三点求两条面上的线，再叉乘得到。当然也可以使用顶点法线平均再得到
			}

		}

		if (point != vec3(0.0f)) {
			objectColor = vec4(object->albedo, 1.0f);
			
			//4.0版本，以撞击点再发射光线，递归求颜色.布林冯模型。但是没有光源，假设所有点受颜色为1.0的光影响
			//因此有问题，代码中光会乘返回的别的物体颜色的，因此颜色值会很小。
			//光线方向是反的!!!!!!!!!
			float attenuation = 1.0f / length(origin - point);//采用物理模型
			float prr = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			//通过光线的direction可以判断normal的方向。叉乘不知道方向.
			normal = dot(normal, direction) < 0 ? normal : -normal;
			normal = normalize(normal);

			if (prr < PRR) {

				std::random_device rd;
				std::default_random_engine rng{ rd() };
				std::uniform_real_distribution<float> inX(-1, 1);
				std::uniform_real_distribution<float> inY(-1, 1);
				std::uniform_real_distribution<float> inZ(-1, 1);

				vec3 inRay = normalize(normal + vec3(inX(rng), inY(rng), inZ(rng)));//使入射光线使向外的
				float diffuse = max(dot(normal, inRay), 0.0f);
				Ray ray(point, inRay);
				vec4 returnColor = ray.getBandD(scene,lights);
				if (returnColor != vec4(0.0f)) {
					color += diffuse * returnColor * objectColor * 1000.0f;//放大效果，乘以1000
				}
			}

			//5.0版本,加入光源
			for (vec3* light : lights) {

				//这里光源发射来的光的强度乘以入射光与法线的cos
				float lightAttenuation = 1.0f / length(*light - point);
				vec3 lightDir = normalize(*light - point);
				vec3 h = normalize(lightDir - direction);
				float cos = max(dot(h, normal), 0.0f);
				color += objectColor * cos * 10.0f * lightAttenuation;//光强度为10.0f;

			}

			color *= attenuation;
		}

	}*/