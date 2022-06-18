#pragma once
#include "Texture.hpp"
#include "Timer.hpp"
namespace QuickSDL {
	class AnimatedTexture: public Texture {
	  public:
		enum WRAP_MODE { once = 0,
						 loop = 1 };
		enum ANIM_DIR { horizontal = 0,
						vertical = 1 };
		//Loads a texture from from file (relative to the exe path)
		//Supports spritesheets
		//x - Starting pixel's X on the spritesheet
		//y - Starting pixel's Y on the spritesheet
		//frameCount - The number of frames in the animation
		//animationSpeed - How long it takes to run the animation in seconds
		//animationDirection - How the animation sprites are arranges in the spritesheet
		AnimatedTexture(std::string filename, int x, int y, int w, int h, int frameCount, float animationSpeed, ANIM_DIR animationDir):
			Texture(filename, x, y, w, h) {
			mTimer = Timer::Instance();
			mStartX = x;
			mStartY = y;
			mFrameCount = frameCount;
			mAnimationSpeed = animationSpeed;
			mTimePerFrame = mAnimationSpeed / (float)mFrameCount;
			mAnimationTimer = 0.0f;
			mAnimationDirection = animationDir;
			mAnimationDone = false;
			mWrapMode = loop;
		}
		~AnimatedTexture() override = default;
		void WrapMode(WRAP_MODE mode) {
			mWrapMode = mode;
		}
		void Update() override {
			if (!mAnimationDone) {
				mAnimationTimer += mTimer->DeltaTime();
				if (mAnimationTimer >= mAnimationSpeed) {
					//Only loop if the wrap mode is loop
					if (mWrapMode == loop)
						mAnimationTimer -= mAnimationSpeed;
					else {
						mAnimationDone = true;
						mAnimationTimer = mAnimationSpeed - mTimePerFrame;
					}
				}

				if (mAnimationDirection == horizontal)
					mClipRect.x = mStartX + (int)(mAnimationTimer / mTimePerFrame) * mWidth;
				else
					mClipRect.y = mStartY + (int)(mAnimationTimer / mTimePerFrame) * mHeight;
			}
		}

	  private:
		Timer *mTimer = nullptr;
		int mStartX = 0; //the first frame's starting pixel's X values on the spritesheet
		int mStartY = 0;
		float mAnimationTimer = 0;
		float mAnimationSpeed = 0;
		float mTimePerFrame = 0;
		int mFrameCount = 0;
		WRAP_MODE mWrapMode;
		ANIM_DIR mAnimationDirection;
		bool mAnimationDone = false;
	};
} // namespace QuickSDL