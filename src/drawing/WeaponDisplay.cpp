#include <windows.h>

#include "../scenes/Scenes.h"

#include "../Globals.h"
#include "../core/String.h"
#include "../recoil/Recoil.h"
#include "../ui/Bitmap.h"
#include "../ui/widgets/Font.h"

namespace Drawing
{

    void DrawWeaponDisplay(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                               : DefenderPrimaryWeapons[SelectedOperatorIndex];

        Font::DrawCenteredText(memDC, operatorName, 0, 260, right, Font::GetLargeFont());

        const char* instruction = "Select a primary weapon:";
        Font::DrawCenteredText(memDC, instruction, 0, 300, right, Font::GetMediumFont());

        const char* weapons[3] = { NULL, NULL, NULL };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

        int imgWidth = 400;
        int imgHeight = 150;

        int totalWidth = weaponCount * imgWidth + (weaponCount - 1);

        int startX = (right - totalWidth) / 2;

        int availableHeight = bottom - 120;
        int contentHeight = imgHeight + 50;
        int startY = 120 + (availableHeight - contentHeight) / 2;

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, NULL);

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * imgWidth;
            int y = startY;

            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(memDC, weaponBmp, x, y, imgWidth, imgHeight, true);

            RECT nameRect = { x, y + imgHeight + 15, x + imgWidth, y + imgHeight + 45 };
            HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetLargeFont());
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(0, 0, 0));
            DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(memDC, oldFont);

            // Draw box around the selected weapon
            if (i == SelectedWeaponIndex)
            {
                RECT boxRect = { x + 2, y + 2, x + imgWidth - 2, y + imgHeight + 50 };
                HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, GetStockObject(NULL_BRUSH));
                HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                HPEN oldPen = (HPEN)SelectObject(memDC, pen);
                Rectangle(memDC, boxRect.left, boxRect.top, boxRect.right, boxRect.bottom);
                SelectObject(memDC, oldPen);
                DeleteObject(pen);
                SelectObject(memDC, oldBrush);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        const int sectionTop = bottom - 250;

        Font::DrawCenteredText(memDC, "Scope", 0, sectionTop, right, Font::GetLargeFont());

        int btnWidth = 250;
        int btnHeight = 50;
        int gap = 60;
        int centerX = right / 2;

        RECT nonMagBtn = { centerX - btnWidth - gap / 2, sectionTop + 60, centerX - gap / 2, sectionTop + 60 + btnHeight };
        RECT magBtn = { centerX + gap / 2, sectionTop + 60, centerX + btnWidth + gap / 2, sectionTop + 60 + btnHeight };

        if (SelectedScopeType == ScopeType::MAGNIFIED)
            FillRect(memDC, &nonMagBtn, CreateSolidBrush(RGB(200, 230, 255)));
        else if (SelectedScopeType == ScopeType::NON_MAGNIFIED)
            FillRect(memDC, &magBtn, CreateSolidBrush(RGB(200, 230, 255)));
        FrameRect(memDC, &nonMagBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));

        DrawText(memDC, "Magnified", -1, &nonMagBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        FrameRect(memDC, &magBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawText(memDC, "Non-Magnifying", -1, &magBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Draw note about the vertical grip
        const char* verticalGripText = "Note that the recoil presets are designed to work best with the vertical grip";
        Font::DrawCenteredText(memDC, verticalGripText, 0, sectionTop + 160, right, Font::GetMediumFont());

        // Back button at the bottom
        RECT backBtn = { 30, bottom - 80, 130, bottom - 31 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }

} // namespace Drawing
