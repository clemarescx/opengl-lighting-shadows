#ifndef _MODEL_H__
#define _MODEL_H__

#include <memory>
#include <string>
#include <vector>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLUtils/VBO.hpp"
#include "GLUtils/Program.hpp"

using GLUtils::Program;
using GLUtils::VBO;
using glm::mat4;

struct MeshPart{
	MeshPart() : first(0), count(0){}
	mat4 transform;
	unsigned int first;
	unsigned int count;
	std::vector<MeshPart> children;
};

class Model{
public:
	Model(std::string filename, bool invert = false);
	~Model();

	MeshPart getMesh(){ return root; }
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> getVertices(){ return vertices; }
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> getNormals(){ return normals; }
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> getColors(){ return colors; }
	
private:
	static void loadRecursive(MeshPart& part, bool invert,
	                          std::vector<float>& vertex_data, std::vector<float>& normal_data,
	                          std::vector<float>& color_data, const aiScene* scene, const aiNode* node);

	static void findBBoxRecursive(const aiScene* scene, const aiNode* node, glm::vec3& min_dim, glm::vec3& max_dim,
	                              aiMatrix4x4* trafo);

	const aiScene* scene;
	MeshPart root;

	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> normals;
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> vertices;
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER>> colors;

	glm::vec3 min_dim;
	glm::vec3 max_dim;

	unsigned int n_vertices;
};

#endif
