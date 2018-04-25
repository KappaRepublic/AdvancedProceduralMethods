////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass(SoundClass* s)
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;

	m_TerrainShader = 0;
	lightShader = 0;
	pLightShader = 0;
	textureShader = 0;
	transShader = 0;
	fogShader = 0;

	m_Light = 0;

	player = 0;
	npc = 0;

	m_RenderTexture = 0;
	m_DebugWindow = 0;

	m_mouse = 0;

	testInputOnce = false;
	inputCameraChange = false;
	overheadCam = false;
	inventoryActive = false;
	equipmentActive = false;
	testWindow = false;

	grassTile = 0;
	waterTile = 0;
	dungeonWallTile = 0;
	dungeonFloorTile = 0;
	beachTile = 0;

	treeModel = 0;
	chestModel = 0;
	crystalModel = 0;

	sound = s;

	outsideMusic = true;
	insideMusic = false;

	skyPlane = 0;
	skyShader = 0;

}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	srand(time(NULL));

	// caveGen.build();
	// caveGen.connectCaves();

	

	/*
	Object* tempObject = new Object();
	tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/chestTexture.png", NULL,
		roomVector.back()->getCenterX(), 0.0f, roomVector.back()->getCenterY(), 0.0f, 0.0f, 0.0f);
	levelObjects.push_back(tempObject);
	*/

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 6.0f;
	cameraY = 18.0f;
	cameraZ = -2.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	// result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/heightmap01.bmp");
	result = m_Terrain->InitializeTerrain(m_Direct3D->GetDevice(), 128,128);   //initialise the flat terrain.
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	lightShader = new LightShaderClass;
	if (!lightShader) {
		return false;
	}

	result = lightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	pLightShader = new PointLightShaderClass;
	if (!pLightShader) {
		return false;
	}

	result = pLightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the point light shader object.", L"Error", MB_OK);
		return false;
	}

	textureShader = new TextureShaderClass;
	if (!textureShader) {
		return false;
	}

	result = textureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	transShader = new TransparentShaderClass;
	if (!transShader) {
		return false;
	}

	result = transShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	fogShader = new FogShaderClass;
	if (!fogShader) {
		return false;
	}

	result = fogShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize fog shader", L"Error", MB_OK);
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	
	// m_Light->SetPosition(6.0f, 3.0f, 6.0f);
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(0.9f, 0.7f, 0.4f, 1.0f);
	m_Light->SetDirection(0.5f, -1.0f, 0.5f);

	// Create the sky and shader
	skyPlane = new SkyPlaneClass;
	skyPlane->Initialize(m_Direct3D->GetDevice(), L"../Engine/data/cloud001.dds", L"../Engine/data/perturb001.dds");

	skyShader = new SkyPlaneShaderClass;
	skyShader->Initialize(m_Direct3D->GetDevice(), hwnd);


	player = new Player;
	if (!player) {
		return false;
	}

	result = player->initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/playerIcon.png", L"../Engine/data/player_status_char_2.png", 6.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the example cube object.", L"Error", MB_OK);
		return false;
	}

	npc = new ModelClass;
	
	result = npc->Initialize(m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/chestTexture.png", NULL, NULL);


	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			levelMap[i][j] = false;
		}
	}

	// Create pre made tiles
	grassTile = new ModelClass();
	grassTile->Initialize(m_Direct3D->GetDevice(), "../Engine/data/floorOutside.txt", L"../Engine/data/grass.jpg", L"../Engine/data/minimapGrass.png", NULL);

	waterTile = new ModelClass;
	waterTile->Initialize(m_Direct3D->GetDevice(), "../Engine/data/floorOutside.txt", L"../Engine/data/water.jpg", L"../Engine/data/minimapWall.png", NULL);

	dungeonWallTile = new ModelClass;
	dungeonWallTile->Initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/fortressWall.jpg", L"../Engine/data/minimapWall.png", NULL);

	dungeonFloorTile = new ModelClass;
	dungeonFloorTile->Initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/stoneFloor.jpg", L"../Engine/data/minimapFloor.png", NULL);

	beachTile = new ModelClass;
	beachTile->Initialize(m_Direct3D->GetDevice(), "../Engine/data/floorOutside.txt", L"../Engine/data/beach.jpg", L"../Engine/data/minimapGrass.png", NULL);

	// Create pre made models
	treeModel = new ModelClass;
	treeModel->Initialize(m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/tree.png", L"../Engine/data/tree.png", NULL);

	chestModel = new ModelClass;
	chestModel->Initialize(m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/chestTexture.png", L"../Engine/data/chestTexture.png", NULL);

	crystalModel = new ModelClass;
	crystalModel->Initialize(m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/divCrystal.png", L"../Engine/data/divCrystal.png", NULL);


	// Fill in map with blank space
	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT; y++) {
			levelLayout[x][y].initialize(m_Direct3D->GetDevice(), waterTile, LevelCell::CellType::water);
			levelLayout[x][y].wall = true;
		}
	}

	airship.init(ObjectType::tree, m_Direct3D->GetDevice(), "../Engine/data/airship.txt", L"../Engine/data/3f6e8e00.png", L"../Engine/data/airship.png",
		7.0f, 0, 6.0f, 0.0f, 0.0f, 0.0f);

	// Generate procedural dungeon
	// generateRooms();
	generateCave();
	generateDungeon();
	processMap();
	
	
	generateChests();
	generateTrees();
	spawnCrystal();
	setPlayerSpawn();
	
	


	// Give the player some random items for funsies
	for (int i = 0; i < 3; i++) {
		Item *tempItem = new Item();
		tempItem->initialize();

		inventory.push_back(tempItem);
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the debug window object.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object.
	result = m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	/*
	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
	{
		// do stuff
	}
	*/

	testWindow = false;


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX11_Init(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	io.DisplaySize.x = 1280.0f;
	io.DisplaySize.y = 720.0f;

	unsigned char* pixels;
	int width, height;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);


	ImGui::StyleColorsClassic();

	my_tool_active = false;

	return true;
}

void ApplicationClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (skyPlane) {
		skyPlane->Shutdown();
		delete skyPlane;
		skyPlane = 0;
	}

	if (skyShader) {
		skyShader->Shutdown();
		delete skyShader;
		skyShader = 0;
	}

	// Release the debug window object.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if (lightShader) {
		lightShader->Shutdown();
		delete lightShader;
		lightShader = 0;
	}

	if (pLightShader) {
		pLightShader->Shutdown();
		delete pLightShader;
		pLightShader = 0;
	}

	if (textureShader) {
		textureShader->Shutdown();
		delete textureShader;
		textureShader = 0;
	}

	if (transShader) {
		transShader->Shutdown();
		delete transShader;
		transShader = 0;
	}

	if (fogShader) {
		fogShader->Shutdown();
		delete fogShader;
		fogShader = 0;
	}

	if (player) {
		player->shutDown();
		delete player;
		player = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	if (grassTile) {
		grassTile->Shutdown();
		delete grassTile;
		grassTile = 0;
	}

	if (waterTile) {
		waterTile->Shutdown();
		delete waterTile;
		waterTile = 0;
	}

	if (dungeonWallTile) {
		dungeonWallTile->Shutdown();
		delete dungeonWallTile;
		dungeonWallTile = 0;
	}

	if (dungeonFloorTile) {
		dungeonFloorTile->Shutdown();
		delete dungeonFloorTile;
		dungeonFloorTile = 0;
	}

	if (beachTile) {
		beachTile->Shutdown();
		delete beachTile;
		beachTile = 0;
	}

	if (treeModel) {
		treeModel->Shutdown();
		delete treeModel;
		treeModel = 0;
	}

	if (chestModel) {
		chestModel->Shutdown();
		delete chestModel;
		chestModel = 0;
	}

	if (crystalModel) {
		crystalModel->Shutdown();
		delete crystalModel;
		crystalModel = 0;
	}

	// Clear lists and vectors
	levelObjects.clear();
	roomVector.clear();
	inventory.clear();

	return;
}

