#pragma once

#include <vector>
#include <time.h>
#include <algorithm>

#include <d3dx10math.h>
#include "Coord.h"

struct DungeonRoom {
	DungeonRoom(int x, int y, int width, int height) {
		x1 = x;
		x2 = x + width;
		y1 = y;
		y2 = y + height;

		roomWidth = width;
		roomHeight = height;

		centerX = ceil((x1 + x2) / 2);
		centerY = ceil((y1 + y2) / 2);
	}

	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	int roomWidth;
	int roomHeight;
	int centerX;
	int centerY;
};

class Dungeon {
public:
	Dungeon();
	Dungeon(const Dungeon& d);
	~Dungeon();

	void generateDungeon();

	int width = 25;
	int height = 25;
	int roomMinWidth = 3;
	int roomMaxWidth = 10;
	int roomMinHeight = 3;
	int roomMaxHeight = 10;
	int noOfAttempts = 50;

	int widthAdjust = 10;
	int heightAdjust = 10;

	std::vector<DungeonRoom> roomVector;
	std::vector<std::vector<int>> map;

protected:
	void generateRooms();
	void processMap();

	void createHCorridor(int x1, int x2, int y);
	void createVCorridor(int y1, int y2, int x);

	bool checkIntersection(DungeonRoom& roomA, DungeonRoom& roomB);

	bool checkNeighboursForFloor(int x, int y);
};
