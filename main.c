#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#define MAX_ASTEROIDS 1
#define MAX_BULLETS 20
#define BULLET_LIFETIME 0.8f

int main(void)
{
    const int screenWidth = 1024;
    const int screenHeight = 768;
    bool debugMode = false;

    InitWindow(screenWidth, screenHeight, "Asteroid Shooter");

    SetTargetFPS(60);

    Player player = {0};
	player = InitPlayer(screenWidth, screenHeight, &player);
//	Vector2 playerOrigin = {(float) player.tex.width/2 * player.scale, (float) player.tex.height/2 * player.scale};
//	Vector2 playerOrigin = {0, 0};
//	Rectangle playerSourceRect = {0, 0, player.tex.width * player.scale, player.tex.height * player.scale};

	Texture2D bulletTexture = LoadTexture("../resources/fire04.png");
    Bullet bullets[MAX_BULLETS];

    for (int i = 0; i < MAX_BULLETS; i++)
    {
		bullets[i] = InitBullet(&bullets[i], bulletTexture.width, bulletTexture.height);
    }
	Rectangle bulletSourceRect = (Rectangle){0, 0, (float) bulletTexture.width, (float) bulletTexture.height};
	Vector2 bulletOrigin = {bulletTexture.width/2, bulletTexture.height/2};

    int bulletIterator = 0;

    Texture2D asteroidTexture = LoadTexture("../resources/Meteors/meteorBrown_big1.png");
    Asteroid asteroids[MAX_ASTEROIDS];

    for (int i = 0; i < MAX_ASTEROIDS; ++i)
    {
		asteroids[i] = InitAsteroid(&asteroids[i], asteroidTexture.width, asteroidTexture.height, screenWidth, screenHeight);
    }
	Rectangle asteroidSourceRect = (Rectangle){0, 0, (float) asteroidTexture.width, (float) asteroidTexture.height};
	Vector2 asteroidOrigin = {asteroidTexture.width/2, asteroidTexture.height/2};

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};

    float frameTime;

    while (!WindowShouldClose())
    {
        // ----------------------------------------------
        // Update variables
        // ----------------------------------------------
        frameTime = GetFrameTime();

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        player.acceleration = (Vector2){0, 0};

        UpdatePlayer(&player, bulletIterator, bullets, MAX_BULLETS, frameTime);
//        player.rect = (Rectangle){player.position.x, player.position.y, (float) player.tex.width * player.scale, (float) player.tex.height * player.scale};

        UpdateBullets(bullets, asteroids, bulletTexture, MAX_BULLETS, BULLET_LIFETIME, MAX_ASTEROIDS, frameTime);

        UpdateAsteroids(asteroids, MAX_ASTEROIDS, &player, asteroidTexture, frameTime);

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);

//	    DrawTextureEx(player.tex, player.position, player.rotation, player.scale, player.tint);
//	    DrawTexturePro(player.tex, playerSourceRect, player.rect, Vector2Zero(), player.rotation, player.tint);

        DrawPlayer(&player);

        for (int i = 0; i < MAX_BULLETS; i++)
        {
	        if (bullets[i].visible)
	        {
		        DrawRectangleRec(bullets[i].rect, BLUE);
		        DrawTexturePro(bulletTexture, bulletSourceRect, bullets[i].rect, Vector2Scale(bulletOrigin, bullets[i].scale), bullets[i].rotation, bullets[i].tint);
	        }
        }

        for (int i = 0; i < MAX_ASTEROIDS; ++i)
        {
	        DrawRectangleRec(asteroids[i].rect, BLUE);
            DrawTexturePro(asteroidTexture, asteroidSourceRect, asteroids[i].rect, Vector2Scale(asteroidOrigin, asteroids[i].scale), asteroids[i].rotation, asteroids[i].tint);
        }

        if (debugMode)
        {
            DrawFPS(5, 0);
            DrawText(TextFormat("Player Pos: (%.2f, %.2f)", player.position.x, player.position.y), 5, 20, 20, RAYWHITE);
            DrawText(TextFormat("Player Velocity: (%.2f, %.2f)", player.velocity.x, player.velocity.y), 5, 40, 20, RAYWHITE);
            DrawText(TextFormat("Player Acceleration: (%.2f, %.2f)", player.acceleration.x, player.acceleration.y), 5, 60, 20, RAYWHITE);
	        DrawText(TextFormat("Player Rectangle: (%.2f, %.2f, %.2f, %.2f", player.rect.x, player.rect.y, player.rect.width, player.rect.height), 5, 80, 20, RAYWHITE);
            DrawText(TextFormat("Player Rotation: %.2f", player.rotation), 5, 100, 20, RAYWHITE);
            DrawText(TextFormat("Bullet Iterator: %d", bulletIterator), 5, 120, 20, RAYWHITE);
            DrawLineV(player.position, Vector2Add(player.position, Vector2Scale(player.velocity, 5)), GREEN);
	        DrawCircle(screenWidth/2, screenHeight/2, 1, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}