bool ApplicationClass::Frame()
{
	bool result;

	ImGuiIO& io = ImGui::GetIO();
	// Call a new frame for ImGui
	ImGui_ImplDX11_NewFrame();

	mHandler.callMainMenu(true, inventoryActive, equipmentActive);
	mHandler.callInventory(inventoryActive, *player, inventory);
	mHandler.callEquipment(equipmentActive, *player);
	mHandler.callPlayerActions(true, *player, levelObjects, inventory);
	mHandler.callUpdateLog(true);
	mHandler.callPlayerStatus(true, *player);


	ImGui::Begin("Minimap", &testWindow, ImVec2(196, 196), ImGuiWindowFlags_NoResize);
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::Image(m_RenderTexture->GetShaderResourceView(), ImVec2(284, 160));
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		float focus_sz = 32.0f;
		float focus_x = io.MousePos.x - pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > 284 - focus_sz) focus_x = 284 - focus_sz;
		float focus_y = io.MousePos.y - pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > 160 - focus_sz) focus_y = 160 - focus_sz;
		ImGui::Text("Min: (%.2f, %.2f)", focus_x, focus_y);
		ImGui::Text("Max: (%.2f, %.2f)", focus_x + focus_sz, focus_y + focus_sz);
		ImVec2 uv0 = ImVec2((focus_x) / 284, (focus_y) / 160);
		ImVec2 uv1 = ImVec2((focus_x + focus_sz) / 284, (focus_y + focus_sz) / 160);
		ImGui::Image(m_RenderTexture->GetShaderResourceView(), ImVec2(128, 128), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::EndTooltip();
	}
	ImGui::End();

	ImGui::Begin("Generate Dungeon", &testWindow);
	if (ImGui::TreeNode("Basic Level Generation"))
	{
		ImGui::InputInt("Level Width", &levelWidth);
		ImGui::InputInt("Level Height", &levelHeight);
		ImGui::InputInt("No. of Room Attempts", &noOfAttempts);
		ImGui::InputInt("Max Room Width", &maxRoomWidth);
		ImGui::InputInt("Max Room Height", &maxRoomHeight);
		ImGui::InputInt("Min Room Width", &minRoomWidth);
		ImGui::InputInt("Min Room Height", &minRoomHeight);

		if (ImGui::Button("Generate Level")) {
		}

		ImGui::TreePop();
	}
	ImGui::End();

	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if(!result)
	{
		return false;
	}

	if (player->equippedWeapon != -1) {
		m_Text->updateTextWithItem(1, *inventory.at(player->equippedWeapon), m_Direct3D->GetDeviceContext());
	}
	// m_Text->updateTextWithItem(2, *inventory.at(1), m_Direct3D->GetDeviceContext());

	skyPlane->Frame();

	// Music?
	if (!insideMusic) {
		if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z].type == LevelCell::CellType::dFloor) {
			sound->StopWaveFile();

			sound->loadNew("../Engine/data/dungeon.wav");

			insideMusic = true;
			outsideMusic = false;

			sound->PlayWaveFile();
		}
	}

	if (!outsideMusic) {
		if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z].type != LevelCell::CellType::dFloor) {
			sound->StopWaveFile();

			sound->loadNew("../Engine/data/world.wav");

			insideMusic = false;
			outsideMusic = true;

			sound->PlayWaveFile();
		}
	}

	


	return result;
}

