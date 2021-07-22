//
// Created by Chris LaVoy on 5/16/21.
//

#ifndef SPACE_SHOOTER_UTILS_H
#define SPACE_SHOOTER_UTILS_H

#include "raylib.h"
#include "raymath.h"
#include "main.h"

void ScreenLoop(Vector2 *position);
Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime);
Rectangle UpdateRectangle(Vector2 *position, Texture2D tex, float scale);
void ResetGame(Player *player, Asteroid *asteroids, Bullet *bullets, Texture2D bulletTexture, int maxAsteroids);

#endif //SPACE_SHOOTER_UTILS_H