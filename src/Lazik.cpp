#include "..\include\Lazik.h"


Lazik::Lazik(GLfloat x, GLfloat y, GLfloat z)
	: Figura(x, y, z)
{
	setParams();
}

Lazik::~Lazik()
{
}

void Lazik::draw()
{
	// drawing wheels
	// left right; front middle rear
	Walec lfWheel(
		xpos,
		ypos + ylen,
		zpos,
		wheelRadius,
		-wheelHeight
	);

	Walec lmWheel(
		xpos,
		ypos + ylen / 3,
		zpos,
		wheelRadius,
		-wheelHeight
	);
	Walec lrWheel(
		xpos,
		ypos,
		zpos,
		wheelRadius,
		-wheelHeight
	);

	wheels.insert(wheels.end(), { lfWheel,lmWheel,lrWheel });

	Walec rfWheel(
		xpos + xlen + 4 * axleRadius,
		ypos + ylen,
		zpos,
		wheelRadius,
		wheelHeight
	);

	Walec rmWheel(
		xpos + xlen + 4 * axleRadius,
		ypos + ylen / 3,
		zpos,
		wheelRadius,
		wheelHeight
	);

	Walec rrWheel(
		xpos + xlen + 4 * axleRadius,
		ypos,
		zpos,
		wheelRadius,
		wheelHeight
	);


	wheels.insert(wheels.end(), { rfWheel,rmWheel,rrWheel });
	//TODO: znalezc lepszy pomysl na to ^

	for (auto wheel : wheels)
	{
		wheel.setColor(1, 0, 0);
		wheel.draw();
	}

	//drawing wheel axles
	//left right; front top bottom rear
	Walec lfAxle(
		xpos,
		ypos + ylen,
		zpos,
		axleRadius,
		xpos + 2 * axleRadius,
		ypos + (2.f / 3.f)*ylen,
		zpos + platformEvelation + platformThickness / 2
	);

	Walec ltAxle(
		xpos + axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation,
		axleRadius,
		xpos + 2 * axleRadius,
		ypos + (2.f / 3.f)*ylen,
		zpos + platformEvelation + platformThickness / 2
	);

	Walec lbAxle(
		xpos,
		ypos + ylen / 3,
		zpos,
		axleRadius,
		xpos + axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation
	);

	Walec lrAxle(
		xpos,
		ypos,
		zpos,
		axleRadius,
		xpos + axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation
	);

	axles.insert(axles.end(), { lfAxle,ltAxle,lbAxle, lrAxle });

	Walec rfAxle(
		xpos + xlen + 4 * axleRadius,
		ypos + ylen,
		zpos,
		axleRadius,
		xpos + +xlen + 2 * axleRadius,
		ypos + (2.f / 3.f)*ylen,
		zpos + platformEvelation + platformThickness / 2
	);

	Walec rtAxle(
		xpos + xlen + 3 * axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation,
		axleRadius,
		xpos + xlen + 2 * axleRadius,
		ypos + (2.f / 3.f)*ylen,
		zpos + platformEvelation + platformThickness / 2
	);

	Walec rbAxle(xpos + xlen + 4 * axleRadius,
		ypos + ylen / 3,
		zpos,
		axleRadius,
		xpos + xlen + 3 * axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation
	);

	Walec rrAxle(
		xpos + xlen + 4 * axleRadius,
		ypos,
		zpos,
		axleRadius,
		xpos + xlen + 3 * axleRadius,
		ypos + ylen / 6,
		zpos + (2.f / 3.f)*platformEvelation
	);

	axles.insert(axles.end(), { rfAxle,rtAxle,rbAxle,rrAxle });
	//TODO: znalezc lepszy pomysl na to ^

	for (auto axle : axles)
	{
		axle.setColor(0, 0, 1);
		axle.draw();
	}

	//drawing platform
	Szescian platform(
		xpos + 2 * axleRadius,
		ypos,
		zpos + platformEvelation,
		xlen,
		ylen,
		platformThickness
	);

	platform.setColor(0, 1, 0);
	platform.draw();

	//drawing camera
	Walec cameraStand(
		xpos + xlen + 2 * axleRadius - 1.5*cameraStandRadius,  // xpos + xlen + 2 * axleRadius puts center of the base in right-front corner
		ypos + ylen - 1.5*cameraStandRadius, // ypos + ylen, as above
		zpos + platformEvelation + platformThickness,
		cameraStandRadius,
		cameraStandHeight //temporary, parameterized
	);

	cameraStand.setColor(1, 0, 1);
	cameraStand.draw(-90, 0, 1, 0);

	Szescian cameraBox(
		xpos + xlen + 2 * axleRadius - 1.5*cameraStandRadius - cameraxlen / 2,
		ypos + ylen - 1.5*cameraStandRadius - cameraylen / 2,
		zpos + platformEvelation + platformThickness + cameraStandHeight,
		cameraxlen,
		cameraylen,
		camerazlen
	);

	cameraBox.setColor(1, 0, 0);
	cameraBox.draw();

	//drawing radio
	Walec radioStand(
		xpos + xlen + 2 * axleRadius - 1.5*radioStandRadius,
		ypos + 1.5*cameraStandRadius,
		zpos + platformEvelation + platformThickness,
		radioStandRadius,
		radioStandHeight
	);

	radioStand.setColor(1, 0, 1);
	radioStand.draw(-90, 0, 1, 0);

	Walec radioTop(
		xpos + xlen + 2 * axleRadius - 1.5*radioStandRadius,
		ypos + 1.5*cameraStandRadius,
		zpos + platformEvelation + platformThickness + radioStandHeight,
		radioTopRadius,
		radioTopHeight
	);

	radioTop.setColor(1, 0, 1);
	radioTop.draw(-90, 0, 1, 0);

	//drawing flag
	Walec flagPole(
		xpos + 2 * axleRadius + 5 * flagPoleRadius,
		ypos + 5 * flagPoleRadius,
		zpos + platformEvelation + platformThickness,
		flagPoleRadius,
		flagPoleHeight
	);

	flagPole.setColor(1, 0, 1);
	flagPole.draw(-90, 0, 1, 0);

	Szescian flag(
		xpos + 2 * axleRadius + 5 * flagPoleRadius,
		ypos + 5 * flagPoleRadius + flagylen / 2,
		zpos + platformEvelation + platformThickness + flagPoleHeight,
		-flagxlen,
		-flagylen,
		-flagzlen
	);
	flag.setColor(0, 0, 0);
	flag.draw();

	//drawing platform top
	Szescian platformTopBack(
		xpos + 2 * axleRadius,
		ypos + 5 * radioStandRadius,
		zpos + platformEvelation + platformThickness,
		xlen,
		platformTopxlen*(2.f / 3.f),
		platformTopHeight
	);

	platformTopBack.setColor(0.22f, 0.72f, 0.80f);
	platformTopBack.draw();

	TriangularPrism platformTopFront(
		xpos + xlen + 2 * axleRadius,
		ypos + 5 * radioStandRadius + platformTopxlen * (2.f / 3.f),
		zpos + platformEvelation + platformThickness,

		platformTopHeight, // w z po rotacji

		platformTopxlen*(1.f / 3.f),  // w y po rotacji
		xlen // w x po rotacji

	);

	platformTopFront.setColor(0.22f, 0.72f, 0.80f);

	platformTopFront.draw(-90, 0, 1, 0);

	// drawing floor
	//Szescian floor(-10000, -10000, -wheelRadius, 20000, 20000, 0.1);
	//floor.setColor(0.59f, 0.27f, 0.08f);
	//floor.draw();

	TriangularPrism kamien(
		-20,
		-20,
		-wheelRadius,
		15,
		20,
		30
	);

	kamien.setColor(0.6, 0.6, 0.6);
	//kamien.draw(-90,0,1,0);

}

void Lazik::setParams()
{
	//TODO: parametryzacja i opis

	wheelRadius = 8;
	wheelHeight = 10;

	axleRadius = wheelHeight / 5;
	axleHeight = xlen + 4 * axleRadius; //unused

	xlen = 6 * wheelRadius;
	ylen = 8 * wheelRadius;
	zlen = wheelRadius; // unused
	platformEvelation = 15;
	platformThickness = 10; // same as zlen?

	cameraStandRadius = 1.5*axleRadius;
	cameraStandHeight = 3 * cameraStandRadius;
	cameraxlen = ylen / 3;
	cameraylen = xlen / 3;
	camerazlen = platformThickness / 3;

	radioStandRadius = axleRadius;
	radioStandHeight = 8 * radioStandRadius;
	radioTopRadius = radioStandRadius * 2;
	radioTopHeight = radioStandHeight / 2;

	flagPoleRadius = 0.5f;
	flagPoleHeight = 3 * cameraStandHeight;
	flagxlen = 20;
	flagylen = 0.5f;
	flagzlen = 10;

	platformTopxlen = ylen - cameraxlen;
	platformTopHeight = 1.5*platformThickness;
}