#pragma once
#include <iostream>
#include <map>
#include <GL/glew.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Collision
{
public:
	Collision();
	~Collision();
	GLboolean checkCollision(std::map<int, std::map<int, glm::mat4>> &mModel, glm::vec3 camPosition);
};

