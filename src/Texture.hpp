#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "AssetManager.hpp"
#include "GameEntity.hpp"
namespace QuickSDL {
	class Texture: public GameEntity {
	  protected:
		//The SDL_Texture to be rendered
		SDL_Texture *mTex = nullptr;

		//Used to render the texture
		Graphics *mGraphics = nullptr;

		//Width of the texture
		int mWidth = 0;
		//Height of the texture
		int mHeight = 0;

		//True if the texture is loaded from a spritesheet
		bool mClipped = false;

		//Is used to render the texture on the screen
		SDL_Rect mRenderRect = {0,0,0,0};
		//Is used to clip the texture from a spritesheet
		SDL_Rect mClipRect = {0,0,0,0};

	  public:
		//--------------------------------------------------------------
		//Loads a whole texture from a file (relative to the exe path)
		//Note: For spritesheets use the other contructor
		//--------------------------------------------------------------
		Texture(std::string filename);
		//-------------------------------------------------------------
		//Loads a texture from from file (relative to the exe path)
		//Supports spritesheets
		//x - Starting pixel's X on the spritesheet
		//y - Starting pixel's Y on the spritesheet
		//w - The width of the clipped sprite
		//h - The height of the clipped sprite
		//-------------------------------------------------------------
		Texture(std::string filename, int x, int y, int w, int h);
		//------------------------------------------------------------
		//Converts the given text into a texture to be rendered
		//Note: fontpath is relative to the exe path
		//size - The size of the text to be rendered
		//color - The color of the text to be rendered
		//------------------------------------------------------------
		Texture(std::string text, std::string fontpath, int size, SDL_Color color);
		virtual ~Texture() = 0;

		//----------------------------------------------
		//Called to render the texture to the screen
		//----------------------------------------------
		virtual void Render() override;
	};
} // namespace QuickSDL
#endif
