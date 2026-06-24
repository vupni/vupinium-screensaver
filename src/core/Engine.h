#pragma once
#include "Effect.h"

class Engine {
public:
    void setEffect(Effect* effect);
    void run();

private:
    Effect* current = nullptr;
    Vector2 startingMousePos;
    float startupTimer = 0.0f;
    
    bool windowShouldClose();
};