void ApplicationClass::generateRooms()
{
	int randX, randY, randWidth, randHeight;
	D3DXVECTOR2 newCenter;
	newCenter.x = 0;
	newCenter.y = 0;

	levelObjects.clear();
	roomVector.clear();
	// Initialise the level layout
	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			levelLayout[i][j].initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/wall.jpg", L"../Engine/data/minimapWall.png");
			levelLayout[i][j].wall = true;
		}
	}

	// Generate a room with '''random''' values
	randX = rand() % 25;
	randY = rand() % 25;
	randWidth = rand() % 10 + 1;
	if (randWidth < 3) {
		randWidth = 3;
	}
	randHeight = rand() % 10 + 1;
	if (randHeight < 3) {
		randHeight = 3;
	}

	Room* tempRoom = new Room;
	tempRoom->initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/woodenFloor.jpg", randX, randY, randWidth, randHeight);

	roomVector.push_back(tempRoom);

	// Loop for amount of rooms to create
	for (int i = 0; i < NUM_OF_ROOMS - 1; i++) {
		// Generate a room with '''random''' values
		randX = rand() % 25;
		randY = rand() % 25;
		randWidth = rand() % 10 + 1;
		if (randWidth < 3) {
			randWidth = 3;
		}
		randHeight = rand() % 10 + 1;
		if (randHeight < 3) {
			randHeight = 3;
		}

		Room* tempRoom = new Room;
		tempRoom->initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/woodenFloor.jpg", randX, randY, randWidth, randHeight);

		// Check if the room collides with any existing room

		bool failed = false;

		for (Room* const& i : roomVector) {
			if (tempRoom->intersects(*i)) {
				// Break out of the loop
				failed = true;
				break;
			}
			else {
				failed = false;
			}
		}

		// If the room generation succeeded 
		if (!failed) {
			// Generate the corridor between new room and last one
			newCenter.x = tempRoom->getCenterX();
			newCenter.y = tempRoom->getCenterY();

			if (roomVector.size() != 0) 
			{
				D3DXVECTOR2 prevCenter;
				prevCenter.x = roomVector.back()->getCenterX();
				prevCenter.y = roomVector.back()->getCenterY();

				int randDirection = rand() % 2;

				if (randDirection == 0) {
					createHCorridor((int)prevCenter.x, (int)newCenter.x, (int)prevCenter.y);
					createVCorridor((int)prevCenter.y, (int)newCenter.y, (int)newCenter.x);
				}
				else {
					createVCorridor((int)prevCenter.y, (int)newCenter.y, (int)prevCenter.x);
					createHCorridor((int)prevCenter.x, (int)newCenter.x, (int)newCenter.y);
				}
			}

			roomVector.push_back(tempRoom);

			// For testing, spawn a random object in the room

			Object* tempObject = new Object();

			
			if (rand() % 2 == 0) {
				tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/chestTexture.png", L"../Engine/data/chestTexture.png",
					roomVector.back()->getCenterX(), 0.0f, roomVector.back()->getCenterY(), 0.0f, 0.0f, 0.0f);
			}
			else {
				tempObject->init(ObjectType::npc, m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/npc.png", L"../Engine/data/npc.png",
					roomVector.back()->getCenterX(), 0.0f, roomVector.back()->getCenterY(), 0.0f, 0.0f, 0.0f);
			}

			levelObjects.push_back(tempObject);
			
			/*
			delete tempObject;
			tempObject = 0;
			*/
			
			

		}
		
		/*
		delete tempRoom;
		tempRoom = 0;
		*/
	}

	adjustLevel();
	// clearUnusedCells();
}

