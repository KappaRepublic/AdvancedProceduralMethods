#include "Player.h"

Player::Player()
{
	mesh = 0;
	equippedWeapon = -1;
}

Player::Player(const Player &)
{
}

Player::~Player()
{
}

bool Player::initialize(ID3D11Device * device, char * modelFilename, WCHAR * textureFilename, WCHAR* statusFilename, float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	bool result;

	setPosition(posX, posY, posZ);
	setRotation(rotX, rotY, rotZ);

	levelPosX = (int)posX;
	levelPosY = (int)posY;

	mesh = new ModelClass;
	mesh->Initialize(device, modelFilename, textureFilename, statusFilename, NULL);

	inMotion = false;

	// Set up player stats
	healthPointsMax = 100;
	healthPointsCur = healthPointsMax;

	return true;
}

void Player::shutDown()
{
	if (mesh) {
		delete mesh;
		mesh = 0;
	}
}

void Player::interact(vector<Object*>& objs, vector<Item*>& inv)
{
	// Check if the player is facing any interactable objects, and perform the correct functionality for any object that is found.

	// 0 is up, 1 is right, 2 is down, 3 is left
	switch (direction) {
	case 0:
		for (int i = 0; i < objs.size(); i++) {
			if (objs.at(i)->getPosition().x == (int)getPosition().x && objs.at(i)->getPosition().z == getPosition().z + 1 == true) {
				if (objs.at(i)->getObjectType() == ObjectType::chest) {
					// Add an item to the player's inventory
					inv.push_back(objs.at(i)->chestOpen());
					// Delet the opened chest from the level objects vector
					objs.erase(objs.begin() + i);
				}
			}
		}
		break;
	case 1:
		for (int i = 0; i < objs.size(); i++) {
			if (objs.at(i)->getPosition().x == (int)getPosition().x + 1 && objs.at(i)->getPosition().z == getPosition().z == true) {
				if (objs.at(i)->getObjectType() == ObjectType::chest) {
					// Add an item to the player's inventory
					inv.push_back(objs.at(i)->chestOpen());
					// Delet the opened chest from the level objects vector
					objs.erase(objs.begin() + i);
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < objs.size(); i++) {
			if (objs.at(i)->getPosition().x == (int)getPosition().x && objs.at(i)->getPosition().z == getPosition().z - 1 == true) {
				if (objs.at(i)->getObjectType() == ObjectType::chest) {
					// Add an item to the player's inventory
					inv.push_back(objs.at(i)->chestOpen());
					// Delet the opened chest from the level objects vector
					objs.erase(objs.begin() + i);
				}
			}
		}
		break;
	case 3:
		for (int i = 0; i < objs.size(); i++) {
			if (objs.at(i)->getPosition().x == (int)getPosition().x - 1 && objs.at(i)->getPosition().z == getPosition().z + 1 == true) {
				if (objs.at(i)->getObjectType() == ObjectType::chest) {
					// Add an item to the player's inventory
					inv.push_back(objs.at(i)->chestOpen());
					// Delet the opened chest from the level objects vector
					objs.erase(objs.begin() + i);
				}
			}
		}
		break;
	default:
		break;

	}
}


void Player::setPosition(float posX, float posY, float posZ)
{
	position.x = posX;
	position.y = posY;
	position.z = posZ;
}

void Player::setRotation(float rotX, float rotY, float rotZ) {
	rotation.x = rotX;
	rotation.y = rotY;
	rotation.z = rotZ;
}

void Player::moveRight(bool keyDown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keyDown)
	{

		rightSpeed = frameTime * 0.01f;

	}
	else
	{
		rightSpeed = 0.0f;
	}

	// Convert degrees to radians.
	radians = rotation.y * 0.0174532925f;

	// Update the position.
	position.x += cosf(radians) * rightSpeed;
	position.z += sinf(radians) * rightSpeed;

	return;
}

void Player::moveLeft(bool keyDown)
{
	float radians;

	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keyDown)
	{
		leftSpeed = frameTime * 0.01f;
	}
	else
	{
		leftSpeed = 0.0f;
	}


	// Convert degrees to radians.
	radians = rotation.y * 0.0174532925f;

	// Update the position.
	position.x -= cosf(radians) * leftSpeed;
	position.z -= sinf(radians) * leftSpeed;

	return;
}

