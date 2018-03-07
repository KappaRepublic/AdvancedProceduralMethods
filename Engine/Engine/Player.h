#pragma once

#define MOVESPEED 0.125f
#include "modelclass.h"

class Player {
public:
	Player();
	Player(const Player&);
	~Player();

	bool initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	void shutDown();

	ModelClass* getMesh() { return mesh; };
	void setPosition(float posX, float posY, float posZ);
	void setRotation(float rotX, float rotY, float rotZ);
	D3DXVECTOR3 getPosition() { return position; };
	D3DXVECTOR3 getRotation() { return rotation; };

	int levelPosX, levelPosY;

	void setFrameTime(float time) { frameTime = time; };
	int getDirection() { return direction; };

	// Movement
	void moveRight(bool keyDown);
	void moveLeft(bool keyDown);
	void moveForward(bool keyDown, bool colliding);
	void moveBackward(bool keyDown, bool colliding);
	void turnLeft(bool keyDown);
	void turnRight(bool keyDown);
private:
	ModelClass* mesh;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	float frameTime;
	float forwardSpeed, backwardSpeed;
	float rightSpeed, leftSpeed;
	float leftTurnSpeed, rightTurnSpeed;

	float targetX, targetZ;
	float targetRot;

	int direction = 0;

	bool inMotion;
};