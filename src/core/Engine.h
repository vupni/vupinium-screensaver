#pragma once
#include "./Effect.h"
#include "../utils/random/RNG.h"


class Engine {
public:
    RNG rng;

    void setEffect(Effect* effect);
    void run();

private:
    Effect* current = nullptr;
    Vector2 startingMousePos;
    float startupTimer = 0.0f;

    bool windowShouldClose();
};