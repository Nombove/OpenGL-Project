#pragma once

#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include "Szescian.h"
#include "Walec.h"
#include "TriangularPrism.h"
#include <vector>

class Lazik : public Figura
{
public:
	Lazik(GLfloat x, GLfloat y, GLfloat z);
	~Lazik();

	void draw();
	void setParams();

private:
	//TODO: Renaming to platformXlen etc.
	GLfloat xlen; // platform width
	GLfloat ylen; // platform length
	GLfloat zlen; // platform height

	GLfloat wheelRadius; // promien ko³a
	GLfloat wheelHeight; // wysokosc (szerokosc) kola
	std::vector<Walec> wheels;

	GLfloat axleRadius; // promien osi
	GLfloat axleHeight; // wysokosc (szerokosc) osi
	std::vector<Walec> axles;

	GLfloat platformEvelation; // podniesienie platformy nad poziom osi
	GLfloat platformThickness; // grubosc platformy

	GLfloat cameraStandRadius;
	GLfloat cameraStandHeight;
	GLfloat cameraxlen;
	GLfloat cameraylen;
	GLfloat camerazlen;

	GLfloat radioStandRadius;
	GLfloat	radioStandHeight;
	GLfloat radioTopRadius;
	GLfloat radioTopHeight;

	GLfloat flagPoleRadius;
	GLfloat flagPoleHeight;
	GLfloat flagxlen;
	GLfloat flagylen;
	GLfloat flagzlen;

	GLfloat platformTopxlen;
	GLfloat platformTopHeight;


};

