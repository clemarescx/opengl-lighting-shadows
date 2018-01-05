#pragma once
#include "Program.hpp"
#include "GLUtils.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GLUtils
{
	class ShadowProgram : public Program{
	public:
		ShadowProgram(){}

		virtual bool init(){
			if (!Program::init()){
				std::cerr << "Could not create ShadowProgram" << std::endl;
				return false;
			}
			std::string vs_src = readFile("shaders/shadow.vert");
			std::string fs_src = readFile("shaders/shadow.frag");
			attachShader(vs_src, GL_VERTEX_SHADER);
			attachShader(fs_src, GL_FRAGMENT_SHADER);

			link();
			MVP_matrix_loc = getUniform("model_view_projection");
		}

		void set_MVP_matrix(const glm::mat4& mvp){
			glUniformMatrix4fv(MVP_matrix_loc, 1, GL_FALSE, glm::value_ptr(mvp));
		}

	private:
		GLint MVP_matrix_loc;
	};
}
