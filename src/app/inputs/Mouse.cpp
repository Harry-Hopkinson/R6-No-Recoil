#include "Inputs.h"

#include <windows.h>

namespace Inputs
{

    bool IsMouseADS()
    {
        return (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
    }

    bool IsMouseFiring()
    {
        return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
    }

    void MoveMouseRaw(float dx, float dy)
    {
        static float accumX = 0.0f;
        static float accumY = 0.0f;

        accumX += dx;
        accumY += dy;

        LONG moveX = static_cast<LONG>(accumX);
        LONG moveY = static_cast<LONG>(accumY);

        if (moveX != 0 || moveY != 0)
        {
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dx = moveX;
            input.mi.dy = moveY;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            SendInput(1, &input, sizeof(INPUT));

            accumX -= moveX;
            accumY -= moveY;
        }
    }

} // namespace Inputs
