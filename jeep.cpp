
#include "jeep.h"



jeep::jeep(double x, double y, double z)
{
	size[0] = x;
	size[1] = y;
	size[2] = z;

	/*center[0] = sizeX / 2 + x;
	center[1] = 0;
	center[2] = sizeZ / 2 + z;*/

	
}


jeep::~jeep()
{
}

double * jeep::getCenter()
{
	return center;
}

void jeep::draw()
{	
	double x = size[0]-50, y = size[1], z = size[2]-25;

	kola(10, 10, x, y, z);	
	kolpaki(5, 5, x, y, z);
	lacznik(2, 60, x, y, z);

	nadwozie(x, y, z);

	//antena(2, 50, 90, 5, 20);
	//antena(2, 50, 90, 5, 50);
}

void jeep::kola(double r, double h, double ox, double oy, double oz)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc kol z jednej strony
	{
		double OX = ox + i, OY = oy, OZ = oz;

		double x, y, alpha, PI = 3.14;
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.0, 0);
		glVertex3d(0 + OX, 0 + OY, 0 + OZ);
		for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, h + OZ);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();


		//druga para kol

		OZ += 60; //OZ przesuwa prawe kola 
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.0, 0);
		glVertex3d(0 + OX, 0 + OY, 0 + OZ);
		for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, h + OZ);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();
	}
}

void jeep::kolpaki(double r, double h, double ox, double oy, double oz)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc kolpaków z jednej strony
	{
		//double OX = 0 + i, OY = 0, OZ = -2;	//-2 zeby wystawaly z kol
		double OX = ox + i, OY = oy, OZ = oz -2;

		double x, y, alpha, PI = 3.14;
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.8, 0.8);
		glVertex3d(0 + OX, 0 + OY, 0 + OZ);
		for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, h + OZ);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();


		//druga para kol

		OZ = oz + 60 + 7; //OZ przesuwa prawe kolpaki (65 - zerowa wartosc wychodzenia z kola xD) czyli 65+2=67 zeby bylo sy,etrycznie do 1 prary kolpakow
		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, 0 + OZ);
		for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, h + OZ);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();
	}


}

void jeep::lacznik(double r, double h, double ox, double oy, double oz)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc poloczen kol 
	{
		//double OX = 0 + i, OY = 0, OZ = 5;	//OZ=5 do polowy kol
		double OX = ox + i, OY = oy, OZ = oz;

		double x, y, alpha, PI = 3.14;
		glBegin(GL_QUAD_STRIP);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();

		//mozna wywalic to co na dole ale wtedy zostanie sciana boczna

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.4, 0.4);
		glVertex3d(0 + OX, 0 + OY, 0 + OZ);
		for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, 0 + OZ);
		}
		glEnd();



		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0 + OX, 0 + OY, h + OZ);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = r * sin(alpha);
			y = r * cos(alpha);
			glVertex3d(x + OX, y + OY, h + OZ);
		}
		glEnd();
	}
}

void jeep::blotnikPrzod(double r1, double r2, double h, double d)
{
	double PI = 3.14, alpha, x, y;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.1, 0.0, 0);	//czarny
	glVertex3d(0, 0, 0);
	for (alpha = PI; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r1 * sin(alpha);
		y = r1 * cos(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (alpha = 0; alpha >= -PI; alpha -= PI / 8.0)
	{
		x = r1 * sin(alpha);
		y = r1 * cos(alpha);
		glVertex3d(x, y, h);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, h);
	for (alpha = 0; alpha >= -PI; alpha -= PI / 8.0)
	{
		x = r1 * sin(alpha);
		y = r1 * cos(alpha);
		glVertex3d(x, y, h);
	}
	glEnd();

}


void jeep::scian(float x, float y, float z, float platform_X, float platform_Y, float platform_Z) {


	GLfloat p_x[3] = { x, y, z };
	GLfloat p_xx[3] = { x + platform_X, y, z };
	GLfloat p_xz[3] = { x, y, z + platform_Z };
	GLfloat p_xxz[3] = { x + platform_X, y, z + platform_Z };

	GLfloat g_x[3] = { x , y + platform_Y, z };
	GLfloat g_xx[3] = { x + platform_X, y + platform_Y, z };
	GLfloat g_xz[3] = { x, y + platform_Y, z + platform_Z };
	GLfloat g_xxz[3] = { x + platform_X, y + platform_Y, z + platform_Z };

	glBegin(GL_TRIANGLE_STRIP);		//podstawa
	glVertex3fv(p_x);
	glVertex3fv(p_xz);
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//góra
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//przód
	glVertex3fv(p_x);
	glVertex3fv(p_xz);
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//boki
	glVertex3fv(p_x);
	glVertex3fv(p_xx);
	glVertex3fv(g_x);
	glVertex3fv(g_xx);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);		//boki
	glVertex3fv(p_xz);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xz);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//ty³
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();
}

void jeep::maska(float x, float y, float z, float platform_X, float platform_Y, float platform_Z) {

	float zzz = platform_Z / 5;

	GLfloat p_x[3] = { x, y, z };
	GLfloat p_xx[3] = { x + platform_X, y, z };
	GLfloat p_xz[3] = { x, y, z + platform_Z };
	GLfloat p_xxz[3] = { x + platform_X, y, z + platform_Z };

	GLfloat g_x[3] = { x + platform_X / 2 , y + platform_Y, z + zzz };
	GLfloat g_xx[3] = { x + platform_X, y + platform_Y, z + zzz };
	GLfloat g_xz[3] = { x + platform_X / 2, y + platform_Y, z + platform_Z - zzz };
	GLfloat g_xxz[3] = { x + platform_X, y + platform_Y, z + platform_Z - zzz };

	glBegin(GL_TRIANGLE_STRIP);		//podstawa
	glVertex3fv(p_x);
	glVertex3fv(p_xz);
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//góra
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//przód
	glVertex3fv(p_x);
	glVertex3fv(p_xz);
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//boki
	glVertex3fv(p_x);
	glVertex3fv(p_xx);
	glVertex3fv(g_x);
	glVertex3fv(g_xx);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);		//boki
	glVertex3fv(p_xz);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xz);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//ty³
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();
}

void jeep::nadwozie(float x, float y, float z) {

	z += 10;
	float nadwozieX = 100, nadwozieY = 5, nadwozieZ = 50;
	float maskaX = 60, maskaY = 35, maskaZ = 50;
	float pakaX = nadwozieX - maskaX, pakaY = 8, pakaZ = 2;
	float oddalenie_scian = 3;


	maska(x, y + nadwozieY, z, maskaX, maskaY, maskaZ);
	scian(x, y, z, nadwozieX, nadwozieY, nadwozieZ);
	scian(x + maskaX, y + nadwozieY, z + oddalenie_scian, pakaX, pakaY, pakaZ);
	scian(x + maskaX, y + nadwozieY, z + nadwozieZ - oddalenie_scian - pakaZ, pakaX, pakaY, pakaZ);

	scian(x + nadwozieX - pakaZ, y + nadwozieY, z + oddalenie_scian + pakaZ,
		pakaZ, pakaY, nadwozieZ - 2 * (oddalenie_scian + pakaZ));
}


void jeep::antena(double r, double h, double OX, double OY, double OZ)
{

	double x, z, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.8, 0.0, 0);
	glVertex3d(0 + OX, 0 + OY, 0 + OZ);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(x + OX, 0 + OY, z + OZ);

	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(x + OX, 0 + OY, z + OZ);
		glVertex3d(OX, OY + h, OZ);
	}
	glEnd();
}