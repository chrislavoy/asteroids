//
// Created by chris on 7/20/2021.
//

#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "raylib.h"
#include "raymath.h"
#include "main.h"

Asteroid* InitAsteroids(int maxAsteroids, const int screenWidth, const int screenHeight, Texture2D *textures);
void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, float frameTime, Sound playerExplosion);
void DrawAsteroids(Asteroid *asteroids, int maxAsteroids, bool debugMode);
bool AnyAsteroidsAlive();
int GetAsteroidsAliveCount();
void DecrementAsteroidCount();
void ResetAsteroids(Asteroid *asteroids, int maxAsteroids);

#endif //ASTEROIDS_ASTEROID_H
