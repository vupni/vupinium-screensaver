#pragma once
#include <random>
#include <chrono>

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

    int direction() {
        return intRange(0, 1) ? 1 : -1;
    }

private:
    std::mt19937 gen;
};