#pragma once
#include "objbuf.h"

int objRead::loadObj_normalize_center(const char* filename) {
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		assert(false);
		return false;
	}

	while (1) {

		char lineHeader[16384];

		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);


			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX)maxX = vertex.x;
			if (vertex.y > maxY)maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;
			count_vertex++;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec3 uv;
			fscanf(objFile, "%f %f %f\n", &uv.x, &uv.y,&uv.z);
			

			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

		

			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			//¼öÁ¤
			uvlndices.push_back(1);
			uvlndices.push_back(1);
			uvlndices.push_back(1);
			/*uvlndices.push_back(uvIndex[0]);
			uvlndices.push_back(uvIndex[1]);
			uvlndices.push_back(uvIndex[2]);*/
			normallndices.push_back(normalIndex[0]);
			normallndices.push_back(normalIndex[1]);
			normallndices.push_back(normalIndex[2]);
		}
	}
	aveX = sumX / count_vertex;
	aveY = sumY / count_vertex;
	aveZ = sumZ / count_vertex;
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x;
		temp.y = temp.y;
		temp.z = temp.z;

		//temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		//temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		//temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		outvertex.push_back(temp);
	}
	for (unsigned int i = 0; i < uvlndices.size(); i++) {
		unsigned int uvIndex = uvlndices[i];
		glm::vec3 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normallndices.size(); i++) {
		unsigned int normalIndex = normallndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}
