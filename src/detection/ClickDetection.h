#pragma once

typedef struct HWND__* HWND;

namespace ClickDetection
{

    void OperatorSelection(HWND hwnd, const int mouseX, const int mouseY);

    void WeaponDisplay(HWND hwnd, int right, int bottom,
                       int mouseX, int mouseY);

    void AttachmentDisplay(HWND hwnd, int right, int bottom,
                           int mouseX, int mouseY);

} // namespace ClickDetection
