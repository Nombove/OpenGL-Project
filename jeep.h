#pragma once
#include <windows.h>
#include <gl\gl.h>             
#include <gl\glu.h>  
#include <cmath>

class jeep
{
public:
	jeep(double x, double y, double z);
	~jeep();
	void draw();
	double center[3];
	double *getCenter();


private:
	void kola(double r, double h, double OX, double OY, double OZ);
	void kolpaki(double r, double h, double ox, double oy, double oz);
	void lacznik(double r, double h, double ox, double oy, double oz);
	void blotnikPrzod(double r1, double r2, double h, double d);
	void scian(float x, float y, float z, float platform_X, float platform_Y, float platform_Z);
	void maska(float x, float y, float z, float platform_X, float platform_Y, float platform_Z);
	void nadwozie(float x, float y, float z);
	void antena(double r, double h, double OX, double OY, double OZ);

	double size[3];
	double sizeX;
	double sizeZ;

};

