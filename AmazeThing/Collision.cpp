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
GLboolean Collision::checkCollision(std::map<int, std::map<int, glm::mat4>> &mModel, glm::vec3 camPosition)
{
	GLfloat size = 0.5, camSize = 0.5;
	for (GLfloat x = camPosition.x - 1; x < (camPosition.x + 1); x++) {
		for (GLfloat z = camPosition.z - 1; z < (camPosition.z + 1); z++) {
			if (mModel[x][z][3].x + size >= camPosition.x && camPosition.x + camSize >= mModel[x][z][3].x
				&&
				mModel[x][z][3].z + size >= camPosition.z && camPosition.z + camSize >= mModel[x][z][3].z) {
				std::cout << "Cam collision x: " << camPosition.x << " z: " << camPosition.z <<  " +Size x: " << camPosition.x+camSize << " z: " << camPosition.z + camSize << std::endl;
				std::cout << "Model collision x: " << mModel[x][z][3].x << " z: " << mModel[x][z][3].z << " +Size x: " << mModel[x][z][3].x + size << " z: " << mModel[x][z][3].z + size << std::endl << std::endl;
				return true;
			}
		}
	}

	return false;
}
