#include "Font.hpp"

HFONT Font::FontMedium = nullptr;
HFONT Font::FontLarge = nullptr;

void Font::CreateFonts()
{
    FontLarge = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                          DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    FontMedium = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
}

void Font::Cleanup()
{
    if (FontLarge)
    {
        DeleteObject(FontLarge);
        FontLarge = nullptr;
    }

    if (FontMedium)
    {
        DeleteObject(FontMedium);
        FontMedium = nullptr;
    }
}

HFONT Font::GetMediumFont()
{
    return FontMedium;
}

HFONT Font::GetLargeFont()
{
    return FontLarge;
}

void Font::DrawRightAlignedText(HDC hdc, LPCSTR text, int yOffset, int fontSize, const RECT& rect)
{
    HFONT selectedFont = (fontSize >= 28) ? FontLarge : FontMedium;
    HFONT oldFont = (HFONT)SelectObject(hdc, selectedFont);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, static_cast<int>(strlen(text)), &textSize);
    int textX = rect.right - 475 + (400 - textSize.cx) / 2;

    TextOut(hdc, textX, yOffset, text, static_cast<int>(strlen(text)));
    SelectObject(hdc, oldFont);
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
