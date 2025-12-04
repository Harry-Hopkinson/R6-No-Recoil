#pragma once

typedef struct HWND__* HWND;

namespace ClickDetection
{

    void OperatorSelection(HWND hwnd, int windowWidth, int windowHeight, int mouseX, int mouseY);

    void WeaponDisplay(HWND hwnd, int windowWidth, int windowHeight, int mouseX, int mouseY);

} // namespace ClickDetection
