#include "raylib.h"
#include "raymath.h"
#include "main.h"
#include "bullet.h"
#include "asteroid.h"
#include "player.h"
#include "utils.h"

#define MAX_ASTEROIDS 20
#define MAX_BULLETS 20

int main(void)
{
    const int screenWidth = 1024;
    const int screenHeight = 768;
    bool debugMode = false;

    InitWindow(screenWidth, screenHeight, "Asteroid Shooter");

    SetTargetFPS(60);

    Player player = {0};

	Texture2D bulletTexture = LoadTexture("../resources/fire04.png");
    Bullet bullets[MAX_BULLETS];

	Rectangle bulletSourceRect = (Rectangle){0, 0, (float) bulletTexture.width, (float) bulletTexture.height};
	Vector2 bulletOrigin = {(float) bulletTexture.width/2, (float) bulletTexture.height/2};

    int bulletIterator = 0;

    Texture2D asteroidTexture = LoadTexture("../resources/Meteors/meteorBrown_big1.png");
    Asteroid asteroids[MAX_ASTEROIDS];

	Rectangle asteroidSourceRect = (Rectangle){0, 0, (float) asteroidTexture.width, (float) asteroidTexture.height};
	Vector2 asteroidOrigin = {(float) asteroidTexture.width/2, (float) asteroidTexture.height/2};

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};

    ResetGame(&player, asteroids, bullets, bulletTexture, asteroidTexture);

    while (!WindowShouldClose())
    {
        // ----------------------------------------------
        // Update variables
        // ----------------------------------------------
        float frameTime = GetFrameTime();

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        if (IsKeyPressed(KEY_R)) ResetGame(&player, asteroids, bullets, bulletTexture, asteroidTexture);

        player.acceleration = (Vector2){0, 0};

        UpdatePlayer(&player, &bulletIterator, bullets, MAX_BULLETS, frameTime);

        UpdateBullets(bullets, asteroids, &player, bulletTexture, MAX_BULLETS, MAX_ASTEROIDS, frameTime);

        UpdateAsteroids(asteroids, MAX_ASTEROIDS, &player, asteroidTexture, frameTime);

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);

        DrawPlayer(&player, debugMode);

        DrawBullets(bullets, MAX_BULLETS, bulletTexture, bulletSourceRect, bulletOrigin, debugMode);

        DrawAsteroids(asteroids, MAX_ASTEROIDS, asteroidTexture, asteroidSourceRect, asteroidOrigin, debugMode);

        if (debugMode)
        {
            DrawFPS(5, 0);
            DrawText(TextFormat("Player Pos: (%.2f, %.2f)", player.position.x, player.position.y), 5, 20, 20, RAYWHITE);
            DrawText(TextFormat("Player Velocity: (%.2f, %.2f)", player.velocity.x, player.velocity.y), 5, 40, 20, RAYWHITE);
            DrawText(TextFormat("Player Acceleration: (%.2f, %.2f)", player.acceleration.x, player.acceleration.y), 5, 60, 20, RAYWHITE);
	        DrawText(TextFormat("Player Rectangle: (%.2f, %.2f, %.2f, %.2f", player.rect.x, player.rect.y, player.rect.width, player.rect.height), 5, 80, 20, RAYWHITE);
            DrawText(TextFormat("Player Rotation: %.2f", player.rotation), 5, 100, 20, RAYWHITE);
            DrawText(TextFormat("Bullet Iterator: %d", bulletIterator), 5, 120, 20, RAYWHITE);
            DrawText(TextFormat("Shot Cooldown: %f", player.shootCooldown), 5, 140, 20, RAYWHITE);
            DrawLineV(player.position, Vector2Add(player.position, Vector2Scale(player.velocity, 2)), GREEN);
        }

        DrawText(TextFormat("Score: %d", player.score), screenWidth/2 - 45, 20, 20, RAYWHITE);

        if (!player.alive || !AnyAsteroidsAlive(asteroids, MAX_ASTEROIDS)) DrawText("Press 'R' to reset", GetScreenWidth()/3, GetScreenHeight()/2, 40, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}