//
// Created by chris on 7/20/2021.
//

#include "asteroid.h"
#include "utils.h"

Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight)
{
    const float movementSpeed = 50;

    asteroid->position = (Vector2){(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)};
    asteroid->velocity = (Vector2){(float)GetRandomValue(-(int)movementSpeed, (int)movementSpeed), (float)GetRandomValue(-(int)movementSpeed, (int)movementSpeed)};
    asteroid->rotation = (float)GetRandomValue(0, 359);
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