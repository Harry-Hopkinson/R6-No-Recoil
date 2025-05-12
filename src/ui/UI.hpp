#pragma once

#include <string>
#include <vector>

const int WINDOW_WIDTH = 650;
const int WINDOW_HEIGHT = 400;

// Function to center text
void DrawCenteredText(HDC hdc, LPCSTR text, int yOffset, int windowWidth)
{
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);
    int textX = (windowWidth - textSize.cx) / 2;
    TextOut(hdc, textX, yOffset, text, strlen(text));
}

bool DarkTheme = true;
