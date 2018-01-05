#include "DepthFBO.h"
#include "GLUtils/GLUtils.hpp"

DepthFBO::DepthFBO(unsigned int width, unsigned int height){
	this->depth_fbo_width = width;
	this->depth_fbo_height = height;

	// Initialize Depth Texture
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, depth_fbo_width, depth_fbo_height, 0, GL_DEPTH_COMPONENT,
	             GL_FLOAT,
	             nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Now the Frame buffer
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	// attach the texture target which will be used once we're done drawing to the FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	glDrawBuffer(GL_NONE); // no colors written or read from the shadow map
	glReadBuffer(GL_NONE);
	CHECK_GL_ERRORS();

	auto fboLoaded = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboLoaded != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "FrameBuffer not loaded correctly" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


DepthFBO::~DepthFBO(){
	glDeleteFramebuffers(1, &depth_fbo);
}

void DepthFBO::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
}

void DepthFBO::setup(){
	glViewport(0, 0, depth_fbo_width, depth_fbo_height);
	CHECK_GL_ERROR();
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	// as per official doc: offset applied before the depth test is performed 
	// and before the value is written into the depth buffer.
	// Used to avoid depth fighting (also known as "Shadow acne". Yuck.).
	glPolygonOffset(2.f, 4.f);
	CHECK_GL_ERROR();
}

void DepthFBO::tear_down(){
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void DepthFBO::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthFBO::bindTexture(GLuint texture_unit){
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
}

void DepthFBO::unbindTexture(){
	glBindTexture(GL_TEXTURE_2D, 0);
}
