#include "./DVDScreensaver.h"
#include <cstdlib>

void DVDScreensaver::init(RNG& externalRng) {
    rng = externalRng;
    float angle = rng.floatRange(0.0f, 2 * PI);
    angle = ((int)(angle) % 90 < 10) ? angle + 10 : angle;
    float velX = rng.floatRange(250.0f, 330.0f) * cos(angle);
    float velY = rng.floatRange(180.0f, 220.0f) * sin(angle);
    vel = { velX, velY };
    color = RED;
    scale = 0.18f;
    shouldDrawLogo = rng.intRange(0, 3);

    logo = LoadTexture("assets/dvd-logo.png");

    logoWidth = shouldDrawLogo ? logo.width * scale : 150;
    logoHeight = shouldDrawLogo ? logo.height * scale : 80;

    float centerX = (GetScreenWidth() - logoWidth) / 2;
    float centerY = (GetScreenHeight() - logoHeight) / 2;
    pos = { centerX, centerY };
}

void DVDScreensaver::update(float deltaTime) {
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    bool bounced = false;

    if (pos.x <= 0 || pos.x + logoWidth >= GetScreenWidth()) {
        vel.x *= -1;
        bounced = true;
    }

    if (pos.y <= 0 || pos.y + logoHeight >= GetScreenHeight()) {
        vel.y *= -1;
        bounced = true;
    }

    if (bounced) {
        color = {
            (unsigned char)(rng.intRange(0, 255)),
            (unsigned char)(rng.intRange(0, 255)),
            (unsigned char)(rng.intRange(0, 255)),
            255
        };
    }
}

void DVDScreensaver::draw() {
    if (shouldDrawLogo)
    {
        DrawTextureEx(logo, pos, 0.0f, scale, color);
    }
    else
    {
        DrawRectangle((int)pos.x, (int)pos.y, logoWidth, logoHeight, color);

        const char* text = "DVD";
        int fontSize = logoHeight / 2;

        int textWidth = MeasureText(text, fontSize);
        int textHeight = fontSize; // raylib uses fontSize as height approximation

        int textX = (int)pos.x + (logoWidth - textWidth) / 2;
        int textY = (int)pos.y + (logoHeight - textHeight) / 2;

        DrawText(text, textX, textY, fontSize, BLACK);
    }
}