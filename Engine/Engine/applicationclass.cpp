////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
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

	m_Light = 0;

	player = 0;
	npc = 0;

	testInputOnce = false;
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

	// EXAMPLE CODE BEYOND THIS POINT
	player = new Player;
	if (!player) {
		return false;
	}

	result = player->initialize(m_Direct3D->GetDevice(), "../Engine/data/player.txt", L"../Engine/data/doom.png", 6.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the example cube object.", L"Error", MB_OK);
		return false;
	}

	npc = new ModelClass;
	
	result = npc->Initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/alpha.png", NULL, NULL);


	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			levelMap[i][j] = false;
		}
	}

	/*
	levelMap[0][0] = true;
	levelMap[0][1] = true;

	anArrayOfBools = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
					   0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0,
					   0, 0, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0,
					   0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0,
					   0, 0, 1, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0,
					   0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   0, 0, 1, 2, 1, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   0, 0, 1, 2, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 2, 2, 2, 2, 2, 1, 1, 2, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
					   1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };


	int tempCounter = 0;


	// Initialise the level layout
	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			
			levelLayout[i][j].wall = anArrayOfBools.at(tempCounter);
				tempCounter++;

			if (levelLayout[i][j].wall == 1) {
				
				levelLayout[i][j].initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/testTexture.png");
				// levelLayout[i][j].wall = true;
			}
			else {
				levelLayout[i][j].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/floorTexture.png");
				// levelLayout[i][j].wall = false;
			}

			
		}
	}
	*/

	

	// Generate procedural dungeon
	generateRooms();

	// Give the player some random items for funsies
	for (int i = 0; i < 3; i++) {
		Item *tempItem = new Item();
		tempItem->initialize();

		inventory.push_back(tempItem);
	}

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

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


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

	m_Text->updateTextWithItem(1, *inventory.at(0), m_Direct3D->GetDeviceContext());
	// m_Text->updateTextWithItem(2, *inventory.at(1), m_Direct3D->GetDeviceContext());


	return result;
}


