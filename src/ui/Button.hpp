#pragma once

#include <vector>
#include "../Globals.hpp"

class Button
{
    public:
        Button(HWND parent, int x, int y, int width, int height, const char* text, int id)
            : hwndButton(nullptr), id(id)
        {
            hwndButton = CreateWindow
            (
                "BUTTON", text,
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                x, y, width, height,
                parent, (HMENU)(uintptr_t)id, GetModuleHandle(NULL), NULL
            );
        }

        HWND GetHWND() const { return hwndButton; }
    private:
        HWND hwndButton;
        int id;
};
std::vector<Button> Buttons;

void ShowAllButtons(bool show)
{
    for (const auto& button : Buttons)
    {
        ShowWindow(button.GetHWND(), show ? SW_SHOW : SW_HIDE);
    }
}

void CreateLandingPageButtons(HWND hwnd)
{
    Buttons.clear();
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2 + 80;

    Buttons.emplace_back(hwnd, centerX - 250, centerY, 200, 80, "ATTACKERS", 4);
    Buttons.emplace_back(hwnd, centerX + 50 , centerY, 200, 80, "DEFENDERS", 5);

    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 375, WINDOW_HEIGHT - 100, 160, 40, "Support the Project", 7);
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 200, WINDOW_HEIGHT - 100, 160, 40, "View on GitHub", 8);
}

void CreateOperatorSelectionButtons(HWND hwnd)
{
    Buttons.clear();
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 525, 570, 150, 40, "Toggle Recoil", 1);
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 355, 570, 150, 40, "Change Mode", 2);
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 180, 570, 150, 40, "Toggle Key", 3);

    // Change the existing buttons to switch teams instead of setting the view
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 450 , 620, 150, 40, IsAttackerView ? "Switch to Defenders" : "Switch to Attackers", IsAttackerView ? 5 : 4);
    Buttons.emplace_back(hwnd, WINDOW_WIDTH - 1156, 715, 150, 40, "Back to Menu", 6);
}
