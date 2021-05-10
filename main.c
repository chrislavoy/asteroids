#include "raylib.h"
#include "raymath.h"

#define MAX_ASTEROIDS 4
#define MAX_BULLETS 20
#define BULLET_LIFETIME 0.8f

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
    Color tint;
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
} Bullet;

typedef struct Asteroid
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Rectangle rect;
    Color tint;
} Asteroid;


// ----------------------------------------------
// Forward declaring functions
// ----------------------------------------------
Player InitPlayer(const int screenWidth, const int screenHeight, Player *player);
Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight);
Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight);
void ScreenLoop(Vector2 *position);
void Shoot(Player *player, Bullet *bullet);
Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime);

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

    int bulletIterator = 0;

    Texture2D asteroidTexture = LoadTexture("../resources/Meteors/meteorBrown_big1.png");
    Asteroid asteroids[MAX_ASTEROIDS];

    for (int i = 0; i < MAX_ASTEROIDS; ++i)
    {
		asteroids[i] = InitAsteroid(&asteroids[i], asteroidTexture.width, asteroidTexture.height, screenWidth, screenHeight);
    }

    Texture2D background = LoadTexture("../resources/darkPurple.png");
    Rectangle backgroundRect = {0, 0, (float) background.width, (float) background.height};
    Rectangle backgroundDestRect = {0, 0, (float)screenWidth, (float)screenHeight};
    Rectangle destRect;
    Rectangle bulletDestRect;
    Rectangle asteroidDestRect;
    Vector2 origin = {(float) player.tex.width/2, (float) player.tex.height/2};

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

        destRect = (Rectangle){player.position.x, player.position.y, (float) player.tex.width, (float) player.tex.height};

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].visible)
            {
                bullets[i].position = UpdatePosition(&bullets[i].position, &bullets[i].velocity, frameTime);
                bullets[i].lifetime -= 1 * frameTime;

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
        }

        // ----------------------------------------------
        // Drawing logic
        // ----------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureTiled(background, backgroundRect, backgroundDestRect, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
        DrawTexturePro(player.tex, player.rect, destRect, origin, player.rotation, player.tint);

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            bulletDestRect = (Rectangle){bullets[i].position.x, bullets[i].position.y, (float) bulletTexture.width, (float) bulletTexture.height};
            if (bullets[i].visible) DrawTexturePro(bulletTexture, bullets[i].rect, bulletDestRect, origin, bullets[i].rotation, bullets[i].tint);
        }

        for (int i = 0; i < MAX_ASTEROIDS; ++i)
        {
            asteroidDestRect = (Rectangle){asteroids[i].position.x, asteroids[i].position.y, (float) asteroidTexture.width, (float) asteroidTexture.height};
            DrawTexturePro(asteroidTexture, asteroids[i].rect, asteroidDestRect, origin, asteroids[i].rotation, asteroids[i].tint);
        }

        if (debugMode)
        {
            DrawFPS(5, 0);
            DrawText(TextFormat("Player Pos: (%.2f, %.2f)", player.position.x, player.position.y), 5, 20, 20, RAYWHITE);
            DrawText(TextFormat("Player Velocity: (%.2f, %.2f)", player.velocity.x, player.velocity.y), 5, 40, 20, RAYWHITE);
            DrawText(TextFormat("Player Acceleration: (%.2f, %.2f)", player.acceleration.x, player.acceleration.y), 5, 60, 20, RAYWHITE);
            DrawText(TextFormat("Player Rotation: %.2f", player.rotation), 5, 80, 20, RAYWHITE);
            DrawText(TextFormat("Bullet Iterator: %d", bulletIterator), 5, 100, 20, RAYWHITE);
            DrawLineV(player.position, Vector2Add(player.position, Vector2Scale(player.velocity, 5)), GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

Player InitPlayer(const int screenWidth, const int screenHeight, Player *player)
{
	player->position = (Vector2) {(float)screenWidth / 2, (float)screenHeight / 2};
	player->acceleration = Vector2Zero();
	player->velocity = Vector2Zero();
	player->rotation = 0.0f;
	player->tint = WHITE;
	player->tex = LoadTexture("../resources/playerShip.png");
	player->rect = (Rectangle) {0, 0, (float) (*player).tex.width, (float) (*player).tex.height};
	player->scale = 1.0f;
	player->movementSpeed = 5.0f;
	return *player;
}

Bullet InitBullet(Bullet *bullet, const int texWidth, const int texHeight)
{
	bullet->position = (Vector2){0, 0};
	bullet->velocity = (Vector2){0, 0};
	bullet->rotation = 0;
	bullet->scale = 1.0f;
	bullet->movementSpeed = 1000.0f;
	bullet->lifetime = BULLET_LIFETIME;
	bullet->rect = (Rectangle) {0, 0, (float)texWidth, (float)texHeight};
	bullet->tint = WHITE;
	bullet->visible = false;
	return *bullet;
}

Asteroid InitAsteroid(Asteroid *asteroid, const int texWidth, const int texHeight, const int screenWidth, const int screenHeight)
{
	const float movementSpeed = 150;

	asteroid->position = (Vector2){GetRandomValue(0, screenWidth), GetRandomValue(0, screenHeight)};
	asteroid->velocity = (Vector2){GetRandomValue(-movementSpeed, movementSpeed), GetRandomValue(-movementSpeed, movementSpeed)};
	asteroid->rotation = GetRandomValue(0, 359);
	asteroid->tint = WHITE;
	asteroid->rect = (Rectangle) {0, 0, (float) texWidth, (float) texHeight};
	return *asteroid;
}

void ScreenLoop(Vector2 *position)
{
	const float screenWidth = (float) GetScreenWidth();
	const float screenHeight = (float) GetScreenHeight();
	const float x_offset = 50.0f;
	const float y_offset = 40.0f;

	if (position->x <= 0 - x_offset) position->x = screenWidth + x_offset;
	else if (position->x >= screenWidth + x_offset) position->x = 0 - x_offset;
	if (position->y <= 0 - y_offset) position->y = screenHeight + y_offset;
	else if (position->y >= screenHeight + y_offset) position->y = 0 - y_offset;
}

void Shoot(Player *player, Bullet *bullet) {
	bullet->position = player->position;
	bullet->rotation = player->rotation;
	bullet->velocity = player->velocity;
	bullet->velocity = Vector2Add(Vector2Scale(Vector2Rotate((Vector2){0, -1}, player->rotation), bullet->movementSpeed), bullet->velocity);
	bullet->visible = true;
}

Vector2 UpdatePosition(Vector2 *position, Vector2 *velocity, float frameTime)
{
	Vector2 newPos = (Vector2){
			position->x + (velocity->x * frameTime),
			position->y + (velocity->y * frameTime)
	};

	ScreenLoop(&newPos);

	return newPos;
}