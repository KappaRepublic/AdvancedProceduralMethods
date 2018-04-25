#pragma once

#include "modelclass.h"
#include "Item.h"

enum ObjectType {
	stairs = 0,
	npc = 1,
	chest = 2,
	tree = 3,
	crystal = 4
};


class Object {
public:
	Object();
	Object(const Object &ob);
	~Object();

	bool init(ObjectType type, ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, WCHAR* iconTextureFilename, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	bool init(ObjectType type, ID3D11Device* device, ModelClass* model, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	void shutDown();

	float distanceFromPlayer;

	Item* chestOpen();

	ModelClass* getMesh() { return mesh; };
	D3DXVECTOR3 getPosition() { return position; };
	D3DXVECTOR3 getRotation() { return rotation; };
	ObjectType getObjectType() { return objectType; };
private:
	ObjectType objectType;
	ModelClass* mesh;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
};