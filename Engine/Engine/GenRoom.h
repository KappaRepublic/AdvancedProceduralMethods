#pragma once


#include <vector>
#include "Coord.h"

class GenRoom {
public:
	GenRoom();

	GenRoom(std::vector<Coord> roomTiles, std::vector<std::vector<int>> map);

	static void connectRooms(GenRoom& roomA, GenRoom& roomB);

	bool isConnected(GenRoom otherRoom);
		
	
	bool operator!=(const GenRoom& rhs);
	bool operator==(const GenRoom& rhs);

	std::vector<Coord> tiles;
	std::vector<Coord> edgeTiles;
	std::vector<GenRoom> connectedRooms;

	int roomSize;
};
