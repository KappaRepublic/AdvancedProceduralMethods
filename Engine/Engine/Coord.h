#pragma once

class Coord {
public:
	int tileX;
	int tileY;

	Coord() {

	}

	Coord(int x, int y) {
		tileX = x;
		tileY = y;
	}



	bool operator==(const Coord& rhs)
	{
		return (tileX == rhs.tileX && tileY == rhs.tileY);
	}

};