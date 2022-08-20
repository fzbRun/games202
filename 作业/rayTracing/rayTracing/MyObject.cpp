#include"MyObject.h"

MyObject::MyObject(vec3 position, float* vertices, int num, vec3 size, vec3 albedo, vec4 rotate) {

	this->position = position;
	this->vertices = vertices;
	this->num = num;
	this->size = size;
	this->albedo = albedo;
	this->rotate = rotate;

	//对物体每个顶点与法线进行旋转，缩放，平移
	for (int i = 0; i < num * 8; i += 8) {

		//如果不需要旋转那么我们就直接算减少性能消耗
		if (rotate == vec4(0.0f)) {
			//顶点
			this->vertices[i] *= size.x;
			this->vertices[i+1] *= size.y;
			this->vertices[i+2] *= size.z;
			this->vertices[i] += position.x;
			this->vertices[i+1] += position.y;
			this->vertices[i+2] += position.z;
		}
		else {

			vec3 point = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			mat4 model = mat4();
			model = scale(model, size);
			model = glm::rotate(model, radians(rotate.w), vec3(rotate));
			model = translate(model, position);
			point = vec3(model * vec4(point, 1.0f));
			//顶点
			this->vertices[i] = point.x;
			this->vertices[i+1] = point.y;
			this->vertices[i+2] = point.z;
			
			//法线
			model = transpose(inverse(model));
			vec3 normal = vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
			normal = vec3(model * vec4(normal,1.0f));
			this->vertices[i + 3] = normal.x;
			this->vertices[i + 4] = normal.y;
			this->vertices[i + 5] = normal.z;
			
		}

	}

	makeBox();

}

void MyObject::makeBox() {

	float maxX = (float)-DBL_MAX;
	float minX = (float)DBL_MAX;
	float maxY = (float)-DBL_MAX;
	float minY = (float)DBL_MAX;
	float maxZ = (float)-DBL_MAX;
	float minZ = (float)DBL_MAX;

	for (int i = 0; i < num * 8; i+=8) {

		if (vertices[i] > maxX) {
			maxX = vertices[i];
		}
		else if (vertices[i] < minX) {
			minX = vertices[i];
		}

		if (vertices[i+1] > maxY) {
			maxY = vertices[i+1];
		}
		else if (vertices[i+1] < minY) {
			minY = vertices[i+1];
		}

		if (vertices[i+2] > maxZ) {
			maxZ = vertices[i+2];
		}
		else if (vertices[i+2] < minZ) {
			minZ = vertices[i+2];
		}

	}

	box = new float[6]{ minX, minY, minZ, maxX, maxY, maxZ };

}