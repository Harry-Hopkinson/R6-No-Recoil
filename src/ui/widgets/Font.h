#pragma once

#include "../core/CompilerUtils.h"

typedef struct HFONT__* HFONT;
typedef struct HDC__* HDC;
typedef const char* LPCSTR;

class Font
{
public:
    static void CreateFonts();
    static void Cleanup();

    static FORCE_INLINE HFONT GetMediumFont()   { return FontMedium; }
    static FORCE_INLINE HFONT GetLargeFont()    { return FontLarge; }
    static FORCE_INLINE HFONT GetTitleFont()    { return FontTitle; }
    static FORCE_INLINE HFONT GetSubtitleFont() { return FontSubtitle; }
    static FORCE_INLINE HFONT GetDescFont()     { return FontDesc; }

    static void DrawCenteredText(HDC hdc, LPCSTR text, int x, int y, int width, HFONT font);

private:
    static HFONT FontMedium;
    static HFONT FontLarge;
    static HFONT FontTitle;
    static HFONT FontSubtitle;
    static HFONT FontDesc;
};
