#include "Collision.h"

Collision::Collision()
 {
}


Collision::~Collision()
{
}

GLboolean Collision::checkCollision(std::map<int, std::map<int, glm::mat4>> &mModel, glm::vec3 camPosition)
{
	GLint size = 1, camSize = 1;
	for (int x = camPosition.x - 1; x < (camPosition.x + 1); x++) {
		for (int z = camPosition.z - 1; z < (camPosition.z + 1); z++) {
			if (mModel[x][z][3].x + size >= camPosition.x && camPosition.x + camSize >= mModel[x][z][3].x
				&&
				mModel[x][z][3].z + size >= camPosition.z && camPosition.z + camSize >= mModel[x][z][3].z) {
				return true;
			}
		}
	}

	return false;
}
