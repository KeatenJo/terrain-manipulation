#pragma once

const int WIDTH = 100;
const int HEIGHT = 100;
const double WaterHeight = .5;

class Chunk {
public:
	Chunk();
	void Draw(int i, int j);
private:

};

class Terrain {
public:
	Terrain();
	void GenerateChunks();
	void Draw();

	void DrawWater();

private:
	Chunk mChunks[WIDTH][HEIGHT];
};

double generateZLevel(double i, double j);