//
// Created by chris on 7/19/2021.
//

#include "raylib.h"
#include "raymath.h"
#include "utils.h"

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player)
{
    player->position = (Vector2) {(float)screenWidth / 2, (float)screenHeight / 2};
    player->acceleration = Vector2Zero();
    player->velocity = Vector2Zero();
    player->rotation = 0.0f;
    player->tint = WHITE;
    player->tex = LoadTexture("../resources/playerShip.png");
    player->rect = (Rectangle) {0, 0, (float) (*player).tex.width, (float) (*player).tex.height};
    player->origin = (Vector2) {(float) player->tex.width/2 * player->scale, (float) player->tex.height/2 * player->scale};
    player->scale = 1.0f;
    player->sourceRect = (Rectangle) {0, 0, player->tex.width * player->scale, player->tex.height * player->scale};
    player->movementSpeed = 5.0f;
    return *player;
}

void UpdatePlayer(Player *player, int bulletIterator, Bullet *bullets, int maxBullets, float frameTime)
{
    player->acceleration = (Vector2){0, 0};

    if (IsKeyDown(KEY_W))
        player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), player->movementSpeed);
    if (IsKeyDown(KEY_S))
        player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, 1}, player->rotation), player->movementSpeed);
    if (IsKeyDown(KEY_A)) player->rotation -= 5;
    if (IsKeyDown(KEY_D)) player->rotation += 5;

    if (IsKeyDown(KEY_X)) player->velocity = (Vector2){0, 0};

    if (IsKeyPressed(KEY_SPACE))
    {
        bulletIterator = (bulletIterator + 1) % ((int)maxBullets - 1);
        Shoot(player, &bullets[bulletIterator]);
    }

    player->velocity = Vector2Add(player->velocity, player->acceleration);

    player->position = UpdatePosition(&player->position, &player->velocity, frameTime);

    player->rect = UpdateRectangle(&player->position, player->tex, player->scale);
}

void DrawPlayer(Player *player)
{
    DrawRectangleRec(player->rect, BLUE);
    DrawTexturePro(player->tex, player->sourceRect, player->rect, player->origin, player->rotation, player->tint);
}