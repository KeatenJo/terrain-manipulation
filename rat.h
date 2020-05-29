#pragma once
#include "terrain.h"
class Rat
{
public:
	
	Rat();
	void InitRat(double x, double y, double z, double deg, Terrain& terrain);
	
	void Draw();

	void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

	void DrawCircle(double x, double y, double radius);

	void SpinLeft();

	void SpinRight();

	double getDeg();

	double getDx();

	double getDy();

	void ScurryForward();

	double getX();

	double getY();

	double getDir();

	double getZ();


private:
	double mx, my, mz, mdeg;
	Terrain mTerrainPointer;

};

