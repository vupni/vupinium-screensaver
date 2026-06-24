#include "raylib.h"
#include <cstdlib>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    bool drawLogo = true;
    float scale = 0.1f;

    InitWindow(screenWidth, screenHeight, "DVD Bounce");
    SetTargetFPS(60);

    Vector2 pos = { 200, 200 };
    Vector2 vel = { 4, 3 };

    Texture2D logo = LoadTexture("assets/dvd-logo.png");
    Color color = RED;

    int logoWidth = drawLogo ? logo.width * scale : 120;
    int logoHeight = drawLogo ? logo.height * scale : 60;

    while (!WindowShouldClose())
    {

        pos.x += vel.x;
        pos.y += vel.y;

        bool bounced = false;

        // Left / Right collision
        if (pos.x <= 0 || pos.x + logoWidth >= screenWidth)
        {
            vel.x *= -1;
            bounced = true;
        }

        // Top / Bottom collision
        if (pos.y <= 0 || pos.y + logoHeight >= screenHeight)
        {
            vel.y *= -1;
            bounced = true;
        }

        // Change color on bounce (classic screensaver effect)
        if (bounced)
        {
            color = (Color) {
                (unsigned char)(rand() % 255),
                (unsigned char)(rand() % 255),
                (unsigned char)(rand() % 255),
                255 };
        }

        // ---- DRAW ----
        BeginDrawing();
        ClearBackground(BLACK);

        if (drawLogo)
        {
            DrawTextureEx(logo, pos, 0.0f, scale, color);
        }
        else
        {
            DrawRectangle((int)pos.x, (int)pos.y, logoWidth, logoHeight, color);
            DrawText("DVD", (int)pos.x + 35, (int)pos.y + 20, 20, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
}