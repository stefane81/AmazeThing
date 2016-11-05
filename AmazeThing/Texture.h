#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
#include "Shader.h"

class Texture
{
public:
	Texture();
	~Texture();

	void Texture::bind(Shader& ourShader);
private:
	// Load and create a texture 
	GLuint texture1;
	GLuint texture2;
};

