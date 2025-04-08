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

        // Crouch spam when C key is held
        if (EnableCrouchMacro && (GetAsyncKeyState(CrouchKey) & 0x8000))
        {
            INPUT input[2] = {};

            // Key down
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = CrouchKey;

            // Key up
            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = CrouchKey;
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;

            SendInput(2, input, sizeof(INPUT));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
