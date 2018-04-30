////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#define LEVEL_WIDTH 100
#define LEVEL_HEIGHT 100

#define NUM_OF_ROOMS 24

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "soundclass.h"

#include "terrainshaderclass.h"
#include "lightshaderclass.h"
#include "PointLightShaderClass.h"
#include "textureshaderclass.h"
#include "transparentshaderclass.h"
#include "Fogshaderclass.h"
#include "skyplaneclass.h"
#include "skyplaneshaderclass.h"

#include "lightclass.h"
#include "perlinnoise.h"
#include "modelclass.h"
#include "Room.h"

#include "LevelCell.h"
#include "Player.h"
#include "Item.h"
#include "Rendertextureclass.h"
#include "Debugwindowclass.h"
#include "Object.h"

#include "Airship.h"

#include "CaveGenerator.h"
#include "Automata.h"
#include "Dungeon.h"

#include <list>
#include <time.h>
#include <algorithm>
#include <fstream>


/// IMGUI //
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "MenuHandler.h"


struct sortStruct {
	inline bool operator() (const Object* obj1, const Object* obj2) {
		return (obj2->distanceFromPlayer < obj1->distanceFromPlayer);
	}
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass(SoundClass* sound);
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	// Level structure
	bool levelMap[LEVEL_WIDTH][LEVEL_HEIGHT];
	std::list<Room*> roomVector;
	std::vector<Item*> inventory;
	std::vector<Object*> levelObjects;
	Object crystal;

	void generateRooms();
	void generateCave();
	void generateTrees();
	void generateChests();
	void spawnCrystal();
	void adjustLevel();
	void createHCorridor(int x1, int x2, int y);
	void createVCorridor(int y1, int y2, int x);
	void clearUnusedCells();

	void generateDungeon();
	void processMap();
	bool checkValidEntrance(int x, int y);
	bool checkIfBeach(int x, int y);
	int getNeighbouringWalls(int x, int y);
	void setPlayerSpawn();

	LevelCell levelLayout[LEVEL_WIDTH][LEVEL_HEIGHT];
	
	SoundClass* sound;

	bool outsideMusic;
	bool insideMusic;

	CaveGenerator caveGen;
	Automata autoGen;
	Dungeon dungeon;

	Airship airship;

	int treeChance = 9;

	// ImGuiIO& io;

private:
	bool RenderToTexture();
	bool RenderScene();

	bool HandleInput(float);
	bool RenderGraphics();

	bool checkForCollision(int direction, bool movingForward);

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;

	TerrainShaderClass* m_TerrainShader;
	LightShaderClass* lightShader;
	PointLightShaderClass* pLightShader;
	TextureShaderClass* textureShader;
	TransparentShaderClass* transShader;
	FogShaderClass* fogShader;

	LightClass* m_Light;

	Player* player;

	ModelClass* npc;

	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;

	// Pre-Made Tiles
	ModelClass* grassTile;
	ModelClass* waterTile;
	ModelClass* dungeonWallTile;
	ModelClass* dungeonFloorTile;
	ModelClass* beachTile;

	// Pre-Made Objects
	ModelClass* treeModel;
	ModelClass* chestModel;
	ModelClass* crystalModel;

	SkyPlaneClass* skyPlane;
	SkyPlaneShaderClass* skyShader;


	IDirectInputDevice8* m_mouse;

	bool testInputOnce;
	bool inputCameraChange;
	bool overheadCam;
	bool my_tool_active;

	// Menu stuff
	bool equipmentActive;
	bool inventoryActive;
	bool testWindow;

	// Dungeon position
	int dungeonPositionX = 25;
	int dungeonPositionY = 25;

	// Player spawn stuff
	int minDungeonDistance = 25;

	// Menu Handler
	MenuHandler mHandler;

};

#endif