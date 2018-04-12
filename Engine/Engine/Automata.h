#pragma once

#include <vector>
#include <queue>
#include <time.h>
#include <algorithm>

#include "GenRoom.h"
#include "Coord.h"

class Automata {
public:

	

	Automata();
	Automata(const Automata &automata);
	~Automata();

	void generateMap();
	void processMap();
	void connectClosestRoom(std::vector<GenRoom> allRooms);
	void createPassage(GenRoom roomA, GenRoom roomB, Coord tileA, Coord tileB);
	void drawCircle(Coord c, int r);
	std::vector<Coord> getLine(Coord from, Coord to);
	std::vector<std::vector<Coord>> getRegions(int tileType);
	void smoothMap();
	void randomFillMap();
	int getSurroundingWallCount(int mapX, int mapY);
	std::vector<Coord> getRegionTiles(int startX, int startY);
	bool isInMapRange(int x, int y);

	std::vector<std::vector<int>> getMap() {
		return map;
	};

	int width = 100;
	int height = 100;
	int randomFillPercent = 45;
	int iterations = 2;

	std::vector<std::vector<int> > map;
};
