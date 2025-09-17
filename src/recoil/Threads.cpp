#include "Threads.h"

#include "../Globals.h"
#include "../recoil/Recoil.h"

#include <thread>
#include <windows.h>

void MoveMouseRaw(int dx, int dy)
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

void ApplyRecoil()
{
    bool horizontalFlip = false;

    while (Running)
    {
        if (EnableRC && (GetAsyncKeyState(VK_RBUTTON) & 0x8000)) // ADS
        {
            while (GetAsyncKeyState(VK_LBUTTON) & 0x8000) // Firing
            {
                int baseX = CurrentRecoil * 2;
                MoveMouseRaw(baseX, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ToggleRecoil()
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
