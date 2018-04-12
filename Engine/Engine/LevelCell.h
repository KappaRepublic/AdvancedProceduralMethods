#pragma once

#include "modelclass.h"

class LevelCell {
public:

	enum CellType {
		water = 0,
		grass = 1,
		dWall = 2,
		dFloor = 3,
		beach = 4
	};

	LevelCell();
	LevelCell(const LevelCell&);
	~LevelCell();

	bool initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, WCHAR * textureFilename2);
	bool initialize(ID3D11Device* device, ModelClass* model, CellType t);
	void shutDown();

	bool wall;
	bool empty;

	CellType type;

	ModelClass* getMesh() { return cellMesh; };
private:
	ModelClass* cellMesh;

};
