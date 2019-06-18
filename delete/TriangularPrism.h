#pragma once
#include "Figura.h"
class TriangularPrism :
	public Figura
{
public:
	TriangularPrism(GLfloat x, GLfloat y, GLfloat z, GLfloat xlen, GLfloat ylen, GLfloat hlen);
	~TriangularPrism();

	void draw(GLfloat deg = 0, GLfloat rotx = 0, GLfloat roty = 0, GLfloat rotz = 0);

private:
	GLfloat xlen;
	GLfloat ylen;
	GLfloat h;
};

