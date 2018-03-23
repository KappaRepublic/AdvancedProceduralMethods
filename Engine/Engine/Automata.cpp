#include "Automata.h"

Automata::Automata()
{
	srand(time(NULL));
}

Automata::Automata(const Automata & automata)
{
}

Automata::~Automata()
{
}

void Automata::generateMap()
{
	// Fill in map vector
	for (int i = 0; i < width; i++) {
		map.push_back(std::vector<int>());
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			map[i].push_back(0);
		}
	}

	// Randomly fill some starting values;
	randomFillMap();

	for (int i = 0; i < iterations; i++) {
		smoothMap();
	}
}

void Automata::smoothMap()
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int neighbourWallTiles = getSurroundingWallCount(x, y);

			if (neighbourWallTiles > 4) {
				map[x][y] = 1;
			}
			else if (neighbourWallTiles < 4) {
				map[x][y] = 0;
			}
		}
	}
}

int Automata::getSurroundingWallCount(int mapX, int mapY)
{
	int wallCount = 0;
	for (int neighbourX = mapX - 1; neighbourX <= mapX + 1; neighbourX++) {
		for (int neighbourY = mapY - 1; neighbourY <= mapY + 1; neighbourY++) {
			if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height) {
				if (neighbourX != mapX || neighbourY != mapY) {
					wallCount += map[neighbourX][neighbourY];
				}
			}
			else {
				wallCount++;
			}
		}
	}

	return wallCount;
}

void Automata::randomFillMap()
{
	// Randomly assign if a cell is open or closed
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			// Ensure that all the values on the border are walls
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				map[x][y] = 1;
			} else if ((rand() % 100) + 1 < randomFillPercent) {
				map[x][y] = 1;
			}
			else {
				map[x][y] = 0;
			}
		}
	}
}


