#pragma once

#include <cstdlib>
#include <ctime>

inline void InitRandom()
{
    static bool initialized = false;
    if (!initialized)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initialized = true;
    }
}

inline int GetRandomInt(int min, int max)
{
    InitRandom();
    return min + (std::rand() % (max - min + 1));
}
