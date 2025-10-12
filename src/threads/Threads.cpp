#include "Threads.h"

#ifdef USE_CONTROLLER
#include <Xinput.h>
#endif

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

#ifdef USE_CONTROLLER
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
#endif

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
#ifdef USE_CONTROLLER
            // Poll controller state
            XINPUT_STATE controllerState;
            ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
            bool controllerConnected = (XInputGetState(0, &controllerState) == ERROR_SUCCESS);
#endif

            bool isADS = IsMouseADS()
#ifdef USE_CONTROLLER
                || (controllerConnected && IsControllerADS(controllerState))
#endif
                ;

            if (EnableRC && isADS)
            {
                while (
                    IsMouseFiring()
#ifdef USE_CONTROLLER
                    || (controllerConnected && IsControllerFiring(controllerState))
#endif
                )
                {
                    int baseX = CurrentRecoil.Horizontal;
                    int baseY = CurrentRecoil.Vertical * 2;

                    MoveMouseRaw(baseX, baseY);

                    std::this_thread::sleep_for(std::chrono::milliseconds(5));

#ifdef USE_CONTROLLER
                    // Update controller state inside fire loop
                    if (controllerConnected)
                        XInputGetState(0, &controllerState);
#endif
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