void ApplicationClass::generateCave()
{
	levelObjects.clear();
	roomVector.clear();

	autoGen.generateMap();

	for (int x = 0; x < autoGen.width; x++) {
		for (int y = 0; y < autoGen.height; y++) {
			if (autoGen.getMap()[x][y] == 1) {
				levelLayout[x][y].initialize(m_Direct3D->GetDevice(), waterTile, LevelCell::CellType::water);
				levelLayout[x][y].wall = true;
			}
			else if (autoGen.getMap()[x][y] == 0) {
				levelLayout[x][y].initialize(m_Direct3D->GetDevice(), grassTile, LevelCell::CellType::grass);
				levelLayout[x][y].wall = false;
			}
			else if (autoGen.getMap()[x][y] == 2) {
				levelLayout[x][y].initialize(m_Direct3D->GetDevice(), "../Engine/data/floorOutside.txt", L"../Engine/data/woodenFloor.jpg", L"../Engine/data/minimapFloor.png");
				levelLayout[x][y].wall = false;
			}


			// levelLayout[x][y].wall = autoGen.getMap()[x][y];

		}
	}

	
}

void ApplicationClass::generateTrees() {
	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT; y++) {

			if (levelLayout[x][y].type == LevelCell::CellType::grass) {
				if ((rand() % 100 + 1) < treeChance) {


					Object* tempObject = new Object();


					tempObject->init(ObjectType::tree, m_Direct3D->GetDevice(), treeModel,
						x, 0, y, 0.0f, 0.0f, 0.0f);

					levelObjects.push_back(tempObject);


				}
			}
		}
	}
}


// Generate chests in certain positions aroubd the map
// The chances for certain tiles spawning chests is as follows

// Outside Dungeon:

// Inside Dungeon:
// No walls beside tile ~1%
// 1 Wall beside tile ~3%
// 2 Walls beside tile ~6%
// 3 Walls beside tile (dead end) ~100%

void ApplicationClass::generateChests()
{
	int randValue = 0;
	Object* tempObject = new Object();

	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT; y++) {
			// If the checked tile is dungeon floor
			if (levelLayout[x][y].type == LevelCell::CellType::dFloor) {
				switch (getNeighbouringWalls(x, y)) {
				case 0:
					randValue = rand() % 100;
					if (randValue == 0) {
						// Generate a new chest to place at current position
						
						tempObject = new Object();

						tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), chestModel,
							x, 0, y, 0.0f, 0.0f, 0.0f);

						levelObjects.push_back(tempObject);
					}
					break;
				case 1:
					randValue = rand() % 33;
					if (randValue == 0) {
						// Generate a new chest to place at current position

						tempObject = new Object();

						tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), chestModel,
							x, 0, y, 0.0f, 0.0f, 0.0f);

						levelObjects.push_back(tempObject);
					}
					break;
				case 2:
					randValue = rand() % 17;
					if (randValue == 0) {
						// Generate a new chest to place at current position

						tempObject = new Object();

						tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), chestModel,
							x, 0, y, 0.0f, 0.0f, 0.0f);

						levelObjects.push_back(tempObject);
					}
					break;
				case 3:
					// Generate a new chest to place at current position

					tempObject = new Object();

					tempObject->init(ObjectType::chest, m_Direct3D->GetDevice(), chestModel,
						x, 0, y, 0.0f, 0.0f, 0.0f);

					levelObjects.push_back(tempObject);
					break;
				default:
					break;
				}
			}
		}
	}

	delete tempObject;
	tempObject = 0;
}

void ApplicationClass::spawnCrystal()
{
	int randChance = 0;

	// Loop until a spawn point is established
	while (true) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			for (int y = 0; y < LEVEL_HEIGHT; y++) {
				if (levelLayout[x][y].type != LevelCell::CellType::dFloor) {
					// If the checked cell is not grass, move onto the next loop
					continue;
				}
				else {
					randChance = rand() % 100;
					if (randChance == 0) {
						// tempObject = new Object();

						crystal.init(ObjectType::crystal, m_Direct3D->GetDevice(), crystalModel,
							x, 0, y, 0.0f, 0.0f, 0.0f);

						return;
					}

				}
			}
		}
	}
}


