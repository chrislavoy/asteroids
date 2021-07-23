//
// Created by chris on 7/20/2021.
//

#include <stdlib.h>
#include "asteroid.h"
#include "utils.h"

int activeAsteroidCount = 0;
float colliderSize[3] = {35.0f, 15.0f, 10.0f};
int movementSpeed[3] = {75, 125, 200};

Asteroid* InitAsteroids(int maxAsteroids, const int screenWidth, const int screenHeight, Texture2D *textures)
{
	Asteroid *asteroids = (Asteroid*) calloc(maxAsteroids, sizeof(Asteroid));
	activeAsteroidCount = maxAsteroids;

	for (int i = 0; i < maxAsteroids; ++i)
	{
	    int level = GetRandomValue(1, 3);
        asteroids[i].level = level;
		asteroids[i].position = (Vector2){(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)};
		asteroids[i].velocity = (Vector2){(float)GetRandomValue(-movementSpeed[level - 1], movementSpeed[level - 1]), (float)GetRandomValue(-movementSpeed[level - 1], movementSpeed[level - 1])};
		asteroids[i].rotation = (float)GetRandomValue(0, 359);
		asteroids[i].tint = WHITE;
		asteroids[i].colliderRadius = colliderSize[asteroids[i].level - 1];
		asteroids[i].alive = true;
        switch (asteroids[i].level) {
            case 1:
                asteroids[i].tex = textures[GetRandomValue(0, 3)];
                break;
            case 2:
                asteroids[i].tex = textures[GetRandomValue(4, 5)];
                break;
            case 3:
                asteroids[i].tex = textures[GetRandomValue(6, 7)];
                break;
        }
        asteroids[i].sourceRect = (Rectangle){0, 0, (float) asteroids[i].tex.width, (float) asteroids[i].tex.height};
        asteroids[i].origin = (Vector2) {(float) asteroids[i].tex.width/2, (float) asteroids[i].tex.height/2};
        asteroids[i].rect = (Rectangle) {0, 0, (float) asteroids[i].tex.width, (float) asteroids[i].tex.height};

		if (Vector2Distance(asteroids[i].position, (Vector2){(float) screenWidth/2, (float) screenHeight/2}) < 200.0f)
			Vector2AddValue(asteroids[i].position, 200);
	}

    return asteroids;
}

void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, float frameTime, Sound playerExplosion)
{
    for (int i = 0; i < maxAsteroids; ++i)
    {
        if (asteroids[i].alive)
        {
            asteroids[i].position = UpdatePosition(&asteroids[i].position, &asteroids[i].velocity, frameTime);
            asteroids[i].rect = UpdateRectangle(&asteroids[i].position, asteroids[i].tex, 1);

            if (player->alive)
            {
                if (CheckCollisionCircles(player->position, player->colliderRadius, asteroids[i].position, asteroids[i].colliderRadius))
                {
                    player->alive = false;
                    PlaySound(playerExplosion);
                }

            }
            for (int j = 0; j < maxAsteroids; j++)
            {
                if (i != j)
                {
                    if (asteroids[j].alive)
                    {
                        if (CheckCollisionCircles(asteroids[i].position, asteroids[i].colliderRadius, asteroids[j].position, asteroids[j].colliderRadius))
                        {
                            Vector2 collisionNormal = Vector2Subtract(asteroids[i].velocity, asteroids[j].velocity);
                            float angleI = Vector2Angle(asteroids[i].velocity, collisionNormal);
                            float angleJ = Vector2Angle(asteroids[j].velocity, collisionNormal);

                            asteroids[i].velocity = Vector2Rotate(asteroids[i].velocity, angleI);
                            asteroids[j].velocity = Vector2Rotate(asteroids[j].velocity, angleJ);
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

            if (debugMode)
            {
                DrawCircleLines(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].colliderRadius, GREEN);
                DrawText(TextFormat("Level: %d", asteroids[i].level), asteroids[i].position.x, asteroids[i].position.y, 10, RAYWHITE);
                DrawText(TextFormat("Collider: %f", asteroids[i].colliderRadius), asteroids[i].position.x, asteroids[i].position.y + 10, 10, RAYWHITE);
                DrawText(TextFormat("Tex ID: %d", asteroids[i].tex.id), asteroids[i].position.x, asteroids[i].position.y + 20, 10, RAYWHITE);
            }
        }
    }
}

bool AnyAsteroidsAlive()
{
	return activeAsteroidCount > 0;
}

void ResetAsteroids(Asteroid *asteroids, int maxAsteroids)
{
	for (int i = 0; i < maxAsteroids; ++i)
	{
		asteroids[i].position = (Vector2){(float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(0, GetScreenHeight())};
		asteroids[i].velocity = (Vector2){(float)GetRandomValue(-movementSpeed[asteroids[i].level - 1], movementSpeed[asteroids[i].level - 1]), (float)GetRandomValue(-movementSpeed[asteroids[i].level - 1], movementSpeed[asteroids[i].level - 1])};
		asteroids[i].rotation = (float)GetRandomValue(0, 359);
//		asteroids[i].tint = WHITE;
//        asteroids[i].colliderRadius = colliderSize[asteroids[i].level - 1];
		asteroids[i].alive = true;
//		asteroids[i].level = 3;

		if (Vector2Distance(asteroids[i].position, (Vector2){(float) GetScreenWidth()/2, (float) GetScreenHeight()/2}) < 200.0f)
			Vector2AddValue(asteroids[i].position, 200);
	}

	activeAsteroidCount = maxAsteroids;
}

int GetAsteroidsAliveCount()
{
	return activeAsteroidCount;
}

void DecrementAsteroidCount()
{
	activeAsteroidCount--;
}
