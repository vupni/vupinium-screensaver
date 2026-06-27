#include "./MatrixRainScreensaver.h"
#include "../../utils/AssetsLoader.h"
#include "../../utils/drawing/drawShadowText.h"
#include <algorithm>

const float COLUMN_COUNT_DIVISOR = 1.6f;
const float COLUMN_SPACING_RANDOMNESS = 0.3f;
const float MIN_SPAWN_INTERVAL = 0.08f;
const float MAX_SPAWN_INTERVAL = 0.32f;
const float MIN_FADE_SPEED = 2.0f;
const float MAX_FADE_SPEED = 3.5f;
const float ROW_COUNT_MIN_MULTIPLIER = 1.2f;
const float ROW_COUNT_MAX_MULTIPLIER = 2.5f;
const float FLASH_DURATION = 0.18f;

static Color MakeFlashBoxColor(float flashProgressTime) {
    return { 60, 255, 60, (unsigned char)(100 * flashProgressTime) };
}
static Color MakeMatrixGreenColor(float brightness) {
    return { 30, (unsigned char)(brightness * 255), 30, 255 };
}
static std::string generateRandomGlyph(RNG& rng) {
    auto encodeUtf8 = [] (uint32_t cp) {
        std::string out;
        if (cp <= 0x7F) {
            out.push_back(static_cast<char>(cp));
        }
        else if (cp <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else {
            out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        return out;
        };

    int roll = rng.intRange(0, 99);
    uint32_t cp = 0;
    if (roll < 72) {
        int asciiRoll = rng.intRange(0, 99);
        if (asciiRoll < 50) {
            cp = (uint32_t)rng.intRange('A', 'Z');
        }
        else if (asciiRoll < 85) {
            cp = (uint32_t)rng.intRange('a', 'z');
        }
        else {
            cp = (uint32_t)rng.intRange('0', '9');
        }
    }
    else if (roll < 90) {
        cp = (uint32_t)rng.intRange(0x3040, 0x309F);
    }
    else if (roll < 95) {
        cp = (uint32_t)rng.intRange(0x0590, 0x05FF);
    }
    else if (roll < 98) {
        cp = (uint32_t)rng.intRange(0x0400, 0x04FF);
    }
    else if (roll < 99) {
        cp = (uint32_t)rng.intRange(0x30A0, 0x30FF);
    }
    else {
        cp = (uint32_t)rng.intRange(0x4E00, 0x4E5F);
    }

    return encodeUtf8(cp);
}

void MatrixRainScreensaver::init(const RNG& externalRng) {
    rng = externalRng;
    fontSize = 18;

    loadFont();

    Vector2 measure = MeasureTextEx(font, "W", fontSize, 0);

    charWidth = (measure.x > 1.0f) ? measure.x : (float)fontSize;
    charHeight = (measure.y > 1.0f) ? measure.y : (float)fontSize;
    columnSpacing = charWidth * COLUMN_COUNT_DIVISOR;

    int baseRowCount = (int)(GetScreenHeight() / charHeight);

    int columnCount = (int)(GetScreenWidth() / columnSpacing);

    columns.clear();
    columns.reserve(columnCount);

    for (int i = 0; i < columnCount; i++) {
        columns.push_back(createColumn(i, baseRowCount));
    }
}

void MatrixRainScreensaver::loadFont() {
    std::vector<int> fontChars;

    auto appendRange = [&] (int start, int end) {
        for (int characterPosition = start; characterPosition <= end; characterPosition++) {
            fontChars.push_back(characterPosition);
        }
        };

    for (const auto& range : glyphRanges) {
        appendRange(range.start, range.end);
    }

    bool loadedFont = false;

    font = AssetsLoader::LoadFontTTF(
        "fonts/MatrixRain/SevenFifteenMonoRounded-Regular.ttf",
        fontSize,
        fontChars.data(),
        (int)fontChars.size()
    );

    if (font.texture.id != 0 && font.glyphCount > 0) {
        usingCustomFont = true;
        useUnicodeGlyphs = true;
        loadedFont = true;
    }

    if (!loadedFont) {
        font = GetFontDefault();
        usingCustomFont = false;
        useUnicodeGlyphs = false;
    }
}
MatrixRainScreensaver::Column MatrixRainScreensaver::createColumn(int position, int baseRowCount) {
    Column column;
    column.x = position * columnSpacing +
        rng.floatRange(-columnSpacing * COLUMN_SPACING_RANDOMNESS, columnSpacing * COLUMN_SPACING_RANDOMNESS);

    column.spawnInterval = rng.floatRange(MIN_SPAWN_INTERVAL, MAX_SPAWN_INTERVAL);
    column.spawnTimer = rng.floatRange(0.0f, column.spawnInterval);
    column.fadeSpeed = rng.floatRange(MIN_FADE_SPEED, MAX_FADE_SPEED);

    int computedRowCountValue = baseRowCount * rng.floatRange(ROW_COUNT_MIN_MULTIPLIER, ROW_COUNT_MAX_MULTIPLIER);
    column.rowCount = std::max(1, computedRowCountValue);
    column.spawnIndex = rng.intRange(0, column.rowCount - 1);

    column.chars.reserve(column.rowCount);

    for (int j = 0; j < column.rowCount; j++) {
        column.chars.emplace_back();
        Char& glyph = column.chars.back();
        glyph.value = generateCharacter();

        glyph.brightness = 0.0f;
        glyph.flashTimer = 0.0f;
    }
    return column;
}

void MatrixRainScreensaver::update(float deltaTime) {
    for (auto& column : columns) {
        column.spawnTimer += deltaTime;

        for (auto& ch : column.chars) {
            ch.brightness = std::max(0.0f, ch.brightness - deltaTime / column.fadeSpeed);

            if (ch.flashTimer > 0.0f) {
                ch.flashTimer = std::max(0.0f, ch.flashTimer - deltaTime);
            }
        }

        if (column.spawnTimer >= column.spawnInterval && !column.chars.empty()) {
            column.spawnIndex = (column.spawnIndex + 1) % column.rowCount;

            Char& target = column.chars[column.spawnIndex];

            target.value = generateCharacter();

            target.brightness = 1.0f;
            target.flashTimer = FLASH_DURATION;

            column.spawnTimer = 0.0f;
        }
    }
}

void MatrixRainScreensaver::draw() {
    for (const auto& column : columns) {
        for (int row = 0; row < (int)column.chars.size(); row++) {
            const Char& ch = column.chars[row];
            float y = row * charHeight;

            if (y > GetScreenHeight()) {
                continue;
            }
            if (ch.flashTimer > 0.01f) {
                float flashProgressTime = ch.flashTimer / FLASH_DURATION;

                Vector2 size = MeasureTextEx(font, ch.value.c_str(), (float)fontSize, 0.0f);

                float boxWidth = std::max(size.x * 0.95f, (float)fontSize * 0.6f);
                float boxHeight = std::max(size.y * 0.95f, (float)fontSize * 0.6f);

                float boxX = column.x + (charWidth - boxWidth) * 0.5f;
                float boxY = y + (charHeight - boxHeight) * 0.5f;

                DrawRectangle((int)boxX, (int)boxY, (int)boxWidth, (int)boxHeight,
                    MakeFlashBoxColor(flashProgressTime));
            }

            if (ch.brightness > 0.01f) {
                Color color = MakeMatrixGreenColor(ch.brightness);

                Vector2 pos = { column.x, y };

                DrawShadowText(font, ch.value, pos, (float)fontSize, color);
            }
        }
    }
}

std::string MatrixRainScreensaver::generateCharacter() {
    return useUnicodeGlyphs
        ? generateRandomGlyph(rng)
        : std::string(1, rng.randomVisibleChar());
}

MatrixRainScreensaver::~MatrixRainScreensaver() {
    if (usingCustomFont)
        UnloadFont(font);
}