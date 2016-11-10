#include "VertexData.h"

VertexData::VertexData()
{
}

VertexData::~VertexData()
{
}

std::vector<GLfloat> VertexData::getVertexData()
{
	return vertices;
}

vector<vector<GLfloat> > VertexData::getMap()
{
	return map;
}

