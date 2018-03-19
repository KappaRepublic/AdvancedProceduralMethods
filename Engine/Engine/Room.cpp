#include "Room.h"

Room::Room()
{
}

Room::Room(const Room &)
{
}

Room::~Room()
{
}

void Room::initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, int x, int y, int width, int height)
{
	x1 = x;
	x2 = x + width;
	y1 = y;
	y2 = y + height;

	roomWidth = width;
	roomHeight = height;

	centerX = ceil((x1 + x2) / 2);
	centerY = ceil((y1 + y2) / 2);

	floorMesh = new ModelClass;
	floorMesh->Initialize(device, modelFilename, textureFilename, NULL, NULL);

	// floorTerrain = new TerrainClass;
	// floorTerrain->Initialize(device, NULL);
	// floorTerrain->InitializeTerrain(device, width, height);
}

bool Room::intersects(Room &check)
{
	return (x1 <= check.getX2() && x2 >= check.getX1() && y1 <= check.getY2() && y2 >= check.getY1());
}
