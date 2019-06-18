#include "..\include\Szescian.h"


Szescian::Szescian(GLfloat x, GLfloat y, GLfloat z, GLfloat lenx, GLfloat leny, GLfloat lenz)
	: Figura(x, y, z)
	, xlen{ lenx }
	, ylen{ leny }
	, zlen{ lenz }
{
}

Szescian::~Szescian()
{
}

void Szescian::draw()
{
	//podany punkt jest jednym z rogow
	GLfloat sa[3] = { xpos, ypos, zpos };
	GLfloat sb[3] = { xpos + xlen, ypos, zpos };
	GLfloat sc[3] = { xpos + xlen, ypos + ylen ,zpos };
	GLfloat sd[3] = { xpos,ypos + ylen, zpos };
	GLfloat se[3] = { xpos,ypos, zpos + zlen };
	GLfloat sf[3] = { xpos + xlen, ypos, zpos + zlen };
	GLfloat sg[3] = { xpos + xlen, ypos + ylen,zpos + zlen };
	GLfloat sh[3] = { xpos,ypos + ylen,zpos + zlen };

	// Sciany skladowe
	glColor3fv(color);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sa);
	glVertex3fv(sb);
	//glVertex3fv(sc);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glEnd();

	//	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sb);
	glVertex3fv(sf);
	//glVertex3fv(sg);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glEnd();

	//	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sf);
	glVertex3fv(se);
	//glVertex3fv(sh);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	//glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(se);
	glVertex3fv(sa);
	//glVertex3fv(sd);
	glVertex3fv(sh);
	glVertex3fv(sd);
	glEnd();

	//glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sd);
	glVertex3fv(sc);
	//glVertex3fv(sg);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	//glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sa);
	glVertex3fv(sb);
	//	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sf);
	glEnd();
}
