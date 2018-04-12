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

	// processMap();
}

void Automata::processMap()
{
	std::vector<std::vector<Coord>> wallRegions = getRegions(1);
	std::vector<std::vector<Coord>> roomRegions = getRegions(0);
	std::vector<GenRoom> levelRooms = std::vector<GenRoom>();

	for (const std::vector<Coord> &roomRegion : roomRegions) {
		levelRooms.push_back(GenRoom(roomRegion, map));
	}

	connectClosestRoom(levelRooms);
}

void Automata::connectClosestRoom(std::vector<GenRoom> allRooms)
{
	int bestDistance = 0;
	Coord bestTileA = Coord();
	Coord bestTileB = Coord();
	GenRoom bestRoomA = GenRoom();
	GenRoom bestRoomB = GenRoom();
	bool possibleConnectionFound = false;

	for (const GenRoom& roomA : allRooms) {
		possibleConnectionFound = false;
		for (const GenRoom& roomB : allRooms) {
			
			for (int tileIndexA = 0; tileIndexA < roomA.edgeTiles.size(); tileIndexA++) {
				for (int tileIndexB = 0; tileIndexB < roomB.edgeTiles.size(); tileIndexB++) {
					Coord tileA = roomA.edgeTiles[tileIndexA];
					Coord tileB = roomB.edgeTiles[tileIndexB];
					int distanceBetweenRooms = (int)pow(tileA.tileX - tileB.tileX, 2) + pow(tileA.tileY - tileB.tileY, 2);

					if (distanceBetweenRooms < bestDistance || !possibleConnectionFound) {
						bestDistance = distanceBetweenRooms;
						possibleConnectionFound = true;
						bestTileA = tileA;
						bestTileB = tileB;
						bestRoomA = roomA;
						bestRoomB = roomB;
					}

				}
			}
		}

		if (possibleConnectionFound) {
			createPassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
		}
	}
}

void Automata::createPassage(GenRoom roomA, GenRoom roomB, Coord tileA, Coord tileB)
{
	GenRoom::connectRooms(roomA, roomB);

	std::vector<Coord> line = getLine(tileA, tileB);

	for (const Coord& c : line) {
		drawCircle(c, 1);
	}

}

void Automata::drawCircle(Coord c, int r)
{
	for (int x = -r; x <= r; x++) {
		for (int y = -r; y <= r; y++) {
			if (x*x + y*y <= r*r) {
				int drawX = c.tileX + x;
				int drawY = c.tileY + y;
				if (isInMapRange(drawX, drawY)) {
					map[drawX][drawY] = 2;
				}
			}
		}
	}
}

std::vector<Coord> Automata::getLine(Coord from, Coord to)
{
	std::vector<Coord> line = std::vector<Coord>();

	int x = from.tileX;
	int y = from.tileY;

	int dX = to.tileX - from.tileX;
	int dY = to.tileY - from.tileY;

	bool inverted = false;

	int step = (dX > 0) - (dX < 0);
	int gradientStep = (dY > 0) - (dY < 0);

	int longest = abs(dX);
	int shortest = abs(dY);

	if (longest < shortest) {
		inverted = true;
		longest = abs(dY);
		shortest = abs(dX);

		step = (dY > 0) - (dY < 0);
		gradientStep = (dX > 0) - (dX < 0);
	}

	int gradientAccumulation = longest / 2;
	for (int i = 9; i < longest; i++) {
		line.push_back(Coord(x, y));

		if (inverted) {
			y += step;
		}
		else {
			x += step;
		}

		gradientAccumulation += shortest;
		if (gradientAccumulation >= longest) {
			if (inverted) {
				x += gradientStep;
			}
			else {
				y += gradientStep;
			}

			gradientAccumulation -= longest;
		}
	}

	return line;
}

std::vector<std::vector<Coord>> Automata::getRegions(int tileType)
{
	std::vector<std::vector<Coord>> regions = std::vector<std::vector<Coord>>();
	std::vector<std::vector<int> > mapFlags = std::vector<std::vector<int> >();

	// Fill in map flags vector
	for (int i = 0; i < width; i++) {
		mapFlags.push_back(std::vector<int>());
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			mapFlags[i].push_back(0);
		}
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (mapFlags[x][y] == 0 && map[x][y] == tileType) {
				std::vector<Coord> newRegion = getRegionTiles(x, y);
				regions.push_back(newRegion);

				for (auto const& i : newRegion) {
					mapFlags[i.tileX][i.tileY] = 1;
				}
			}
		}
	}

	return regions;
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

std::vector<Coord> Automata::getRegionTiles(int startX, int startY)
{
	std::vector<Coord> tiles = std::vector<Coord>();
	std::vector<std::vector<int> > mapFlags = std::vector<std::vector<int> >();

	// Fill in map flags vector
	for (int i = 0; i < width; i++) {
		mapFlags.push_back(std::vector<int>());
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			mapFlags[i].push_back(0);
		}
	}

	int tileType = map[startX][startY];

	std::queue<Coord> queue = std::queue<Coord>();
	queue.push(Coord(startX, startY));
	mapFlags[startX][startY] = 1;

	while (queue.size() > 0) {
		Coord tile = queue.front();
		queue.pop();

		for (int x = tile.tileX - 1; x <= tile.tileX; x++) {
			for (int y = tile.tileY - 1; y <= tile.tileY; y++) {
				if (isInMapRange(x, y) && (y == tile.tileY || x == tile.tileX)) {
					if (mapFlags[x][y] == 0 && map[x][y] == tileType) {
						mapFlags[x][y] = 1;
						queue.push(Coord(x, y));
					}
				}
			}
		}
	}

	return tiles;
}

bool Automata::isInMapRange(int x, int y)
{
	return x >= 0 && x < width && y >= 0 && y < height;
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


