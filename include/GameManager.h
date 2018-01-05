#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <memory>

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>

#include "Timer.h"
#include "GLUtils/CubeMap.hpp"
#include "Model.h"
#include "VirtualTrackball.h"

#include "GLUtils/ShadowProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include "GLUtils/BlinnPhongProgram.h"
#include "DepthFBO.h"


/**
 * This class handles the game logic and display.
 * Uses SDL as the display manager, and glm for 
 * vector and matrix computations
 */
class GameManager {
public:

	/**
	 * Constructor
	 */
	GameManager();

	/**
	 * Destructor
	 */
	~GameManager();

	/**
	 * Initializes the game, including the OpenGL context
	 * and data required
	 */
	void init();

	void printDebug();
	/**
	 * The main loop of the game. Runs the SDL main loop
	 */
	void play();

	/**
	 * Quit function
	 */
	void quit();

	/**
	 * Function that handles rendering into the OpenGL context
	 */
	void render();

protected:
	/**
	 * Creates the OpenGL context using SDL
	 */
	void createOpenGLContext();

	/**
	 * Sets states for OpenGL that we want to keep persistent
	 * throughout the game
	 */
	void setOpenGLStates();

	/**
	 * Creates the matrices for the OpenGL transformations,
	 * perspective, etc.
	 */
	void createMatrices();

	/**
	 * Compiles, attaches, links, and sets uniforms for
	 * a simple OpenGL blinn_phong_program
	 */
	void createSimpleProgram();

	/**
	 * Creates vertex array objects
	 */
	void createVAO();
	
	static const unsigned int window_width = 800;
	static const unsigned int window_height = 600;

	static const float cube_vertices_data[];
	static const float cube_normals_data[];

	float near_plane;
	float far_plane;
	float fovy;
	bool showShadowMap;

	glm::mat4 scale_bias_matrix;
	mat4 depth_VP;

private:
	struct POV_entity{
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
	} camera, light;

	enum MeshIndex{
		BUNNY = 0,
		CUBE
	};
	enum RenderMode {
		RENDERMODE_PHONG,
		RENDERMODE_WIREFRAME,
		RENDERMODE_HIDDEN_LINE,
		RENDERMODE_FLAT,
	};

	void zoomIn();
	void zoomOut();
	void renderMeshRecursive(
		MeshPart& mesh,
		const std::shared_ptr<GLUtils::BlinnPhongProgram>& program,
		const mat4& view_matrix,
		const mat4& model_matrix,
		const mat4& projection_matrix,
		glm::vec3 light_position
	);

	void render_bunny_depth(MeshPart& mesh,
							const std::shared_ptr<Program>& program,
							const mat4& view_projection_matrix,
							const mat4& model_matrix);

	void GameManager::renderCubeMap(const mat4& model, const mat4& view, const mat4& projection, const mat4& shadow_MVP, const POV_entity&
	                                light);

	SDL_Window* main_window; //< Our window handle
	SDL_GLContext main_context; //< Our opengl context handle 
	RenderMode render_mode_enum = RENDERMODE_FLAT; //< The current method of rendering

	// Where the model/mesh data will be stored (1 VAO refers to 1+ VBOs)
	GLuint main_scene_vao[2]; 


	std::shared_ptr<DepthFBO> depth_fbo;
	std::shared_ptr<GLUtils::CubeMap> diffuse_cubemap;
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER> > cube_vertices, cube_normals;

	float zoom;
	Timer fps_timer;
	VirtualTrackball cam_trackball;


	std::shared_ptr<Model> bunny_model, cube_model;
	std::shared_ptr<GLUtils::Program> cube_program;
	std::shared_ptr<GLUtils::ShadowProgram> shadow_program;
	std::shared_ptr<GLUtils::BlinnPhongProgram> bunny_program;


	glm::mat4 bunny_model_matrix, cube_model_matrix;

};
 
#endif // _GAMEMANAGER_H_
