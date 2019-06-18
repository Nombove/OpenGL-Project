#include "..\include\TriangularPrism.h"


TriangularPrism::TriangularPrism(GLfloat x, GLfloat y, GLfloat z, GLfloat xlen, GLfloat ylen, GLfloat hlen)
	:Figura(x, y, z)
	, xlen{ xlen }
	, ylen{ ylen }
	, h{ hlen }
{
}

TriangularPrism::~TriangularPrism()
{
}

void TriangularPrism::draw(GLfloat rotdeg, GLfloat rotx, GLfloat roty, GLfloat rotz)
{

	GLfloat sa[3] = { xpos, ypos, zpos };
	GLfloat sb[3] = { xpos + xlen, ypos, zpos };
	GLfloat sc[3] = { xpos, ypos + ylen ,zpos };
	GLfloat sd[3] = { xpos, ypos, zpos + h };
	GLfloat se[3] = { xpos + xlen, ypos, zpos + h };
	GLfloat sf[3] = { xpos, ypos + ylen ,zpos + h };


	glPushMatrix();

	glColor3fv(color);

	if (rotdeg)
	{
		glTranslatef(xpos, ypos, zpos);
		glRotatef(rotdeg, rotx, roty, rotz);
		glTranslatef(-xpos, -ypos, -zpos);
	}

	glBegin(GL_TRIANGLES);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glVertex3fv(se);
	glVertex3fv(sf);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sf);
	glVertex3fv(sc);
	glVertex3fv(se);

	glVertex3fv(sb);

	glVertex3fv(sd);
	glVertex3fv(sa);

	glEnd();

	glPopMatrix();

}


