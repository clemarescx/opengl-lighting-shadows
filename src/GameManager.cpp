#include "GameManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stdexcept>
#include <glm/gtx/transform2.hpp>
#include "GLUtils/DebugOutput.hpp"
#include "GLUtils/GLUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>

using std::cerr;
using std::endl;
using GLUtils::VBO;
using GLUtils::Program;
using GLUtils::ShadowProgram;
using GLUtils::BlinnPhongProgram;
using GLUtils::readFile;

const float GameManager::cube_vertices_data[] = {
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
};

const float GameManager::cube_normals_data[] = {
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

void debug_mat_content(glm::mat4& mat){
	auto l = mat.length();
	std::cout << "[\n";
	for (int i = 0; i < l; i++){
		for (int j = 0; j < l; j++){
			std::cout << mat[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << " ]" << std::endl;
}


GameManager::GameManager(){
	fps_timer.restart();
	showShadowMap = false;
	zoom = 1;
	near_plane = 0.5f;
	far_plane = 30.0f;
	fovy = 45.0f;

}


GameManager::~GameManager(){}

void GameManager::init(){
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::stringstream err;
		err << "Could not initialize SDL: " << SDL_GetError();
		THROW_EXCEPTION(err.str());
	}
	atexit(SDL_Quit);

	ilInit();
	iluInit();

	createOpenGLContext();
	setOpenGLStates();
	CHECK_GL_ERROR();
	createMatrices();

	createSimpleProgram();
	CHECK_GL_ERROR();
	createVAO();
	CHECK_GL_ERROR();
	depth_fbo.reset(new DepthFBO(window_width, window_height));
	CHECK_GL_ERROR();

}

void GameManager::printDebug(){
	std::cout << "Fovy / zoom : " << fovy << " / " << zoom << '\n';
}

void GameManager::createOpenGLContext(){
	//Set OpenGL major an minor versions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Use double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Use framebuffer with 16 bit depth buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Use framebuffer with 8 bit for red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // Use framebuffer with 8 bit for green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); // Use framebuffer with 8 bit for blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // Use framebuffer with 8 bit for alpha
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Initalize video
	main_window = SDL_CreateWindow("Westerdals - PG6200 Reworked Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                               window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!main_window){
		THROW_EXCEPTION("SDL_CreateWindow failed");
	}

	//Create OpenGL context
	main_context = SDL_GL_CreateContext(main_window);


	// Init glew
	// glewExperimental is required in openGL 3.3 
	// to create forward compatible contexts 
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK){
		std::stringstream err;
		err << "Error initializing GLEW: " << glewGetErrorString(glewErr);
		THROW_EXCEPTION(err.str());
	}

	//enabling KHR debugging
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDEBUGPROC(GLUtils::DebugOutput::myCallback), nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);


	// Unfortunately glewInit generates an OpenGL error, but does what it's
	// supposed to (setting function pointers for core functionality).
	// Lets do the ugly thing of swallowing the error....
	glGetError();

	cam_trackball.setWindowSize(window_width, window_height);
}

void GameManager::setOpenGLStates(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	CHECK_GL_ERRORS();
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glViewport(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void GameManager::createMatrices(){
	bunny_model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(3));
	cube_model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(far_plane * 0.75f));

	camera.projection = glm::perspective(fovy / zoom, window_width / (float)window_height, near_plane, far_plane);
	camera.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

	
	light.position = glm::vec3(10, 0, 0);
	// the aspect ratio is the same as the final viewport, so that the shadow won't be rendered crooked
	light.projection = glm::perspective(fovy, window_width / (float)window_height, near_plane, far_plane);
	light.view = glm::lookAt(light.position, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));

	scale_bias_matrix = glm::scale(glm::translate(mat4(1.0f), glm::vec3(0.5f)), glm::vec3(0.5f));
	debug_mat_content(scale_bias_matrix);
}

void GameManager::createSimpleProgram(){

	/// create shadow program 
	shadow_program.reset(new ShadowProgram());
	shadow_program->init();
	CHECK_GL_ERROR();
	/// create final render program for the rabbit
	bunny_program.reset(new BlinnPhongProgram());
	bunny_program->init();
	bunny_program->use();
	bunny_program->set_colour(glm::vec3(.0f, 1.f, .8f));
	bunny_program->disuse();
	CHECK_GL_ERROR();


	std::string fs_src = readFile("shaders/cube_map.frag");
	std::string vs_src = readFile("shaders/cube_map.vert");
	std::string gs_src = readFile("shaders/cube_map.geom");

	cube_program.reset(new Program(vs_src, gs_src, fs_src));
	cube_program->use();
	diffuse_cubemap.reset(new GLUtils::CubeMap("cubemaps/diffuse/", "jpg"));
	glm::vec3 colour = glm::vec3(.9f, 0.9f, 1.0f);
	glUniform3fv(cube_program->getUniform("colour"), 1, glm::value_ptr(colour));
	glProgramUniform1i(cube_program->name, cube_program->getUniform("cubemap"), 1);
	cube_program->disuse();

}

