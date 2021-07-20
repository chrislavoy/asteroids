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
    float scale;
    float movementSpeed;
    Texture2D tex;
    Rectangle rect;
    Rectangle sourceRect;
    Vector2 origin;
    Color tint;
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
} Bullet;

typedef struct Asteroid
{
	Vector2 position;
	Vector2 velocity;
	float rotation;
	float scale;
	Rectangle rect;
	Color tint;
} Asteroid;

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player);
Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight);
Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight);
void ScreenLoop(Vector2 *position);
void Shoot(Player *player, Bullet *bullet);
Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime);
Rectangle UpdateRectangle(Vector2 *position, Texture2D tex, float scale);
void UpdatePlayer(Player *player, int bulletIterator, Bullet *bullets, int maxBullets, float frameTime);
void UpdateBullets(Bullet *bullets, Asteroid *asteroids, Texture2D bulletTexture, int maxBullets, float bulletLifetime, int maxAsteroids, float frameTime);
void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, Texture2D asteroidTexture, float frameTime);
void DrawPlayer(Player *player);

#endif //SPACE_SHOOTER_UTILS_H