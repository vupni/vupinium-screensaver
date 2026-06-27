#pragma once
#include <random>
#include <chrono>
#include <cstdint>
#include <string>

class RNG {
public:
    RNG() {
        uint64_t timeSeed =
            std::chrono::high_resolution_clock::now().time_since_epoch().count();

        std::random_device rd;
        std::seed_seq seq { rd(), (uint32_t)timeSeed, (uint32_t)(timeSeed >> 32) };

        gen.seed(seq);
    }

    int intRange(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    float floatRange(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    char randomVisibleChar() {
        int charType = intRange(0, 2);
        switch (charType) {
            case 0:
                return static_cast<char>(intRange('0', '9'));
            case 1:
                return static_cast<char>(intRange('A', 'Z'));
            case 2:
                return static_cast<char>(intRange('a', 'z'));
            default:
                return '!';
        }
    }

    int direction() {
        return intRange(0, 1) ? 1 : -1;
    }

    Color randomColor(bool shouldRandomAlpha = false) {
        return {
            (unsigned char)intRange(0, 255),
            (unsigned char)intRange(0, 255),
            (unsigned char)intRange(0, 255),
            (unsigned char)(shouldRandomAlpha ? intRange(0, 255) : 255)
        };
    }

private:
    std::mt19937 gen;
};