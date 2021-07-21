//
// Created by Chris LaVoy on 5/16/21.
//

#include "raylib.h"

#ifndef SPACE_SHOOTER_UTILS_H
#define SPACE_SHOOTER_UTILS_H

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float rotation;
//    float scale;
    float movementSpeed;
    Texture2D tex;
    Rectangle rect;
    Rectangle sourceRect;
    Vector2 origin;
    Color tint;
    float colliderRadius;
} Player;

typedef struct Bullet {
	Vector2 position;
	Vector2 velocity;
	float rotation;
	float scale;
	float movementSpeed;
	float lifetime;
	Rectangle rect;
	Color tint;
	bool visible;
    float colliderRadius;
} Bullet;

typedef struct Asteroid
{
	Vector2 position;
	Vector2 velocity;
	float rotation;
	float scale;
	Rectangle rect;
	Color tint;
	float colliderRadius;
	bool alive;
	int level;
} Asteroid;

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player);
Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight);
Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight);
void ScreenLoop(Vector2 *position);
void Shoot(Player *player, Bullet *bullet);
Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime);
Rectangle UpdateRectangle(Vector2 *position, Texture2D tex, float scale);
void UpdatePlayer(Player *player, int *bulletIterator, Bullet *bullets, int maxBullets, float frameTime);
void UpdateBullets(Bullet *bullets, Asteroid *asteroids, Texture2D bulletTexture, int maxBullets, float bulletLifetime, int maxAsteroids, float frameTime);
void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, Texture2D asteroidTexture, float frameTime);
void DrawPlayer(Player *player, bool debugMode);
void DrawBullets(Bullet *bullets, int maxBullets, Texture2D bulletTexture, Rectangle bulletSourceRect, Vector2 bulletOrigin, bool debugMode);
void DrawAsteroids(Asteroid *asteroids, int maxAsteroids, Texture2D asteroidTexture, Rectangle asteroidSourceRect, Vector2 asteroidOrigin, bool debugMode);

#endif //SPACE_SHOOTER_UTILS_H