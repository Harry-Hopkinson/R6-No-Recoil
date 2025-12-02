#include "../Globals.h"
#include "../recoil/Recoil.h"

#include "../core/Keys.h"
#include "../core/String.h"
#include "../files/Files.h"
#include "../scenes/Scenes.h"

#include "../ui/Bitmap.h"
#include "../ui/Themes.h"
#include "../ui/widgets/Font.h"

#include "../utils/LayoutUtils.h"

#include <cstdio>

namespace Drawing
{

    void DrawWeaponDisplay(HDC memDC, int right, int bottom)
    {
        const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView ? AttackerWeapons[SelectedOperatorIndex]
                                               : DefenderWeapons[SelectedOperatorIndex];

        // Titles
        SetBkMode(memDC, TRANSPARENT);

        Font::DrawCenteredText(memDC, operatorName, 0, 220, right, Font::GetLargeFont());
        Font::DrawCenteredText(memDC, "Select a weapon:", 0, 260, right, Font::GetMediumFont());

        // Parse weapon list
        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons);

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(weaponCount, right, bottom, startX, startY);

        // Get dimensions
        int weaponWidth = LayoutUtils::WeaponDisplayLayout::GetWeaponWidth(right);
        int weaponHeight = LayoutUtils::WeaponDisplayLayout::GetWeaponHeight(bottom);
        int spacing = LayoutUtils::WeaponDisplayLayout::GetWeaponSpacing(right);

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, nullptr);

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (weaponWidth + spacing);
            int y = startY;

            // Draw weapon image
            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(memDC, weaponBmp, x, y, weaponWidth, weaponHeight, 0, true);

            // Weapon name
            HFONT weaponFont = Font::GetLargeFont();
            HFONT oldFont = (HFONT)SelectObject(memDC, weaponFont);
            SetTextColor(memDC, TextColour);

            RECT nameRect = { x, y + weaponHeight + 15, x + weaponWidth,
                              y + weaponHeight + 45 };
            DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(memDC, oldFont);

            const int buttonWidth = 200;
            const int buttonHeight = 45;
            const int buttonSpacing = 10;

            int buttonStartY = y + weaponHeight + 60;
            int buttonStartX = x + (weaponWidth - buttonWidth) / 2;

            // Key bindings
            const int keyWidth = 100;
            const int keyHeight = 30;

            const int keyStartX = x + weaponWidth - 250;
            const int keyStartY = y + weaponHeight + 225;
            RECT keyRect = { keyStartX, keyStartY, keyStartX + keyWidth, keyStartY + keyHeight };

            char keyBindText[16];
            snprintf(keyBindText, sizeof(keyBindText), "Key Bind %d", i + 1);
            DrawText(memDC, keyBindText, -1, &keyRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            char enabledText[16];
            snprintf(
                enabledText, sizeof(enabledText), "Toggled: %s",
                (PrimaryKeyEnabled && i == 0) || (SecondaryKeyEnabled && i == 1) || (TertiaryKeyEnabled && i == 2) ? "Yes"
                                                                                                                   : "No");
            keyRect.top += 20;
            keyRect.bottom += 20;
            DrawText(memDC, enabledText, -1, &keyRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Enable key bind button underneath text
            RECT buttonRect = { keyStartX, keyStartY + keyHeight + 20, keyStartX + keyWidth,
                                keyStartY + keyHeight + 20 + keyHeight };
            FrameRect(memDC, &buttonRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
            DrawText(memDC, "Toggle", -1, &buttonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            for (int p = 0; p < 3; ++p)
            {
                WeaponRecoil recoilData = Files::GetWeaponData(weapons[i], p + 1);

                RECT btnRect = { buttonStartX, buttonStartY + p * (buttonHeight + buttonSpacing), buttonStartX + buttonWidth,
                                 buttonStartY + p * (buttonHeight + buttonSpacing) + buttonHeight };

                HBRUSH lineBrush = CreateSolidBrush(LineColour);
                FrameRect(memDC, &btnRect, lineBrush);

                char presetText[32];
                snprintf(
                    presetText, sizeof(presetText), "Preset %d: V:%.2f, H:%.2f", p + 1, recoilData.Vertical,
                    recoilData.Horizontal);

                HFONT btnFont = Font::GetMediumFont();
                HFONT oldBtnFont = (HFONT)SelectObject(memDC, btnFont);

                SetTextColor(memDC, TextColour);
                DrawText(memDC, presetText, -1, &btnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(memDC, oldBtnFont);

                DeleteObject(lineBrush);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        // Note text
        int sectionTop = bottom - LayoutUtils::WeaponDisplayLayout::GetSectionOffset(bottom);
        const char* noteText = "Click a weapon or preset to select recoil settings.";
        Font::DrawCenteredText(memDC, noteText, 0, sectionTop + 260, right, Font::GetMediumFont());

        // Back button
        RECT backBtn = { 30, bottom - 80, 130, bottom - 31 };
        DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(GRAY_BRUSH));
    }

} // namespace Drawing
