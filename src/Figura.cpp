#include "..\include\Figura.h"



Figura::Figura(GLfloat x, GLfloat y, GLfloat z)
	: xpos{ x }
	, ypos{ y }
	, zpos{ z }
{
}


Figura::~Figura()
{
}

void Figura::setColor(GLfloat r, GLfloat g, GLfloat b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Figura::draw()
{
}
