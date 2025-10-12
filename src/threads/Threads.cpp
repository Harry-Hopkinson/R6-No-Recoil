#include "Threads.h"

#include <Xinput.h>

#include <windows.h>
#include <thread>

#include "../Globals.h"
#include "../recoil/Recoil.h"
#include "../files/Files.h"

// Mouse: Right-click held = ADS
bool IsMouseADS()
{
    return (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
}

// Mouse: Left-click held = fire
bool IsMouseFiring()
{
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}

// Controller: LT held and RT pressed = ADS + fire
bool IsControllerADS(const XINPUT_STATE& state)
{
    const BYTE LT = state.Gamepad.bLeftTrigger;
    const BYTE RT = state.Gamepad.bRightTrigger;

    const BYTE ADS_THRESHOLD = 30;
    const BYTE FIRE_THRESHOLD = 30;

    return (LT > ADS_THRESHOLD && RT > FIRE_THRESHOLD);
}

// Controller: RT held = fire
bool IsControllerFiring(const XINPUT_STATE& state)
{
    const BYTE RT = state.Gamepad.bRightTrigger;
    const BYTE FIRE_THRESHOLD = 30;
    return (RT > FIRE_THRESHOLD);
}

// Move mouse by (dx, dy)
void MoveMouseRaw(int dx, int dy)
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

namespace Threads
{

    void ApplyRecoil()
    {
        while (Running)
        {
            XINPUT_STATE controllerState;
            ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
            bool controllerConnected = (XInputGetState(0, &controllerState) == ERROR_SUCCESS);

            bool isADS = IsMouseADS() ||
                         (controllerConnected && IsControllerADS(controllerState));

            if (EnableRC && isADS)
            {
                while (IsMouseFiring() ||
                       (controllerConnected && IsControllerFiring(controllerState)))
                {
                    int moveX = 0;
                    int moveY = 0;

                    int baseX = CurrentRecoil.Horizontal;
                    int baseY = CurrentRecoil.Vertical * 2;

                    if (controllerConnected)
                    {
                        // Update controller state continuously
                        XInputGetState(0, &controllerState);

                        SHORT stickX = controllerState.Gamepad.sThumbRX;
                        SHORT stickY = controllerState.Gamepad.sThumbRY;

                        // Deadzone compensation
                        const SHORT DEADZONE = 8000;
                        if (abs(stickX) < DEADZONE) stickX = 0;
                        if (abs(stickY) < DEADZONE) stickY = 0;

                        // Normalize to [-1, 1]
                        float normalizedX = (float)stickX / 32767.0f;
                        float normalizedY = (float)stickY / 32767.0f;

                        // Allow movement left/right when firing
                        int lookX = static_cast<int>(normalizedX * 12.0f);
                        int lookY = static_cast<int>(-normalizedY * 12.0f);

                        moveX = baseX + lookX;
                        moveY = baseY + lookY;
                    }
                    else
                    {
                        moveX = baseX;
                        moveY = baseY;
                    }

                    MoveMouseRaw(moveX, moveY);

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
                Files::SaveConfig();
                InvalidateRect(FindWindow(NULL, "R6 No Recoil"), NULL, TRUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

} // namespace Threads
