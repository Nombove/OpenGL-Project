#pragma once
#include <windows.h>  
#include <gl\gl.h>             
#include <gl\glu.h>  
#include "OBJ_Loader.h"
#include "Figura.h"

class Terrain : public Figura
{
public:
	Terrain(const char *filename, GLfloat x, GLfloat y, GLfloat z);
	~Terrain();

	void draw();

private:
	const char *filename;
//	GLfloat xpos, ypos, zpos;
};

