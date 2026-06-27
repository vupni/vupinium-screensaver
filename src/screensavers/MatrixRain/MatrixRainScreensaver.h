#pragma once
#include "../../core/Effect.h"
#include "../../utils/RNG.h"
#include "raylib.h"
#include <vector>
#include <string>

class MatrixRainScreensaver : public Effect {
public:
    void init(const RNG& externalRng) override;
    void update(float deltaTime) override;
    void draw() override;
    ~MatrixRainScreensaver() override;

    struct Char {
        std::string value;
        float brightness;
        float flashTimer;
    };
    struct Column {
        std::vector<Char> chars;
        float x;
        float spawnTimer;
        float spawnInterval;
        float fadeSpeed;
        int spawnIndex;
        int rowCount;
    };

private:

    struct GlyphRange
    {
        int start;
        int end;
    };

    static constexpr GlyphRange glyphRanges[] =
    {
        { '0', '9' }, // ASCII digits (0-9)
        { 'A', 'Z' }, // ASCII uppercase English letters
        { 'a', 'z' }, // ASCII lowercase English letters

        { 0x0400, 0x04FF }, // Cyrillic (Russian, Ukrainian, Bulgarian, Serbian, etc.)
        { 0x0590, 0x05FF }, // Hebrew
        { 0x3040, 0x309F }, // Hiragana (Japanese)
        { 0x30A0, 0x30FF }, // Katakana (Japanese)
        { 0x4E00, 0x4E5F }  // Small subset of CJK Unified Ideographs (Chinese/Japanese/Korean)
    };

    RNG rng;
    std::vector<Column> columns;

    int fontSize;
    float charWidth;
    float charHeight;
    float columnSpacing;

    Font font;
    bool usingCustomFont = false;
    bool useUnicodeGlyphs = false;

    void loadFont();
    Column createColumn(int position, int baseRowCount);
    std::string generateCharacter();
};