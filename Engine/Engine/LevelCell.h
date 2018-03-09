#pragma once

#include "modelclass.h"

class LevelCell {
public:
	LevelCell();
	LevelCell(const LevelCell&);
	~LevelCell();

	bool initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, WCHAR * textureFilename2);
	void shutDown();

	bool wall;
	bool empty;

	ModelClass* getMesh() { return cellMesh; };
private:
	ModelClass* cellMesh;

};
