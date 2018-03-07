#pragma once

#include "modelclass.h"

class LevelCell {
public:
	LevelCell();
	LevelCell(const LevelCell&);
	~LevelCell();

	bool initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename);
	void shutDown();

	bool wall;

	ModelClass* getMesh() { return cellMesh; };
private:
	ModelClass* cellMesh;

};
