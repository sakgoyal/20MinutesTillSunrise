#pragma once
#include "Graphics.hpp"
#include <SDL_mixer.h>
#include <map>
using std::map;
using std::string;
namespace QuickSDL {
	class AssetManager {
	  public:
		static AssetManager *Instance() {
			if (!sInstance) sInstance = new AssetManager();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		SDL_Texture *GetTexture(string filename) {
			//Get the full path of the file
			string fullPath = SDL_GetBasePath();
			fullPath.append("Assets/" + filename);

			//If the file has not been already loaded, load it and add it to the mTextures map
			if (mTextures[fullPath] == nullptr)
				mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);

			//returning the cached file from the map
			return mTextures[fullPath];
		}
		SDL_Texture *GetText(string text, string filename, int size, SDL_Color color) {
			//Get the font from the font cache
			TTF_Font *font = GetFont(filename, size);

			//The key takes into account the font, text, size, and color to differentiate text textures
			string key = text + filename + (char)size + (char)color.r + (char)color.b + (char)color.g;

			//If the same text has not been rendered before, render it and add it to the mText map
			if (mText[key] == nullptr)
				mText[key] = Graphics::Instance()->CreateTextTexture(font, text, color);

			//returning the cached texture containing the text
			return mText[key];
		}
		Mix_Music *GetMusic(string filename) {
			//Get the full path of the WAV file
			string fullPath = SDL_GetBasePath();
			fullPath.append("Assets/" + filename);

			//If the file has not been already loaded, load it and add it to the mMusic map
			if (mMusic[fullPath] == nullptr) {
				mMusic[fullPath] = Mix_LoadMUS(fullPath.c_str());
				//Error handling for file loading
				if (mMusic[fullPath] == NULL)
					printf("Music Loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
			}

			//returning the cached file from the map
			return mMusic[fullPath];
		}
		Mix_Chunk *GetSFX(string &filename) {
			//Get the full path of the WAV file
			string fullPath = SDL_GetBasePath();
			fullPath.append("Assets/" + filename);

			//If the file has not been already loaded, load it and add it to the mSFX map
			if (mSFX[fullPath] == nullptr) {
				mSFX[fullPath] = Mix_LoadWAV(fullPath.c_str());
				//Error handling for file loading
				if (mSFX[fullPath] == NULL)
					printf("SFX Loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
			}

			//returning the cached file from the map
			return mSFX[fullPath];
		}

	  private:
		AssetManager() = default;
		~AssetManager() {
			//Freeing all loaded assets
			for (auto &[fir, sec]: mTextures) {
				if (sec) SDL_DestroyTexture(sec);
			}
			for (auto &[fir, sec]: mText) {
				if (sec) SDL_DestroyTexture(sec);
			}
			for (auto &[fir, sec]: mFonts) {
				if (sec) TTF_CloseFont(sec);
			}
			for (auto &[fir, sec]: mMusic) {
				if (sec) Mix_FreeMusic(sec);
			}
			for (auto &[fir, sec]: mSFX) {
				if (sec) Mix_FreeChunk(sec);
			}
			mTextures.clear();
			mText.clear();
			mFonts.clear();
			mMusic.clear();
			mSFX.clear();
		}
		TTF_Font *GetFont(string &filename, int size) {
			string fullPath = SDL_GetBasePath();
			fullPath.append("Assets/" + filename);
			string key = fullPath + (char)size;
			if (!mFonts[key]) {
				mFonts[key] = TTF_OpenFont(fullPath.c_str(), size);
				if (!mFonts[key]) printf("Font Loading Error: Font-%s Error-%s", filename.c_str(), TTF_GetError());
			}
			return mFonts[key];
		}
		map<string, SDL_Texture *> mTextures; //Used to cache all loaded assets
		map<string, SDL_Texture *> mText;
		map<string, TTF_Font *> mFonts;
		map<string, Mix_Music *> mMusic;
		map<string, Mix_Chunk *> mSFX;
		static inline AssetManager *sInstance = NULL;
	};
} // namespace QuickSDL