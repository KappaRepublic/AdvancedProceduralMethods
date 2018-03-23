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

#define LEVEL_WIDTH 36
#define LEVEL_HEIGHT 36

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

#include "terrainshaderclass.h"
#include "lightshaderclass.h"
#include "PointLightShaderClass.h"
#include "textureshaderclass.h"
#include "transparentshaderclass.h"
#include "Fogshaderclass.h"

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

#include "CaveGenerator.h"

#include <list>
#include <time.h>
#include <algorithm>
#include <fstream>


/// IMGUI //
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "MenuHandler.h"




////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
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

	void generateRooms();
	void adjustLevel();
	void createHCorridor(int x1, int x2, int y);
	void createVCorridor(int y1, int y2, int x);
	void clearUnusedCells();

	LevelCell levelLayout[LEVEL_WIDTH][LEVEL_HEIGHT];

	CaveGenerator caveGen;

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

	IDirectInputDevice8* m_mouse;

	bool testInputOnce;
	bool inputCameraChange;
	bool overheadCam;
	bool my_tool_active;

	// Menu stuff
	bool equipmentActive;
	bool inventoryActive;
	bool testWindow;

	// Dungeon generation parameters
	int levelWidth = 36;
	int levelHeight = 36;
	int noOfAttempts = 24;
	int maxRoomWidth = 10;
	int maxRoomHeight = 10;
	int minRoomWidth = 3;
	int minRoomHeight = 3;

	// Menu Handler
	MenuHandler mHandler;

};

#endif