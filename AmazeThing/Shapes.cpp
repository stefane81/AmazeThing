#include "Shapes.h"



Shapes::Shapes()
{
}


Shapes::~Shapes()
{
}

std::vector<GLfloat> Shapes::getShape(Shape shape)
{
	// have switch statement in future
	switch (shape)
	{
	case Shape::CUBE:
		return cube;
			break;
	case Shape::PLANE:
		return plane;
		break;
	default:
		break;
	}
	return cube;
}
