#pragma once
#include "../definitions.h"

class Grid
{
public:
	Grid(int size);
	~Grid();

private:
	void DrawGrid(int HALF_GRID_SIZE);
};

