#pragma once

#include <cmrc/cmrc.hpp>
#include "raylib.h"
#include <string_view>

CMRC_DECLARE(screensaver_assets);

namespace AssetsLoader
{
    inline cmrc::file Get(std::string_view path) {
        static auto fs = cmrc::screensaver_assets::get_filesystem();
        return fs.open(std::string(path));
    }

    inline Texture2D LoadTexture(std::string_view path) {
        auto file = Get(path);

        Image img = LoadImageFromMemory(
            ".png",
            reinterpret_cast<const unsigned char*>(file.begin()),
            static_cast<int>(file.size())
        );

        Texture2D tex = LoadTextureFromImage(img);
        UnloadImage(img);
        return tex;
    }

    inline Font LoadFontTTF(std::string_view path, int fontSize, int* glyphs = nullptr, int glyphCount = 0) {
        auto file = Get(path);

        return LoadFontFromMemory(
            ".ttf",
            reinterpret_cast<const unsigned char*>(file.begin()),
            static_cast<int>(file.size()),
            fontSize,
            glyphs,
            glyphCount
        );
    }
}