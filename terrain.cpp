#include <stdlib.h>
#include <cmath>
#include "terrain.h"
#include "glut.h"

Chunk::Chunk() {

}

Terrain::Terrain( ) {

}

void Terrain::GenerateChunks()
{
}

double generateZLevel(double i, double j)
{


	double z = 0.0;
	double scale = .75;
	z += sin(i);
	z += cos(j);
	z += sin(i * 2);
	z *= cos(j / 2 + 7);
	z = z - cos(i+.2);
	z = z - sin(.9 / j);
	z = z * sin(2*.556) + cos(.7457/.224587) / 2;
	z += sin(j * .7/ i );
	z += cos(j * .5);
	z += .6;

	//z -= .8;
	return z * scale;
}

void Terrain::Draw()
{
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			mChunks[i][j].Draw(i, j);
		}
	}
	DrawWater();
}

void Terrain::DrawWater() {
	glColor3d(0, .5, .75);
	glBegin(GL_QUADS);
	glVertex3d(0, 0, WaterHeight);
	glVertex3d(WIDTH, 0, WaterHeight);
	glVertex3d(WIDTH, HEIGHT, WaterHeight);
	glVertex3d(0, HEIGHT, WaterHeight);
	glEnd();
	return;
}

void Chunk::Draw(int i, int j)
{
	double z_level = generateZLevel(i, j);
	/*unsigned char r = (i * 78456 + j * 11459) % 256;
	unsigned char g = (i * 98572 + j * 24657) % 256;
	unsigned char b = (i * 14978 + j * 33645) % 256;*/

	double g = .4 * z_level;
	glColor3d(0.0, g, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(i, j, generateZLevel(i, j));
	glVertex3d(i+1, j, generateZLevel(i+1, j));
	glVertex3d(i+1, j+1, generateZLevel(i+1, j+1));
	glVertex3d(i, j+1, generateZLevel(i, j+1));
	glEnd();

}
