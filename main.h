//
// Created by chris on 7/20/2021.
//

#ifndef ASTEROIDS_MAIN_H
#define ASTEROIDS_MAIN_H

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float rotation;
    float scale;
    float movementSpeed;
    Texture2D tex;
    Rectangle rect;
    Rectangle sourceRect;
    Vector2 origin;
    Color tint;
    float colliderRadius;
    float shootCooldown;
    int score;
} Player;

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float scale;
    float movementSpeed;
    float lifetime;
    Rectangle rect;
    Color tint;
    bool visible;
    float colliderRadius;
} Bullet;

typedef struct Asteroid
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float scale;
    Rectangle rect;
    Color tint;
    float colliderRadius;
    bool alive;
    int level;
} Asteroid;

#endif //ASTEROIDS_MAIN_H
