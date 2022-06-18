#pragma once
#include "AssetManager.hpp"
namespace QuickSDL {
	class AudioManager {
	  public:
		static AudioManager *Instance() {
			if (!sInstance) sInstance = new AudioManager();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		//Sets the BGM to play using the provided file, loops = -1 is infinite looping (default: -1)
		void PlayMusic(std::string filename, int loops = -1) {
			Mix_PlayMusic(mAssetMgr->GetMusic(filename), loops);
		}
		static void PauseMusic() {
			if (Mix_PlayingMusic()) Mix_PauseMusic();
		}
		static void ResumeMusic() {
			if (Mix_PausedMusic()) Mix_ResumeMusic();
		}

		//Plays a sound effect from the file provided
		//plays if for the given loops (default: 0)
		//on the given channel (default: 0)
		void PlaySFX(std::string filename, int loops = 0, int channel = 0) {
			//Play the Mix_Chunk* loaded in the AssetManager so that the file is not reloaded every time
			Mix_PlayChannel(channel, mAssetMgr->GetSFX(filename), loops);
		}

	  private:
		AudioManager() {
			mAssetMgr = AssetManager::Instance();
			//Initialize the SDL_mixer using 44.1 khz and 2 channels (change if needed) and handle initialization errors
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
				printf("Mixer Initialization Error: %s\n", Mix_GetError());
			}
		}
		~AudioManager() {
			mAssetMgr = NULL;
			Mix_Quit();
		}
		static inline AudioManager *sInstance = nullptr;
		AssetManager *mAssetMgr;
	};
} // namespace QuickSDL
