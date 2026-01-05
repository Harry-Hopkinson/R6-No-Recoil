#include "Inputs.h"

#include <windows.h>

#define abs(x) ((x) < 0 ? -(x) : (x))

namespace Inputs
{

    bool IsControllerConnected()
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        return (XInputGetState(0, &state) == ERROR_SUCCESS);
    }

    XINPUT_STATE GetControllerState()
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        XInputGetState(0, &state);
        return state;
    }

    void GetControllerStickInput(
        const XINPUT_STATE& state, float& outX, float& outY)
    {
        const SHORT DEADZONE = 8000;
        SHORT stickX = state.Gamepad.sThumbRX;
        SHORT stickY = state.Gamepad.sThumbRY;

        if (abs(stickX) < DEADZONE)
            stickX = 0;
        if (abs(stickY) < DEADZONE)
            stickY = 0;

        outX = static_cast<float>(stickX) / 32767.0f;
        outY = static_cast<float>(stickY) / 32767.0f;
    }

    bool IsControllerADS(const XINPUT_STATE& state)
    {
        const BYTE LT = state.Gamepad.bLeftTrigger;
        const BYTE RT = state.Gamepad.bRightTrigger;

        const BYTE ADS_THRESHOLD = 30;
        const BYTE FIRE_THRESHOLD = 30;

        return (LT > ADS_THRESHOLD && RT > FIRE_THRESHOLD);
    }

    bool IsControllerFiring(const XINPUT_STATE& state)
    {
        const BYTE RT = state.Gamepad.bRightTrigger;
        const BYTE FIRE_THRESHOLD = 30;
        return (RT > FIRE_THRESHOLD);
    }

} // namespace Inputs