void ApplicationClass::generateRooms()
{
	int randX, randY, randWidth, randHeight;
	D3DXVECTOR2 newCenter;
	newCenter.x = 0;
	newCenter.y = 0;

	roomVector.clear();
	// Initialise the level layout
	for (int j = 0; j < LEVEL_HEIGHT; j++) {
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			levelLayout[i][j].initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/testTexture.png");
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
	tempRoom->initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/testTexture.png", randX, randY, randWidth, randHeight);

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
		tempRoom->initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/testTexture.png", randX, randY, randWidth, randHeight);

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

		if (!failed) {
			// Generate the corridor between new room and last one
			newCenter.x = tempRoom->getCenterX();
			newCenter.y = tempRoom->getCenterY();

			if (roomVector.size() != 0) {
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

			
		}
		

		/*
		delete tempRoom;
		tempRoom = 0;
		*/
	}

	adjustLevel();
}

void ApplicationClass::adjustLevel()
{
	for (Room* const& i : roomVector) {
		for (int y = i->getY1(); y < i->getY2(); y++) {
			for (int x = i->getX1(); x < i->getX2(); x++) {
				levelLayout[x+1][y+1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/floorTexture.png");
				levelLayout[x+1][y+1].wall = false;
			}	
		}	
	}

	player->setPosition(roomVector.front()->getCenterX(), 0.0f, roomVector.front()->getCenterY());
}

void ApplicationClass::createHCorridor(int x1, int x2, int y)
{
	for (int x = min(x1, x2); x <= max(x1, x2); x++) {
		levelLayout[x + 1][y + 1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/floorTexture.png");
		levelLayout[x + 1][y + 1].wall = false;
	}
}

void ApplicationClass::createVCorridor(int y1, int y2, int x)
{
	for (int y = min(y1, y2); y <= max(y1, y2); y++) {
		levelLayout[x + 1][y + 1].initialize(m_Direct3D->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/floorTexture.png");
		levelLayout[x + 1][y + 1].wall = false;
	}
}

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
	player->moveForward(keyDown);

	keyDown = m_Input->isSPressed();
	player->moveBackward(keyDown);

	keyDown = m_Input->isDPressed();
	player->turnRight(keyDown);

	keyDown = m_Input->isQPressed();
	player->moveLeft(keyDown);

	keyDown = m_Input->isEPressed();
	if (keyDown) {
		if (!testInputOnce) {
			inventory.at(0)->initialize();
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

	m_Light->SetPosition(player->getPosition().x, 2.0f, player->getPosition().z);


	// m_Camera->SetPosition(player->getPosition().x, -8.0f, player->getPosition().z);
	// m_Position->SetPosition(player->getPosition().x, 14.0f, player->getPosition().z - 5.5f);


	// OVERHEAD CAM
	// m_Position->SetPosition(player->getPosition().x, 50.0f, player->getPosition().z);
	// m_Position->SetRotation(90.0f, player->getRotation().y, 0.0f);

	// FIRST PERSON CAM
	m_Position->SetPosition(player->getPosition().x, 0.0f, player->getPosition().z);
	m_Position->SetRotation(0.0f, player->getRotation().y, 0.0f);
	
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


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

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

	D3DXMatrixRotationX(&worldMatrix, 2.0f);
	D3DXMatrixTranslation(&translateMatrix, player->getPosition().x, 0.0f, player->getPosition().z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);
	
	player->getMesh()->Render(m_Direct3D->GetDeviceContext());
	result = lightShader->Render(m_Direct3D->GetDeviceContext(), player->getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), 0.0f, player->getMesh()->getTexture1());

	if (!result) {
		return false;
	}

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOffAlphaBlending();
	
	// Pre created level render
	/*
	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {

			if (!levelLayout[i][j].wall == 0)
			{

				// Translate to the walls position
				D3DXMatrixTranslation(&worldMatrix, (float)i, 0.0f, (float)j);

				levelLayout[i][j].getMesh()->Render(m_Direct3D->GetDeviceContext());
				
				//  lightShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				// 	m_Light->GetDirection(), m_Light->GetDiffuseColor(), 0.0f, levelLayout[i][j].getMesh()->getTexture1());
				
				pLightShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
					levelLayout[i][j].getMesh()->getTexture1(), &m_Light->GetDiffuseColor(), &m_Light->GetPosition());
			}
		}
	}
	*/

	// Render generated rooms

	// EVEN NUMBERS ARE POSITIONED INCORRECTLY

	/*
	for (auto const& i : roomVector) {
		D3DXMatrixTranslation(&worldMatrix, i->getCenterX(), -0.5f, i->getCenterY());
		D3DXMatrixScaling(&scaleMatrix, (float)i->getWidth(), 0.0f, (float)i->getHeight());
		D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);

		i->getFloorMesh()->Render(m_Direct3D->GetDeviceContext());
		textureShader->Render(m_Direct3D->GetDeviceContext(), i->getFloorMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, i->getFloorMesh()->getTexture1());
	}
	*/

	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			// Translate to the walls position
			D3DXMatrixTranslation(&worldMatrix, (float)i, 0.0f, (float)j);

			levelLayout[i][j].getMesh()->Render(m_Direct3D->GetDeviceContext());
			pLightShader->Render(m_Direct3D->GetDeviceContext(), levelLayout[i][j].getMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			levelLayout[i][j].getMesh()->getTexture1(), &m_Light->GetDiffuseColor(), &m_Light->GetPosition());
		}
	}

	/*
	for (std::list<Room*>::iterator it = roomVector.begin(); it != roomVector.end(); it++) {
		D3DXMatrixTranslation(&worldMatrix, it->getCenterX(), 0.0f, roomVector.at(i)->getCenterY());

		

		roomVector.(i)->getFloorMesh()->Render(m_Direct3D->GetDeviceContext());
		textureShader->Render(m_Direct3D->GetDeviceContext(), roomVector.at(i)->getFloorMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, roomVector.at(i)->getFloorMesh()->getTexture1());

	}
	*/

	/*
	for (int i = 0; i < roomVector.size(); i++) {
		
		D3DXMatrixTranslation(&worldMatrix, roomVector.at(i)->getCenterX(), 0.0f, roomVector.at(i)->getCenterY());

		roomVector.(i)->getFloorMesh()->Render(m_Direct3D->GetDeviceContext());
		textureShader->Render(m_Direct3D->GetDeviceContext(), roomVector.at(i)->getFloorMesh()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, roomVector.at(i)->getFloorMesh()->getTexture1());
		
	}
	*/

	cameraPos = m_Camera->GetPosition();

	modelPos.x = 8.0f;
	modelPos.y = 0.0f;
	modelPos.z = 8.0f;

	angle = atan2(modelPos.x - cameraPos.x, modelPos.z - cameraPos.z) * (180.0 / D3DX_PI);

	rotation = (float)angle * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPos.x, modelPos.y, modelPos.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();
	// D3DXMatrixTranslation(&worldMatrix, 8.0f, 0.0f, 8.5f);

	npc->Render(m_Direct3D->GetDeviceContext());
	textureShader->Render(m_Direct3D->GetDeviceContext(), npc->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, npc->getTexture1());

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	D3DXMatrixRotationX(&worldMatrix, 0.0f);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();
		
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();


	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}