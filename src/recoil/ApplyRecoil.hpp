#pragma once

#include "../core/utils/random.hpp"

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

                mouse_event(MOUSEEVENTF_MOVE, finalX, finalY, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
