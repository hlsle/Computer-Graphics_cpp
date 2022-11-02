#pragma once
#include "stdafx.h"

struct objRead {
	std::vector<unsigned int> vertexIndices, uvlndices, normallndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> outvertex, outnormal;
	std::vector<glm::vec3> outuv;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

	//--추가 선언--
	int count_vertex = 0;
		
	

	//--         --

	int loadObj_normalize_center(const char* filename);
};