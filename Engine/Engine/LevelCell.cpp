#include "LevelCell.h"

LevelCell::LevelCell()
{
	cellMesh = 0;
}

LevelCell::LevelCell(const LevelCell &)
{
}

LevelCell::~LevelCell()
{
}

bool LevelCell::initialize(ID3D11Device * device, char * modelFilename, WCHAR * textureFilename, WCHAR * textureFilename2)
{
	bool result;

	cellMesh = new ModelClass;
	if (!cellMesh) {
		return false;
	}

	result = cellMesh->Initialize(device, modelFilename, textureFilename, textureFilename2, NULL);
	if (!result) {
		return false;
	}

	return true;
}

void LevelCell::shutDown()
{
	if (cellMesh) {
		delete cellMesh;
		cellMesh = 0;
	}
}

