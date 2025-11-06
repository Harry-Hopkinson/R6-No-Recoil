#pragma once

typedef struct HFONT__* HFONT;
typedef struct HDC__* HDC;
typedef const char* LPCSTR;

class Font
{
public:
    static void CreateFonts();
    static void Cleanup();

    static HFONT GetMediumFont()   { return FontMedium; }
    static HFONT GetLargeFont()    { return FontLarge; }
    static HFONT GetDescFont()     { return FontDesc; }

    static void DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font);

private:
    static HFONT FontMedium;
    static HFONT FontLarge;
    static HFONT FontDesc;
};
