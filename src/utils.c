//
// Created by Chris LaVoy on 5/16/21.
//

#include "utils.h"
#include "player.h"
#include "bullet.h"
#include "asteroid.h"

#define MAX_ASTEROIDS 20
#define MAX_BULLETS 20

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

void ResetGame(Player *player, Asteroid *asteroids, Bullet *bullets, Texture2D bulletTexture, int maxAsteroids)
{
    *player = InitPlayer(GetScreenWidth(), GetScreenHeight(), player, false);

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = InitBullet(&bullets[i], bulletTexture.width, bulletTexture.height);
    }

	ResetAsteroids(asteroids, maxAsteroids);

    bool checkForCollisions = true;

    while(checkForCollisions)
    {
        bool foundCollision = false;

        for (int i = 0; i < MAX_ASTEROIDS; ++i) {
            for (int j = 0; j < MAX_ASTEROIDS; ++j) {
                if (i != j)
                {
                    if (Vector2Distance(asteroids[i].position, asteroids[j].position) < 100.0f)
                    {
                        asteroids[i].position.x += 200;
                        asteroids[j].position.y += 200;
                        foundCollision = true;
                    }

                    if (Vector2Distance(asteroids[i].position, (Vector2){(float) GetScreenWidth()/2, (float) GetScreenHeight()/2}) < 200.0f)
                    {
                        asteroids[i].position.x += 200;
                        asteroids[j].position.y += 200;
                        foundCollision = true;
                    }
                }
            }
        }

        if (!foundCollision)
            checkForCollisions = false;
    }
}