//
// Created by chris on 7/20/2021.
//

#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "raylib.h"
#include "raymath.h"
#include "main.h"

Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight);
void UpdateAsteroids(Asteroid *asteroids, int maxAsteroids, Player *player, Texture2D asteroidTexture, float frameTime);
void DrawAsteroids(Asteroid *asteroids, int maxAsteroids, Texture2D asteroidTexture, Rectangle asteroidSourceRect, Vector2 asteroidOrigin, bool debugMode);
bool AnyAsteroidsAlive(Asteroid *asteroids, int maxAsteroids);

#endif //ASTEROIDS_ASTEROID_H