void ApplicationClass::adjustLevel()
{
	for (Room* const& i : roomVector) {
		for (int y = i->getY1(); y < i->getY2(); y++) {
			for (int x = i->getX1(); x < i->getX2(); x++) {
				levelLayout[x+1][y+1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/woodenFloor.jpg", L"../Engine/data/minimapFloor.png");
				levelLayout[x+1][y+1].wall = false;
			}	
		}	
	}

	player->setPosition(roomVector.front()->getCenterX(), 0.0f, roomVector.front()->getCenterY());
}

void ApplicationClass::createHCorridor(int x1, int x2, int y) {
	for (int x = min(x1, x2); x <= max(x1, x2); x++) {
		levelLayout[x + 1][y + 1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/woodenFloor.jpg", L"../Engine/data/minimapFloor.png");
		levelLayout[x + 1][y + 1].wall = false;
	}
}

void ApplicationClass::createVCorridor(int y1, int y2, int x) {
	for (int y = min(y1, y2); y <= max(y1, y2); y++) {
		levelLayout[x + 1][y + 1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/woodenFloor.jpg", L"../Engine/data/minimapFloor.png");
		levelLayout[x + 1][y + 1].wall = false;
	}
}

void ApplicationClass::clearUnusedCells() {
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelLayout[x + 1][y].wall &&
				levelLayout[x - 1][y].wall &&
				levelLayout[x][y + 1].wall &&
				levelLayout[x][y - 1].wall) {
				levelLayout[x][y].empty = true;
			}
		}
	}
}

void ApplicationClass::generateDungeon()
{
	dungeon.generateDungeon();

	for (int x = 0; x < dungeon.width + dungeon.widthAdjust; x++) {
		for (int y = 0; y < dungeon.height + dungeon.heightAdjust; y++) {
			if (dungeon.map[x][y] == 1) {
				levelLayout[x + dungeonPositionX][y + dungeonPositionY].initialize(m_Direct3D->GetDevice(), dungeonWallTile, LevelCell::CellType::dWall);
				levelLayout[x + dungeonPositionX][y + dungeonPositionY].wall = true;
			}
			else if (dungeon.map[x][y] == 0) {
				levelLayout[x + dungeonPositionX][y + dungeonPositionY].initialize(m_Direct3D->GetDevice(), dungeonFloorTile, LevelCell::CellType::dFloor);
				levelLayout[x + dungeonPositionX][y + dungeonPositionY].wall = false;
			}
		}
	}

}

// After the map has been generated, loop through once more to adjust tiles based on rules.
void ApplicationClass::processMap()
{
	int entrancesToMake = 3;
	int entrancesMade = 0;

	do {
		// Create a suitable entrance to the dungeon
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			for (int y = 0; y < LEVEL_HEIGHT; y++) {
				if (levelLayout[x][y].type == LevelCell::CellType::dWall) {
					if (checkValidEntrance(x, y)) {
						// Place entrance based on a random chance
						int randChance = rand() % 100;
						if (randChance == 0) {
							// Create an entry point to the dungeon
							levelLayout[x][y].initialize(m_Direct3D->GetDevice(), dungeonFloorTile, LevelCell::CellType::dFloor);
							levelLayout[x][y].wall = false;
							// entrance is created
							entrancesMade += 1;
							// End checks for entrance
							// return;
						}
					}
				}
			}
		}
	} while (entrancesMade < 3);

	// Process beach tiles
	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT; y++) {
			// Check if tile is grass
			if (levelLayout[x][y].type == LevelCell::CellType::grass) {
				if (checkIfBeach(x, y)) {
					levelLayout[x][y].initialize(m_Direct3D->GetDevice(), beachTile, LevelCell::CellType::beach);
					levelLayout[x][y].wall = false;
				}
			}
		}
	}
}

// Returns true if the checked tile is a wall, with a dungeon floor on one side and grass on the other
bool ApplicationClass::checkValidEntrance(int x, int y)
{
	bool floorFound = false;
	bool grassFound = false;

	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			// Don't check cells outside of range
			if (neighbourX < 0 || neighbourX > LEVEL_WIDTH || neighbourY < 0 || neighbourY > LEVEL_HEIGHT) {
				// Continue to next cell to be checked
				continue;
			}
			else {
				if (neighbourX == x || neighbourY == y) {
					if (levelLayout[neighbourX][neighbourY].type == LevelCell::CellType::grass) {
						grassFound = true;
					}
					else if (levelLayout[neighbourX][neighbourY].type == LevelCell::CellType::dFloor) {
						floorFound = true;
					}
				}
			}
		}
	}

	return grassFound && floorFound;
}

// Returns true if the checked tile is next to a water tile
bool ApplicationClass::checkIfBeach(int x, int y)
{
	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			if (neighbourX < 0 || neighbourX > LEVEL_WIDTH || neighbourY < 0 || neighbourY > LEVEL_HEIGHT) {
				// Continue to next cell to be checked
				continue;
			}
			else {
				if (levelLayout[neighbourX][neighbourY].type == LevelCell::CellType::water) {
					return true;
				}
			}
		}
	}
	return false;
}

