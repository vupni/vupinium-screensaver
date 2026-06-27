#pragma once
#include "raylib.h"
#include <string>

void DrawShadowText(Font font, const std::string& text, Vector2 pos, float size, Color color) {
    DrawTextEx(font, text.c_str(), pos, size, 0.0f, color);
    DrawTextEx(font, text.c_str(), { pos.x + 1, pos.y }, size, 0.0f, color);
}