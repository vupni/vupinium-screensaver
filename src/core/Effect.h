#pragma once
#include "raylib.h"
#include "../utils/random/RNG.h"

class Effect {
public:
    virtual void init(RNG& rng) {}
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
    virtual ~Effect() {}
};