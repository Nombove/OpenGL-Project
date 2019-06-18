#include "..\include\Walec.h"


Walec::Walec(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h)
	: Figura(x, y, z)
	, rlen{ r }
	, hlen{ h }
	, xtpos{ x + h }
	, ytpos{ y }
	, ztpos{ z }
{
}

Walec::Walec(GLfloat xb, GLfloat yb, GLfloat zb, GLfloat r_, GLfloat xt, GLfloat yt, GLfloat zt)
	: Figura(xb, yb, zb)
	, rlen{ r_ }
	, xtpos{ xt }
	, ytpos{ yt }
	, ztpos{ zt }
{
}

Walec::~Walec()
{
}

void Walec::draw(GLfloat rotdeg, GLfloat rotx, GLfloat roty, GLfloat rotz)
{
	//podany punkt jest srodkiem jednej z podstaw

	GLfloat alpha, y_, z_, PI = 3.14;

	glPushMatrix();

	if (rotdeg)
	{
		glTranslatef(xpos, ypos, zpos);
		glRotatef(rotdeg, rotx, roty, rotz);
		glTranslatef(-xpos, -ypos, -zpos);
	}

	glBegin(GL_TRIANGLE_FAN);
	glColor3fv(color);

	glVertex3d(xpos, ypos, zpos);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y_ = ypos + rlen * sin(alpha);
		z_ = zpos + rlen * cos(alpha);
		glVertex3d(xpos, y_, z_);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y_ = ypos + rlen * sin(alpha);
		z_ = zpos + rlen * cos(alpha);
		glVertex3d(xpos, y_, z_);
		//glVertex3d(xtpos, y_, z_);

		y_ = ytpos + rlen * sin(alpha);
		z_ = ztpos + rlen * cos(alpha);
		//glVertex3d(xpos, y_, z_);
		glVertex3d(xtpos, y_, z_);

	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(xtpos, ytpos, ztpos);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		y_ = ytpos + rlen * sin(alpha);
		z_ = ztpos + rlen * cos(alpha);
		glVertex3d(xtpos, y_, z_);
	}
	glEnd();

	glPopMatrix();
}

