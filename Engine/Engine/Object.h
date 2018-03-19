#pragma once

#include "modelclass.h"

enum ObjectType {
	stairs = 0,
	npc = 1,
	chest = 2
};


class Object {
public:
	Object();
	Object(const Object &ob);
	~Object();

	bool init(ObjectType type, ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, WCHAR* iconTextureFilename, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	void shutDown();

	ModelClass* getMesh() { return mesh; };
	D3DXVECTOR3 getPosition() { return position; };
	D3DXVECTOR3 getRotation() { return rotation; };
private:
	ObjectType objectType;
	ModelClass* mesh;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
};