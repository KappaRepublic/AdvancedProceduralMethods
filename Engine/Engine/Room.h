#pragma once

#include <math.h>
#include "modelclass.h"
#include "terrainclass.h"

class Room {
public:
	Room();
	Room(const Room&);
	~Room();

	void initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, int x, int y, int width, int height);
	bool intersects(Room &check);

	// Getters

	int getX1() { return x1; };
	int getX2() { return x2; };
	int getY1() { return y1; };
	int getY2() { return y2; };
	int getWidth() { return roomWidth; };
	int getHeight() { return roomHeight; };
	float getCenterX() { return centerX; };
	float getCenterY() { return centerY; };

	ModelClass* getFloorMesh() { return floorMesh; };
	TerrainClass* getTerrain() { return floorTerrain; };

private:
	int x1, x2, y1, y2;
	int roomWidth, roomHeight;
	int centerX, centerY;

	ModelClass* floorMesh;
	TerrainClass* floorTerrain;
};