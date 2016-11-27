#include "Collision.h"

Collision::Collision()
 {
}


Collision::~Collision()
{
}

/*
Takes the location of Camera and searches for collision in +-1 from camera position on x and y axis. If collision returns true else false.
*/
GLboolean Collision::checkCollision(std::map<int, std::map<int, glm::vec3>> &mModel, glm::vec3 camPosition)
{
	GLfloat modelSize = 0.8, camSize = 0.2;
	for (GLint x = camPosition.x - 1; x < (camPosition.x + 1); x++) {
		for (GLint z = camPosition.z - 1; z < (camPosition.z + 1); z++) {
			if (abs(mModel[x][z].x - camPosition.x) < (modelSize + camSize)
				&&
				abs(mModel[x][z].z - camPosition.z) < (modelSize + camSize)) {
				std::cout << "Cam collision xz [" << camPosition.x << " , " << camPosition.z <<  "] [" << camPosition.x+camSize << " , " << camPosition.z + camSize << "]" << std::endl;
				std::cout << "Model collision xz [" << mModel[x][z].x << " , " << mModel[x][z].z << "] [" << mModel[x][z].x + modelSize << " , " << mModel[x][z].z + modelSize << "]"<< std::endl << std::endl;
				return true;
			}
		}
	}

	return false;
}
