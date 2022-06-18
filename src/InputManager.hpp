//(To be expanded later to handle touch and contoller input)           //
#pragma once
#include "MathHelper.hpp"
#include <SDL.h>
namespace QuickSDL {
	class InputManager {
	  public:
		enum MOUSE_BUTTON { left = 0,
							right,
							middle,
							back,
							forward };

	  private:
		static InputManager *sInstance;
		Uint8 *mPrevKeyboardState;
		const Uint8 *mKeyboardState;
		int mKeyLength = 0;
		Uint32 mPrevMouseState = 0;
		Uint32 mMouseState = 0;
		int mMouseXPos = 0;
		int mMouseYPos = 0;

	  public:
		static InputManager *Instance();
		static void Release() {
			delete sInstance;
			sInstance = NULL;
		}
		bool KeyDown(SDL_Scancode scanCode);
		bool KeyPressed(SDL_Scancode scanCode); //Returns true only on the frame that the key given in scanCode is pressed down
		bool KeyReleased(SDL_Scancode scanCode);

		bool MouseButtonDown(MOUSE_BUTTON botton);
		//Returns true only on the frame that the given mouse button is pressed
		bool MouseButtonPressed(MOUSE_BUTTON button);
		bool MouseButtonReleased(MOUSE_BUTTON button);
		Vector2 MousePos();
		//Updates the Input States (should be called once per frame before any input check)
		void Update();
		//Sets the current frame's key states as the previous key states (to be called in the LateUpdate)
		//No input checks should be made in the current frame after this function is called
		void UpdatePrevInput();

	  private:
		InputManager() {
			mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
			//Setting mPrevKeyboardState to be the same length as mKeyboardState
			mPrevKeyboardState = new Uint8[mKeyLength];
			//Copying the contents of mKeyboardState into mPrevKeyboardState so that it contains data on the first frame
			memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
		}
		~InputManager() {
			//Clearing the previous keyboard state array
			delete[] mPrevKeyboardState;
			mPrevKeyboardState = NULL;
		}
	};
} // namespace QuickSDL