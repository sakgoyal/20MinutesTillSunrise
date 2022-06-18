#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <iostream>
#include <string>
namespace QuickSDL {
	class Graphics {
	  public:
		static Graphics *Instance() {
			if (!sInstance) sInstance = new Graphics();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
			sInitialized = false;
		}
		static bool Initialized() {
			return sInitialized;
		}

		SDL_Texture *LoadTexture(std::string &path) {
			SDL_Surface *surface = IMG_Load(path.c_str());
			if (!surface) {
				printf("Image Load Error: Path(%s) - Error(%s)\n", path.c_str(), IMG_GetError());
				return nullptr;
			}

			SDL_Texture *tex = SDL_CreateTextureFromSurface(mRenderer, surface);
			if (!tex) {
				printf("Create Texture Error: %s\n", SDL_GetError());
				return nullptr;
			}
			SDL_FreeSurface(surface);
			return tex;
		} //Note: Should only be used by the AssetManager to cache the textures for reuse
		SDL_Texture *CreateTextTexture(TTF_Font *font, std::string &text, SDL_Color color) {
			SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
			if (!surface) {
				printf("Text Render Error: %s\n", TTF_GetError());
				return nullptr;
			}
			SDL_Texture *tex = SDL_CreateTextureFromSurface(mRenderer, surface);
			if (!tex) {
				printf("Text Texture Creation Error: %s\n", SDL_GetError());
				return nullptr;
			}
			SDL_FreeSurface(surface);
			return tex;
		} //Note: Should only be used by the AssetManager to cache the textures for reuse
		void ClearBackBuffer() {
			SDL_RenderClear(mRenderer);
		} //Clears all rendered textures from the back buffer, usually used at the begining of the frame's render
		void DrawTexture(SDL_Texture *tex, SDL_Rect *clip = nullptr, SDL_Rect *rend = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) {
			SDL_RenderCopyEx(mRenderer, tex, clip, rend, angle, nullptr, flip);
		}//Draws the given texture to the back buffer
		void Render() {
			SDL_RenderPresent(mRenderer);
		} //Refreshes the back buffer. Usually called at then end of the frame's render
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 600;
		const char *WINDOW_TITLE = "20 Minutes Till Sunrise";

	  private:
		Graphics() {
			sInitialized = Init();
		}
		~Graphics() {
			SDL_DestroyWindow(mWindow);
			SDL_DestroyRenderer(mRenderer);
			mRenderer = NULL;
			mWindow = NULL;
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
		bool Init() {
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
				return false;
			}
			mWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (!mWindow) {
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				printf("Window Creation Error: %s\n", SDL_GetError());
				return false;
			}
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (!mRenderer) {
				std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				return false;
			}
			SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
			if (int flags = IMG_INIT_PNG; !(IMG_Init(flags) & flags)) {
				std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
				return false;
			}
			if (TTF_Init() == -1) {
				std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
				return false;
			}
			return true;
		}
		static inline Graphics *sInstance = nullptr;
		static inline bool sInitialized = false;
		SDL_Window *mWindow = nullptr;
		SDL_Renderer *mRenderer = nullptr;
	};
} // namespace QuickSDL
