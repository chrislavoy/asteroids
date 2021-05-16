#include "raylib.h"
#include "raymath.h"
#include "utils.h"

//#define MAX_ASTEROIDS 4
#define MAX_ASTEROIDS 0
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

        player.acceleration = (Vector2){0, 0};

        if (IsKeyPressed(KEY_F1)) debugMode = !debugMode;

        if (IsKeyDown(KEY_W))
            player.acceleration = Vector2Scale(Vector2Rotate((Vector2){0, -1}, player.rotation), player.movementSpeed);
        if (IsKeyDown(KEY_S))
            player.acceleration = Vector2Scale(Vector2Rotate((Vector2){0, 1}, player.rotation), player.movementSpeed);
        if (IsKeyDown(KEY_A)) player.rotation -= 5;
        if (IsKeyDown(KEY_D)) player.rotation += 5;

        if (IsKeyDown(KEY_X)) player.velocity = (Vector2){0, 0};

        if (IsKeyPressed(KEY_SPACE))
        {
            bulletIterator = (bulletIterator + 1) % ((int)MAX_BULLETS - 1);
            Shoot(&player, &bullets[bulletIterator]);
        }

        player.velocity = Vector2Add(player.velocity, player.acceleration);

        player.position = UpdatePosition(&player.position, &player.velocity, frameTime);

	    player.rect = UpdateRectangle(&player.position, player.tex, player.scale);
//        player.rect = (Rectangle){player.position.x, player.position.y, (float) player.tex.width * player.scale, (float) player.tex.height * player.scale};

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].visible)
            {
                bullets[i].position = UpdatePosition(&bullets[i].position, &bullets[i].velocity, frameTime);
	            bullets[i].rect = UpdateRectangle(&bullets[i].position, bulletTexture, bullets[i].scale);
//                bullets[i].rect = (Rectangle){bullets[i].position.x, bullets[i].position.y, bulletTexture.width * bullets[i].scale, bulletTexture.height * bullets[i].scale};
                bullets[i].lifetime -= 1 * frameTime;

	            for (int j = 0; j < MAX_ASTEROIDS; j++)
	            {
	            	if (CheckCollisionRecs(bullets[i].rect, asteroids[j].rect))
		            {
	            		asteroids[j].tint = RED;
		            }
	            }

                if (bullets[i].lifetime <= 0)
                {
                    bullets[i].visible = false;
                    bullets[i].lifetime = BULLET_LIFETIME;
                }
            }
        }

        for (int i = 0; i < MAX_ASTEROIDS; ++i)
        {
            asteroids[i].position = UpdatePosition(&asteroids[i].position, &asteroids[i].velocity, frameTime);
	        asteroids[i].rect = UpdateRectangle(&asteroids[i].position, asteroidTexture, asteroids[i].scale);
//            asteroids[i].rect = (Rectangle){asteroids[i].position.x, asteroids[i].position.y, asteroidTexture.width * asteroids[i].scale, asteroidTexture.height * asteroids[i].scale};

            if (CheckCollisionRecs(asteroids[i].rect, player.rect))
            	player.tint = RED;

            for (int j = 0; j < MAX_ASTEROIDS; j++)
            {
            	if (i != j)
	            {
            		if (CheckCollisionRecs(asteroids[i].rect, asteroids[j].rect))
		            {
            			// TODO: Rework collision logic
//            			asteroids[i].velocity = (Vector2){asteroids[i].velocity.x * -1, asteroids[i].velocity.y * -1};
//			            asteroids[j].velocity = (Vector2){asteroids[j].velocity.x * -1, asteroids[j].velocity.y * -1};
//	                    Rectangle collisionRect = GetCollisionRec(asteroids[i].rect, asteroids[j].rect);
//			            TraceLog(LOG_INFO, TextFormat("x: %f, y: %f, width: %f, height: %f", collisionRect.x, collisionRect.y, collisionRect.width, collisionRect.height));
						if (asteroids[i].rect.x < asteroids[j].rect.x)
						{
							asteroids[i].velocity = (Vector2){-asteroids[i].velocity.x, asteroids[i].velocity.y};
							asteroids[j].velocity = (Vector2){-asteroids[j].velocity.x, asteroids[j].velocity.y};
						}
						else
						{
							asteroids[i].velocity = (Vector2){asteroids[i].velocity.x, -asteroids[i].velocity.y};
							asteroids[j].velocity = (Vector2){asteroids[j].velocity.x, -asteroids[j].velocity.y};
						}
		            }
	            }
            }
        }

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
	    DrawRectangleRec(player.rect, BLUE);
//        DrawTexturePro(player.tex, playerSourceRect, player.rect, playerOrigin, player.rotation, player.tint);
	    DrawTextureEx(player.tex, player.position, player.rotation, player.scale, player.tint);
//	    DrawTexturePro(player.tex, playerSourceRect, player.rect, Vector2Zero(), player.rotation, player.tint);

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