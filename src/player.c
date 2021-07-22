//
// Created by chris on 7/19/2021.
//

#include "player.h"
#include "utils.h"

#define MAX_VELOCITY 250.0f
#define MAX_BULLETS 20
#define SHOT_COOLDOWN 0.2f
#define BULLET_LIFETIME 0.5f

int upKey;
int downKey;
int leftKey;
int rightKey;

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player, bool arrowKeys)
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
    player->movementSpeed = 250.0f;
    player->colliderRadius = 40.0f;
    player->shootCooldown = 0.0f;
    player->score = 0;
    player->alive = true;

    if (arrowKeys)
    {
	    upKey = KEY_UP;
	    downKey = KEY_DOWN;
	    leftKey = KEY_LEFT;
	    rightKey = KEY_RIGHT;
    }
    else
    {
	    upKey = KEY_W;
	    downKey = KEY_S;
	    leftKey = KEY_A;
	    rightKey = KEY_D;
    }

    return *player;
}

void UpdatePlayer(Player *player, int *bulletIterator, Bullet *bullets, int maxBullets, float frameTime)
{
    if (player->alive)
    {
        player->acceleration = (Vector2) {0, 0};
        player->shootCooldown -= 1 * frameTime;
        if (player->shootCooldown <= 0) player->shootCooldown = 0;

        if (IsKeyDown(upKey)) player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), player->movementSpeed);
        if (IsKeyDown(downKey)) player->acceleration = Vector2Scale(Vector2Rotate((Vector2){0, 1}, player->rotation), player->movementSpeed);
        if (IsKeyDown(leftKey)) player->rotation -= 180 * frameTime;
        if (IsKeyDown(rightKey)) player->rotation += 180 * frameTime;
        if (IsKeyDown(KEY_X)) player->velocity = (Vector2) {0, 0};
        if (IsKeyDown(KEY_SPACE) && player->shootCooldown <= 0)
        {
            *bulletIterator = (*bulletIterator + 1) % ((int)maxBullets - 1);
            Shoot(player, &bullets[*bulletIterator]);
            player->shootCooldown = SHOT_COOLDOWN;
        }

        player->velocity = Vector2Add(player->velocity, Vector2Scale(player->acceleration, frameTime));
        if (Vector2Length(player->velocity) > MAX_VELOCITY)
        {
            player->velocity = Vector2Scale(Vector2Normalize(player->velocity), MAX_VELOCITY);
            Vector2Scale(player->velocity, frameTime);
        }

        player->position = UpdatePosition(&player->position, &player->velocity, frameTime);
        player->rect = UpdateRectangle(&player->position, player->tex, 1);

        if (player->rotation > 360) player->rotation -= 360.0f;
        if (player->rotation < 0) player->rotation += 360.0f;
    }
}

void DrawPlayer(Player *player, bool debugMode)
{
    if (player->alive)
    {
        DrawTexturePro(player->tex, player->sourceRect, player->rect, player->origin, player->rotation, player->tint);
        if (debugMode) DrawCircleLines((int) player->position.x, (int) player->position.y, player->colliderRadius, GREEN);
    }
}

void Shoot(Player *player, Bullet *bullet) {
    bullet->position = player->position;
    bullet->rotation = player->rotation;
    bullet->velocity = player->velocity;
    bullet->velocity = Vector2Add(Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), bullet->movementSpeed), bullet->velocity);
    bullet->visible = true;
    bullet->lifetime = BULLET_LIFETIME;
}