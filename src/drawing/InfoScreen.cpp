#include <windows.h>

#include "../ui/widgets/Font.h"

namespace Scenes
{

    void DrawInfoScreen(HDC memDC, int right)
    {
        SetBkMode(memDC, TRANSPARENT);

        // Title
        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
        SetTextColor(memDC, RGB(220, 50, 50));
        RECT infoTitleRect = { 60, 30, right - 60, 80 };
        DrawText(memDC, "IMPORTANT SETUP", -1, &infoTitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Subtitle
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(100, 100, 100));
        RECT infoSubRect = { 60, 80, right - 60, 120 };
        DrawText(
                    memDC, "For best performance and accuracy, follow these steps:", -1, &infoSubRect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE
                );

        // Helper lambda for drawing lines
        auto DrawLine = [&](const wchar_t* text, int x, int& y, int right, HFONT font = nullptr,
                                      COLORREF color = RGB(60, 60, 60))
        {
            if (font) SelectObject(memDC, font);
            SetTextColor(memDC, color);
            RECT r = { x, y, right, y + 28 };
            DrawTextW(memDC, text, -1, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            y += 40;
        };

        SelectObject(memDC, Font::GetSubtitleFont());
        int y = 140;
        int left = 80;
        int x = right - 80;

        DrawLine(L"1. Disable Windows mouse acceleration (Enhance Pointer Precision):", left, y, x);

        DrawLine(L"• Open Control Panel > Mouse > Pointer Options.", left + 20, y, x);
        DrawLine(L"• Uncheck 'Enhance pointer precision'. Click Apply.", left + 20, y, x);

        y += 10;
        DrawLine(L"2. Enable raw mouse input in Rainbow Six Siege:", left, y, x);

        DrawLine(L"• Close the game.", left + 20, y, x);
        DrawLine(
                    L"• Open GameSettings.ini in Documents\\My Games\\Rainbow Six - Siege\\RandomNumber",
                    left + 20, y, x);
        DrawLine(L"• Set RawInputMouseKeyboard=1 and save.", left + 20, y, x);
        DrawLine(L"• Restart the game.", left + 20, y, x);

        y += 100;
        DrawLine(
                    L"These steps are required for consistent recoil compensation.", right / 2 - 350, y, right,
                    Font::GetSubtitleFont(), RGB(220, 50, 50));

        SelectObject(memDC, oldFont);
    }

} // namespace Scenes
