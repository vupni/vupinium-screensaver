#pragma once
#include "./Effect.h"
#include "../utils/random/RNG.h"
#include <vector>
#include <memory>

class Engine {
public:
    Engine();
    RNG rng;

    void setEffect(std::unique_ptr<Effect> effect);
    void run();

private:
    std::unique_ptr<Effect> currentEffect;
    std::vector<std::unique_ptr<Effect>> allEffects;
    Vector2 startingMousePos;
    float startupTimer = 0.0f;

    bool windowShouldClose();
};