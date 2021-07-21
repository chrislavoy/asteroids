//
// Created by chris on 7/19/2021.
//

#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#define MAX_VELOCITY 150.0f
#define SHOT_COOLDOWN 0.2f
#define BULLET_LIFETIME 0.8f

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player)
{
    player->position = (Vector2) {(float)screenWidth / 2, (float)screenHeight / 2};
    player->acceleration = Vector2Zero();
    player->velocity = Vector2Zero();
    player->rotation = 0.0f;
    player->tint = WHITE;
    player->tex = LoadTexture("../resources/playerShip.png");
    player->rect = (Rectangle) {0, 0, (float) (*player).tex.width * player->scale, (float) (*player).tex.height * player->scale};
    player->origin = (Vector2) {(float) player->tex.width/2, (float) player->tex.height/2};
    player->scale = 1.0f;
    player->sourceRect = (Rectangle) {0, 0, (float)player->tex.width, (float)player->tex.height};
    player->movementSpeed = 5.0f;
    player->colliderRadius = 40.0f;
    player->shootCooldown = 0.0f;
    player->score = 0;
    return *player;
}

void UpdatePlayer(Player *player, int *bulletIterator, Bullet *bullets, int maxBullets, float frameTime)
{
    player->acceleration = (Vector2) {0, 0};
    player->shootCooldown -= 1 * frameTime;
    if (player->shootCooldown <= 0) player->shootCooldown = 0;

    if (IsKeyDown(KEY_W)) player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), player->movementSpeed);
    if (IsKeyDown(KEY_S)) player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, 1}, player->rotation), player->movementSpeed);
    if (IsKeyDown(KEY_A)) player->rotation -= 5;
    if (IsKeyDown(KEY_D)) player->rotation += 5;
    if (IsKeyDown(KEY_X)) player->velocity = (Vector2) {0, 0};
    if (IsKeyDown(KEY_SPACE) && player->shootCooldown <= 0)
    {
        *bulletIterator = (*bulletIterator + 1) % ((int)maxBullets - 1);
        Shoot(player, &bullets[*bulletIterator]);
        player->shootCooldown = SHOT_COOLDOWN;
    }

    player->velocity = Vector2Add(player->velocity, player->acceleration);
    if (Vector2Length(player->velocity) > MAX_VELOCITY)
        player->velocity = Vector2Scale(Vector2Normalize(player->velocity), MAX_VELOCITY);

    player->position = UpdatePosition(&player->position, &player->velocity, frameTime);
    player->rect = UpdateRectangle(&player->position, player->tex, 1);

    if (player->rotation > 360) player->rotation -= 360.0f;
    if (player->rotation < 0) player->rotation += 360.0f;
}

void DrawPlayer(Player *player, bool debugMode)
{
    DrawTexturePro(player->tex, player->sourceRect, player->rect, player->origin, player->rotation, player->tint);
    if (debugMode) DrawCircleLines((int) player->position.x, (int) player->position.y, player->colliderRadius, GREEN);
}

void Shoot(Player *player, Bullet *bullet) {
    bullet->position = player->position;
    bullet->rotation = player->rotation;
    bullet->velocity = player->velocity;
    bullet->velocity = Vector2Add(Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), bullet->movementSpeed), bullet->velocity);
    bullet->visible = true;
    bullet->lifetime = BULLET_LIFETIME;
}