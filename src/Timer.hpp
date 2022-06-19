#pragma once
#include <SDL.h>
namespace QuickSDL {
	class Timer {
	  public:
		static auto Instance() {
			if (!sInstance) sInstance = new Timer();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		void Reset() {
			mStartTicks = SDL_GetTicks();
			mElapsedTicks = 0;
			mDelataTime = 0.0f;
		} //Resets the time elapsed back to 0, is usually called after each frame so that mDeltaTime is the time elapsed since last frame
		float DeltaTime() {
			return mDelataTime;
		} //Time elapsed in seconds since the last Reset
		void TimeScale(float t = 1.0f) {
			mTimeScale = t;
		}
		float TimeScale() {
			return mTimeScale;
		} //Can be used to speed up or slowdown all transformations by multiplying it by the change in position or rotation
		void Update() {
			mElapsedTicks = SDL_GetTicks() - mStartTicks;
			mDelataTime = (float)mElapsedTicks * 0.001f; //Converting milliseconds to seconds
		}												 //Should be called in the EarlyUpdate, updates the time elapsed as well as the DeltaTime since the last Reset

	  private:
		Timer() {
			Reset();
		}
		~Timer() = default;
		static inline Timer *sInstance = nullptr;
		unsigned int mStartTicks = 0;	//Contains the time of the last reset
		unsigned int mElapsedTicks = 0; //Stores the number of milliseconds since the last reset
		float mDelataTime = 0;			//Stores the time elapsed since the last reset in seconds
		float mTimeScale = 1;			//Can be used to speed up or slowdown all entities that transform using it
	};
} // namespace QuickSDL
