#include <windows.h>
#include "../Globals.hpp"

#include "ToggleRecoil.hpp"

#include <thread>

void ToggleRecoilListener()
{
    while (Running)
    {
        if (UseToggleKey && (GetAsyncKeyState(ToggleKey) & 0x8000))
        {
            EnableRC = !EnableRC;
            InvalidateRect(FindWindow(NULL, "R6 No Recoil"), NULL, TRUE);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
