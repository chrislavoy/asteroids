#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#define MAX_ASTEROIDS 5
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

	Texture2D bulletTexture = LoadTexture("../resources/fire04.png");
    Bullet bullets[MAX_BULLETS];

    for (int i = 0; i < MAX_BULLETS; i++)
    {
		bullets[i] = InitBullet(&bullets[i], bulletTexture.width, bulletTexture.height);
    }
	Rectangle bulletSourceRect = (Rectangle){0, 0, (float) bulletTexture.width, (float) bulletTexture.height};
	Vector2 bulletOrigin = {(float) bulletTexture.width/2, (float) bulletTexture.height/2};

    int bulletIterator = 0;

    Texture2D asteroidTexture = LoadTexture("../resources/Meteors/meteorBrown_big1.png");
    Asteroid asteroids[MAX_ASTEROIDS];

    for (int i = 0; i < MAX_ASTEROIDS; ++i)
    {
		asteroids[i] = InitAsteroid(&asteroids[i], asteroidTexture.width, asteroidTexture.height, screenWidth, screenHeight);
    }

    bool checkForCollisions = true;

    while(checkForCollisions)
    {
        bool foundCollision = false;

        for (int i = 0; i < MAX_ASTEROIDS; ++i) {
            for (int j = 0; j < MAX_ASTEROIDS; ++j) {
                if (i != j)
                {
                    if (Vector2Distance(asteroids[i].position, asteroids[j].position) < 100.0f)
                    {
//                        Vector2AddValue(asteroids[i].position, (float) GetRandomValue(200, 500));
                        asteroids[i].position.x += 200;
                        asteroids[j].position.y += 200;
                        foundCollision = true;
                    }

                    if (Vector2Distance(asteroids[i].position, (Vector2){(float) screenWidth/2, (float) screenHeight/2}) < 200.0f)
                    {
//                        Vector2AddValue(asteroids[i].position, (float) GetRandomValue(200, 500));
                        asteroids[i].position.x += 200;
                        asteroids[j].position.y += 200;
                        foundCollision = true;
                    }
                }
            }
        }

        if (!foundCollision)
            checkForCollisions = false;
    }

	Rectangle asteroidSourceRect = (Rectangle){0, 0, (float) asteroidTexture.width, (float) asteroidTexture.height};
	Vector2 asteroidOrigin = {(float) asteroidTexture.width/2, (float) asteroidTexture.height/2};
//    Vector2 asteroidOrigin = Vector2Zero();

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};

    while (!WindowShouldClose())
    {
        // ----------------------------------------------
        // Update variables
        // ----------------------------------------------
        float frameTime = GetFrameTime();

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        player.acceleration = (Vector2){0, 0};

        UpdatePlayer(&player, &bulletIterator, bullets, MAX_BULLETS, frameTime);

        UpdateBullets(bullets, asteroids, bulletTexture, MAX_BULLETS, BULLET_LIFETIME, MAX_ASTEROIDS, frameTime);

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
            DrawLineV(player.position, Vector2Add(player.position, Vector2Scale(player.velocity, 5)), GREEN);
	        DrawCircle(screenWidth/2, screenHeight/2, 1, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}