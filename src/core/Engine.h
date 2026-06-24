#pragma once
#include "./Effect.h"
#include "../utils/random/RNG.h"
#include <vector>
#include <memory>

class Engine {
public:
    Engine();
    RNG rng;

    void setEffect(std::shared_ptr<Effect> effect);
    void run();

private:
    std::shared_ptr<Effect> currentEffect;
    std::vector<std::shared_ptr<Effect>> allEffects;
    Vector2 startingMousePos;
    float startupTimer = 0.0f;

    bool windowShouldClose();
};