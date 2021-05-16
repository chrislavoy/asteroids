//
// Created by Chris LaVoy on 5/16/21.
//

#include "utils.h"
#include "raylib.h"
#include "raymath.h"

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player)
{
	player->position = (Vector2) {(float)screenWidth / 2, (float)screenHeight / 2};
	player->acceleration = Vector2Zero();
	player->velocity = Vector2Zero();
	player->rotation = 0.0f;
	player->tint = WHITE;
	player->tex = LoadTexture("../resources/playerShip.png");
	player->rect = (Rectangle) {0, 0, (float) (*player).tex.width, (float) (*player).tex.height};
	player->scale = 1.0f;
	player->movementSpeed = 5.0f;
	return *player;
}

Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight)
{
	bullet->position = (Vector2){0, 0};
	bullet->velocity = (Vector2){0, 0};
	bullet->rotation = 0;
	bullet->scale = 1.0f;
	bullet->movementSpeed = 1000.0f;
	bullet->lifetime = 0.8f;
	bullet->rect = (Rectangle) {0, 0, (float)texWidth, (float)texHeight};
	bullet->tint = WHITE;
	bullet->visible = false;
	return *bullet;
}

Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight)
{
	const float movementSpeed = 150;

	asteroid->position = (Vector2){GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)};
	asteroid->velocity = (Vector2){GetRandomValue(-movementSpeed, movementSpeed), GetRandomValue(-movementSpeed, movementSpeed)};
	asteroid->rotation = GetRandomValue(0, 359);
	asteroid->tint = WHITE;
	asteroid->rect = (Rectangle) {0, 0, (float) texWidth, (float) texHeight};
	asteroid->scale = 1;
	return *asteroid;
}

void ScreenLoop(Vector2 *position)
{
	const float screenWidth = (float) GetScreenWidth();
	const float screenHeight = (float) GetScreenHeight();
	const float x_offset = 50.0f;
	const float y_offset = 40.0f;

	if (position->x <= 0 - x_offset) position->x = screenWidth + x_offset;
	else if (position->x >= screenWidth + x_offset) position->x = 0 - x_offset;
	if (position->y <= 0 - y_offset) position->y = screenHeight + y_offset;
	else if (position->y >= screenHeight + y_offset) position->y = 0 - y_offset;
}

void Shoot(Player *player, Bullet *bullet) {
	bullet->position = player->position;
	bullet->rotation = player->rotation;
	bullet->velocity = player->velocity;
	bullet->velocity = Vector2Add(Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), bullet->movementSpeed), bullet->velocity);
	bullet->visible = true;
}

Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime)
{
	Vector2 newPos = (Vector2){
			position->x + (velocity->x * frameTime),
			position->y + (velocity->y * frameTime)
	};

	ScreenLoop(&newPos);

	return newPos;
}

Rectangle UpdateRectangle(Vector2 *position, Texture2D tex, float scale)
{
//	{(float) player.tex.width/2 * player.scale, (float) player.tex.height/2 * player.scale};
//	return (Rectangle){position->x - tex.width/2, position->y + tex.height/2, tex.width * scale, tex.height * scale};
	return (Rectangle){position->x, position->y, tex.width * scale, tex.height * scale};

//	return (Rectangle)
//	{
//		position->x - tex.width/2 * scale,
//		position->y - tex.height/2 * scale,
//		tex.width * scale,
//		tex.height * scale
//	};
}