/*
This error checker code is from https://gamedev.stackexchange.com/questions/111933/gldrawelements-crash
by user LukGus.
*/
#pragma once

#include <string>
#include <iostream>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

/*
Check GL errors and print into cout. Call
after every GL operation.
*/
void _CheckGLError(const char* file, int line) {
	GLenum err(glGetError());

	while(err != GL_NO_ERROR) {
		std::string error;
		switch(err) {
		case GL_INVALID_OPERATION:  error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:       error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:      error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:      error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}

	return;
}