void Player::moveForward(bool keyDown, bool colliding)
{
	float radians;

	if (!inMotion)
	{
		// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
		if (keyDown)
		{
			if (!colliding) {
				forwardSpeed = frameTime * 0.005f;
				backwardSpeed = 0.0f;
				rightTurnSpeed = 0.0f;
				leftTurnSpeed = 0.0f;
				inMotion = true;
			}
			else {
				forwardSpeed = 0.0f;
			}
		}
		else
		{
			forwardSpeed = 0.0f;
		}
		

		switch (direction) {
		case 0:
			targetZ = (position.z + 1.0f);
			targetX = position.x;
			break;
		case 1:
			targetZ = position.z;
			targetX = (position.x + 1.0f);
			break;
		case 2:
			targetZ = (position.z - 1.0f);
			targetX = position.x;
			break;
		case 3:
			targetZ = position.z;
			targetX = (position.x - 1.0f);
			break;
		default:
			break;
		}

		// targetZ = (position.z + 1.0f);

		// Convert degrees to radians.
		radians = rotation.y * 0.0174532925f;

		// Update the position.
		position.x += sinf(radians) * forwardSpeed;
		position.z += cosf(radians) * forwardSpeed;

		
	}
	else {
		if (forwardSpeed > 0.0f) {

			// Convert degrees to radians.
			radians = rotation.y * 0.0174532925f;

			// Update the position.
			position.x += sinf(radians) * forwardSpeed;
			position.z += cosf(radians) * forwardSpeed;


			switch (direction) {
			case 0:
				if (position.z >= targetZ) {
					position.z = targetZ;
					inMotion = false;
				}
				break;
			case 1:
				if (position.x >= targetX) {
					position.x = targetX;
					inMotion = false;
				}
				break;
			case 2:
				if (position.z <= targetZ) {
					position.z = targetZ;
					inMotion = false;
				}
				break;
			case 3:
				if (position.x <= targetX) {
					position.x = targetX;
					inMotion = false;
				}
				break;
			default:
				break;
			}
		}
		/*
		
		*/
	}

	return;
	/*
	if (keyDown) {
		position.z += MOVESPEED;
	}
	*/
}

void Player::moveBackward(bool keyDown, bool colliding)
{
	float radians;

	if (!inMotion)
	{
		// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
		if (keyDown)
		{
			if (!colliding) {
				forwardSpeed = 0.0f;
				backwardSpeed = frameTime * 0.005f;
				rightTurnSpeed = 0.0f;
				leftTurnSpeed = 0.0f;
				inMotion = true;
			}
			else {
				backwardSpeed = 0.0f;
			}
		}
		else
		{
			backwardSpeed = 0.0f;
		}

		switch (direction) {
		case 0:
			targetZ = (position.z - 1.0f);
			targetX = position.x;
			break;
		case 1:
			targetZ = position.z;
			targetX = (position.x - 1.0f);
			break;
		case 2:
			targetZ = (position.z + 1.0f);
			targetX = position.x;
			break;
		case 3:
			targetZ = position.z;
			targetX = (position.x + 1.0f);
			break;
		default:
			break;
		}

		// Convert degrees to radians.
		radians = rotation.y * 0.0174532925f;

		// Update the position.
		position.x -= sinf(radians) * backwardSpeed;
		position.z -= cosf(radians) * backwardSpeed;
	}
	else {
		if (backwardSpeed > 0.0f) {
			// Convert degrees to radians.
			radians = rotation.y * 0.0174532925f;

			// Update the position.
			position.x -= sinf(radians) * backwardSpeed;
			position.z -= cosf(radians) * backwardSpeed;


			switch (direction) {
			case 0:
				if (position.z <= targetZ) {
					position.z = targetZ;
					inMotion = false;
				}
				break;
			case 1:
				if (position.x <= targetX) {
					position.x = targetX;
					inMotion = false;
				}
				break;
			case 2:
				
				if (position.z >= targetZ) {
					position.z = targetZ;
					inMotion = false;
				}
				break;
			case 3:
				
				if (position.x >= targetX) {
					position.x = targetX;
					inMotion = false;
				}
				break;
			default:
				break;
			}
		}
	}

	return;
}

void Player::turnLeft(bool keyDown) {
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.

	if (!inMotion) {
		if (keyDown)
		{
			forwardSpeed = 0.0f;
			backwardSpeed = 0.0f;
			leftTurnSpeed = frameTime * 0.3f;
			rightTurnSpeed = 0.0f;
			inMotion = true;

			direction--;
			if (direction < 0) {
				direction = 3;
			}

		}
		else
		{
			leftTurnSpeed = 0.0f;
		}

		targetRot = (rotation.y - 90.0f);
		if (targetRot < 0.0f) {
			targetRot = 270.0f;
		}

		// Update the rotation.
		rotation.y -= leftTurnSpeed;

		// Keep the rotation in the 0 to 360 range.
		if (rotation.y < 0.0f)
		{
			rotation.y += 360.0f;
		}
	}
	else {
		if (leftTurnSpeed > 0.0f) {
			// Update the rotation.
			rotation.y -= leftTurnSpeed;

			if (rotation.y <= targetRot) {
				rotation.y = targetRot;
				inMotion = false;
			}
		}
	}

	return;
}

void Player::turnRight(bool keyDown) {
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (!inMotion) {
		if (keyDown)
		{
			forwardSpeed = 0.0f;
			backwardSpeed = 0.0f;
			rightTurnSpeed = frameTime * 0.3f;
			leftTurnSpeed = 0.0f;
			inMotion = true;

			direction++;
			if (direction > 3) {
				direction = 0;
			}

		}
		else
		{
			rightTurnSpeed = 0.0f;
		}

		targetRot = (rotation.y + 90.0f);
		if (targetRot > 360.0f) {
			targetRot = 90.0f;
		}

		// Update the rotation.
		rotation.y += rightTurnSpeed;

		// Keep the rotation in the 0 to 360 range.
		if (rotation.y > 360.0f)
		{
			rotation.y -= 360.0f;
		}
	}
	else {
		if (rightTurnSpeed > 0.0f)
		{
			// Update the rotation.
			rotation.y += rightTurnSpeed;

			if (rotation.y >= targetRot) {
				rotation.y = targetRot;
				inMotion = false;
			}
		}
	}

	return;
}
