#pragma once

#include <windows.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

void DrawCenteredText(HDC hdc, LPCSTR text, int yOffset, int windowWidth);

extern bool DarkTheme;
