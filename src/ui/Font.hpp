#pragma once

#include <windows.h>

class Font
{
public:
    static void CreateFonts();
    static void Cleanup();

    static HFONT GetMediumFont();
    static HFONT GetLargeFont();

    static void DrawRightAlignedText(HDC hdc, LPCSTR text, int yOffset, int fontSize, const RECT& rect);
    static void DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font);

private:
    static HFONT FontMedium;
    static HFONT FontLarge;
};
