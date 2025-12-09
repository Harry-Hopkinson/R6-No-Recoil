#pragma once

typedef struct HWND__* HWND;

namespace ClickDetection
{

    void OperatorSelection(HWND hwnd, const int windowWidth, const int windowHeight, int mouseX, int mouseY);

    void WeaponDisplay(HWND hwnd, const int windowWidth, const int windowHeight, int mouseX, int mouseY);

} // namespace ClickDetection
