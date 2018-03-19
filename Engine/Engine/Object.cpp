#include "Object.h"

Object::Object()
{
	mesh = 0;
}

Object::Object(const Object & ob)
{
}

Object::~Object()
{
}

bool Object::init(ObjectType type, ID3D11Device * device, char * modelFilename, WCHAR * textureFilename, WCHAR * iconTextureFilename, float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	mesh = new ModelClass();
	mesh->Initialize(device, modelFilename, textureFilename, iconTextureFilename, NULL);
	position = D3DXVECTOR3(posX, posY, posZ);
	rotation = D3DXVECTOR3(rotX, rotY, rotZ);

	objectType = type;
	
	return false;
}

void Object::shutDown()
{
	if (mesh) {
		delete mesh;
		mesh = 0;
	}
}
