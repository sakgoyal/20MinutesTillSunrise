#pragma once
#include "AssetManager.hpp"
#include "GameEntity.hpp"
namespace QuickSDL {
	using std::string;
	class Texture: public GameEntity {
	  protected:
		SDL_Texture *mTex = nullptr;
		Graphics *mGraphics = nullptr;
		int mWidth = 0;
		int mHeight = 0;
		bool mClipped = false;
		SDL_Rect mRenderRect = {0, 0, 0, 0};
		SDL_Rect mClipRect = {0, 0, 0, 0};

	  public:
		Texture(string &filename) {
			mGraphics = Graphics::Instance();
			mTex = AssetManager::Instance()->GetTexture(filename);
			SDL_QueryTexture(mTex, NULL, NULL, &mWidth, &mHeight);
			mRenderRect = {0, 0, mWidth, mHeight};
		} //Loads a whole texture from a file (relative to the exe path) (NO SPRITESHEET)
		//Loads a spritesheet texture from from file (relative to the exe path)
		//x,y - Starting pixel's X,Y on the spritesheet
		Texture(string &filename, int x, int y, int w, int h) {
			mClipped = true;
			mWidth = w;
			mHeight = h;
			mGraphics = Graphics::Instance();
			mTex = AssetManager::Instance()->GetTexture(filename);

			mRenderRect = {0, 0, w, h};
			mClipRect = {x, y, mWidth, mHeight}; //Setting the clipped rectangle to only get the needed texture from the spritesheet
		}
		//Converts the given text into a texture to be rendered
		//Note: fontpath is relative to the exe path
		Texture(string &text, string &fontpath, int size, SDL_Color color) {
			mGraphics = Graphics::Instance();
			mTex = AssetManager::Instance()->GetText(text, fontpath, size, color);
			SDL_QueryTexture(mTex, NULL, NULL, &mWidth, &mHeight);

			mRenderRect.w = mWidth;
			mRenderRect.h = mHeight;
		}
		~Texture() override {
			mTex = NULL;
			mGraphics = NULL;
		}
		void Render() override {
			const Vector2 pos = Pos(world);
			const Vector2 scale = Scale(world);
			mRenderRect.x = (int)(pos.x - (float)mWidth * scale.x * 0.5f);
			mRenderRect.y = (int)(pos.y - (float)mHeight * scale.y * 0.5f);

			mRenderRect.w = (int)((float)mWidth * scale.x); //Scales the width and height according to the scale of the GameEntity
			mRenderRect.h = (int)((float)mHeight * scale.y);

			mGraphics->DrawTexture(mTex, (mClipped ? &mClipRect : nullptr), &mRenderRect, (double)Rotation(world));
		}
	};
} // namespace QuickSDL