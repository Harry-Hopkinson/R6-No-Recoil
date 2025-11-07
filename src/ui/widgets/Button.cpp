#include "Button.h"

#include <windows.h>

Button::Button(HWND parent, int x, int y, int width, int height, const char* text, int id)
    : hwndButton(nullptr)
    , id(id)
{
    hwndButton = CreateWindow(
        "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x, y, width, height, parent,
        reinterpret_cast<HMENU>(static_cast<uintptr_t>(id)), GetModuleHandle(NULL), NULL);
}

HWND Button::GetHWND() const
{
    return hwndButton;
}

namespace
{
    std::vector<Button> ButtonsVector;
}

namespace Buttons
{

    const std::vector<Button>& GetButtons()
    {
        return ButtonsVector;
    }

    void ClearButtons()
    {
        for (const auto& button : ButtonsVector)
        {
            HWND hwnd = button.GetHWND();
            if (hwnd && IsWindow(hwnd)) DestroyWindow(hwnd);
        }
        ButtonsVector.clear();
    }

    void CreateOperatorSelectionButtons(HWND hwnd)
    {
        ClearButtons();

        int startX = WINDOW_WIDTH - 200 - (350 / 2);
        int startY = (WINDOW_HEIGHT - 90) / 2;

        ButtonsVector.emplace_back(hwnd, startX + 90, startY, 170, 40, "Toggle Recoil", 1);
        ButtonsVector.emplace_back(hwnd, startX, startY + 50, 170, 40, "Toggle Key", 2);

        ButtonsVector.emplace_back(
            hwnd, startX + 180, startY + 50, 170, 40, IsAttackerView ? "Switch to Defenders" : "Switch to Attackers",
            IsAttackerView ? 4 : 3);

        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 375, WINDOW_HEIGHT - 150, 160, 40, "Support the Project", 5);
        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 200, WINDOW_HEIGHT - 150, 160, 40, "View on GitHub", 6);
        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 287, WINDOW_HEIGHT - 100, 160, 40, "Join the Discord", 7);

        // Vertical Recoil
        ButtonsVector.emplace_back(hwnd, startX + 240, startY - 150, 80, 40, "+", 8);
        ButtonsVector.emplace_back(hwnd, startX + 150, startY - 150, 80, 40, "-", 9);

        // Horizontal Recoil
        ButtonsVector.emplace_back(hwnd, startX + 240, startY - 100, 80, 40, "+", 10);
        ButtonsVector.emplace_back(hwnd, startX + 150, startY - 100, 80, 40, "-", 11);

        ButtonsVector.emplace_back(hwnd, startX + 100, startY + 100, 140, 40, "Save Config", 12);
    }

} // namespace Buttons
