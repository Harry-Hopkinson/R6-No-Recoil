#include "UI.hpp"

const int WINDOW_WIDTH = 650;
const int WINDOW_HEIGHT = 400;
bool DarkTheme = true;

void DrawCenteredText(HDC hdc, LPCSTR text, int yOffset, int windowWidth)
{
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);
    int textX = (windowWidth - textSize.cx) / 2;
    TextOut(hdc, textX, yOffset, text, strlen(text));
}
