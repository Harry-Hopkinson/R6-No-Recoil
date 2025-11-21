#include "Font.h"

#include "../Themes.h"

#include <windows.h>

HFONT Font::FontMedium = nullptr;
HFONT Font::FontLarge = nullptr;
HFONT Font::FontDesc = nullptr;

void Font::CreateFonts()
{
    FontMedium = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    FontLarge = CreateFont(
        32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    FontDesc = CreateFont(
        18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
}

void Font::Cleanup()
{
    DeleteObject(FontMedium);
    FontMedium = nullptr;

    DeleteObject(FontLarge);
    FontLarge = nullptr;

    DeleteObject(FontDesc);
    FontDesc = nullptr;
}

void Font::DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font)
{
    if (!hdc || !text || !font) return;

    HFONT oldFont = (HFONT)SelectObject(hdc, font);

    COLORREF oldColor = GetTextColor(hdc);
    int oldBkMode = SetBkMode(hdc, TRANSPARENT);

    SetTextColor(hdc, TextColour);

    // Measure text size to center
    SIZE textSize{};
    GetTextExtentPoint32A(hdc, text, static_cast<int>(strlen(text)), &textSize);
    int textX = x + (width - textSize.cx) / 2;

    TextOutA(hdc, textX, y, text, static_cast<int>(strlen(text)));

    SetTextColor(hdc, oldColor);
    SetBkMode(hdc, oldBkMode);

    if (oldFont) SelectObject(hdc, oldFont);
}
