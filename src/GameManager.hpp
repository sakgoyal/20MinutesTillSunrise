#pragma once
#include "AudioManager.hpp"
#include "InputManager.hpp"
#include "Texture.hpp"
#include "Timer.hpp"
namespace QuickSDL {
	class GameManager {
	  public:
		static GameManager *Instance() {
			if (!sInstance) sInstance = new GameManager();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		//Contains the game loop, is called once in the main function
		void Run() {
			while (!mQuit) {
				mTimer->Update();
				SDL_Event mEvents; // Used to catch the event when the user exits the game
				while (SDL_PollEvent(&mEvents)) {
					if (mEvents.type == SDL_QUIT) {
						mQuit = true;
					}
				}
				if (mTimer->DeltaTime() >= (1.0f / FRAME_RATE)) { //Frame rate limiter
					EarlyUpdate();
					Update();
					LateUpdate();
					Render();
				}
			}
		}

	  private:
		GameManager() {
			mQuit = false;
			mGraphics = Graphics::Instance();			//Initialize SDL
			if (!Graphics::Initialized()) mQuit = true; // Quits the game if SDL fails to initialize
			mTimer = Timer::Instance();
			mAssetMgr = AssetManager::Instance();
			mInputMgr = InputManager::Instance();
			mAudioMgr = AudioManager::Instance();
		}
		~GameManager() {
			Timer::Release();
			Graphics::Release();
			AudioManager::Release();
			AssetManager::Release();
			InputManager::Release();
			mTimer = NULL;
			mAssetMgr = NULL;
			mAudioMgr = NULL;
			mGraphics = NULL;
			mInputMgr = NULL;
		}
		//Is called before Update, and is used for things that need to be updated first. for example: updating input state
		void EarlyUpdate() {
			//Updating the input state before any other updates are run to make sure the Input check is accurate
			mInputMgr->Update();
		}
		//Used to update entities, all transformations are to be done in this functions
		void Update() {
			//GameEntity Updates should happen here
		}
		//Is called after Update and is used for things that need to be updated last. for example: collision detection or resetting the timer
		void LateUpdate() {
			//Any collision detection should happen here

			mInputMgr->UpdatePrevInput();
			mTimer->Reset();
		}
		//Clears the back buffer, and then is used to render all game entities. Is called after Late Update
		void Render() {
			mGraphics->ClearBackBuffer(); //Clears the last frame's render from the back buffer

			//All other rendering is to happen here

			mGraphics->Render(); //Renders the current frame
		}

		const float FRAME_RATE = 120;
		bool mQuit;
		Timer *mTimer = nullptr; // Used to limit the frame rate
		Graphics *mGraphics = nullptr;
		AssetManager *mAssetMgr = nullptr;
		InputManager *mInputMgr = nullptr;
		AudioManager *mAudioMgr = nullptr;
		static inline GameManager *sInstance = nullptr;
	};
} // namespace QuickSDL
