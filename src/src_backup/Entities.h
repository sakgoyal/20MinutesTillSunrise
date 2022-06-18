#pragma once
#include <SDL.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>
enum class EntityType { player,
						enemy,
						bullet,
						powerup,
						wall,
						none };
enum class EnemyType { normal,
					   fast,
					   slow,
					   boss };
enum class PowerupType { health,
						 speed,
						 none };
enum class WallType { vertical,
					  horizontal,
					  none };
enum class BulletType { normal,
						fast,
						slow,
						bomb,
						freeze };

class Entity {
  public:
	//virtual ~Entity() = 0;
	virtual void setHealth(int h) { health = h; }
	virtual void setScore(int s) { score = s; }
	virtual void takeHit(const int dt) { health -= dt; }
	virtual void setSize(uint8_t rad) { size = rad; }
	virtual void setColor(uint8_t r, uint8_t g, uint8_t b) {
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	virtual void setPosition(int xPos, int yPos) {
		this->posX = xPos;
		this->posY = yPos;
	}
	virtual void updateVel(const Uint8 *state) = 0;
	virtual void update() = 0;
	virtual void render(const double rot = 0, SDL_RendererFlip f = SDL_FLIP_NONE) {
		destRect = {(int)posX, (int)posY, EntityW, EntityH};
		SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rot, nullptr, f);
	};
	virtual std::vector<int> getPosition() const {
		return {(int)posX, (int)posY};
	}
	friend std::ostream &operator<<(std::ostream &os, const Entity &dt);
	int EntityW = 0, EntityH = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