void GameManager::createVAO(){
	// create 2 "objects" to render
	glGenVertexArrays(2, main_scene_vao);

	glBindVertexArray(main_scene_vao[BUNNY]);

	// under the hood, the bunny_model constructor creates a VBO 
	// (actual storage in OpenGL) for the various data concerning the bunny, 
	// depending on the WaveFront object (.obj) file structure.
	// In the bunny model case, a VBO will be made to load vertices and face normals.
	// Those will be connected to main_scene_vao[BUNNY], since this is the one
	// that was last bound with glBindVertexArray
	// Actually, a VBO and VAO will only be bound upon the next call to vertexAttribxxx.
	// It is possible not to use VAOs at all and only use VBOs, which sometimes 
	// is the better way to go (SOMETIMES!).
	bunny_model.reset(new Model("models/bunny.obj", false));

	bunny_model->getVertices()->bind();
	shadow_program->setAttributePointer("position", 3);
	bunny_program->setAttributePointer("position", 3);

	bunny_model->getNormals()->bind();
	bunny_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();
	glBindVertexArray(0);

	// bind cube_program input to corresponding cube data
	glBindVertexArray(main_scene_vao[CUBE]);
	cube_vertices.reset(new VBO<GL_ARRAY_BUFFER>(cube_vertices_data, sizeof(cube_vertices_data)));
	cube_normals.reset(new VBO<GL_ARRAY_BUFFER>(cube_normals_data, sizeof(cube_normals_data)));

	cube_vertices->bind();
	cube_program->setAttributePointer("position", 3);
	cube_normals->bind();
	cube_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();

	bunny_model->getVertices()->unbind();
	CHECK_GL_ERROR();

	glBindVertexArray(0);
}

void GameManager::render_bunny_depth(MeshPart& mesh,
                                     const std::shared_ptr<Program>& program,
									 const mat4& depth_VP_mat,
                                     const mat4& model_matrix){

	const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 light_mvp = depth_VP_mat * meshpart_model_matrix;

	program->use();
	glUniformMatrix4fv(program->getUniform("model_view_projection"), 1, GL_FALSE, value_ptr(light_mvp));
	CHECK_GL_ERRORS();
	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	CHECK_GL_ERRORS();
	for (int i = 0; i < (int)mesh.children.size(); ++i){
		render_bunny_depth(mesh.children.at(i), program, depth_VP_mat, meshpart_model_matrix);
	}
	program->disuse();
}


void GameManager::renderCubeMap(const glm::mat4& model_mat, const glm::mat4& view, const glm::mat4& projection,
                                const glm::mat4& shadow_MVP, const POV_entity& light_POV){
	glm::mat4 view_proj_mat = projection * view;

	glm::mat4 model_mat_inverse = glm::inverse(model_mat);
	glm::mat4 model_view_mat_inverse = glm::inverse(view * model_mat);

	glm::vec3 light_pos = glm::mat3(model_mat_inverse) * light_POV.position / model_mat_inverse[3].w;
	glm::vec3 camera_pos = glm::vec3(model_view_mat_inverse[3] / model_view_mat_inverse[3].w);


	cube_program->use();
	glUniform3fv(cube_program->getUniform("light_position"), 1, glm::value_ptr(light_pos));
	glUniform3fv(cube_program->getUniform("camera_position"), 1, glm::value_ptr(camera_pos));

	glUniformMatrix4fv(cube_program->getUniform("model_mat"), 1, 0, glm::value_ptr(model_mat));
	glUniformMatrix4fv(cube_program->getUniform("view_proj_mat"), 1, 0, glm::value_ptr(view_proj_mat));
	CHECK_GL_ERROR();

	glUniformMatrix4fv(cube_program->getUniform("shadow_MVP"), 1, 0, glm::value_ptr(shadow_MVP));
	CHECK_GL_ERROR();

	glDrawArrays(GL_TRIANGLES, 0, 36);
	cube_program->disuse();
	CHECK_GL_ERROR();

}

void GameManager::renderMeshRecursive(
	MeshPart& mesh,
	const std::shared_ptr<GLUtils::BlinnPhongProgram>& program,
	const mat4& view_matrix,
	const mat4& model_matrix,
	const mat4& projection_matrix,
	glm::vec3 light_position
){
	const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 view_proj_mat = projection_matrix * view_matrix;

	program->use();
	program->set_MVP_matrix(view_proj_mat * meshpart_model_matrix);

	program->set_light_position(-light.position);	// origin - light pos
	program->set_camera_position(-camera.position); // origin - cam pos
	CHECK_GL_ERROR();

	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	CHECK_GL_ERROR();
	for (int i = 0; i < (int)mesh.children.size(); ++i)
		renderMeshRecursive(mesh.children.at(i), program, view_matrix, meshpart_model_matrix, projection_matrix,
		                    light_position);
	program->disuse();
}

