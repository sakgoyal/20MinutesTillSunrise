#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "AudioManager.hpp"
#include "InputManager.hpp"
#include "Texture.hpp"
#include "Timer.hpp"
namespace QuickSDL {
	class GameManager {
	  private:
		//Needed to make GameManager a singleton class
		static GameManager *sInstance;

		//The target frame rate of the game
		const int FRAME_RATE = 120;

		//Used to exit the game loop
		bool mQuit;

		//List of Managers to be initialized and released
		Graphics *mGraphics;
		AssetManager *mAssetMgr;
		InputManager *mInputMgr;
		AudioManager *mAudioMgr;

		// Used to limit the frame rate
		Timer *mTimer;

		// Used to catch the event when the user exits the game
		SDL_Event mEvents;

	  public:
		//-----------------------------------------
		//Returns a pointer to the class instance
		//-----------------------------------------
		static GameManager *Instance();
		//-----------------------------------------------------
		//Releases the class instance and sets it back to NULL
		//Is called when the game is closed
		//Releases all other managers
		//-----------------------------------------------------
		static void Release();

		//-------------------------------------------------------------
		//Contains the game loop, is called once in the main function
		//-------------------------------------------------------------
		void Run();

	  private:
		//------------------------------------------------------------------------------------------
		//Contructor is private so that Instance() must be used to get an instance when needed
		//------------------------------------------------------------------------------------------
		GameManager();
		//-------------------------------------------------------------------------------------
		//Destructor is private so that the instance can only be destroyed using Release()
		//-------------------------------------------------------------------------------------
		~GameManager();

		//--------------------------------------------------------------------------------
		//Is called before Update, and is used for things that need to be updated first
		//    for example: updating input state
		//--------------------------------------------------------------------------------
		void EarlyUpdate();
		//------------------------------------------------------------------------------
		//Used to update entities, all transformations are to be done in this functions
		//------------------------------------------------------------------------------
		void Update();
		//------------------------------------------------------------------------------
		//Is called after Update and is used for things that need to be updated last
		//    for example: collision detection or resetting the timer
		//------------------------------------------------------------------------------
		void LateUpdate();

		//----------------------------------------------------------------------
		//Clears the back buffer, and then is used to render all game entities
		//Is called after Late Update
		//----------------------------------------------------------------------
		void Render();
	};
} // namespace QuickSDL
#endif
