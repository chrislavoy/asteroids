//
// Created by chris on 7/20/2021.
//

#ifndef ASTEROIDS_PLAYER_H
#define ASTEROIDS_PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "main.h"

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player, bool arrowKeys);
void UpdatePlayer(Player *player, int *bulletIterator, Bullet *bullets, int maxBullets, float frameTime, Sound shootSound);
void DrawPlayer(Player *player, bool debugMode);
void Shoot(Player *player, Bullet *bullet);
void UnloadPlayer(Player *player);

#endif //ASTEROIDS_PLAYER_H