  protected:
	EntityType entityType = EntityType::none;
	double posX = 0, posY = 0, maxX = 800, maxY = 800, minX = 0, minY = 0;
	double velX = 0, velY = 0;
	int maxVel = 3;
	int health = 3, maxHealth = 3;
	int damage = 0, bullets = 0, ammo = 0, range = 0;
	int size = 0, score = 0;
	int color[3] = {0, 0, 0};
	SDL_Texture *texture = nullptr;
	SDL_Rect destRect = {0, 0, 0, 0};
	SDL_Renderer *renderer = nullptr;
};
class Bullet: public Entity {
  public:
	//~Bullet() override = default;
	BulletType type = BulletType::normal;
	Bullet(BulletType bt, int maxBulletSpeed) {
		type = bt;
		if (bt == BulletType::normal) {
			size = 1;
			damage = 2;
			color[0] = 255;
			color[1] = 0;
			color[2] = 255;
			range = 10;
		} else if (bt == BulletType::fast) {
			size = 2;
			damage = 1;
			color[0] = 255;
			color[1] = 255;
			color[2] = 0;
			range = 15;
		} else if (bt == BulletType::slow) {
			size = 2;
			damage = 3;
			color[0] = 0;
			color[1] = 0;
			color[2] = 255;
			range = 20;
		}
	}
};
class Enemy: public Entity {
  public:
	Enemy() { type = EnemyType::normal; }
	//~Enemy() override = default;
	Enemy(const char *textureSheet, SDL_Renderer *ren, EnemyType t) {
		entityType = EntityType::enemy;
		type = t;
		renderer = ren;
		texture = TextureManager::LoadTexture(textureSheet, ren);
		SDL_QueryTexture(texture, nullptr, nullptr, &EntityW, &EntityH);
		posX = posY = score = 0;
		velX = velY = 0;
		bullets = 10;
	}
	explicit Enemy(EnemyType ET) {
		type = ET;
		switch (ET) {
			case EnemyType::normal:
				bullets = 0;
				health = 3;
				break;
			case EnemyType::fast:
				bullets = 0;
				health = 2;
				break;
			case EnemyType::slow:
				bullets = 2;
				health = 3;
				break;
			case EnemyType::boss:
				bullets = 2;
				health = 3;
				dash = true;
				break;
		}
	}
	void update() override {
		posX += velX;
		posY += velY;
		if (posX < 0) posX = 0;
		if (posY < 0) posY = 0;
		if (posX > maxX - EntityW) posX = maxX - EntityW;
		if (posY > maxY - EntityH) posY = maxY - EntityH;
	}
	void updateVel(const Uint8 *state) override {
		velY = state[SDL_SCANCODE_DOWN] - state[SDL_SCANCODE_UP];
		velX = state[SDL_SCANCODE_RIGHT] - state[SDL_SCANCODE_LEFT];
		velX *= maxVel;
		velY *= maxVel;
		//std::cout << velX << " " << velY << std::endl;
	}
	EnemyType type;
	bool dash = false;
};
class Player: public Entity {
  public:
	Player() {
		posX = 0;
		posY = 0;
		velX = 0;
		velY = 0;
		score = 0;
		bullets = 10;
	}
	//~Player() override = default;
	Player(const char *textureSheet, SDL_Renderer *ren) {
		entityType = EntityType::player;
		renderer = ren;
		texture = TextureManager::LoadTexture(textureSheet, ren);
		SDL_QueryTexture(texture, nullptr, nullptr, &EntityW, &EntityH);
		posX = posY = score = 0;
		velX = velY = 0;
		bullets = 10;
		constexpr int range = 6;
		maxX = ((maxX + EntityW) / 2) + range;
		maxY = ((maxY + EntityH) / 2) + range;
		minX = ((maxX + EntityW) / 2) - range;
		minY = ((maxY + EntityH) / 2) - range;
	}
	void update() override {
		posX += velX;
		posY += velY;
		posX = std::max(minX + EntityW, posX);
		posY = std::max(minY + EntityH, posY);
		posX = std::min(maxX - EntityW, posX);
		posY = std::min(maxY - EntityH, posY);
	}
	void updateVel(const Uint8 *state) override {
		if (state[SDL_SCANCODE_LSHIFT]) maxVel = 5;
		else
			maxVel = 3;
		velY = ((state[SDL_SCANCODE_S] | state[SDL_SCANCODE_DOWN]) - (state[SDL_SCANCODE_W] | state[SDL_SCANCODE_UP]));
		velX = ((state[SDL_SCANCODE_D] | state[SDL_SCANCODE_RIGHT]) - (state[SDL_SCANCODE_A] | state[SDL_SCANCODE_LEFT]));

		if (double det = (velX * velX) + (velY * velY); det > 1) {
			const double scale = sqrt(det);
			velX /= scale;
			velY /= scale;
		}
		velX *= maxVel;
		velY *= maxVel;
	}
};
class LevelEntity {
  public:
	LevelEntity(const char *textureSheet, SDL_Renderer *ren, int windowW, int windowH) {
		renderer = ren;
		texture = TextureManager::LoadTexture(textureSheet, ren);
		SDL_QueryTexture(texture, nullptr, nullptr, &EntityW, &EntityH);
		posX = posY = 0;
		velX = velY = 0;
		WindowHeight = windowH;
		WindowWidth = windowW;
	}
	void setPosition(int xPos, int yPos) {
		this->posX = xPos;
		this->posY = yPos;
	}
	void render() {
		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		const int numDupeX = (WindowWidth / w) + 1;
		const int numDupeY = (WindowHeight / h) + 1;
		for (int i = -numDupeY; i < numDupeY * 2; i++) {
			for (int j = -numDupeX; j < numDupeX * 2; j++) {
				SDL_Rect rect = {(int)((j * w) + posX), (int)((i * h) + posY), w, h};
				SDL_RenderCopy(renderer, texture, nullptr, &rect);
			}
		}
	}
	std::vector<double> getDirection() { return {-velX, -velY}; }
	void update() {
		posX += velX;
		posY += velY;
		posX = std::max((double)-WindowWidth, posX);
		posY = std::max((double)-WindowHeight, posY);
		posX = std::min(maxX, posX);
		posY = std::min(maxY, posY);
	}
	void updateVel(const Uint8 *state) {
		if (state[SDL_SCANCODE_LSHIFT]) maxVel = 5;
		else
			maxVel = 3;
		velY = -((state[SDL_SCANCODE_S] | state[SDL_SCANCODE_DOWN]) - (state[SDL_SCANCODE_W] | state[SDL_SCANCODE_UP]));
		velX = -((state[SDL_SCANCODE_D] | state[SDL_SCANCODE_RIGHT]) - (state[SDL_SCANCODE_A] | state[SDL_SCANCODE_LEFT]));

		if (double det = (velX * velX) + (velY * velY); det > 1) {
			const double scale = sqrt(det);
			velX /= scale;
			velY /= scale;
		}
		velX *= maxVel;
		velY *= maxVel;
		//std::cout << velX << " " << velY << std::endl;
	}

  protected:
	double maxX = 800, maxY = 800;
	double posX = 0, posY = 0;
	double velX = 0, velY = 0, maxVel = 3;
	int EntityW = 0, EntityH = 0;
	int WindowWidth = 800, WindowHeight = 800;
	SDL_Texture *texture = nullptr;
	SDL_Rect destRect = {0, 0, 0, 0};
	SDL_Renderer *renderer = nullptr;
};
inline std::ostream &operator<<(std::ostream &os, const Entity &dt) {
	os << "Xpos: " << dt.getPosition()[0] << ", Ypos: " << dt.getPosition()[1] << std::endl;
	return os;
}