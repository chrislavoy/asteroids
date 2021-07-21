//
// Created by chris on 7/20/2021.
//

#ifndef ASTEROIDS_BULLET_H
#define ASTEROIDS_BULLET_H

#include "raylib.h"
#include "main.h"

Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight);
void UpdateBullets(Bullet *bullets, Asteroid *asteroids, Player *player, Texture2D bulletTexture, int maxBullets, int maxAsteroids, float frameTime);
void DrawBullets(Bullet *bullets, int maxBullets, Texture2D bulletTexture, Rectangle bulletSourceRect, Vector2 bulletOrigin, bool debugMode);

#endif //ASTEROIDS_BULLET_H
