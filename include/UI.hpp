#pragma once

#include <string>
#include <vector>

const int WINDOW_WIDTH = 650;
const int WINDOW_HEIGHT = 400;

class Button
{
    public:
        Button(HWND parent, int x, int y, int width, int height, const std::string& text, int id)
            : hwndButton(nullptr), id(id)
        {
            hwndButton = CreateWindow(
                "BUTTON", text.c_str(),
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                x, y, width, height,
                parent, (HMENU)(uintptr_t)id, GetModuleHandle(NULL), NULL);
        }
    private:
        HWND hwndButton;
        int id;
};
std::vector<Button> Buttons;

// Function to center text
void DrawCenteredText(HDC hdc, LPCSTR text, int yOffset, int windowWidth)
{
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);
    int textX = (windowWidth - textSize.cx) / 2;
    TextOut(hdc, textX, yOffset, text, strlen(text));
}

bool DarkTheme = true;
void ToggleTheme()
{
    DarkTheme = !DarkTheme;
}
