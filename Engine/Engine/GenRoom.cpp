#include "GenRoom.h"

GenRoom::GenRoom() {

}

GenRoom::GenRoom(std::vector<Coord> roomTiles, std::vector<std::vector<int>> map) {
	tiles = roomTiles;
	roomSize = tiles.size();
	connectedRooms = std::vector<GenRoom>();

	edgeTiles = std::vector<Coord>();
	for (auto const& tile : tiles) {
		for (int x = tile.tileX - 1; x <= tile.tileX + 1; x++) {
			for (int y = tile.tileY - 1; y <= tile.tileY + 1; y++) {
				if (x == tile.tileX || y == tile.tileY) {
					if (map[x][y] == 1) {
						edgeTiles.push_back(tile);
					}
				}
			}
		}
	}
}

void GenRoom::connectRooms(GenRoom & roomA, GenRoom & roomB)
{
	roomA.connectedRooms.push_back(roomB);
	roomB.connectedRooms.push_back(roomA);
}

bool GenRoom::isConnected(GenRoom otherRoom)
{
	return std::find(connectedRooms.begin(), connectedRooms.end(), otherRoom) != connectedRooms.end();
	/*
	for (const GenRoom& testRoom : connectedRooms) {
	if (testRoom.tiles == otherRoom.tiles && testRoom.edgeTiles == otherRoom.edgeTiles && testRoom.connectedRooms == otherRoom.connectedRooms && testRoom.roomSize == otherRoom.roomSize) {
	return true;
	}
	}

	return false;
	*/
	
}

bool GenRoom::operator!=(const GenRoom & rhs)
{
	if (tiles.size() != rhs.tiles.size()) {
		return true;
	}


	for (int x = 0; x < tiles.size(); x++) {
		if (tiles[x] == rhs.tiles[x]) {

		}
		else {
			break;
		}

		return false;
	}


	if (edgeTiles.size() != rhs.edgeTiles.size()) {
		return true;
	}


	for (int x = 0; x < edgeTiles.size(); x++) {
		if (edgeTiles[x] == rhs.edgeTiles[x]) {

		}
		else {
			break;
		}

		return false;
	}


	if (connectedRooms.size() != rhs.connectedRooms.size()) {
		return true;
	}


	for (int x = 0; x < connectedRooms.size(); x++) {
		if (connectedRooms[x] == rhs.connectedRooms[x]) {

		}
		else {
			break;
		}

		return false;

	}



	return true;
}

bool GenRoom::operator==(const GenRoom & rhs)
{
	if (tiles.size() != rhs.tiles.size()) {
		return false;
	}


	for (int x = 0; x < tiles.size(); x++) {
		if (tiles[x] == rhs.tiles[x]) {

		}
		else {
			return false;
		}

	}


	if (edgeTiles.size() != rhs.edgeTiles.size()) {
		return false;
	}


	for (int x = 0; x < edgeTiles.size(); x++) {
		if (edgeTiles[x] == rhs.edgeTiles[x]) {

		}
		else {
			return false;
		}
	}


	if (connectedRooms.size() != rhs.connectedRooms.size()) {
		return false;
	}


	for (int x = 0; x < connectedRooms.size(); x++) {
		if (connectedRooms[x] == rhs.connectedRooms[x]) {

		}
		else {
			return false;
		}
	}



	return true;
}
