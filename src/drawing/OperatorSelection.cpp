#include "../Globals.h"
#include "../recoil/Recoil.h"

#include "../ui/Bitmap.h"
#include "../ui/Themes.h"
#include "../ui/widgets/Button.h"
#include "../ui/widgets/Font.h"

#include "../utils/LayoutUtils.h"

#include <cstdio>

inline constexpr float CURRENT_VERSION = 3.1f;

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
        DrawText(
            memDC, btn.text, -1, &textRect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    void DrawOperatorSelection(HDC memDC, int right, int bottom)
    {
        const auto& bitmaps = Bitmap::GetCurrentBitmapList();

        HPEN pen = CreatePen(PS_SOLID, 1, LineColour);
        HGDIOBJ oldPen = SelectObject(memDC, pen);

        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, TextColour);

        SetBkMode(memDC, TRANSPARENT);

        int cellSize = LayoutUtils::OperatorGridLayout::GetCellSize(
            right, bottom);

        for (size_t i = 0; i < bitmaps.size(); ++i)
        {
            int x, y;
            LayoutUtils::OperatorGridLayout::GetCellPosition(
                i, right, bottom, x, y);

            Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, cellSize, cellSize, 45);
        }

        // Draw info box
        const int padding = 30;
        const int innerPadding = 10;
        RECT infoBoxRect = { padding, 10, right - padding, 40 };

        // Center info box with grid
        HBRUSH bgBrush = CreateSolidBrush(BackgroundColour);
        HGDIOBJ oldBrush = SelectObject(memDC, bgBrush);
        Rectangle(
            memDC, infoBoxRect.left, infoBoxRect.top, infoBoxRect.right,
            infoBoxRect.bottom);

        const int totalWidth = infoBoxRect.right - infoBoxRect.left;
        const int sectionWidth = totalWidth / 5;
        RECT textRect;

        // Status
        textRect = { infoBoxRect.left + innerPadding, infoBoxRect.top + 5,
                     infoBoxRect.left + sectionWidth - innerPadding,
                     infoBoxRect.bottom - 5 };
        char statusText[50];
        sprintf_s(statusText, "Status: %s", EnableRC ? "Enabled" : "Disabled");
        DrawText(
            memDC, statusText, -1, &textRect,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Recoil
        textRect = { infoBoxRect.left + sectionWidth + innerPadding,
                     infoBoxRect.top + 5,
                     infoBoxRect.left + 2 * sectionWidth - innerPadding,
                     infoBoxRect.bottom - 5 };
        char recoilText[60];
        sprintf_s(
            recoilText, sizeof(recoilText), "Recoil: V: %.2f  H: %.2f",
            CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
        DrawText(
            memDC, recoilText, -1, &textRect,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Toggle Key
        textRect = { infoBoxRect.left + 2 * sectionWidth + innerPadding,
                     infoBoxRect.top + 5,
                     infoBoxRect.left + 3 * sectionWidth - innerPadding,
                     infoBoxRect.bottom - 5 };
        char toggleText[50];
        sprintf_s(toggleText, "Toggle: %s", UseToggleKey ? "On" : "Off");
        DrawText(
            memDC, toggleText, -1, &textRect,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Rapid Fire
        textRect.left = textRect.right + innerPadding;
        textRect.right = infoBoxRect.left + 4 * sectionWidth - innerPadding;
        char rapidFireText[50];
        sprintf_s(
            rapidFireText, "Rapid Fire: %s",
            RapidFire ? "Enabled" : "Disabled");
        DrawText(
            memDC, rapidFireText, -1, &textRect,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Version
        textRect = { infoBoxRect.left + 4 * sectionWidth + innerPadding,
                     infoBoxRect.top + 5,
                     infoBoxRect.left + 5 * sectionWidth - innerPadding,
                     infoBoxRect.bottom - 5 };
        char versionText[30];
        sprintf_s(versionText, "Version %.1f", CURRENT_VERSION);
        DrawText(
            memDC, versionText, -1, &textRect,
            DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

        // Draw recoil labels
        RECT verticalLabelRect = { right - 340, (bottom - 90) / 2 - 140,
                                   right - 150, (bottom - 90) / 2 - 120 };
        RECT horizontalLabelRect = { right - 355, (bottom - 90) / 2 - 90,
                                     right - 150, (bottom - 90) / 2 - 70 };
        DrawText(
            memDC, "Vertical Recoil:", -1, &verticalLabelRect,
            DT_LEFT | DT_SINGLELINE | DT_VCENTER);
        DrawText(
            memDC, "Horizontal Recoil:", -1, &horizontalLabelRect,
            DT_LEFT | DT_SINGLELINE | DT_VCENTER);

        // Draw all buttons
        for (const auto& btn : Buttons::GetButtons())
            DrawButton(memDC, btn);

        // Draw vertical line
        int lineX = static_cast<int>(right * 0.66f);
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
