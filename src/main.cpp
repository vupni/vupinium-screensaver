#include "raylib.h"
#include "./core/Engine.h"

int main() {
    InitWindow(
        GetMonitorWidth(0),
        GetMonitorHeight(0),
        "Screensaver"
    );

    ToggleFullscreen();
    HideCursor();

    Engine engine = Engine();

    engine.run();

    CloseWindow();
}