#include "../Globals.h"
#include "../recoil/Recoil.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"
#include "../utils/GdiHelpers.h"
#include "../utils/LayoutUtils.h"

#include <cstdio>
#include <windows.h>

namespace Drawing
{

    void DrawOperatorSelection(HDC memDC, int right, int bottom)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();

        // Draw operator bitmaps in grid
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x, y;
            LayoutUtils::OperatorGridLayout::GetCellPosition(i, x, y);
            Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, 
                              LayoutUtils::OperatorGridLayout::CELL_SIZE, 
                              LayoutUtils::OperatorGridLayout::CELL_SIZE, true);
        }

        SetBkMode(memDC, TRANSPARENT);

        RECT infoBoxRect = { 20, 10, right - 400, 40 };

        // Draw info box background and border
        GdiHelpers::FillRectColor(memDC, infoBoxRect, RGB(248, 249, 250));
        GdiHelpers::DrawRectBorder(memDC, infoBoxRect, RGB(220, 220, 220));

        // Draw info text
        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(60, 60, 60));

        RECT textRect = {};
        int sectionWidth = (infoBoxRect.right - infoBoxRect.left) / 4;

        // Status
        textRect.left = infoBoxRect.left + 15;
        textRect.top = infoBoxRect.top + 5;
        textRect.right = infoBoxRect.left + sectionWidth;
        textRect.bottom = infoBoxRect.bottom - 5;
        char statusText[50];
        wsprintfA(statusText, "Status: %s", EnableRC ? "ENABLED" : "DISABLED");
        DrawText(memDC, statusText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Toggle key
        textRect.left = infoBoxRect.left + sectionWidth + 10;
        textRect.right = infoBoxRect.left + 2 * sectionWidth;
        char toggleText[50];
        wsprintfA(toggleText, "Toggle: %s", UseToggleKey ? "ON" : "OFF");
        DrawText(memDC, toggleText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Recoil settings
        textRect.left = infoBoxRect.left + 2 * sectionWidth + 10;
        textRect.right = infoBoxRect.left + 3 * sectionWidth;
        char recoilText[60];
        sprintf_s(recoilText, sizeof(recoilText), "Recoil: V:%.1f H:%.1f", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
        DrawText(memDC, recoilText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Vertical separator line
        HPEN separatorPen = CreatePen(PS_SOLID, 3, RGB(180, 180, 180));
        HPEN oldSepPen = (HPEN)SelectObject(memDC, separatorPen);
        MoveToEx(memDC, 760, 60, NULL);
        LineTo(memDC, 760, bottom - 20);
        SelectObject(memDC, oldSepPen);
        DeleteObject(separatorPen);

        SelectObject(memDC, oldFont);
    }

} // namespace Drawing
