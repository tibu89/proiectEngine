#pragma once

#include <GL\glew.h>

struct Mesh
{
	int start;

	int numVertices;

public:
	Mesh( int _s, int _nV) : start(_s), numVertices(_nV){}
};