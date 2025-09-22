#include "../scenes/Scenes.h"

#include "../Globals.h"
#include "../recoil/Recoil.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"

namespace Scenes
{

    void DrawOperatorSelection(HDC memDC, const RECT& rect)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x = 30 + (i % 6) * (110 + 10);
            int y = 50 + (int)(i / 6) * (110 + 10);

            Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, 110, 110, true);
        }

        SetBkMode(memDC, TRANSPARENT);

        RECT infoBoxRect = { 20, 10, rect.right - 400, 40 };

        // Draw info box background
        HBRUSH infoBrush = CreateSolidBrush(RGB(248, 249, 250));
        FillRect(memDC, &infoBoxRect, infoBrush);
        DeleteObject(infoBrush);

        // Draw info box border
        HPEN infoPen = CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
        HPEN oldPen = (HPEN)SelectObject(memDC, infoPen);
        Rectangle(memDC, infoBoxRect.left, infoBoxRect.top, infoBoxRect.right, infoBoxRect.bottom);
        SelectObject(memDC, oldPen);
        DeleteObject(infoPen);

        // Draw info text in the box
        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(60, 60, 60));

        // Create sections within the info box
        int sectionWidth = (infoBoxRect.right - infoBoxRect.left) / 4;

        // Status section
        RECT statusRect = { infoBoxRect.left + 15, infoBoxRect.top + 5, infoBoxRect.left + sectionWidth,
                            infoBoxRect.bottom - 5 };
        char statusText[50];
        wsprintfA(statusText, "Status: %s", EnableRC ? "ENABLED" : "DISABLED");
        DrawText(memDC, statusText, -1, &statusRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Toggle key section
        RECT toggleRect = { infoBoxRect.left + sectionWidth + 10, infoBoxRect.top + 5,
                            infoBoxRect.left + 2 * sectionWidth, infoBoxRect.bottom - 5 };
        char toggleText[50];
        wsprintfA(toggleText, "Toggle: %s", UseToggleKey ? "ON" : "OFF");
        DrawText(memDC, toggleText, -1, &toggleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Recoil settings section
        RECT recoilRect = { infoBoxRect.left + 2 * sectionWidth + 10, infoBoxRect.top + 5,
                            infoBoxRect.left + 3 * sectionWidth, infoBoxRect.bottom - 5 };
        char recoilText[60];
        wsprintfA(recoilText, "Recoil: V:%d H:%d", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
        DrawText(memDC, recoilText, -1, &recoilRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Add a vertical separator line
        HPEN separatorPen = CreatePen(PS_SOLID, 3, RGB(180, 180, 180));
        HPEN oldSepPen = (HPEN)SelectObject(memDC, separatorPen);
        MoveToEx(memDC, 760, 60, NULL);
        LineTo(memDC, 760, rect.bottom - 20);
        SelectObject(memDC, oldSepPen);
        DeleteObject(separatorPen);

        SelectObject(memDC, oldFont);
    }

} // namespace Scenes
