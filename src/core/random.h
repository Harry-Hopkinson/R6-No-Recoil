#pragma once

inline void InitRandom()
{
    static bool initialised = false;
    if (!initialised)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initialised = true;
    }
}

inline int GetRandomInt(int min, int max)
{
    InitRandom();
    return min + (std::rand() % (max - min + 1));
}
