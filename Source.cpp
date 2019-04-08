#include "szescian/definitions.h"
#include "szescian/Features/Grid.h"
#include "szescian/Features/Obj_Loader.h"

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;


static GLsizei lastHeight;
static GLsizei lastWidth;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag��wek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury


// Declaration for Window procedure
LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);



// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
	{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
	}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat nRange = 150.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;
		
	fAspect=(GLfloat)w/(GLfloat)h;
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
	{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	// Black brush
	glColor3f(0.0,0.0,0.0);
	}

// LoadBitmapFile
// opis: �aduje map� bitow� z pliku i zwraca jej adres.
//       Wype�nia struktur� nag��wka.
//	 Nie obs�uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wska�nik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag��wek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	unsigned int		imageIdx = 0;			// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk�adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag��wek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag��wek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wska�nik pozycji pliku na pocz�tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pami�� buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda�o si� przydzieli� pami��
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta�y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk�adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wska�nik bufora zawieraj�cego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}
// Called to draw scene

void cegla(double x, double y, double z)
{
	//gora dol
	glColor3f(0.8f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(x + 24, y, z);
	glVertex3f(x + 24, y, z + 12);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + 12);
	glEnd();

	glColor3f(0.8f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP); //odwrocenie kolejnosci, bo koloruje od dolu
	glVertex3f(x, y + 6, z);
	glVertex3f(x, y + 6, z + 12);
	glVertex3f(x + 24, y + 6, z);
	glVertex3f(x + 24, y + 6, z + 12);
	glEnd();

	//przod tyl

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP); //odwrocenie kolejnosci, bo koloruje od dolu
	glVertex3f(x + 24, y, z);
	glVertex3f(x, y, z);
	glVertex3f(x + 24, y + 6, z);
	glVertex3f(x, y + 6, z);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP); //odwrocenie kolejnosci, bo koloruje od dolu
	glVertex3f(x + 24, y + 6, z + 12);
	glVertex3f(x, y + 6, z + 12);
	glVertex3f(x + 24, y, z + 12);
	glVertex3f(x, y, z + 12);
	glEnd();

	//boki

	glColor3f(0.9f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP); //odwrocenie kolejnosci, bo koloruje od dolu
	glVertex3f(x + 24, y, z + 12);
	glVertex3f(x + 24, y, z);
	glVertex3f(x + 24, y + 6, z + 12);
	glVertex3f(x + 24, y + 6, z);
	glEnd();

	glColor3f(0.9f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP); //odwrocenie kolejnosci, bo koloruje od dolu
	glVertex3f(x, y + 6, z + 12);
	glVertex3f(x, y + 6, z);
	glVertex3f(x, y, z + 12);
	glVertex3f(x, y, z);
	glEnd();

}

void mur(int ile_cegiel, int ile_warstw, double x, double y, double z) {

	int i, j;
	double x_temp;

	for (i = 0; i < ile_warstw; i++) {
		x_temp = x;
		if (i % 2 == 1) {
			x_temp = x_temp - 12;
			for (j = 0; j <= ile_cegiel; j++) {
				cegla(x_temp, y, z);
				x_temp = x_temp + 25;
			}
		}
		else {
			for (j = 0; j < ile_cegiel; j++) {
				cegla(x_temp, y, z);
				x_temp = x_temp + 25;
			}
		}
		y = y - 7;
	}

}

void szescian(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow

		GLfloat sa[3] = { 0.0f,0.0f,0.0f };
		GLfloat sb[3] = { 10.0f,0.0f,0.0f };
		GLfloat sc[3] = { 10.0f,10.0f,0.0f };
		GLfloat sd[3] = { 0.0f,10.0f,0.0f };
		GLfloat se[3] = { 0.0f,0.0f,-10.0f };
		GLfloat sf[3] = { 10.0f,0.0f,-10.0f };
		GLfloat sg[3] = { 10.0f,10.0f,-10.0f };
		GLfloat sh[3] = { 0.0f,10.0f,-10.0f };


		// Sciany skladowe
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sc);
		glVertex3fv(sd);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(sg);
		glVertex3fv(sc);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sf);
		glVertex3fv(se);
		glVertex3fv(sh);
		glVertex3fv(sg);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(se);
		glVertex3fv(sa);
		glVertex3fv(sd);
		glVertex3fv(sh);
		glEnd();

		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glVertex3fv(sg);
		glVertex3fv(sh);
		glEnd();

		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(se);
		glEnd();
	}
}

