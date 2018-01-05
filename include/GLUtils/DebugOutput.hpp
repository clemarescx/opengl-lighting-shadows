#pragma once
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <cassert>

/*
 * Taken from tutorial page: 
 * DEBUGGING OPENGL PART II - In2GPU.com
 * by: Sergiu Craitoiu
 * link: http://in2gpu.com/2015/05/29/debugging-opengl-part-ii-debug-output/
 */
namespace GLUtils
{
	using std::cout;
	using std::endl;

	class DebugOutput{
	public:
		DebugOutput(){}
		~DebugOutput(){}

		static void CALLBACK myCallback(GLenum source,
		                                GLenum type,
		                                GLuint id,
		                                GLenum severity,
		                                GLsizei length,
		                                const GLchar* msg,
		                                GLvoid* data){
			//display warnings/errors however you like
			cout << "\n**********Debug Output**************" << endl;
			cout << "source: " << getStringForSource(source).c_str() << endl;
			cout << "type: " << getStringForType(type).c_str() << endl;
			cout << "severity: " << getStringForSeverity(severity).c_str() << endl;
			cout << "debug call: " << msg << endl;
			cout << "\n************************" << endl;
		}

	private:
		//Parsing code from OpenGL Shader Language CookBook SE
		//https://github.com/daw42/glslcookbook
		static std::string getStringForType(GLenum type){
			switch (type){
				case GL_DEBUG_TYPE_ERROR:
					return "Error";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
					return "Deprecated behavior";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					return "Undefined behavior";
				case GL_DEBUG_TYPE_PORTABILITY:
					return "Portability issue";
				case GL_DEBUG_TYPE_PERFORMANCE:
					return "Performance issue";
				case GL_DEBUG_TYPE_MARKER:
					return "Stream annotation";
				case GL_DEBUG_TYPE_OTHER_ARB:
					return "Other";
				default:
					assert(false);
					return "";
			}
		}

		static std::string getStringForSource(GLenum source){
			switch (source){
				case GL_DEBUG_SOURCE_API:
					return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
					return "Window system";
				case GL_DEBUG_SOURCE_SHADER_COMPILER:
					return "Shader compiler";
				case GL_DEBUG_SOURCE_THIRD_PARTY:
					return "Third party";
				case GL_DEBUG_SOURCE_APPLICATION:
					return "Application";
				case GL_DEBUG_SOURCE_OTHER:
					return "Other";
				default:
					assert(false);
					return "";
			}
		}

		static std::string getStringForSeverity(GLenum severity){
			switch (severity){
				case GL_DEBUG_SEVERITY_HIGH:
					return "High";
				case GL_DEBUG_SEVERITY_MEDIUM:
					return "Medium";
				case GL_DEBUG_SEVERITY_LOW:
					return "Low";
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					return "Notification";
				default:
					assert(false);
					return ("");
			}
		}

	};
}
