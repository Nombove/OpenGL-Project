// Gl_template.c
//Wy��czanie b��d�w przed "fopen"
#define  _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION

// �adowanie bibliotek:

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
//#  pragma comment(lib, "lib/glew32.lib") 
#endif

#define _USE_MATH_DEFINES



// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif

#include "include/GL/glew.h"
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
//#include <math.h>				// Define for sqrt
#include <cmath>
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.
#include "jeep.h"
#include "AntTweakBar.h"
#include <time.h>

//#include "include/Lazik.h"
#include "include/Terrain.h"

#include "include/stb_image.h"


#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14


clock_t startTime = clock(); 
clock_t testTime;
clock_t timePassed;
GLfloat secondsPassed;



//GLfloat jeepPointsStatic[4]{ 47.5, 49.5, -57.5, -57.5 };
GLfloat jeepPointsStatic[4]{ 55, 45, -51, -41 };

GLfloat  jeepPoints[4]{ 0 };

GLfloat axMove = 0.f;
GLfloat angleSpeed = 10.0f;
GLfloat swingRad = 0.0f;
GLfloat axMoveDeg = 0;

GLfloat const_velocity = 0.5f;
GLfloat velocityL = 0.0f;
GLfloat velocityR = 0.0f;
GLfloat velocity = 0.0f;
GLfloat momentumConst = 0.1*const_velocity;
bool velocityUpdate = 0;
bool collision = false;
bool colliding = false;
int collisionTimer = 0;
bool points[5]{ false };


GLfloat posX = 0, posY = 0, posZ = 0;
jeep lazik(0, 0, 0);
GLfloat s_pred = 15;
GLfloat pred = 0;
GLfloat PI = M_PI;
GLfloat center[3]{50, 0, 25};

std::vector<GLfloat> midPointLocation{ 0.0f,0.0f,0.0f };
std::vector<GLfloat> midPointLocationScaled{ 0,0,0 };


// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;
static GLfloat rotSpeed = 10.0f;

static GLfloat zoom = 0.0f;
static GLfloat fov = 3000.0f;
static GLsizei lastHeight;
static GLsizei lastWidth;

static GLfloat cameraX = 500.0f;
static GLfloat cameraY = 50.0f;
static GLfloat cameraZ = 200.0f;
static GLfloat cameraSpeed = 20.0f;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nagłówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury
unsigned int tekstury[2];

// Declaration for Window procedure
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);


// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector		
	length = (float)sqrt((vector[0] * vector[0]) +
		(vector[1] * vector[1]) +
		(vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}


unsigned int LoadTexture(const char* file, GLenum textureSlot)
{
	GLuint texHandle;
	// Copy file to OpenGL
	glGenTextures(textureSlot, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	const auto data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, nrChannels, width , height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		// nie udalo sie zaladowac pliku
	}
	stbi_image_free(data);
	return texHandle;
}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
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
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}



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



void axxxes() {

	glBegin(GL_LINES);
	glColor3f(0.2, 0.8, 0.1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.8, 0.2, 0.1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.1, 0.2, 0.8);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
}


// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	/*
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);
	*/
	// Establish perspective: 

	gluPerspective(60.0f, fAspect, 1.0, fov);
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
	TwInit(TW_OPENGL, NULL);

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);



	//t = clock();
	



	TwInit(TW_OPENGL, NULL);
	TwBar *bar;
	bar = TwNewBar("Collect all points");

	TwAddVarRW(bar, "Angle", TW_TYPE_FLOAT, &axMoveDeg, "precision=1");
	TwAddVarRW(bar, "x position", TW_TYPE_FLOAT, &posX, "precision=1");
	TwAddVarRW(bar, "y position", TW_TYPE_FLOAT, &posZ, "precision=1");
	TwAddVarRW(bar, "velocityL", TW_TYPE_FLOAT, &velocityL, "precision=1");
	TwAddVarRW(bar, "velocityR", TW_TYPE_FLOAT, &velocityR, "precision=1");
	TwAddVarRW(bar, "velocity", TW_TYPE_FLOAT, &velocity, "precision=1");


	TwAddVarRW(bar, "Best Time ", TW_TYPE_FLOAT, &secondsPassed, " group=Winners precision=1");

	TwAddVarRW(bar, "Your Time", TW_TYPE_FLOAT, &secondsPassed, " group=Time precision=1");

	TwAddVarRW(bar, "1", TW_TYPE_BOOLCPP, &points[0], "group=Points");
	TwAddVarRW(bar, "2", TW_TYPE_BOOLCPP, &points[1], "group=Points");
	TwAddVarRW(bar, "3", TW_TYPE_BOOLCPP, &points[2], "group=Points");
	TwAddVarRW(bar, "4", TW_TYPE_BOOLCPP, &points[3], "group=Points");
	TwAddVarRW(bar, "5", TW_TYPE_BOOLCPP, &points[4], "group=Points");

	/*TwAddVarRW(bar, "1", TW_TYPE_FLOAT, &jeepPoints[0], "precision=1");
	TwAddVarRW(bar, "2", TW_TYPE_FLOAT, &jeepPoints[1], "precision=1");
	TwAddVarRW(bar, "3", TW_TYPE_FLOAT, &jeepPoints[2], "precision=1");
	TwAddVarRW(bar, "4", TW_TYPE_FLOAT, &jeepPoints[3], "precision=1");*/


	



	GLfloat refresh = 0.1;
	TwSetParam(bar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);

	int barSize[2] = { 224, 500 };
	TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);

	
}


unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaźnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nagłówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany składowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nagłówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nagłówek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaźnik pozycji pliku na początku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamięć buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy udało się przydzielić pamięć
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zostały wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami składowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaźnik bufora zawierającego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}


void kolizja(double x, double y, double z, double s) {

	int size = s;
	glBegin(GL_POLYGON);

	glVertex3f(x, z, y);       // P1
	glVertex3f(x, z, y + size);       // P2
	glVertex3f(x + size, z, y + size);       // P3
	glVertex3f(x + size, z, y);       // P4
	glEnd();

}
// Called to draw scene
void RenderScene(void)
{		
	bool stop = true;
	for (auto a : points) 
		if (!a) stop = false;
	
	if (!stop) {
		testTime = clock();
		timePassed = testTime - startTime;
		secondsPassed = timePassed / (double)CLOCKS_PER_SEC;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if (posX >= 150 && posX <= 350 && posZ >=-50 && posZ >= 150)
	//	pred = -pred;
	

	
	glPushMatrix();
		
		velocity = (velocityL + velocityR) / 2;
		velocityUpdate = 0;

		if (velocityL != velocityR)
		{	
			
			if (swingRad = 60*(velocityL + velocityR) / (2 * (velocityL - velocityR)))
				axMove += atan2(swingRad, 0) - atan2(swingRad, velocity);

		}

		posX += velocity * sin(axMove);
		posZ += velocity * cos(axMove);

	
		gluLookAt(posX, posY + 180, posZ - 300, posX, posY, posZ, 0.0, 1.0, 0.0);
		//gluLookAt(posX, posY + 250, posZ -1, posX, posY, posZ, 0.0, 1.0, 0.0);


		kolizja(200, 0, 0, 100);

		kolizja(100, 300, -10, 50);
		kolizja(400, 50, -10, 50);
		kolizja(-200, 0, -10, 50);
		kolizja(600, -50, -10, 50);
		kolizja(-100, -400, -10, 50);
		axxxes();

		//200, 0
		//300, 100
		for (int i = 0; i < 4; i++) {

			jeepPoints[i] = jeepPointsStatic[i];
		}

		jeepPoints[0] = posX + jeepPointsStatic[0];
		jeepPoints[1] = posZ + jeepPointsStatic[1];
		jeepPoints[2] = posX + jeepPointsStatic[2];
		jeepPoints[3] = posZ + jeepPointsStatic[3];

			collision = !(jeepPoints[0] < 200 || jeepPoints[1] < 0 || jeepPoints[2] > 300 || jeepPoints[3] > 100);
			

			if (!points[0])	points[0] = !(jeepPoints[0] < 100 || jeepPoints[1] < 300 || jeepPoints[2] > 150 || jeepPoints[3] > 350);
			if (!points[1]) points[1] = !(jeepPoints[0] < 400 || jeepPoints[1] < 50 || jeepPoints[2] > 450 || jeepPoints[3] > 100);
			if (!points[2]) points[2] = !(jeepPoints[0] < -200 || jeepPoints[1] < 0 || jeepPoints[2] > -150 || jeepPoints[3] > 50);
			if (!points[3])	points[3] = !(jeepPoints[0] < 600 || jeepPoints[1] < -50 || jeepPoints[2] > 650 || jeepPoints[3] > 0);
			if (!points[4])	points[4] = !(jeepPoints[0] < -100 || jeepPoints[1] < -400 || jeepPoints[2] > -5 || jeepPoints[3] > -350);



		glPushMatrix();
			glScalef(10, 5, 10);

			glTranslatef(0, 0, 100);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			Terrain powierzchnia("objects/mars.obj",0,-0.1,0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tekstury[0]);
			powierzchnia.draw();
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glTranslatef(0, 0, -100);

		glPopMatrix();


		/*glPushMatrix();
			Terrain objekty("objects/rock/rock.obj", 50, -100, 4);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tekstury[1]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			objekty.draw();
		glPopMatrix();*/

		glPushMatrix();

			glTranslatef(posX , posY, posZ);
			
					if (velocityL > 0)
					{
						if (velocityL - momentumConst > 0)
							velocityL -= momentumConst;
						else velocityL = 0;
					}
					else
					{
						if (velocityL + momentumConst < 0)
							velocityL += momentumConst;
						else velocityL = 0;
					}


					if (velocityR > 0)
					{
						if (velocityR - momentumConst > 0)
							velocityR -= momentumConst;
						else velocityR = 0;
					}
					else
					{
						if (velocityR + momentumConst < 0)
							velocityR += momentumConst;
						else velocityR = 0;
					}

					midPointLocationScaled = { midPointLocation[0] ,midPointLocation[2]  ,midPointLocation[1] };

					glTranslatef(midPointLocationScaled[0], midPointLocationScaled[2], midPointLocationScaled[1]);
					glRotatef(axMove * 180 / GL_PI, 0.0f, 1.0f, 0.0f); 
					glTranslatef(-midPointLocationScaled[0], -midPointLocationScaled[2], -midPointLocationScaled[1]); 
					glRotatef(90, 0, 1, 0);
					//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			jeep lazik(0, 0, 0);
			lazik.draw();
			
		
		glPopMatrix();

		axMoveDeg = fmod(axMove * 180 / GL_PI, 360);

		if (!colliding && collision)
		{
			velocityL = -0.6*velocityL;
			velocityR = -0.6*velocityR;
			velocity = -0.6*velocity;
			colliding = 1;
			collisionTimer = 5;
		}

		if (collisionTimer)
			collisionTimer--;
		else
			colliding = 0;

	TwDraw();
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
	BYTE RedRange, GreenRange, BlueRange;
	// Range for each color entry (7,7,and 3)


// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if (!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;

	// Loop through all the palette entries
	for (i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}


	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}


// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;

	// Register the window class
	if (RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
		lpszAppName,
		lpszAppName,

		// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

		// Window position and size
		50, 50,
		400, 400,
		NULL,
		NULL,
		hInstance,
		NULL);

	// If window was not created, quit
	if (hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND    hWnd,
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
		//glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

		tekstury[0]=LoadTexture("objects/mars.png", 1);
		tekstury[1] = LoadTexture("objects/rock.png", 1);

		// ładuje pierwszy obraz tekstury:
		//bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

	/*	glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
		
		if (bitmapData)
			free(bitmapData);

		// ładuje drugi obraz tekstury:
		//	bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);
			*/
		// ustalenie sposobu mieszania tekstury z tłem
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
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
		InvalidateRect(hWnd, NULL, FALSE); // loop

	}
	break;

	// Windows is telling the application that it may modify
	// the system palette.  This message in essance asks the 
	// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette)
		{
			int nRet;

			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);

			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		}
		break;


		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd))
		{
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);

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
		if (wParam == VK_UP)
		{
			velocityL += const_velocity;
			velocityR += const_velocity;
			velocityUpdate = 1;
		}

		if (wParam == VK_DOWN)
		{
			velocityL -= const_velocity;
			velocityR -= const_velocity;
			velocityUpdate = 1;
		}


		if (wParam == VK_LEFT) {
			velocityL += const_velocity;
			velocityR -= const_velocity;
			velocityUpdate = 1;
		}

		if (wParam == VK_RIGHT) 
		{
			velocityR += const_velocity;
			velocityL -= const_velocity;
			velocityUpdate = 1;
		}

		if (wParam == VK_CONTROL)
		{
			posY -= const_velocity;
			velocityUpdate = 1;
		}
		if (wParam == VK_SHIFT)
		{
			posY += const_velocity;
			velocityUpdate = 1;
		}
		if (wParam == VK_SPACE)
		{
			velocityR = velocityL = 0;
			velocityUpdate = 1;
			posX = 0;
			posZ = 0;
			posY = 0;
			axMove = 0;
			for (int i = 0; i < 5; i++)
				points[i] = false;
			startTime = clock();
		}
		/*int xd = 10;
		if (velocityL > xd)	velocityL = xd;
		if (velocityL < -xd)	velocityL = -xd;
		if (velocityR > xd)	velocityR = xd;
		if (velocityR < -xd)	velocityR = -xd;*/



		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	// A menu command
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// Exit the program
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;

		}
	}
	break;


	default:   // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, wParam, lParam));

	}
	if (TwEventWin(hWnd, message, wParam, lParam)) // send event message to AntTweakBar
		return 0;

	return (0L);
}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{

	switch (message)
	{
		// Initialize the dialog box
	case WM_INITDIALOG:
	{
		int i;
		GLenum glError;

		i = 0;
		do {
			glError = glGetError();
			/*SetDlgItemText(hDlg,IDC_ERROR1+i,gluErrorString(glError));*/
			i++;
		} while (i < 6 && glError != GL_NO_ERROR);


		return (TRUE);
	}
	break;

	// Process command messages
	case WM_COMMAND:
	{
		// Validate and Make the changes
		if (LOWORD(wParam) == IDOK)
			EndDialog(hDlg, TRUE);
	}
	break;

	// Closed from sysbox
	case WM_CLOSE:
		TwTerminate();

		EndDialog(hDlg, TRUE);
		break;
	}

	return FALSE;
}
