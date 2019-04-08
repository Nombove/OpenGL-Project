#include "Grid.h"



Grid::Grid(int size)
{
	DrawGrid(size);
}


Grid::~Grid()
{
}

void Grid::DrawGrid(int HALF_GRID_SIZE)
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