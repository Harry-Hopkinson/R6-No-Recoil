#include "../Globals.h"
#include "../recoil/Recoil.h"

#include "../ui/Bitmap.h"
#include "../ui/Themes.h"
#include "../ui/widgets/Font.h"

#include "../utils/LayoutUtils.h"

#include <cstdio>

inline constexpr float CURRENT_VERSION = 3.0f;

namespace Drawing
{

    void DrawOperatorSelection(HDC memDC, int right, int bottom)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();

        COLORREF textColor = DarkTheme ? RGB(230, 230, 230) : RGB(40, 40, 40);
        COLORREF lineColor = DarkTheme ? RGB(180, 180, 180) : RGB(0, 0, 0);

        HPEN pen = CreatePen(PS_SOLID, 1, lineColor);
        HGDIOBJ oldPen = SelectObject(memDC, pen);
        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, textColor);
        SetBkMode(memDC, TRANSPARENT);

        // Draw operator bitmaps
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x, y;
            LayoutUtils::OperatorGridLayout::GetCellPosition(i, x, y);
            Bitmap::DrawBitmap(
                memDC, bitmaps[i], x, y, LayoutUtils::OperatorGridLayout::CELL_SIZE, LayoutUtils::OperatorGridLayout::CELL_SIZE,
                45);
        }

        RECT infoBoxRect = { 40, 10, right - 355, 40 };
        HBRUSH bgBrush = CreateSolidBrush(DarkTheme ? RGB(40, 40, 40) : RGB(240, 240, 240));
        HGDIOBJ oldBrush = SelectObject(memDC, bgBrush);

        Rectangle(memDC, infoBoxRect.left, infoBoxRect.top, infoBoxRect.right - 75, infoBoxRect.bottom);

        const int sectionWidth = (infoBoxRect.right - infoBoxRect.left) / 4;
        RECT textRect;

        textRect = { infoBoxRect.left + 15, infoBoxRect.top + 5, infoBoxRect.left + sectionWidth, infoBoxRect.bottom - 5 };
        char statusText[50];
        sprintf_s(statusText, "Status: %s", EnableRC ? "ENABLED" : "DISABLED");
        DrawText(memDC, statusText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        textRect = { infoBoxRect.left + sectionWidth + 10, infoBoxRect.top + 5, infoBoxRect.left + 2 * sectionWidth,
                     infoBoxRect.bottom - 5 };
        char toggleText[50];
        sprintf_s(toggleText, "Toggle: %s", UseToggleKey ? "ON" : "OFF");
        DrawText(memDC, toggleText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        textRect = { infoBoxRect.left + 2 * sectionWidth + 10, infoBoxRect.top + 5, infoBoxRect.left + 3 * sectionWidth,
                     infoBoxRect.bottom - 5 };
        char recoilText[60];
        sprintf_s(recoilText, sizeof(recoilText), "Recoil: V: %.2f  H: %.2f", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
        DrawText(memDC, recoilText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        textRect = { infoBoxRect.left + 3 * sectionWidth + 10, infoBoxRect.top + 5, infoBoxRect.right - 60,
                     infoBoxRect.bottom - 5 };
        char currentVersionText[50];
        sprintf_s(currentVersionText, "Version: %.1f", CURRENT_VERSION);
        DrawText(memDC, currentVersionText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        RECT verticalLabelRect = { WINDOW_WIDTH - 340, (WINDOW_HEIGHT - 90) / 2 - 140, WINDOW_WIDTH - 150,
                                   (WINDOW_HEIGHT - 90) / 2 - 120 };
        RECT horizontalLabelRect = { WINDOW_WIDTH - 355, (WINDOW_HEIGHT - 90) / 2 - 90, WINDOW_WIDTH - 150,
                                     (WINDOW_HEIGHT - 90) / 2 - 70 };

        DrawText(memDC, "Vertical Recoil:", -1, &verticalLabelRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        DrawText(memDC, "Horizontal Recoil:", -1, &horizontalLabelRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

        MoveToEx(memDC, 760, 60, nullptr);
        LineTo(memDC, 760, bottom - 20);

        // Cleanup
        SelectObject(memDC, oldFont);
        SelectObject(memDC, oldPen);
        DeleteObject(pen);

        // Restore brush
        SelectObject(memDC, oldBrush);
        DeleteObject(bgBrush);
    }

} // namespace Drawing