void walec(double r, double h)
{
	int i;
	for (i = 0; i < 100; i += 25)		//	100/25=ilosc poloczen kol 
	{
		double OX = 0 + i, OY = 0, OZ = 0;
		double x, y, alpha, PI = 3.14;
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

void ramie(double r1, double r2, double h, double d)
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
	//drugie ramie

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.2, 0.0, 0);	//czarny2
	//glVertex3d(d,r2,0);
	//glVertex3d(d, r2, h);
	for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
	{
		x = d + r2 * sin(alpha);
		y = d + r2 * cos(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	//glVertex3d(d, r2, 0);
	for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
	{
		x = d + r2 * sin(alpha);
		y = d + r2 * cos(alpha);
		glVertex3d(x, y, h);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	//glVertex3d(d, r2, h);
	for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
	{
		x = d + r2 * sin(alpha);
		y = d + r2 * cos(alpha);
		glVertex3d(x, y, h);
	}
	glEnd();
}


//funkcje uzyte w laziku
void DrawGrid(int HALF_GRID_SIZE)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i += 10)
	{
		glVertex3f((float)-HALF_GRID_SIZE, (float)i, 0);
		glVertex3f((float)HALF_GRID_SIZE, (float)i, 0);

		glVertex3f((float)i, (float)-HALF_GRID_SIZE, 0);
		glVertex3f((float)i, (float)HALF_GRID_SIZE, 0);
	}
	glEnd();


	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f((float)HALF_GRID_SIZE, 1, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, (float)HALF_GRID_SIZE, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, (float)HALF_GRID_SIZE);
	glEnd();
}


void kolaL(double r, double h)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc kol z jednej strony
	{
		double OX = 0 + i, OY = 0, OZ = 0;
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

		OZ = 60; //OZ przesuwa prawe kola 
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

void kolpaki(double r, double h)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc kolpak�w z jednej strony
	{
		double OX = 0 + i, OY = 0, OZ = -2;	//-2 zeby wystawaly z kol
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

		OZ = 60 + 7; //OZ przesuwa prawe kolpaki (65 - zerowa wartosc wychodzenia z kola xD) czyli 65+2=67 zeby bylo sy,etrycznie do 1 prary kolpakow
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

void lacznik(double r, double h)
{
	int i;
	for (i = 0; i < 120; i += 30)		//	120/30=ilosc poloczen kol 
	{
		double OX = 0 + i, OY = 0, OZ = 5;	//OZ=5 do polowy kol
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


void blotnikPrzod(double r1, double r2, double h, double d)
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


void scian(float x, float y, float z, float platform_X, float platform_Y, float platform_Z) {


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

	glBegin(GL_TRIANGLE_STRIP);		//g�ra
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//prz�d
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

	glBegin(GL_TRIANGLE_STRIP);		//ty�
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();
}

void maska(float x, float y, float z, float platform_X, float platform_Y, float platform_Z) {

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

	glBegin(GL_TRIANGLE_STRIP);		//g�ra
	glVertex3fv(g_x);
	glVertex3fv(g_xz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);		//prz�d
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

	glBegin(GL_TRIANGLE_STRIP);		//ty�
	glVertex3fv(p_xx);
	glVertex3fv(p_xxz);
	glVertex3fv(g_xx);
	glVertex3fv(g_xxz);
	glEnd();
}

void nadwozie(float x, float y, float z) {

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


void antena(double r, double h, double OX, double OY, double OZ)
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


void ladowanie(void)
{
	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile("box_stack.obj");

	// Check to see if it loaded

	// If so continue
	if (loadout)
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// Print Mesh Name
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			// Print Vertices
			file << "Vertices:\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				file << "V" << j << ": " <<
					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
			}

			// Print Indices
			file << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
			}

			// Print Material
			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			// Leave a space to separate from the next mesh
			file << "\n";
		}

		// Close File
		file.close();
	}
	// If not output an error
	else
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}
}

void RenderScene(void)
	{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

														/////////////////////////////////////////////////////////////////
														// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
														/////////////////////////////////////////////////////////////////
	
	//Spos�b na odr�nienie "przedniej" i "tylniej" �ciany wielok�ta:
	//glPolygonMode(GL_BACK,GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	ladowanie();

	Grid grid(200);
	cegla(30, 40, 30);
	DrawGrid(500);
	kolaL(10, 10);	//promien,szerokosc k�
	kolpaki(5, 5);
	lacznik(2, 60);
	//blotnikPrzod(10.0, 10.0, 50.0, 0.0);	//wielkosc 1,wielkosc 2.,szerokosc,jak wysoko ma byc polozone(OY)

	//maska(0.0f, 10.0f, 10.0f,50,20,50);	//polozenie: OY i OZ=10 bo 10 maj� ko�a, 3 ost. param to wielkosc maski w XYZ
	nadwozie(0, 0, 10);

	antena(2, 50, 90, 5, 20);
	antena(2, 50, 90, 5, 50);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
	{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
	}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
	{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)


	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		

	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
	}


// Entry point of all Windows programs
int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int                     nCmdShow)
	{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	// Register the window class
	if(RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
				50, 50,
				800, 800,
				NULL,
				NULL,
				hInstance,
				NULL);

	// If window was not created, quit
	if(hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return int(msg.wParam);
	}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(       HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam)
	{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
		{
		// Window creation, setup for OpenGL
		case WM_CREATE:
			// Store the device context
			hDC = GetDC(hWnd);              

			// Select the pixel format
			SetDCPixelFormat(hDC);          

			// Create palette if needed
			hPalette = GetOpenGLPalette(hDC);

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			SetupRC();
			glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			
			
			// �aduje pierwszy obraz tekstury:
			bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);
			
			glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			
			if(bitmapData)
			free(bitmapData);

			// �aduje drugi obraz tekstury:
			bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			
			if(bitmapData)
			free(bitmapData);

			// ustalenie sposobu mieszania tekstury z t�em
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
			break;

		// Window is being destroyed, cleanup
		case WM_DESTROY:
			// Deselect the current rendering context and delete it
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);

			// Delete the palette if it was created
			if(hPalette != NULL)
				DeleteObject(hPalette);

			// Tell the application to terminate after the window
			// is gone.
			PostQuitMessage(0);
			break;

		// Window is resized.
		case WM_SIZE:
			// Call our function which modifies the clipping
			// volume and viewport
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
		case WM_PAINT:
			{
			// Call OpenGL drawing code
			RenderScene();

			SwapBuffers(hDC);

			// Validate the newly painted client area
			ValidateRect(hWnd,NULL);
			}
			break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
		case WM_QUERYNEWPALETTE:
			// If the palette was created.
			if(hPalette)
				{
				int nRet;

				// Selects the palette into the current device context
				SelectPalette(hDC, hPalette, FALSE);

				// Map entries from the currently selected palette to
				// the system palette.  The return value is the number 
				// of palette entries modified.
				nRet = RealizePalette(hDC);

				// Repaint, forces remap of palette in current window
				InvalidateRect(hWnd,NULL,FALSE);

				return nRet;
				}
			break;

	
		// This window may set the palette, even though it is not the 
		// currently active window.
		case WM_PALETTECHANGED:
			// Don't do anything if the palette does not exist, or if
			// this is the window that changed the palette.
			if((hPalette != NULL) && ((HWND)wParam != hWnd))
				{
				// Select the palette into the device context
				SelectPalette(hDC,hPalette,FALSE);

				// Map entries to system palette
				RealizePalette(hDC);
				
				// Remap the current colors to the newly realized palette
				UpdateColors(hDC);
				return 0;
				}
			break;

		// Key press, check for arrow keys to do cube rotation.
		case WM_KEYDOWN:
			{
			if(wParam == VK_UP)
				xRot-= 5.0f;

			if(wParam == VK_DOWN)
				xRot += 5.0f;

			if(wParam == VK_LEFT)
				yRot -= 5.0f;

			if(wParam == VK_RIGHT)
				yRot += 5.0f;

			if (wParam == VK_SUBTRACT)
				zRot -= 5.0f;

			if (wParam == VK_ADD)
				zRot += 5.0f;

			xRot = GLfloat((const int)xRot % 360);
			yRot = GLfloat((const int)yRot % 360);
			zRot = GLfloat((const int)zRot % 360);

			InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		// A menu command
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				// Exit the program
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;

				// Display the about box
				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						DLGPROC(AboutDlgProc));
					break;
				}
			}
			break;


	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));

	}

    return (0L);
	}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
	{
	
    switch (message)
	{
		// Initialize the dialog box
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;

			// glGetString demo
			SetDlgItemText(hDlg,IDC_OPENGL_VENDOR,LPCSTR(glGetString(GL_VENDOR)));
			SetDlgItemText(hDlg,IDC_OPENGL_RENDERER, LPCSTR(glGetString(GL_RENDERER)));
			SetDlgItemText(hDlg,IDC_OPENGL_VERSION, LPCSTR(glGetString(GL_VERSION)));
			SetDlgItemText(hDlg,IDC_OPENGL_EXTENSIONS, LPCSTR(glGetString(GL_EXTENSIONS)));

			// gluGetString demo
			SetDlgItemText(hDlg,IDC_GLU_VERSION, LPCSTR(gluGetString(GLU_VERSION)));
			SetDlgItemText(hDlg,IDC_GLU_EXTENSIONS, LPCSTR(gluGetString(GLU_EXTENSIONS)));


			// Display any recent error messages
			i = 0;
			do {
				glError = glGetError();
				SetDlgItemText(hDlg,IDC_ERROR1+i, LPCSTR(gluErrorString(glError)));
				i++;
				}
			while(i < 6 && glError != GL_NO_ERROR);


			return (TRUE);
			}
			break;

		// Process command messages
	    case WM_COMMAND:      
			{
			// Validate and Make the changes
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;

		// Closed from sysbox
		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}

	return FALSE;
	}