void GameManager::render(){
	const float elapsed = fps_timer.elapsedAndRestart();

	glm::mat4 rotation = glm::rotate(elapsed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	light.position = glm::mat3(rotation) * light.position;
	light.view = glm::lookAt(light.position, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));


	glm::mat4 view = camera.view * cam_trackball.getTransform();

	depth_VP = light.projection * light.view;
	glm::mat4 depth_bias_vp = scale_bias_matrix * depth_VP;


	///******************* R T T - Shadow map******************************
	if (showShadowMap){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else{
		depth_fbo->bind();

	}

	// ******** prepare shadow FBO + "shrink" viewport to the FBO (texture) size + clear depth
	depth_fbo->setup();

	// ******** render bunny
	glBindVertexArray(main_scene_vao[BUNNY]);
	// glCullFace(GL_BACK); // <- bunny is inverted, so this removes the front to help 
	render_bunny_depth(bunny_model->getMesh(), shadow_program, depth_VP, bunny_model_matrix);
	CHECK_GL_ERROR();
	
	// ******* render cube
	// we're not rendering the cube to the shadow map, 
	// as we're not casting the cube's shadow on anything;

	if (!showShadowMap){

		///******************* FINAL RENDER PASS ************************************
		// ******* prepare actual render-to-screen FBO
		depth_fbo->unbind();
		depth_fbo->tear_down();
		glViewport(0, 0, window_width, window_height); //blow viewport back up to window size
		CHECK_GL_ERROR();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color + depth

		// ******* render cube with shadows
		cube_program->use();
		glCullFace(GL_BACK);
		// bind the created shadow map 
		depth_fbo->bindTexture(0);
		CHECK_GL_ERROR();
		glUniform1i(cube_program->getUniform("shadow_map"), 0);
		CHECK_GL_ERROR();
		// bind the cube's diffuse textures
		diffuse_cubemap->bindTexture(GL_TEXTURE1);
		CHECK_GL_ERROR();
		glBindVertexArray(main_scene_vao[CUBE]);
		CHECK_GL_ERROR();
		renderCubeMap(cube_model_matrix, view, camera.projection, depth_bias_vp, light);
		CHECK_GL_ERROR();
		depth_fbo->unbindTexture();
		diffuse_cubemap->unbindTexture();

		// ******* render bunny with blinn-phong
		glBindVertexArray(main_scene_vao[BUNNY]);
		// For a reason I didn't understand, the bunny is inside-out by default...
		// Hence culling the "front" faces.
		glCullFace(GL_FRONT); 
		renderMeshRecursive(bunny_model->getMesh(), bunny_program, view, bunny_model_matrix, camera.projection,
		                    light.position);
		glBindVertexArray(0);
		CHECK_GL_ERROR();
	}


}


void GameManager::zoomIn(){
	zoom = min( zoom + 10e-4f, 1.f + .015f);
	std::cout << "zoom in: " << zoom << std::endl;
	camera.projection = glm::perspective(fovy / zoom,
	                                     window_width / (float)window_height, near_plane, far_plane);
}

void GameManager::zoomOut(){
	zoom = max(zoom - 10e-4f, 1.f - .015f);
	std::cout << "zoom out: " << zoom << std::endl;
	camera.projection = glm::perspective(fovy / zoom,
	                                     window_width / (float)window_height, near_plane, far_plane);
}

void GameManager::play(){
	bool doExit = false;
	//SDL main loop
	while (!doExit){
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			// poll for pending events
			switch (event.type){
				case SDL_MOUSEBUTTONDOWN:
					cam_trackball.rotateBegin(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONUP:
					cam_trackball.rotateEnd(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEMOTION:
					cam_trackball.rotate(event.motion.x, event.motion.y, zoom);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						case SDLK_d:
							printDebug();
							break;
						case SDLK_ESCAPE:
							doExit = true;
							break;
						case SDLK_q:
							if (event.key.keysym.mod & KMOD_CTRL) doExit = true; //Ctrl+q
							break;
						case SDLK_m:
							showShadowMap = !showShadowMap;
							break;
						case SDLK_RIGHT:
							camera.view = glm::translate(camera.view, glm::vec3(-0.1, 0.0, 0.0));
							break;
						case SDLK_LEFT:
							camera.view = glm::translate(camera.view, glm::vec3(0.1, 0.0, 0.0));
							break;
						case SDLK_UP:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, -0.1, 0.0));
							break;
						case SDLK_DOWN:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.1, 0.0));
							break;
						case SDLK_w:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.0, 0.1));
							break;
						case SDLK_s:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.0, -0.1));
							break;
						case SDLK_PLUS:
							zoomIn();
							break;
						case SDLK_MINUS:
							zoomOut();
							break;
						case SDLK_1:
							render_mode_enum = RENDERMODE_FLAT;
							break;
						case SDLK_2:
							render_mode_enum = RENDERMODE_PHONG;
							break;
						case SDLK_3:
							render_mode_enum = RENDERMODE_WIREFRAME;
							break;
						case SDLK_4:
							render_mode_enum = RENDERMODE_HIDDEN_LINE;
							break;
					}
					break;
				case SDL_QUIT: //e.g., user clicks the upper right x
					doExit = true;
					break;
			}
		}

		//Render, and swap front and back buffers
		render();
		SDL_GL_SwapWindow(main_window);
	}
	quit();
}

void GameManager::quit(){
	std::cout << "Bye bye..." << std::endl;
}
