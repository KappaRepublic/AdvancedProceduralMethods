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

#define NUM_OF_ROOMS 50

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

#include "lightclass.h"
#include "perlinnoise.h"
#include "modelclass.h"
#include "Room.h"

#include "LevelCell.h"
#include "Player.h"
#include "Item.h"
#include "Rendertextureclass.h"
#include "Debugwindowclass.h"

#include <list>
#include <time.h>
#include <algorithm>
#include <fstream>

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
	std::vector<int> anArrayOfBools;
	std::list<Room*> roomVector;
	std::vector<Item*> inventory;

	void generateRooms();
	void adjustLevel();
	void createHCorridor(int x1, int x2, int y);
	void createVCorridor(int y1, int y2, int x);

	LevelCell levelLayout[LEVEL_WIDTH][LEVEL_HEIGHT];

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

	LightClass* m_Light;

	Player* player;

	ModelClass* npc;

	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;

	bool testInputOnce;
	bool inputCameraChange;
	bool overheadCam;

};

#endif