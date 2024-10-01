#include "VisualDebugger.h"
#include <vector>
#include "Extras\Camera.h"
#include "Extras\Renderer.h"
#include "Extras\HUD.h"
#include "MyScene.h"
#include <time.h> // Include time.h for clock() function
#include <windows.h>

namespace VisualDebugger
{
	using namespace physx;

	enum RenderMode
	{
		DEBUG,
		NORMAL,
		BOTH
	};

	enum HUDState
	{
		EMPTY = 0,
		HELP = 1,
		PAUSE = 2
	};

	//function declarations
	void KeyHold();
	void KeySpecial(int key, int x, int y);
	void KeyRelease(unsigned char key, int x, int y);
	void KeyPress(unsigned char key, int x, int y);

	void motionCallback(int x, int y);
	void mouseCallback(int button, int state, int x, int y);
	void exitCallback(void);

	void RenderScene();
	void ToggleRenderMode();
	void HUDInit();

	///simulation objects
	Camera* camera;
	MyScene* scene;
	PxReal delta_time = 1.f/60.f;
	PxReal gForceStrength = 5;

	int score = 0;

	bool enableRimDisaster = false;
	bool increaseRimDisasterSpeed = false;
	float rimDisasterSpeed = 0.01;

	bool enableBouncyCourtInput = false;
	bool bouncyCourtEnabled = false;

	bool enableBouncyRimInput = false;
	bool bouncyRimEnabled = false;

	RenderMode render_mode = NORMAL;
	const int MAX_KEYS = 256;
	bool key_state[MAX_KEYS];
	bool hud_show = true;
	HUD hud;

	bool ballThrown;

	//Init the debugger
	void Init(const char *window_name, int width, int height)
	{
		///Init PhysX
		PhysicsEngine::PxInit();
		scene = new MyScene();
		scene->Init();

		///Init renderer
		Renderer::BackgroundColor(PxVec3(150.f/255.f,150.f/255.f,150.f/255.f));
		Renderer::SetRenderDetail(40);
		Renderer::InitWindow(window_name, width, height);
		Renderer::Init();

		camera = new Camera(PxVec3(0.0f, 5.0f, 20.0f), PxVec3(0.f,-.1f,-1.f), 5.f);

		//initialise HUD
		HUDInit();

		///Assign callbacks
		//render
		glutDisplayFunc(RenderScene);

		//keyboard
		glutKeyboardFunc(KeyPress);
		glutSpecialFunc(KeySpecial);
		glutKeyboardUpFunc(KeyRelease);

		//mouse
		glutMouseFunc(mouseCallback);
		glutMotionFunc(motionCallback);

		//exit
		atexit(exitCallback);

		//init motion callback
		motionCallback(0,0);
	}

