#ifndef _SHADOWFBO_HPP__
#define _SHADOWFBO_HPP__

#include "GLUtils/GLUtils.hpp"

class DepthFBO
{
public:
	DepthFBO(unsigned int width, unsigned int height);
	~DepthFBO();
	void bind();
	void setup();
	void tear_down();
	static void unbind();
	void bindTexture(GLuint texture_unit);
	void unbindTexture();

	unsigned int getWidth() { return depth_fbo_width; }
	unsigned int getHeight() { return depth_fbo_height; }

	GLuint getTexture() { return depth_texture; }
	GLuint getFBO(){ return depth_fbo; }


private:
	GLuint depth_fbo;
	GLuint depth_texture;
	unsigned int depth_fbo_width, depth_fbo_height;
};
#endif