int ApplicationClass::getNeighbouringWalls(int x, int y)
{
	int neighbouringWalls = 0;

	for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
		for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
			if (neighbourX < 0 || neighbourX > LEVEL_WIDTH || neighbourY < 0 || neighbourY > LEVEL_HEIGHT) {
				// Continue to next cell to be checked
				continue;
			}
			else {
				if (neighbourX == x || neighbourY == y) {
					if (levelLayout[neighbourX][neighbourY].type == LevelCell::CellType::dWall) {
						neighbouringWalls += 1;
					}
				}
			}
		}
	}

	return neighbouringWalls;
}

// Spawns the player on a valid tile
void ApplicationClass::setPlayerSpawn()
{
	int randChance = 0;

	// Loop until a spawn point is established
	while (true) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			for (int y = 0; y < LEVEL_HEIGHT; y++) {
				if (levelLayout[x][y].type != LevelCell::CellType::grass) {
					// If the checked cell is not grass, move onto the next loop
					continue;
				}
				else {
					randChance = rand() % 100;
					if (randChance == 0) {
						player->setPosition((float)x, 0.0f, (float)y);
						return;
					}
					
				}
			}
		}
	}
}

// Process any of the player's inputs.
bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);
	player->setFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsSpacePressed();
	m_Terrain->GenerateHeightMap(m_Direct3D->GetDevice(), keyDown);	
	// m_Terrain->MidpointDisplacementHeightMap(m_Direct3D->GetDevice(), keyDown);

	keyDown = m_Input->IsLeftPressed();
	m_Position->MoveLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->MoveRight(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->isAPressed();
	player->turnLeft(keyDown);

	keyDown = m_Input->isWPressed();
	player->moveForward(keyDown, checkForCollision(player->getDirection(), true));
	
	keyDown = m_Input->isSPressed();
	player->moveBackward(keyDown, checkForCollision(player->getDirection(), false));

	keyDown = m_Input->isDPressed();
	player->turnRight(keyDown);

	keyDown = m_Input->isQPressed();
	if (keyDown) {
		if (!inputCameraChange) {
			player->inflictDamageToHP(20);
			overheadCam = !overheadCam;
			inputCameraChange = true;
		}
	}
	else {
		if (inputCameraChange) {
			inputCameraChange = false;
		}
	}

	keyDown = m_Input->isEPressed();
	if (keyDown) {
		if (!testInputOnce) {
			inventory.at(0)->initialize();
			inventory.at(1)->initialize();
			inventory.at(2)->initialize();
			testInputOnce = true;
		}
	}
	else {
		if (testInputOnce) {
			testInputOnce = false;
		}
	}

	keyDown = m_Input->IsZPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	if (m_Input->IsSpacePressed()) {
		generateRooms();
	}

	m_Light->SetPosition(player->getPosition().x, 1.0f, player->getPosition().z);


	// m_Camera->SetPosition(player->getPosition().x, -8.0f, player->getPosition().z);
	// m_Position->SetPosition(player->getPosition().x, 14.0f, player->getPosition().z - 5.5f);

	if (overheadCam) {

		// OVERHEAD CAM
		m_Position->SetPosition(player->getPosition().x, 20.0f, player->getPosition().z);
		m_Position->SetRotation(90.0f, player->getRotation().y, 0.0f);
	}
	else {
		// FIRST PERSON CAM
		m_Position->SetPosition(player->getPosition().x, 0.0f, player->getPosition().z);
		m_Position->SetRotation(0.0f, player->getRotation().y, 0.0f);

		
	}

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	// m_Camera->SetRotation(rotX + 70.0f, rotY, rotZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix, scaleMatrix;
	D3DXVECTOR3 cameraPos, modelPos;
	double angle;
	float rotation;
	bool result;
	float fogColour, fogStart, fogEnd;

	// Set fog colour to gray
	fogColour = 0.2f;
	// Set the start and end of the fog
	fogStart = -2.0f;
	fogEnd = 7.0f;

	D3DXVECTOR3 cPos, cRot;

	cPos = m_Camera->GetPosition();
	cRot = m_Camera->GetRotation();

	// Adjust camera for minimap
	m_Camera->SetPosition(player->getPosition().x, 10.0f, player->getPosition().z);
	m_Camera->SetRotation(90.0f, 0.0f, 0.0f);

	// Render mini map
	RenderToTexture();

	// Re adjust camera
	if (overheadCam) {
		// OVERHEAD CAM
		m_Camera->SetPosition(player->getPosition().x, 20.0f, player->getPosition().z);
		m_Camera->SetRotation(90.0f, player->getRotation().y, 0.0f);
	}
	else {
		// FIRST PERSON CAM
		m_Camera->SetPosition(player->getPosition().x, 0.0f, player->getPosition().z);
		m_Camera->SetRotation(0.0f, player->getRotation().y, 0.0f);
	} 

	/*
	m_Camera->SetPosition(cPos.x, cPos.y, cPos.z);
	m_Camera->SetRotation(cRot.x, cRot.y, cRot.z);
	*/

	// Generate the view matrix based on the camera's position.

	// Clear the scene.
	m_Direct3D->BeginScene(0.52f, 0.8f, 0.92f, 1.0f);

	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);


	D3DXMatrixTranslation(&worldMatrix, player->getPosition().x, 0.0f, player->getPosition().z);

	m_Direct3D->TurnZBufferOff();

	// Enable additive blending so the clouds blend with the sky dome color
	m_Direct3D->EnableSecondBlendState();
	// Render sky plane
	skyPlane->Render(m_Direct3D->GetDeviceContext());
	skyShader->Render(m_Direct3D->GetDeviceContext(), skyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		skyPlane->GetCloudTexture(), skyPlane->GetPerturbTexture(), skyPlane->GetTranslation(), skyPlane->GetScale(),
		skyPlane->GetBrightness());


	m_Direct3D->TurnZBufferOn();

	// Render the terrain buffers.
	/*
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									 m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection());
	if(!result)

	{
		return false;
	}
	*/

	m_Direct3D->TurnOffAlphaBlending();

	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			// Translate to the walls position
			D3DXMatrixTranslation(&worldMatrix, (float)i, 0.0f, (float)j);

			levelLayout[i][j].getMesh()->Render(m_Direct3D->GetDeviceContext());

			if (overheadCam) {
				textureShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
					levelLayout[i][j].getMesh()->getTexture1());
			}
			else {

				// int plX = player->

				if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z].type == LevelCell::CellType::dFloor) {
					fogShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
						levelLayout[i][j].getMesh()->getTexture1(), fogStart, fogEnd);
				}
				else {
					textureShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
						levelLayout[i][j].getMesh()->getTexture1());
				}


				
			}
			/*
			pLightShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				levelLayout[i][j].getMesh()->getTexture1(), &m_Light->GetDiffuseColor(), &m_Light->GetPosition());
			*/
		}
	}

	rotation = (float)player->getRotation().y * 0.0174532925f;

	D3DXMatrixRotationY(&worldMatrix, rotation);
	D3DXMatrixTranslation(&translateMatrix, player->getPosition().x, 0.0f, player->getPosition().z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	m_Direct3D->TurnOnAlphaBlending();

	player->getMesh()->Render(m_Direct3D->GetDeviceContext());
	result = textureShader->Render(m_Direct3D->GetDeviceContext(), player->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		player->getMesh()->getTexture1());

	if (!result) {
		return false;
	}

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render all objects in the level

	D3DXMatrixRotationX(&worldMatrix, 0.0f);

	cameraPos = m_Camera->GetPosition();
	
	if (levelObjects.size() > 0)
	{
		for (int i = 0; i < levelObjects.size(); i++) {
			modelPos.x = levelObjects.at(i)->getPosition().x;
			modelPos.y = 0.0f;
			modelPos.z = levelObjects.at(i)->getPosition().z;

			angle = atan2(modelPos.x - cameraPos.x, modelPos.z - cameraPos.z) * (180.0 / D3DX_PI);

			rotation = (float)angle * 0.0174532925f;

			// Setup the rotation the billboard at the origin using the world matrix.
			D3DXMatrixRotationY(&worldMatrix, rotation);

			// Setup the translation matrix from the billboard model.
			D3DXMatrixTranslation(&translateMatrix, modelPos.x, modelPos.y, modelPos.z);

			// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

			levelObjects.at(i)->getMesh()->Render(m_Direct3D->GetDeviceContext());
			if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z].type == LevelCell::CellType::dFloor) {
				fogShader->Render(m_Direct3D->GetDeviceContext(), levelObjects.at(i)->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, levelObjects.at(i)->getMesh()->getTexture1(), fogStart, fogEnd);

			}
			else {
				textureShader->Render(m_Direct3D->GetDeviceContext(), levelObjects.at(i)->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, levelObjects.at(i)->getMesh()->getTexture1());

			}
			
		}
	}

	modelPos.x = crystal.getPosition().x;
	modelPos.y = 0.0f;
	modelPos.z = crystal.getPosition().z;

	angle = atan2(modelPos.x - cameraPos.x, modelPos.z - cameraPos.z) * (180.0 / D3DX_PI);

	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPos.x, modelPos.y, modelPos.z);

	crystal.getMesh()->Render(m_Direct3D->GetDeviceContext());

	if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z].type == LevelCell::CellType::dFloor) {
		fogShader->Render(m_Direct3D->GetDeviceContext(), crystal.getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, crystal.getMesh()->getTexture1(), fogStart, fogEnd);

	}
	else {
		textureShader->Render(m_Direct3D->GetDeviceContext(), crystal.getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, crystal.getMesh()->getTexture1());

	}



	D3DXMatrixRotationX(&worldMatrix, 0.0f);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
		
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	/*
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}
	*/

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	m_Direct3D->TurnZBufferOff();

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	

	return true;
}

