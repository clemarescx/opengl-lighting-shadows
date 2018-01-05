#ifndef _PROGRAM_HPP__
#define _PROGRAM_HPP__

#include "GameException.h"

#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>

namespace GLUtils {

class Program {
public:
	Program(){ }

	Program(std::string vs, std::string fs) {
		name = glCreateProgram();
		attachShader(vs, GL_VERTEX_SHADER);

		attachShader(fs, GL_FRAGMENT_SHADER);
		link();
	}

	Program(std::string vs, std::string gs, std::string fs) {
		name = glCreateProgram();
		attachShader(vs, GL_VERTEX_SHADER);
		attachShader(gs, GL_GEOMETRY_SHADER);
		attachShader(fs, GL_FRAGMENT_SHADER);
		link();
	}

	virtual bool init(){
		name = glCreateProgram();
		if(name == 0){
			std::cerr << "Could not create program" << std::endl;
			return false;
		}
		return true;
	}

	inline void use() {
		glUseProgram(name);
	}

	static inline void disuse() {
		glUseProgram(0);
	}

	inline GLint getUniform(std::string var) {
		GLint loc = glGetUniformLocation(name, var.c_str());
//		assert(loc >= 0);
		if(loc < 0){
			std::cerr << "No uniform with name '" << var << "' found.\n";
		}
		return loc;
	}

	inline void setAttributePointer(std::string var, unsigned int size, GLenum type=GL_FLOAT, GLboolean normalized=GL_FALSE, GLsizei stride=0, GLvoid* pointer=NULL) {
		GLint loc = glGetAttribLocation(name, var.c_str());
		std::cout << "Location of " << var << " in program " << name << ": " << loc << std::endl;
//		assert(loc >= 0);
		if(loc < 0){
			std::cerr << "No attribute with name '" << var << "' found.\n";
		}
		glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(loc);
	}

	GLuint name; //< OpenGL shader program

	protected:
		void link() const{
			std::stringstream log;

			glLinkProgram(name);

			// check for errors
			GLint linkstatus;
			glGetProgramiv(name, GL_LINK_STATUS, &linkstatus);
			if(linkstatus != GL_TRUE){
				log << "Linking failed!" << std::endl;
				GLint logsize;

				glGetProgramiv(name, GL_INFO_LOG_LENGTH, &logsize);

				if(logsize > 0){
					std::vector < GLchar > infolog(logsize + 1);
					glGetProgramInfoLog(name, logsize, NULL, &infolog[0]);
					log << "--- error log ---" << std::endl;
					log << std::string(infolog.begin(), infolog.end()) << std::endl;
				} else{
					log << "--- empty log message ---" << std::endl;
				}
				THROW_EXCEPTION(log.str());
			}
			
		}

		void attachShader(std::string& src, unsigned int type){
			std::stringstream log;
			// create shader object
			GLuint s = glCreateShader(type);
			if(s == 0){
				log << "Failed to create shader of type " << type << std::endl;
				THROW_EXCEPTION(log.str());
			}

			// set source code and compile
			const GLchar* src_list[1] = {src.c_str()};
			glShaderSource(s, 1, src_list, NULL);
			glCompileShader(s);

			// check for errors
			GLint compile_status;
			glGetShaderiv(s, GL_COMPILE_STATUS, &compile_status);
			if(compile_status != GL_TRUE){
				// compilation failed
				log << "Compilation failed!" << std::endl;
				log << "--- source code ---" << std::endl;
				log << src << std::endl;

				GLint logsize;
				glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logsize);

				if(logsize > 0){
					std::vector<GLchar> infolog(logsize + 1);
					glGetShaderInfoLog(s, logsize, NULL, &infolog[0]);

					log << "--- error log ---" << std::endl;
					log << std::string(infolog.begin(), infolog.end()) << std::endl;
				} else{
					log << "--- empty log message ---" << std::endl;
				}
				THROW_EXCEPTION(log.str());
			}

			glAttachShader(name, s);
		}

	
};

}; //Namespace GLUtils

#endif