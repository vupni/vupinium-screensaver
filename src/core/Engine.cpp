#include "./Engine.h"
#include "raylib.h"
#include "../screensavers/DVD/DVDScreensaver.h"
#include <raymath.h>

Engine::Engine() {
    rng = RNG();
    allEffects.push_back(std::make_shared<DVDScreensaver>());
    int randomEffectIndex = rng.intRange(0, allEffects.size() - 1);
    setEffect(allEffects[randomEffectIndex]);
}

void Engine::setEffect(std::shared_ptr<Effect> effect) {
    this->currentEffect = effect;
    startingMousePos = GetMousePosition();
    currentEffect->init(rng);
}

void Engine::run() {
    SetTargetFPS(60);

    while (!WindowShouldClose() && !windowShouldClose()) {
        float deltaTime = GetFrameTime();

        currentEffect->update(deltaTime);

        BeginDrawing();
        ClearBackground(BLACK);
        currentEffect->draw();
        EndDrawing();
    }
}

bool Engine::windowShouldClose() {
    startupTimer += GetFrameTime();
    if (startupTimer < 1.0f)
    {
        startingMousePos = GetMousePosition();
        return false;
    }
    Vector2 currentMouse = GetMousePosition();
    bool mouseMoved = Vector2Distance(startingMousePos, currentMouse) > 50;
    bool keyPressed = GetKeyPressed()!=0;

    return mouseMoved || keyPressed;
}