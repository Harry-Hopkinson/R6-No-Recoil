#include "../Globals.h"
#include "../recoil/Recoil.h"

#include "../ui/Bitmap.h"
#include "../ui/Themes.h"
#include "../ui/widgets/Button.h"
#include "../ui/widgets/Font.h"

#include "../utils/LayoutUtils.h"

#include <cstdio>

inline constexpr float CURRENT_VERSION = 3.0f;

namespace Drawing
{

    static void DrawButton(HDC memDC, const Button& btn)
    {
        HBRUSH bgBrush = CreateSolidBrush(ButtonColour);
        HPEN pen = CreatePen(PS_SOLID, 1, LineColour);
        HGDIOBJ oldBrush = SelectObject(memDC, bgBrush);
        HGDIOBJ oldPen = SelectObject(memDC, pen);

        Rectangle(memDC, btn.x, btn.y, btn.x + btn.width, btn.y + btn.height);

        SelectObject(memDC, oldBrush);
        SelectObject(memDC, oldPen);
        DeleteObject(bgBrush);
        DeleteObject(pen);

        SetTextColor(memDC, TextColour);
        SetBkMode(memDC, TRANSPARENT);

        RECT textRect = btn.GetRect();
        DrawText(memDC, btn.text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    void DrawOperatorSelection(HDC memDC, int right, int bottom)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();

        HPEN pen = CreatePen(PS_SOLID, 1, LineColour);
        HGDIOBJ oldPen = SelectObject(memDC, pen);

        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, TextColour);

        SetBkMode(memDC, TRANSPARENT);

        // Draw operator bitmaps
        int cellSize = LayoutUtils::OperatorGridLayout::GetCellSize(right);
        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x, y;
            LayoutUtils::OperatorGridLayout::GetCellPosition(i, right, bottom, x, y);
            Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, cellSize, cellSize, 45);
        }

        // Draw info box
        RECT infoBoxRect = { 40, 10, right - 355, 40 };
        HBRUSH bgBrush = CreateSolidBrush(BackgroundColour);
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

        // Draw recoil labels
        RECT verticalLabelRect = { right - 340, (bottom - 90) / 2 - 140, right - 150, (bottom - 90) / 2 - 120 };
        RECT horizontalLabelRect = { right - 355, (bottom - 90) / 2 - 90, right - 150, (bottom - 90) / 2 - 70 };
        DrawText(memDC, "Vertical Recoil:", -1, &verticalLabelRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        DrawText(memDC, "Horizontal Recoil:", -1, &horizontalLabelRect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

        // Draw all buttons
        for (const auto& btn : Buttons::GetButtons())
            DrawButton(memDC, btn);

        // Draw vertical line
        int lineX = static_cast<int>(right * 0.633f);
        MoveToEx(memDC, lineX, 60, nullptr);
        LineTo(memDC, lineX, bottom - 20);

        // Cleanup
        SelectObject(memDC, oldFont);
        SelectObject(memDC, oldPen);
        SelectObject(memDC, oldBrush);

        DeleteObject(pen);
        DeleteObject(bgBrush);
    }

} // namespace Drawing