	void HUDInit()
	{
		//initialise HUD
		//Help Screen
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, "");
		if (scene->currentScore < 10) // If score reaches 10 a congrats message is shown
		{
			hud.AddLine(HELP, "Score 10 to win!");
			hud.AddLine(HELP, "");
			hud.AddLine(HELP, "Score: " + std::to_string(score));
		}
		else {
			hud.AddLine(HELP, "You Scored 10! You Win but feel free to shoot more!");
			hud.AddLine(HELP, "");
			hud.AddLine(HELP, "Score: " + std::to_string(score));
		}
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, " Basketball Controls");
		hud.AddLine(HELP, "    R - Make Throwing Object Move (Press Again to Reset Throwing Object)");
		hud.AddLine(HELP, "    T - Reset The Ball Object");
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, " Disaster Toggle");
		hud.AddLine(HELP, "    Repeat the Button Input to Disable");
		hud.AddLine(HELP, "    1 - Move Rim ( Click '.' to speed it up)");
		hud.AddLine(HELP, "    2 - Make The Rim Bouncy");
		hud.AddLine(HELP, "    3 - Deflated Ball Bounciness");

		// Pause Screen
		hud.AddLine(PAUSE, " Simulation");
		hud.AddLine(PAUSE, "    F9 - select next actor");
		hud.AddLine(PAUSE, "    F10 - pause");
		hud.AddLine(PAUSE, "    F12 - reset");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, " Display");
		hud.AddLine(PAUSE, "    F5 - help on/off");
		hud.AddLine(PAUSE, "    F6 - shadows on/off");
		hud.AddLine(PAUSE, "    F7 - render mode");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, " Camera");
		hud.AddLine(PAUSE, "    W,S,A,D,Q,Z - forward,backward,left,right,up,down");
		hud.AddLine(PAUSE, "    mouse + click - change orientation");
		hud.AddLine(PAUSE, "    F8 - reset view");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, " Force (applied to the selected actor)");
		hud.AddLine(PAUSE, "    I,K,J,L,U,M - forward,backward,left,right,up,down");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "   Simulation paused. Press F10 to continue.");
		//set font size for all screens
		hud.FontSize(0.018f);
		//set font color for all screens
		hud.Color(PxVec3(0.f,0.f,0.f));

	}


	//Start the main loop
	void Start()
	{ 
		glutMainLoop(); 
	}

	//Render the scene and perform a single simulation step
	void RenderScene()
	{
		// Measure the start time of rendering
		clock_t renderStartTime = clock();

		HUDInit(); // Initialising every render to be able to update the score on the HUD
		score = scene->currentScore;

		// <<< Disaster Update >>>

		if (enableRimDisaster == true)
		{
			scene->MoveRimDisaster(true);

		}
		else
		{
			scene->MoveRimDisaster(false);
		}

		// <<<    Bouncy Rim Disaster   >>>

		if (enableBouncyRimInput == true && bouncyRimEnabled == false)
		{
			scene->BouncyRimDisaster();
			bouncyRimEnabled = true;
		}
		if (enableBouncyRimInput == false && bouncyRimEnabled == true)
		{
			scene->ResetRim();
			bouncyRimEnabled = false;
		}

		///   - - - - - - - - Bouncy Court Disaster - - - - - - - - \\\.

		if (enableBouncyCourtInput == true && bouncyCourtEnabled == false)
		{
			scene->DeflatedBallDisaster();
			bouncyCourtEnabled = true;
		}
		if (enableBouncyCourtInput == false && bouncyCourtEnabled == true)
		{
			scene->ResetBall();
			bouncyCourtEnabled = false;
		}

		///   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \\\.

		//handle pressed keys
		KeyHold();

		//start rendering
		Renderer::Start(camera->getEye(), camera->getDir());

		if ((render_mode == DEBUG) || (render_mode == BOTH))
		{
			Renderer::Render(scene->Get()->getRenderBuffer());
		}

		if ((render_mode == NORMAL) || (render_mode == BOTH))
		{
			std::vector<PxActor*> actors = scene->GetAllActors();
			if (actors.size())
				Renderer::Render(&actors[0], (PxU32)actors.size());
		}

		//adjust the HUD state
		if (hud_show)
		{
			if (scene->Pause())
				hud.ActiveScreen(PAUSE);
			else {
				hud.ActiveScreen(HELP);
			}
		}
		else {
			hud.ActiveScreen(EMPTY);
		}

		//render HUD
		hud.Render();

		//finish rendering
		Renderer::Finish();

		//perform a single simulation step
		scene->Update(delta_time);
		hud.~HUD(); // Deleting HUD as it will be replaced by a updated one next frame

		// Measure the end time of rendering
		clock_t renderEndTime = clock();

		// Calculate the duration of rendering in milliseconds
		clock_t renderDuration = (renderEndTime - renderStartTime) * 1000 / CLOCKS_PER_SEC;

		const int targetFPS = 120;
		const int targetFrameTime = 1000 / targetFPS; // Calculate target frame time in milliseconds
		int sleepTime = targetFrameTime - renderDuration;


		if (sleepTime > 0) {
			Sleep(sleepTime);
		}
}

	//user defined keyboard handlers
	void UserKeyPress(int key)
	{
		switch (toupper(key))
		{
		case 'R': // Reset Throwing Object
			if (ballThrown == true)
			{
				scene->GetSelectedActor()->setAngularVelocity(PE::PxVec3(0.0f, 0.0f, 0.0f));
				scene->GetSelectedActor()->setLinearVelocity(PE::PxVec3(0.0f, 0.0f, 0.0f));

				scene->GetSelectedActor()->setGlobalPose(PE::PxTransform(PE::PxVec3(-0.5f, 1.0f, 12.94f), PE::PxQuat(0.75f, PE::PxVec3(0.0f, 1.0f, 0.0f)) * PE::PxQuat(0.15f, PE::PxVec3(1.0f, 0.0f, 0.0f))));
				ballThrown = false;
			}
			else {
				scene->GetSelectedActor()->addForce(PxVec3(0, 100, -50) * 11);

				//scene->GetSelectedActor()->setGlobalPose(PE::PxTransform(PE::PxQuat(0.75f, PE::PxVec3(0.0f, 1.0f, 0.0f))));
				ballThrown = true;
			}

			break;
		case 'T': // Reset Ball to the throwing position
			scene->ResetSphere();
			break;

		case '1':
			if (enableRimDisaster == true)
			{
				enableRimDisaster = false;
			}
			else
			{
				enableRimDisaster = true;
			}
			break;
		case '.':
			if (increaseRimDisasterSpeed == true)
			{
				increaseRimDisasterSpeed = false;
				scene->IncreaseRimSpeed(increaseRimDisasterSpeed);
			}
			else
			{
				increaseRimDisasterSpeed = true;
				scene->IncreaseRimSpeed(increaseRimDisasterSpeed);
			}
			break;
		case '2':
			if (enableBouncyRimInput == true)
			{
				enableBouncyRimInput = false;
			}
			else
			{
				enableBouncyRimInput = true;
			}
			break;
		case '3':
			if (enableBouncyCourtInput == true)
			{
				enableBouncyCourtInput = false;
			}
			else
			{
				enableBouncyCourtInput = true;
			}
			break;
		default:
			break;
		}
	}

	void UserKeyRelease(int key)
	{
		switch (toupper(key))
		{
		
		}
	}

	void UserKeyHold(int key)
	{
	}

	//handle camera control keys
	void CameraInput(int key)
	{
		switch (toupper(key))
		{
		case 'W':
			camera->MoveForward(delta_time);
			break;
		case 'S':
			camera->MoveBackward(delta_time);
			break;
		case 'A':
			camera->MoveLeft(delta_time);
			break;
		case 'D':
			camera->MoveRight(delta_time);
			break;
		case 'Q':
			camera->MoveUp(delta_time);
			break;
		case 'Z':
			camera->MoveDown(delta_time);
			break;
		default:
			break;
		}
	}

	//handle force control keys
	void ForceInput(int key)
	{
		if (!scene->GetSelectedActor())
			return;

		switch (toupper(key))
		{
			// Force controls on the selected actor
		case 'I': //forward
			scene->GetSelectedActor()->addForce(PxVec3(0,0,-1)*gForceStrength);
			break;
		case 'K': //backward
			scene->GetSelectedActor()->addForce(PxVec3(0,0,1)*gForceStrength);
			break;
		case 'J': //left
			scene->GetSelectedActor()->addForce(PxVec3(-1,0,0)*gForceStrength);
			break;
		case 'L': //right
			scene->GetSelectedActor()->addForce(PxVec3(1,0,0)*gForceStrength);
			break;
		case 'U': //up
			scene->GetSelectedActor()->addForce(PxVec3(0,1,0)*gForceStrength);
			break;
		case 'M': //down
			scene->GetSelectedActor()->addForce(PxVec3(0,-1,0)*gForceStrength);
			break;
		default:
			break;
		}
	}

	///handle special keys
	void KeySpecial(int key, int x, int y)
	{
		//simulation control
		switch (key)
		{
			//display control
		case GLUT_KEY_F5:
			//hud on/off
			hud_show = !hud_show;
			break;
		case GLUT_KEY_F6:
			//shadows on/off
			Renderer::ShowShadows(!Renderer::ShowShadows());
			break;
		case GLUT_KEY_F7:
			//toggle render mode
			ToggleRenderMode();
			break;
		case GLUT_KEY_F8:
			//reset camera view
			camera->Reset();
			break;

			//simulation control
		case GLUT_KEY_F9:
			//select next actor
			scene->SelectNextActor();
			break;
		case GLUT_KEY_F10:
			//toggle scene pause
			scene->Pause(!scene->Pause());
			break;
		case GLUT_KEY_F12:
			//resect scene
			scene->Reset();
			break;
		default:
			break;
		}
	}

	//handle single key presses
	void KeyPress(unsigned char key, int x, int y)
	{
		//do it only once
		if (key_state[key] == true)
			return;

		key_state[key] = true;

		//exit
		if (key == 27)
			exit(0);

		UserKeyPress(key);
	}

	//handle key release
	void KeyRelease(unsigned char key, int x, int y)
	{
		key_state[key] = false;
		UserKeyRelease(key);
	}

	//handle holded keys
	void KeyHold()
	{
		for (int i = 0; i < MAX_KEYS; i++)
		{
			if (key_state[i]) // if key down
			{
				CameraInput(i);
				ForceInput(i);
				UserKeyHold(i);
			}
		}
	}

	///mouse handling
	int mMouseX = 0;
	int mMouseY = 0;

	void motionCallback(int x, int y)
	{
		int dx = mMouseX - x;
		int dy = mMouseY - y;

		camera->Motion(dx, dy, delta_time);

		mMouseX = x;
		mMouseY = y;
	}

	void mouseCallback(int button, int state, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
	}

	void ToggleRenderMode()
	{
		if (render_mode == NORMAL)
			render_mode = DEBUG;
		else if (render_mode == DEBUG)
			render_mode = BOTH;
		else if (render_mode == BOTH)
			render_mode = NORMAL;
	}

	///exit callback
	void exitCallback(void)
	{
		delete camera;
		delete scene;
		PhysicsEngine::PxRelease();
	}
}

