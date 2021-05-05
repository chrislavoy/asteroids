#include "raylib.h"
#include "raymath.h"

#define MAX_ASTEROIDS 10
#define MAX_BULLETS 25

typedef struct Player
{
    Vector2 position;
    Vector2 acceleration;
    Vector2 speed;
    float rotation;
    float scale;
    Texture2D tex;
    Rectangle rect;
    Color tint;
} Player;

typedef struct Bullet {
    Vector2 position;
    float speed;
    Texture2D tex;
    Color tint;
} Bullet;

//typedef struct Asteroid
//{
//    Vector2 position;
//    Vector2 velocity;
//    float rotation;
//    float speed;
//    Texture2D tex;
//    Rectangle rect;
//    Color tint;
//} Asteroid;

//void CreateBulletInstance(Player *player)
//{
//    Bullet bullet = {0};
//    bullet.tex = LoadTexture("../resources/fire04.png");
//    bullet.tint = WHITE;
//    bullet.speed = 500.0f;
//}

void ClampPlayerPosition(Player *player)
{
    const float screenWidth = (float) GetScreenWidth();
    const float screenHeight = (float) GetScreenHeight();
    const float x_offset = 50.0f;
    const float y_offset = 40.0f;

    if (player->position.x <= x_offset) player->position.x = x_offset;
    if (player->position.x >= screenWidth - x_offset) player->position.x = screenWidth - x_offset;
    if (player->position.y <= y_offset) player->position.y = y_offset;
    if (player->position.y >= screenHeight - y_offset) player->position.y = screenHeight - y_offset;
}

void Shoot(Player *player, Bullet bullets[]) {

}

int main(void)
{
    const int screenWidth = 1024;
    const int screenHeight = 768;
    bool debugMode = false;

    InitWindow(screenWidth, screenHeight, "Asteroid Shooter");

    SetTargetFPS(60);

    Player player = {0};
    player.position = (Vector2) {(float)screenWidth/2, (float)screenHeight/2};
    player.acceleration = Vector2Zero();
    player.speed = Vector2Zero();
    player.rotation = 0.0f;
    player.tint = WHITE;
    player.tex = LoadTexture("../resources/playerShip.png");
    player.rect = (Rectangle) {0, 0, (float) player.tex.width, (float) player.tex.height};
    player.scale = 1.0f;

    Bullet bullet = {0};
    bullet.tex = LoadTexture("../resources/fire04.png");
    bullet.tint = WHITE;
    bullet.speed = 500.0f;

//    Asteroid asteroids[] = {
//            {{0, 0}, {1, 1}, 20.0f, LoadTexture("../Meteors/meteorBrown_big1.png")}
//    };

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};
    Rectangle destRect;
    Vector2 origin = {(float) player.tex.width/2, (float) player.tex.height/2};

    float frameTime;

    while (!WindowShouldClose())
    {
        // ----------------------------------------------
        // Update variables
        // ----------------------------------------------
//        frameTime = GetFrameTime();

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        player.acceleration = (Vector2){10, 10};

        if (IsKeyDown(KEY_W))
        {
        	player.acceleration = Vector2Scale(Vector2Rotate(player.acceleration, player.rotation), 5.0f);
        }
        if (IsKeyDown(KEY_S))
        {
        	player.acceleration = Vector2Scale(Vector2Rotate(player.acceleration, player.rotation), -5.0f);
        }
        if (IsKeyDown(KEY_A)) player.rotation -= 5;
        if (IsKeyDown(KEY_D)) player.rotation += 5;

        if (player.acceleration.x != 0 && player.acceleration.y != 0)
            player.speed = Vector2Multiply(player.speed, player.acceleration);

        player.position = Vector2Add(player.position, player.speed);

//        if (IsKeyDown(KEY_SPACE)) Shoot(&player, );

        ClampPlayerPosition(&player);

        destRect = (Rectangle){player.position.x, player.position.y, (float) player.tex.width, (float) player.tex.height};

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2) {0, 0}, 0.0f, 1.0f, WHITE);
        DrawTexturePro(player.tex, player.rect, destRect, origin, player.rotation, player.tint);

        if (debugMode)
        {
            DrawFPS(5, 0);
            DrawText(TextFormat("Player Pos: (%.2f, %.2f)", player.position.x, player.position.y), 5, 20, 20, RAYWHITE);
            DrawText(TextFormat("Player Speed: (%.2f, %.2f)", player.speed.x, player.speed.y), 5, 40, 20, RAYWHITE);
            DrawText(TextFormat("Player Acceleration: (%.2f, %.2f)", player.acceleration.x, player.acceleration.y), 5, 60, 20, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}