#include "Dungeon.h"

Dungeon::Dungeon()
{
	srand(time(NULL));
}

Dungeon::Dungeon(const Dungeon & d)
{
}

Dungeon::~Dungeon()
{
}

void Dungeon::generateDungeon()
{
	// Fill in map vector
	for (int i = 0; i < width + widthAdjust; i++) {
		map.push_back(std::vector<int>());
	}

	for (int i = 0; i < width + heightAdjust; i++) {
		for (int j = 0; j < height + 10; j++) {
			map[i].push_back(1);
		}
	}

	generateRooms();
	processMap();
}

void Dungeon::generateRooms()
{
	int randX, randY, randWidth, randHeight;
	D3DXVECTOR2 newCenter;
	newCenter.x = 0;
	newCenter.y = 0;

	// Attempt to generate a room at a random position with random size values
	randX = (rand() % width) + 1;
	randY = (rand() % height) + 1;
	randWidth = rand() % roomMaxWidth + 1;
	if (randWidth < roomMinWidth) {
		randWidth = roomMinWidth;
	}
	randHeight = rand() % roomMaxHeight + 1;
	if (randHeight < roomMinHeight) {
		randHeight = roomMinHeight;
	}

	// Create a room with the generated values
	DungeonRoom room = DungeonRoom(randX, randY, randWidth, randHeight);
	// Add the new room to the vector of rooms
	roomVector.push_back(room);

	// Update the map with the new room
	for (int x = room.x1; x < room.x2; x++) {
		for (int y = room.y1; y < room.y2; y++) {
			map[x][y] == 0;
		}
	}

	// Loop for the amount of attempts to create rooms
	for (int i = 0; i < noOfAttempts - 1; i++) {
		// Attempt to generate a room at a random position with random size values
		randX = rand() % width;
		randY = rand() % height;
		randWidth = rand() % roomMaxWidth + 1;
		if (randWidth < roomMinWidth) {
			randWidth = roomMinWidth;
		}
		randHeight = rand() % roomMaxHeight + 1;
		if (randHeight < roomMinHeight) {
			randHeight = roomMinHeight;
		}

		// Create a room with the generated values
		DungeonRoom room = DungeonRoom(randX, randY, randWidth, randHeight);

		bool failed = false;

		// Loop through the created room collides with any already existing room
		for (DungeonRoom checkedRoom : roomVector) {
			if (checkIntersection(room, checkedRoom)) {
				// Rooms intersect, placement fails
				// Break out of loop
				failed = true;
				break;
			}
		}

		// If the room placment was succesful
		if (!failed) {
			// Generate the corridor between this room and the previous room
			newCenter.x = room.centerX;
			newCenter.y = room.centerY;

			if (roomVector.size() != 0) {
				D3DXVECTOR2 prevCenter;
				prevCenter.x = roomVector.back().centerX;
				prevCenter.y = roomVector.back().centerY;

				int randDirection = rand() % 2;

				if (randDirection == 0) {
					createHCorridor((int)prevCenter.x, (int)newCenter.x, (int)prevCenter.y);
					createVCorridor((int)prevCenter.y, (int)newCenter.y, (int)newCenter.x);
				}
				else {
					createVCorridor((int)prevCenter.y, (int)newCenter.y, (int)prevCenter.x);
					createHCorridor((int)prevCenter.x, (int)newCenter.x, (int)newCenter.y);
				}
			}

			// Add the new room to the vector of rooms
			roomVector.push_back(room);
			// Update the map with the new room
			for (int x = room.x1; x < room.x2; x++) {
				for (int y = room.y1; y < room.y2; y++) {
					map[x][y] = 0;
				}
			}
		}
	}
}

void Dungeon::processMap()
{
	// Loop through the map vector and remove any wall cells that are entirely surrounded by wall
	for (int x = 0; x < width + widthAdjust; x++) {
		for (int y = 0; y < height + heightAdjust; y++) {
			if (!checkNeighboursForFloor(x, y)) {
				map[x][y] = 2;
			}
		}
	}
}

void Dungeon::createHCorridor(int x1, int x2, int y)
{
	for (int x = min(x1, x2); x <= max(x1, x2); x++) {
		map[x][y] = 0;
	}
}

void Dungeon::createVCorridor(int y1, int y2, int x)
{
	for (int y = min(y1, y2); y <= max(y1, y2); y++) {
		map[x][y] = 0;
	}
}

bool Dungeon::checkIntersection(DungeonRoom & roomA, DungeonRoom & roomB)
{
	return (roomA.x1 <= roomB.x2 && roomA.x2 >= roomB.x1 && roomA.y1 <= roomB.y2 && roomA.y2 >= roomB.y1);
}

bool Dungeon::checkNeighboursForFloor(int x, int y)
{
	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			// Don't check cells outside of range
			if (neighbourX < 0 || neighbourX > width + widthAdjust - 1 || neighbourY < 0 || neighbourY > height + heightAdjust - 1) {
				// Continue to next cell to be checked
				continue;
			}
			else {
				if (map[neighbourX][neighbourY] == 0) {
					return true;
				}
			}
		}
	}

	return false;
}
