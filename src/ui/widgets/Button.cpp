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

    std::vector<Button> GetButtons()
    {
        return ButtonsVector;
    }

    void ClearButtons()
    {
        ButtonsVector.clear();
    }

    void HideButtons()
    {
        for (const auto& button : ButtonsVector)
            ShowWindow(button.GetHWND(), SW_HIDE);
    }

    void CreateMainMenuButtons(HWND hwnd)
    {
        HideButtons();
        ButtonsVector.clear();

        int centerX = WINDOW_WIDTH / 2;
        int centerY = WINDOW_HEIGHT / 2 + 80;

        ButtonsVector.emplace_back(hwnd, centerX - 250, centerY, 200, 80, "ATTACKERS", 3);
        ButtonsVector.emplace_back(hwnd, centerX + 50, centerY, 200, 80, "DEFENDERS", 4);

        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 375, WINDOW_HEIGHT - 100, 160, 40, "Support the Project", 6);
        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 200, WINDOW_HEIGHT - 100, 160, 40, "View on GitHub", 7);
    }

    void CreateOperatorSelectionButtons(HWND hwnd)
    {
        HideButtons();
        ButtonsVector.clear();

        int startX = WINDOW_WIDTH - 200 - (350 / 2);
        int startY = (WINDOW_HEIGHT - 90) / 2;

        ButtonsVector.emplace_back(hwnd, startX, startY, 170, 40, "Toggle Recoil", 1);
        ButtonsVector.emplace_back(hwnd, startX + 170 + 10 + 80 + 10, startY, 80, 40, "+", 8);
        ButtonsVector.emplace_back(hwnd, startX + 170 + 10, startY, 80, 40, "-", 9);

        ButtonsVector.emplace_back(hwnd, startX, startY + 40 + 10, 170, 40, "Toggle Key", 2);
        ButtonsVector.emplace_back(
            hwnd, startX + 170 + 10, startY + 40 + 10, 170, 40, IsAttackerView ? "Switch to Defenders" : "Switch to Attackers",
            IsAttackerView ? 4 : 3);

        ButtonsVector.emplace_back(hwnd, WINDOW_WIDTH - 175, WINDOW_HEIGHT - 75, 160, 40, "Back to Menu", 5);
    }

} // namespace Buttons
