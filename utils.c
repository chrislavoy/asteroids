//
// Created by Chris LaVoy on 5/16/21.
//

#include "utils.h"
#include "raylib.h"
#include "raymath.h"

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

void UpdateBullets(Bullet *bullets, Asteroid *asteroids, Texture2D bulletTexture, int maxBullets, float bulletLifetime, int maxAsteroids, float frameTime)
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].visible)
        {
            bullets[i].position = UpdatePosition(&bullets[i].position, &bullets[i].velocity, frameTime);
            bullets[i].rect = UpdateRectangle(&bullets[i].position, bulletTexture, bullets[i].scale);
//                bullets[i].rect = (Rectangle){bullets[i].position.x, bullets[i].position.y, bulletTexture.width * bullets[i].scale, bulletTexture.height * bullets[i].scale};
            bullets[i].lifetime -= 1 * frameTime;

            for (int j = 0; j < maxAsteroids; j++)
            {
                if (CheckCollisionRecs(bullets[i].rect, asteroids[j].rect))
                {
                    asteroids[j].tint = RED;
                }
            }

            if (bullets[i].lifetime <= 0)
            {
                bullets[i].visible = false;
                bullets[i].lifetime = bulletLifetime;
            }
        }
    }
}

void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, Texture2D asteroidTexture, float frameTime)
{
    for (int i = 0; i < maxAsteroids; ++i)
    {
        asteroids[i].position = UpdatePosition(&asteroids[i].position, &asteroids[i].velocity, frameTime);
        asteroids[i].rect = UpdateRectangle(&asteroids[i].position, asteroidTexture, asteroids[i].scale);
//            asteroids[i].rect = (Rectangle){asteroids[i].position.x, asteroids[i].position.y, asteroidTexture.width * asteroids[i].scale, asteroidTexture.height * asteroids[i].scale};

        if (CheckCollisionRecs(asteroids[i].rect, player->rect))
            player->tint = RED;

        for (int j = 0; j < maxAsteroids; j++)
        {
            if (i != j)
            {
                if (CheckCollisionRecs(asteroids[i].rect, asteroids[j].rect))
                {
                    // TODO: Rework collision logic
//            			asteroids[i].velocity = (Vector2){asteroids[i].velocity.x * -1, asteroids[i].velocity.y * -1};
//			            asteroids[j].velocity = (Vector2){asteroids[j].velocity.x * -1, asteroids[j].velocity.y * -1};
//	                    Rectangle collisionRect = GetCollisionRec(asteroids[i].rect, asteroids[j].rect);
//			            TraceLog(LOG_INFO, TextFormat("x: %f, y: %f, width: %f, height: %f", collisionRect.x, collisionRect.y, collisionRect.width, collisionRect.height));
                    if (asteroids[i].rect.x < asteroids[j].rect.x)
                    {
                        asteroids[i].velocity = (Vector2){-asteroids[i].velocity.x, asteroids[i].velocity.y};
                        asteroids[j].velocity = (Vector2){-asteroids[j].velocity.x, asteroids[j].velocity.y};
                    }
                    else
                    {
                        asteroids[i].velocity = (Vector2){asteroids[i].velocity.x, -asteroids[i].velocity.y};
                        asteroids[j].velocity = (Vector2){asteroids[j].velocity.x, -asteroids[j].velocity.y};
                    }
                }
            }
        }
    }
}