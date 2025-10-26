#pragma once

typedef struct HFONT__* HFONT;
typedef struct HDC__* HDC;
typedef const char* LPCSTR;

class Font
{
public:
    static void CreateFonts();
    static void Cleanup();

    static __forceinline HFONT GetMediumFont()   { return FontMedium; }
    static __forceinline HFONT GetLargeFont()    { return FontLarge; }
    static __forceinline HFONT GetTitleFont()    { return FontTitle; }
    static __forceinline HFONT GetSubtitleFont() { return FontSubtitle; }
    static __forceinline HFONT GetDescFont()     { return FontDesc; }

    static void DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font);

private:
    static HFONT FontMedium;
    static HFONT FontLarge;
    static HFONT FontTitle;
    static HFONT FontSubtitle;
    static HFONT FontDesc;
};
