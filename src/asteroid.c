//
// Created by chris on 7/20/2021.
//

#include <stdlib.h>
#include "asteroid.h"
#include "utils.h"

const int movementSpeed = 75;

Asteroid* InitAsteroids(int maxAsteroids, const int screenWidth, const int screenHeight, Texture2D *textures, int texCount)
{
	Asteroid *asteroids = (Asteroid*) calloc(maxAsteroids, sizeof(Asteroid));

	for (int i = 0; i < maxAsteroids; ++i)
	{
		asteroids[i].position = (Vector2){(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)};
		asteroids[i].velocity = (Vector2){(float)GetRandomValue(-movementSpeed, movementSpeed), (float)GetRandomValue(-movementSpeed, movementSpeed)};
		asteroids[i].rotation = (float)GetRandomValue(0, 359);
		asteroids[i].tint = WHITE;
		asteroids[i].scale = 1.0f;
		asteroids[i].colliderRadius = 35.0f;
		asteroids[i].alive = true;
		asteroids[i].level = 3;
		asteroids[i].tex = textures[GetRandomValue(0, texCount)];
		asteroids[i].sourceRect = (Rectangle){0, 0, (float) asteroids[i].tex.width, (float) asteroids[i].tex.height};
		asteroids[i].origin = (Vector2) {(float) asteroids[i].tex.width/2, (float) asteroids[i].tex.height/2};
		asteroids[i].rect = (Rectangle) {0, 0, (float) asteroids[i].tex.width, (float) asteroids[i].tex.height};

		if (Vector2Distance(asteroids[i].position, (Vector2){(float) screenWidth/2, (float) screenHeight/2}) < 200.0f)
			Vector2AddValue(asteroids[i].position, 200);
	}

    return asteroids;
}

void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, float frameTime)
{
    for (int i = 0; i < maxAsteroids; ++i)
    {
        if (asteroids[i].alive)
        {
            if (IsKeyPressed(KEY_MINUS)) asteroids[i].scale -= 0.5f;
            if (IsKeyPressed(KEY_EQUAL)) asteroids[i].scale += 0.5f;

            asteroids[i].position = UpdatePosition(&asteroids[i].position, &asteroids[i].velocity, frameTime);
            asteroids[i].rect = UpdateRectangle(&asteroids[i].position, asteroids[i].tex, asteroids[i].scale);

            if (CheckCollisionCircles(player->position, player->colliderRadius, asteroids[i].position, asteroids[i].colliderRadius))
            {
                player->alive = false;
            }

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

void DrawAsteroids(Asteroid *asteroids, int maxAsteroids, bool debugMode)
{
    for (int i = 0; i < maxAsteroids; ++i)
    {
        if (asteroids[i].alive)
        {
            DrawTexturePro(asteroids[i].tex, asteroids[i].sourceRect, asteroids[i].rect, asteroids[i].origin, asteroids[i].rotation, asteroids[i].tint);

            if (debugMode) DrawCircleLines(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].colliderRadius * asteroids[i].scale, GREEN);
        }
    }
}

bool AnyAsteroidsAlive(Asteroid *asteroids, int maxAsteroids)
{
    int aliveCount = 0;

    for (int i = 0; i < maxAsteroids; ++i)
    {
        if (asteroids[i].alive) aliveCount++;
    }

    return aliveCount > 0;
}

void ResetAsteroids(Asteroid *asteroids, int maxAsteroids)
{
	for (int i = 0; i < maxAsteroids; ++i)
	{
		asteroids[i].position = (Vector2){(float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(0, GetScreenHeight())};
		asteroids[i].velocity = (Vector2){(float)GetRandomValue(-movementSpeed, movementSpeed), (float)GetRandomValue(-movementSpeed, movementSpeed)};
		asteroids[i].rotation = (float)GetRandomValue(0, 359);
		asteroids[i].tint = WHITE;
		asteroids[i].scale = 1.0f;
		asteroids[i].colliderRadius = 35.0f;
		asteroids[i].alive = true;
		asteroids[i].level = 3;

		if (Vector2Distance(asteroids[i].position, (Vector2){(float) GetScreenWidth()/2, (float) GetScreenHeight()/2}) < 200.0f)
			Vector2AddValue(asteroids[i].position, 200);
	}
}
