#pragma once
#include "../core/Effect.h"

class DVDScreensaver : public Effect {
public:
    void init() override;
    void update(float deltaTime) override;
    void draw() override;

private:
    Vector2 pos;
    Vector2 vel;
    Texture2D logo;
    Color color;
    float scale;
    bool shouldDrawLogo;
    double logoWidth;
    double logoHeight;
};