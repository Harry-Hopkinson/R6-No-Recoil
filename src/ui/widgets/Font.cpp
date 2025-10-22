#include "Font.h"

#include <windows.h>

HFONT Font::FontMedium = nullptr;
HFONT Font::FontLarge = nullptr;
HFONT Font::FontTitle = nullptr;
HFONT Font::FontSubtitle = nullptr;
HFONT Font::FontDesc = nullptr;

void Font::CreateFonts()
{
    FontLarge = CreateFont(
        32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    FontMedium = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    FontTitle = CreateFont(
        72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, TEXT("Arial"));

    FontSubtitle = CreateFont(
        28, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

    FontDesc = CreateFont(
        18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
}

void Font::Cleanup()
{
    DeleteObject(FontLarge); FontLarge = nullptr;

    DeleteObject(FontMedium); FontMedium = nullptr;

    DeleteObject(FontTitle); FontTitle = nullptr;

    DeleteObject(FontSubtitle); FontSubtitle = nullptr;

    DeleteObject(FontDesc); FontDesc = nullptr;
}

void Font::DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font)
{
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, static_cast<int>(strlen(text)), &textSize);
    int textX = x + (width - textSize.cx) / 2;
    TextOut(hdc, textX, y, text, static_cast<int>(strlen(text)));
    SelectObject(hdc, oldFont);
}
