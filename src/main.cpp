#include "raylib.h"
#include "./core/Engine.h"
#include "./screensavers/DVDScreensaver.h"

int main() {
    InitWindow(
        GetMonitorWidth(0),
        GetMonitorHeight(0),
        "Screensaver"
    );

    ToggleFullscreen();
    HideCursor();

    Engine engine;
    DVDScreensaver dvd;

    engine.setEffect(&dvd);
    engine.run();

    CloseWindow();
}