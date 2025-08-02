#include "../../Globals.hpp"
#include "../../recoil/Recoil.hpp"

#include "../../core/utils/random.hpp"

#include <windows.h>
#include <thread>

void MoveMouseRaw(int dx, int dy)
{
    INPUT input = {0};
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
        if (EnableRC && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))  // ADS
        {
            while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)  // Firing
            {
                int baseX = CurrentRecoil.Horizontal;
                int baseY = CurrentRecoil.Vertical * 2;

                int finalX = baseX;
                int finalY = baseY + GetRandomInt(-1, 1);

                if (baseX != 0)
                {
                    finalX += GetRandomInt(-1, 1);
                    finalX *= (horizontalFlip ? -1 : 1);
                    horizontalFlip = !horizontalFlip;
                }

                MoveMouseRaw(finalX, finalY);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
