#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
class VertexObject
{
public:
	VertexObject(GLuint  &VAO, GLuint  &VBO, GLuint &lightVAO, GLfloat vertices[]);
	~VertexObject();
};

