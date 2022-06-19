#pragma once
#include "MathHelper.hpp"
#include <SDL.h>
namespace QuickSDL {
	class InputManager {
		// To be expanded later to handle touch and contoller input
	  public:
		enum MOUSE_BUTTON { left = 0,
							right,
							middle,
							back,
							forward };
		static auto Instance() {
			if (!sInstance) sInstance = new InputManager();
			return sInstance;
		}
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		bool KeyDown(SDL_Scancode scanCode) {
			return mKeyboardState[scanCode];
		}
		bool KeyPressed(SDL_Scancode scanCode) {
			return !mPrevKeyboardState[scanCode] && mKeyboardState[scanCode];
		} //Returns true only on the frame that the key given in scanCode is pressed down
		bool KeyReleased(SDL_Scancode scanCode) {
			return mPrevKeyboardState[scanCode] && !mKeyboardState[scanCode];
		}
		bool MouseButtonDown(MOUSE_BUTTON button) {
			Uint32 mask = 0;

			switch (button) {
				case left:
					mask = SDL_BUTTON_LMASK;
					break;
				case right:
					mask = SDL_BUTTON_RMASK;
					break;
				case middle:
					mask = SDL_BUTTON_MMASK;
					break;
				case back:
					mask = SDL_BUTTON_X1MASK;
					break;
				case forward:
					mask = SDL_BUTTON_X2MASK;
					break;
			}
			return (mMouseState & mask);
		}
		bool MouseButtonPosEdge(MOUSE_BUTTON button) {
			Uint32 mask = 0;
			switch (button) {
				case left:
					mask = SDL_BUTTON_LMASK;
					break;
				case right:
					mask = SDL_BUTTON_RMASK;
					break;
				case middle:
					mask = SDL_BUTTON_MMASK;
					break;
				case back:
					mask = SDL_BUTTON_X1MASK;
					break;
				case forward:
					mask = SDL_BUTTON_X2MASK;
					break;
			}
			return !(mPrevMouseState & mask) && (mMouseState & mask);
		}
		bool MouseButtonReleased(MOUSE_BUTTON button) {
			Uint32 mask = 0;

			switch (button) {
				case left:
					mask = SDL_BUTTON_LMASK;
					break;
				case right:
					mask = SDL_BUTTON_RMASK;
					break;
				case middle:
					mask = SDL_BUTTON_MMASK;
					break;
				case back:
					mask = SDL_BUTTON_X1MASK;
					break;
				case forward:
					mask = SDL_BUTTON_X2MASK;
					break;
			}
			return (mPrevMouseState & mask) && !(mMouseState & mask);
		}
		Vector2 MousePos() {
			return {(float)mMouseXPos, (float)mMouseYPos};
		}
		void Update() {
			mMouseState = SDL_GetMouseState(&mMouseXPos, &mMouseYPos);
		} //Updates the Input States (should be called once per frame before any input check)
		void UpdatePrevInput() {
			memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
			mPrevMouseState = mMouseState;
		} //Sets the current frame's key states as the previous key states (to be called in the LateUpdate). No input checks should be made in the current frame after this function is called

	  private:
		InputManager() {
			mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
			mPrevKeyboardState = new Uint8[mKeyLength];
			memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
		}
		~InputManager() {
			delete[] mPrevKeyboardState;
			mPrevKeyboardState = NULL;
		}

		int mKeyLength = 0;
		Uint8 *mPrevKeyboardState = nullptr;
		const Uint8 *mKeyboardState;

		int mMouseXPos = 0;
		int mMouseYPos = 0;
		Uint32 mMouseState = 0;
		Uint32 mPrevMouseState = 0;
		static inline InputManager *sInstance = nullptr;
	};
} // namespace QuickSDL