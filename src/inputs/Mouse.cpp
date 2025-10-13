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

    void MoveMouseRaw(int dx, int dy)
    {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dx = dx;
        input.mi.dy = dy;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
        SendInput(1, &input, sizeof(INPUT));
    }

}