bool ApplicationClass::checkForCollision(int direction, bool movingForward)
{
	// 0 is up, 1 is right, 2 is down, 3 is left
	switch (direction) {
	case 0:
		if (movingForward) {
			if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z + 1].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z + 1 == true) {
						return true;
					}
				}
			}
		}
		else {
			if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z - 1].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z - 1 == true) {
						return true;
					}
				}
			}
		}
		
		break;
	case 1:
		if (movingForward) {
			if (levelLayout[(int)player->getPosition().x + 1][(int)player->getPosition().z].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x + 1 && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z == true) {
						return true;
					}
				}
			}
		}
		else {
			if (levelLayout[(int)player->getPosition().x - 1][(int)player->getPosition().z].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x - 1 && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z == true) {
						return true;
					}
				}
			}
		}
		break;
	case 2:
		if (movingForward) {
			if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z - 1].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z - 1 == true) {
						return true;
					}
				}
			}
		}
		else {
			if (levelLayout[(int)player->getPosition().x][(int)player->getPosition().z + 1].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z + 1 == true) {
						return true;
					}
				}
			}
		}
		break;
	case 3:
		if (movingForward) {
			if (levelLayout[(int)player->getPosition().x - 1][(int)player->getPosition().z].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x - 1 && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z == true) {
						return true;
					}
				}
			}
		}
		else {
			if (levelLayout[(int)player->getPosition().x + 1][(int)player->getPosition().z].wall == true) {
				return true;
			}
			else {
				for (int i = 0; i < levelObjects.size(); i++) {
					if (levelObjects.at(i)->getPosition().x == (int)player->getPosition().x + 1 && levelObjects.at(i)->getPosition().z == (int)player->getPosition().z == true) {
						return true;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return false;
}

bool ApplicationClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix, scaleMatrix;
	D3DXVECTOR3 cameraPos, modelPos;
	double angle;
	float rotation;
	bool result;

	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			// Translate to the walls position
			D3DXMatrixTranslation(&worldMatrix, (float)i, 0.0f, (float)j);

			levelLayout[i][j].getMesh()->Render(m_Direct3D->GetDeviceContext());
			textureShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				levelLayout[i][j].getMesh()->getTexture2() /*,  &m_Light->GetDiffuseColor(), &m_Light->GetPosition() */);
		}
	}

	// m_Direct3D->TurnOnAlphaBlending();

	rotation = (float)player->getRotation().y * 0.0174532925f;

	D3DXMatrixRotationY(&worldMatrix, rotation);
	D3DXMatrixTranslation(&translateMatrix, player->getPosition().x, 0.0f, player->getPosition().z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// m_Direct3D->TurnOnAlphaBlending();

	player->getMesh()->Render(m_Direct3D->GetDeviceContext());
	result = textureShader->Render(m_Direct3D->GetDeviceContext(), player->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		player->getMesh()->getTexture1());

	// m_Direct3D->TurnOffAlphaBlending();


	if (!result) {
		return false;
	}

	// Do some sorting
	// std::vector<Object*> tempList;

	if (levelObjects.size() > 0) {
		for (Object* o : levelObjects) {
			o->distanceFromPlayer = sqrt((float)pow((o->getPosition().x - player->getPosition().x), 2) + pow((o->getPosition().y - player->getPosition().y), 2));
		}
	}

	std::sort(levelObjects.begin(), levelObjects.end(), sortStruct());


	if (levelObjects.size() > 0)
	{
		for (int i = 0; i < levelObjects.size(); i++) {




			modelPos.x = levelObjects.at(i)->getPosition().x;
			modelPos.y = -0.5f;
			modelPos.z = levelObjects.at(i)->getPosition().z;

			// Setup the rotation the billboard at the origin using the world matrix.
			D3DXMatrixRotationX(&worldMatrix, 1.5708f);

			// Setup the translation matrix from the billboard model.
			D3DXMatrixTranslation(&translateMatrix, modelPos.x, modelPos.y, modelPos.z);

			// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

			levelObjects.at(i)->getMesh()->Render(m_Direct3D->GetDeviceContext());
			textureShader->Render(m_Direct3D->GetDeviceContext(), levelObjects.at(i)->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, levelObjects.at(i)->getMesh()->getTexture2());

		}
	}

	modelPos.x = crystal.getPosition().x;
	modelPos.y = -0.5f;
	modelPos.z = crystal.getPosition().z;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationX(&worldMatrix, 1.5708f);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPos.x, modelPos.y, modelPos.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	crystal.getMesh()->Render(m_Direct3D->GetDeviceContext());
	textureShader->Render(m_Direct3D->GetDeviceContext(), crystal.getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, crystal.getMesh()->getTexture2());


	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}