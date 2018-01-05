#include "Model.h"

#include "GameException.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GLUtils/GLUtils.hpp"


Model::Model(std::string filename, bool invert){
	std::vector<float> vertex_data, normal_data, color_data;
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_Quality);// | aiProcess_FlipWindingOrder);
	if (!scene){
		std::string log = "Unable to load mesh from ";
		log.append(filename);
		THROW_EXCEPTION(log);
	}

	//Load the bunny_model recursively into data
	min_dim = glm::vec3(std::numeric_limits<float>::max());
	max_dim = glm::vec3(std::numeric_limits<float>::min());
	findBBoxRecursive(scene, scene->mRootNode, min_dim, max_dim, &trafo);
	std::cout << "Loading data from " << filename << '\n';
	loadRecursive(root, invert, vertex_data, normal_data, color_data, scene, scene->mRootNode);

	//Translate to center
	glm::vec3 translation = (max_dim - min_dim) / glm::vec3(2.0f) + min_dim;
	glm::vec3 scale_helper = glm::vec3(1.0f) / (max_dim - min_dim);
	glm::vec3 scale = glm::vec3(std::min(scale_helper.x, std::min(scale_helper.y, scale_helper.z)));
	if (invert) scale = -scale;

	root.transform = glm::scale(root.transform, scale);
	root.transform = translate(root.transform, -translation);

	n_vertices = vertex_data.size();

	//Create the VBOs from the data.
	if (fmod(static_cast<float>(n_vertices), 3.0f) < 0.000001f)
		vertices.reset(new GLUtils::VBO<GL_ARRAY_BUFFER>(vertex_data.data(), n_vertices * sizeof(float)));
	else
		THROW_EXCEPTION("The number of vertices in the mesh is wrong");
	if (normal_data.size() == n_vertices)
		normals.reset(new GLUtils::VBO<GL_ARRAY_BUFFER>(normal_data.data(), n_vertices * sizeof(float)));
	if (color_data.size() == 4 * n_vertices / 3)
		colors.reset(new GLUtils::VBO<GL_ARRAY_BUFFER>(color_data.data(), n_vertices * sizeof(float)));
}

Model::~Model(){}

void Model::findBBoxRecursive(const aiScene* scene, const aiNode* node,
                              glm::vec3& min_dim, glm::vec3& max_dim, aiMatrix4x4* trafo){
	aiMatrix4x4 prev;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &node->mTransformation);

	for (unsigned int n = 0; n < node->mNumMeshes; ++n){
		const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];
		for (unsigned int t = 0; t < mesh->mNumVertices; ++t){

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min_dim.x = std::min(min_dim.x, tmp.x);
			min_dim.y = std::min(min_dim.y, tmp.y);
			min_dim.z = std::min(min_dim.z, tmp.z);

			max_dim.x = std::max(max_dim.x, tmp.x);
			max_dim.y = std::max(max_dim.y, tmp.y);
			max_dim.z = std::max(max_dim.z, tmp.z);
		}
	}

	for (unsigned int n = 0; n < node->mNumChildren; ++n)
		findBBoxRecursive(scene, node->mChildren[n], min_dim, max_dim, trafo);
	*trafo = prev;
}

void Model::loadRecursive(MeshPart& part, bool invert,
                          std::vector<float>& vertex_data, std::vector<float>& normal_data,
                          std::vector<float>& color_data, const aiScene* scene, const aiNode* node){
	//update transform matrix. notice that we also transpose it
	aiMatrix4x4 m = node->mTransformation;
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
			part.transform[j][i] = m[i][j];

	// draw all meshes assigned to this node
	std::cout << "numMeshes: " << node->mNumMeshes << '\n';
	for (unsigned int n = 0; n < node->mNumMeshes; ++n){
		const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];

		//apply_material(scene->mMaterials[mesh->mMaterialIndex]);

		part.first = vertex_data.size() / 3;
		part.count = mesh->mNumFaces * 3;

		//Allocate data
		vertex_data.reserve(vertex_data.size() + part.count * 3);
		if (mesh->HasNormals())
			normal_data.reserve(normal_data.size() + part.count * 3);
		if (mesh->mColors[0] != nullptr)
			color_data.reserve(color_data.size() + part.count * 4);

		//Add the vertices from file
		for (unsigned int t = 0; t < mesh->mNumFaces; ++t){
			const struct aiFace* face = &mesh->mFaces[t];

			if (face->mNumIndices != 3)
				THROW_EXCEPTION("Only triangle meshes are supported");

			for (unsigned int i = 0; i < face->mNumIndices; i++){
				int index = face->mIndices[i];
				vertex_data.push_back(mesh->mVertices[index].x);
				vertex_data.push_back(mesh->mVertices[index].y);
				vertex_data.push_back(mesh->mVertices[index].z);

				if (mesh->HasNormals()){
					if (!invert){
						normal_data.push_back(mesh->mNormals[index].x);
						normal_data.push_back(mesh->mNormals[index].y);
						normal_data.push_back(mesh->mNormals[index].z);
					}
					else{
						normal_data.push_back(-mesh->mNormals[index].x);
						normal_data.push_back(-mesh->mNormals[index].y);
						normal_data.push_back(-mesh->mNormals[index].z);
					}
				}

				if (mesh->mColors[0] != nullptr){
					color_data.push_back(mesh->mColors[0][index].r);
					color_data.push_back(mesh->mColors[0][index].g);
					color_data.push_back(mesh->mColors[0][index].b);
					color_data.push_back(mesh->mColors[0][index].a);
				}
			}
		}
	}

	// load all children
	std::cout << node->mNumChildren << std::endl;
	for (unsigned int n = 0; n < node->mNumChildren; ++n){
		part.children.push_back(MeshPart());
		loadRecursive(part.children.back(), invert, vertex_data, normal_data, color_data, scene, node->mChildren[n]);
	}
}
