#pragma once
#include <iostream>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Map
{
public:
	Map();
	~Map();
	std::vector<std::vector<GLfloat> > getMap();

private:
	std::vector<std::vector<GLfloat> > map
	{
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,0,0,1,1,1,1,1 },
		{ 1,1,1,0,0,1,1,1,1,1 },
		{ 1,1,1,0,0,1,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 },
		{ 1,1,1,1,0,0,1,1,1,1 }
	};
};

