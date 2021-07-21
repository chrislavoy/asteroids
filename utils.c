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
	bullet->colliderRadius = 15.0f;
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
	asteroid->scale = 1.0f;
    asteroid->colliderRadius = 35.0f;
    asteroid->alive = true;
    asteroid->level = 3;

    if (Vector2Distance(asteroid->position, (Vector2){(float) screenWidth/2, (float) screenHeight/2}) < 200.0f)
        Vector2AddValue(asteroid->position, 200);

	return *asteroid;
}

void UpdateBullets(Bullet *bullets, Asteroid *asteroids, Texture2D bulletTexture, int maxBullets, float bulletLifetime, int maxAsteroids, float frameTime)
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].visible)
        {
            bullets[i].position = UpdatePosition(&bullets[i].position, &bullets[i].velocity, frameTime);
            bullets[i].rect = UpdateRectangle(&bullets[i].position, bulletTexture, bullets[i].scale);
            bullets[i].lifetime -= 1 * frameTime;

            for (int j = 0; j < maxAsteroids; j++)
            {
                if (CheckCollisionCircles(bullets[i].position, bullets[i].colliderRadius, asteroids[j].position, asteroids[j].colliderRadius))
                {
                    asteroids[j].tint = RED;
                    asteroids[j].alive = false;
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
        if (asteroids[i].alive)
        {
            if (IsKeyPressed(KEY_MINUS)) asteroids[i].scale -= 0.5f;
            if (IsKeyPressed(KEY_EQUAL)) asteroids[i].scale += 0.5f;

            asteroids[i].position = UpdatePosition(&asteroids[i].position, &asteroids[i].velocity, frameTime);
            asteroids[i].rect = UpdateRectangle(&asteroids[i].position, asteroidTexture, asteroids[i].scale);

            if (CheckCollisionCircles(player->position, player->colliderRadius, asteroids[i].position, asteroids[i].colliderRadius))
                player->tint = RED;

            for (int j = 0; j < maxAsteroids; j++)
            {
                if (i != j)
                {
                    if (asteroids[j].alive)
                    {
                        if (CheckCollisionCircles(asteroids[i].position, asteroids[i].colliderRadius, asteroids[j].position, asteroids[j].colliderRadius))
                        {
                            // TODO: Rework collision logic
//                    if (asteroids[i].rect.x < asteroids[j].rect.x)
//                    {
//                        asteroids[i].velocity = (Vector2){-asteroids[i].velocity.x, asteroids[i].velocity.y};
//                        asteroids[j].velocity = (Vector2){-asteroids[j].velocity.x, asteroids[j].velocity.y};
//                    }
//                    else
//                    {
//                        asteroids[i].velocity = (Vector2){asteroids[i].velocity.x, -asteroids[i].velocity.y};
//                        asteroids[j].velocity = (Vector2){asteroids[j].velocity.x, -asteroids[j].velocity.y};
//                    }
//                    asteroids[i].velocity = Vector2Reflect(asteroids[i].velocity, asteroids[j].velocity);
//                    asteroids[j].velocity = Vector2Reflect(asteroids[j].velocity, asteroids[i].velocity);

                            asteroids[i].velocity = Vector2Rotate(asteroids[i].velocity, Vector2Angle(asteroids[i].velocity, asteroids[j].velocity));
                            asteroids[j].velocity = Vector2Rotate(asteroids[j].velocity, Vector2Angle(asteroids[j].velocity, asteroids[i].velocity));
                        }
                    }
                }
            }
        }
    }
}

void DrawBullets(Bullet *bullets, int maxBullets, Texture2D bulletTexture, Rectangle bulletSourceRect, Vector2 bulletOrigin, bool debugMode)
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].visible)
        {
            DrawTexturePro(bulletTexture, bulletSourceRect, bullets[i].rect, Vector2Scale(bulletOrigin, bullets[i].scale), bullets[i].rotation, bullets[i].tint);
            if (debugMode) DrawCircleLines(bullets[i].position.x, bullets[i].position.y, bullets[i].colliderRadius, GREEN);
        }
    }
}

void DrawAsteroids(Asteroid *asteroids, int maxAsteroids, Texture2D asteroidTexture, Rectangle asteroidSourceRect, Vector2 asteroidOrigin, bool debugMode)
{
    for (int i = 0; i < maxAsteroids; ++i)
    {
        if (asteroids[i].alive)
        {
            DrawTexturePro(asteroidTexture, asteroidSourceRect, asteroids[i].rect, asteroidOrigin, asteroids[i].rotation, asteroids[i].tint);

            if (debugMode) DrawCircleLines(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].colliderRadius * asteroids[i].scale, GREEN);
        }
    }
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
    return (Rectangle)
    {
        position->x,
        position->y,
        (float) tex.width * scale,
        (float) tex.height * scale
    };
}