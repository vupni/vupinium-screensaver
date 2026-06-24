#include "./Engine.h"
#include "raylib.h"
#include <raymath.h>

void Engine::setEffect(Effect* effect) {
    rng = RNG();
    current = effect;
    startingMousePos = GetMousePosition();
    current->init(rng);
}

void Engine::run() {
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (windowShouldClose()) {
            break;
        }
        float deltaTime = GetFrameTime();

        current->update(deltaTime);

        BeginDrawing();
        ClearBackground(BLACK);
        current->draw();
        EndDrawing();
    }
}

bool Engine::windowShouldClose() {
    startupTimer += GetFrameTime();
    if (startupTimer < 1.0f)
    {
        startingMousePos = GetMousePosition();;
        return false;
    }
    Vector2 currentMouse = GetMousePosition();
    bool mouseMoved = Vector2Distance(startingMousePos, currentMouse) > 50;
    bool keyPressed = GetKeyPressed()!=0;

    return mouseMoved || keyPressed;
}