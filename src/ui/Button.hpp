#pragma once

#include <vector>

class Button
{
    public:
        Button(HWND parent, int x, int y, int width, int height, const char* text, int id)
            : hwndButton(nullptr), id(id)
        {
            hwndButton = CreateWindow(
                "BUTTON", text,
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                x, y, width, height,
                parent, (HMENU)(uintptr_t)id, GetModuleHandle(NULL), NULL);
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
