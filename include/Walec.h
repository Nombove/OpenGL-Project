#pragma once          // Window defines
#include <windows.h>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>  
#include "Figura.h"

class Walec : public Figura
{
public:
	Walec(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h );
	Walec(GLfloat xb, GLfloat yb, GLfloat zb, GLfloat r, GLfloat xt, GLfloat yt, GLfloat zt);
	~Walec();

	void draw(GLfloat deg=0, GLfloat rotx=0, GLfloat roty=0, GLfloat rotz=0);
private:

	GLfloat rlen;
	GLfloat hlen;
	GLfloat xtpos;
	GLfloat ytpos;
	GLfloat ztpos;
};

