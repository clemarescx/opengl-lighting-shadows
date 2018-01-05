#include "GeometryManager.h"

namespace
{
	float cube_vertices[] = {
		0, 1, 1,
		0, 0, 1,
		1, 1, 1,
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,

		1, 1, 1,
		1, 0, 1,
		1, 1, 0,
		1, 0, 1,
		1, 0, 0,
		1, 1, 0,

		1, 1, 0,
		1, 0, 0,
		0, 1, 0,
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,

		0, 1, 0,
		0, 0, 0,
		0, 1, 1,
		0, 0, 0,
		0, 0, 1,
		0, 1, 1,

		0, 1, 1,
		1, 1, 1,
		0, 1, 0,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,

		1, 0, 1,
		0, 0, 1,
		1, 0, 0,
		0, 0, 1,
		0, 0, 0,
		1, 0, 0
	};

	float cube_normals[] = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};

	float cube_texcoords[] = {
		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,

		0, 0,
		0, 1,
		1, 0,
		0, 1,
		1, 1,
		1, 0,
	};
}

const float* GeometryManager::getCubeVertices(){
	return cube_vertices;
}

const float* GeometryManager::getCubeNormals(){
	return cube_normals;
}

const float* GeometryManager::getCubeTexCoords(){
	return cube_texcoords;
}

unsigned int GeometryManager::getCubeNVertices(){
	//Each element in cube_vertices is sizeof(float) bytes,
	//and three consecutive elements form one vertex
	//(x, y, z)-coordinates
	return sizeof(cube_vertices) / (sizeof(float) * 3);
}

unsigned int GeometryManager::getCubeNTriangles(){
	//One triangle consists of three vertices.
	return getCubeNVertices() / 3;
}
