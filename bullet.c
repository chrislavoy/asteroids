//
// Created by chris on 7/20/2021.
//

#include "bullet.h"
#include "utils.h"

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

void UpdateBullets(Bullet *bullets, struct Asteroid *asteroids, struct Player *player, Texture2D bulletTexture, int maxBullets, int maxAsteroids, float frameTime)
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
                if (asteroids[j].alive)
                {
                    if (CheckCollisionCircles(bullets[i].position, bullets[i].colliderRadius, asteroids[j].position, asteroids[j].colliderRadius))
                    {
                        asteroids[j].tint = RED;
                        asteroids[j].alive = false;
                        bullets[i].visible = false;
                        player->score += 10;
                    }
                }
            }

            if (bullets[i].lifetime <= 0)
            {
                bullets[i].visible = false;
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
            if (debugMode) DrawCircleLines((int)bullets[i].position.x, (int)bullets[i].position.y, bullets[i].colliderRadius, GREEN);
        }
    }
}