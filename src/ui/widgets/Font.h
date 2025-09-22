#pragma once

typedef struct HFONT__* HFONT;
typedef struct HDC__* HDC;
typedef const char* LPCSTR;

class Font
{
public:
    static void CreateFonts();
    static void Cleanup();

    static HFONT GetMediumFont();
    static HFONT GetLargeFont();
    static HFONT GetTitleFont();
    static HFONT GetSubtitleFont();
    static HFONT GetDescFont();

    static void DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font);

private:
    static HFONT FontMedium;
    static HFONT FontLarge;
    static HFONT FontTitle;
    static HFONT FontSubtitle;
    static HFONT FontDesc;
};
