#pragma once
#include "Program.hpp"
#include "GLUtils.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GLUtils
{
	class BlinnPhongProgram : public Program{
	public:
		BlinnPhongProgram(){}

		virtual bool init(){
			if (!Program::init()){
				std::cerr << "Could not create BlinnPhongProgram" << std::endl;
				return false;
			}
			std::string vs_src = readFile("shaders/bunny_light.vert");
			std::string fs_src = readFile("shaders/bunny_light.frag");
			attachShader(vs_src, GL_VERTEX_SHADER);
			attachShader(fs_src, GL_FRAGMENT_SHADER);

			link();
			CHECK_GL_ERROR();
			MVP_matrix_loc = getUniform("model_view_projection");
			light_position_loc = getUniform("light_position");
			camera_position_loc = getUniform("camera_position");
			colour_loc = getUniform("colour");
			CHECK_GL_ERROR();

		}

		void set_MVP_matrix(const glm::mat4& mvp) const{
			glUniformMatrix4fv(MVP_matrix_loc, 1, GL_FALSE, glm::value_ptr(mvp));
		}

		void set_light_position(const glm::vec3& light_pos) const{
			glUniform3fv(light_position_loc, 1, glm::value_ptr(light_pos));

		}

		void set_camera_position(const glm::vec3& camera_pos) const{
			glUniform3fv(camera_position_loc, 1, glm::value_ptr(camera_pos));
		}

		void set_colour(const glm::vec3& RGB_colour) const{
			glUniform3fv(colour_loc, 1, glm::value_ptr(RGB_colour));
		}


	private:
		GLint MVP_matrix_loc;
		GLint light_position_loc;
		GLint camera_position_loc;
		GLint colour_loc;
	};
}
