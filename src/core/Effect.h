#pragma once
#include "raylib.h"

class Effect {
public:
    virtual void init() {}
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
    virtual ~Effect() {}
};