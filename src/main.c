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

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "Asteroids");

    InitAudioDevice();

    SetTargetFPS(60);

    Player player = {0};

	Texture2D bulletTexture = LoadTexture("../resources/fire04.png");
    Bullet bullets[MAX_BULLETS];

	Rectangle bulletSourceRect = (Rectangle){0, 0, (float) bulletTexture.width, (float) bulletTexture.height};
	Vector2 bulletOrigin = {(float) bulletTexture.width/2, (float) bulletTexture.height/2};

    int bulletIterator = 0;

	Texture2D asteroidTextures[8] = {
			LoadTexture("../resources/Meteors/meteorBrown_big1.png"),
			LoadTexture("../resources/Meteors/meteorBrown_big2.png"),
			LoadTexture("../resources/Meteors/meteorBrown_big3.png"),
			LoadTexture("../resources/Meteors/meteorBrown_big4.png"),
            LoadTexture("../resources/Meteors/meteorBrown_med1.png"),
            LoadTexture("../resources/Meteors/meteorBrown_med3.png"),
            LoadTexture("../resources/Meteors/meteorBrown_small1.png"),
            LoadTexture("../resources/Meteors/meteorBrown_small2.png")
	};

	Asteroid *asteroids = InitAsteroids(MAX_ASTEROIDS, GetScreenWidth(), GetScreenHeight(), asteroidTextures);

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};

    Sound asteroidExplosion = LoadSound("../resources/Sounds/lowExplosion.ogg");
    Sound shootSound = LoadSound("../resources/Sounds/laser.ogg");
    Sound playerExplosion = LoadSound("../resources/Sounds/explosion.ogg");

    SetMasterVolume(70);

    ResetGame(&player, asteroids, bullets, bulletTexture, MAX_ASTEROIDS);

    while (!WindowShouldClose())
    {
        // ----------------------------------------------
        // Update variables
        // ----------------------------------------------
        float frameTime = GetFrameTime();

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        if (IsKeyPressed(KEY_R)) ResetGame(&player, asteroids, bullets, bulletTexture, MAX_ASTEROIDS);

        player.acceleration = (Vector2){0, 0};

        UpdatePlayer(&player, &bulletIterator, bullets, MAX_BULLETS, frameTime, shootSound);

        UpdateBullets(bullets, asteroids, &player, bulletTexture, MAX_BULLETS, MAX_ASTEROIDS, frameTime, asteroidExplosion);

        UpdateAsteroids(asteroids, MAX_ASTEROIDS, &player, frameTime, playerExplosion);

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);

        DrawPlayer(&player, debugMode);

        DrawBullets(bullets, MAX_BULLETS, bulletTexture, bulletSourceRect, bulletOrigin, debugMode);

        DrawAsteroids(asteroids, MAX_ASTEROIDS, debugMode);

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
	        DrawText(TextFormat("Asteroids: %d", GetAsteroidsAliveCount()), 5, 160, 20, RAYWHITE);
            DrawLineV(player.position, Vector2Add(player.position, Vector2Scale(player.velocity, 2)), GREEN);
        }

        DrawText(TextFormat("Score: %d", player.score), screenWidth/2 - 45, 20, 20, RAYWHITE);

        if (!player.alive || !AnyAsteroidsAlive()) DrawText("Press 'R' to reset", GetScreenWidth()/3, GetScreenHeight()/2, 40, RAYWHITE);

        EndDrawing();
    }

	UnloadPlayer(&player);

	UnloadTexture(bulletTexture);

	for (int i = 0; i < 4; ++i)
	{
		UnloadTexture(asteroidTextures[i]);
	}

	CloseAudioDevice();

    CloseWindow();

    return 0;
}