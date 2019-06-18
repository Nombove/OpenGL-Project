#pragma once

#include <windows.h> 
#include <gl\gl.h>             
#include <gl\glu.h>  
#include "Figura.h"

class Szescian : public Figura
{
public:
	Szescian(GLfloat x, GLfloat y, GLfloat z, GLfloat lenx, GLfloat leny, GLfloat lenz);
	~Szescian();

	void draw();

	GLfloat xlen, ylen, zlen;
};

