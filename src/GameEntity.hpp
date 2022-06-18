#pragma once
#include "MathHelper.hpp"
namespace QuickSDL {
	class GameEntity {
	  public:
		enum SPACE { local = 0,
					 world = 1 };

	  private:
		Vector2 mPos;
		float mRotation = 0;
		Vector2 mScale;
		bool mActive = false;
		GameEntity *mParent = nullptr;

	  public:
		GameEntity(Vector2 pos = VEC2_ZERO) {
			mPos = pos;
			mRotation = 0.0f;
			mActive = true;
			mParent = NULL;
			mScale = VEC2_ONE;
		}
		virtual ~GameEntity() {
			mParent = NULL;
		}
		void Pos(Vector2 pos) {
			mPos = pos;
		}
		//Returns the position of the GameEntity
		//If space = world, the world position is returned (relative to the origin)
		//if space = local, if the object has a parent, the local position is returned (relative to the parent position)
		//       otherwise if the object has no parent, the world position is returned instead (relative to the origim)
		Vector2 Pos(SPACE space = world) {
			if (space == local || mParent == NULL)
				return mPos;

			Vector2 parentScale = mParent->Scale(world);
			//The object's local position is rotated by the parent's rotation
			Vector2 temp = {mPos.x * parentScale.x, mPos.y * parentScale.y};
			Vector2 rotPos = RotateVector(temp, mParent->Rotation(local));

			//The final position also depends on the parent's scale (if the parent is scaled up, the object should be further away from the parent)
			return mParent->Pos(world) + rotPos;
		}

		//Sets the GameEntity's rotation, it only updates local space
		//If the object has a parent, its new rotation is relative to the parent's world rotation
		//If the object has no parent, its new rotation is set to the given rotation
		void Rotation(float rotation) {
			mRotation = rotation;
			if (mRotation > 360.0f) {
				mRotation -= 360.0f * (mRotation / 360);

			} else if (mRotation < 0.0f) {
				mRotation -= 360.0f * ((mRotation / 360) - 1);
			}
		}
		//Returns the rotation of the GameEntity
		//If space = world, the world rotation is returned (total rotation of parent[if it has one] and object)
		//if space = local, if the object has a parent, the local rotation is returned (relative to the parent's rotation)
		//       otherwise if the object has no parent, the world rotation is returned instead (relative to the origim)
		float Rotation(SPACE space = world) {
			if (space == local || mParent == NULL)
				return mRotation;

			return mParent->Rotation(world) + mRotation;
		}

		void Scale(Vector2 scale) {
			mScale = scale;
		}
		//Returns the scale of the GameEntity
		//If space = world, the world scale is returned (product of the parent and local scale)
		//if space = local, if the object has a parent, the local scale is returned (relative to the parent's scale)
		//       otherwise if the object has no parent, the GameEntity's scale is returned instead
		Vector2 Scale(SPACE space = world) {
			if (space == local || mParent == NULL)
				return mScale;

			Vector2 scale = mParent->Scale(world);
			scale.x *= mScale.x;
			scale.y *= mScale.y;

			return scale;
		}

		void Active(bool active) {
			mActive = active;
		}
		bool Active() {
			return mActive;
		}

		//Sets the parent of the GameEnity as the given GameEntity
		//If the GameEntity has no parent, the given GameEntity is set as its parent
		//If the GameEntity has a parent, the given game entity is set as the new parent,
		//       and the lcoal Position/Rotation/Scale are changed to be relative to the new parent (world values are unchanged)
		//If NULL is passed in, the GameEntity is considered without a parent,
		//       and all values are set to be relative to the origin (world values are unchanged)
		void Parent(GameEntity *parent) {
			//If the parent is removed, The Position/Rotation/Scale are the GameEntity's world values, to keep the object looking the same after the removal of the parent;
			if (parent == NULL) {
				mPos = Pos(world);
				mScale = Scale(world);
				mRotation = Rotation(world);

			} else {
				//If the object already has a parent, remove the current parent to get it ready to be the child for the new parent
				if (mParent != NULL)
					Parent(NULL);

				Vector2 parentScale = parent->Scale(world);

				//Setting the local position to be relative to the new parent (while maintaining the same world position as before)
				Vector2 temp = {Pos(world) - parent->Pos(world)};
				mPos = RotateVector(temp, -parent->Rotation(world));
				mPos.x /= parentScale.x;
				mPos.y /= parentScale.y;

				//Setting the local rotation to be relative to the new parent (while maintaining the same world rotation as before)
				mRotation = mRotation - parent->Rotation(world);

				//Setting the new scale to be relative to the new parent (while maintaining the same world scale as before)
				mScale = Vector2(mScale.x / parentScale.x, mScale.y / parentScale.y);
			}

			mParent = parent;
		}
		GameEntity *Parent() {
			return mParent;
		}
		void Translate(Vector2 vec) {
			mPos += vec;
		}
		void Rotate(float amount) {
			mRotation += amount;
		}
		virtual void Update(){}
		virtual void Render(){}
	};
} // namespace QuickSDL