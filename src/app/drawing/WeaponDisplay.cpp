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
        const char* operatorName = IsAttackerView
            ? AttackerNames[SelectedOperatorIndex]
            : DefenderNames[SelectedOperatorIndex];
        const char* weaponStr = IsAttackerView
            ? AttackerWeapons[SelectedOperatorIndex]
            : DefenderWeapons[SelectedOperatorIndex];

        SetBkMode(memDC, TRANSPARENT);

        int titleY = static_cast<int>(bottom * 0.05f);
        int subtitleY = static_cast<int>(bottom * 0.10f);

        Font::DrawCenteredText(
            memDC, operatorName, 0, titleY, right, Font::GetLargeFont());
        Font::DrawCenteredText(
            memDC, "Select a weapon:", 0, subtitleY, right,
            Font::GetMediumFont());

        // Parse weapon list
        const char* weapons[3] = { nullptr, nullptr, nullptr };
        int weaponCount = String::ParseWeaponList(weaponStr, weapons);

        int startX, startY;
        LayoutUtils::WeaponDisplayLayout::GetWeaponStartPosition(
            weaponCount, right, bottom, startX, startY);

        int weaponWidth = LayoutUtils::WeaponDisplayLayout::GetWeaponWidth(
            right);
        int weaponHeight = LayoutUtils::WeaponDisplayLayout::GetWeaponHeight(
            bottom);
        int spacing = LayoutUtils::WeaponDisplayLayout::GetWeaponSpacing(right);

        SetStretchBltMode(memDC, HALFTONE);
        SetBrushOrgEx(memDC, 0, 0, nullptr);

        int buttonWidth = static_cast<int>(weaponWidth * 0.5f);
        int buttonHeight = static_cast<int>(bottom * 0.049f);
        int buttonSpacing = static_cast<int>(bottom * 0.011f);

        int keyWidth = static_cast<int>(weaponWidth * 0.25f);
        int keyHeight = static_cast<int>(bottom * 0.032f);

        for (int i = 0; i < weaponCount; ++i)
        {
            int x = startX + i * (weaponWidth + spacing);
            int y = startY;

            HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);
            Bitmap::DrawBitmap(
                memDC, weaponBmp, x, y, weaponWidth, weaponHeight, 0, true);

            // Weapon name
            HFONT weaponFont = Font::GetLargeFont();
            HFONT oldFont = (HFONT)SelectObject(memDC, weaponFont);
            SetTextColor(memDC, TextColour);

            int nameOffsetY = static_cast<int>(weaponHeight * 0.1f);
            int nameHeight = static_cast<int>(bottom * 0.049f);

            RECT nameRect = { x, y + weaponHeight + nameOffsetY,
                              x + weaponWidth,
                              y + weaponHeight + nameOffsetY + nameHeight };
            DrawText(
                memDC, weapons[i], -1, &nameRect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(memDC, oldFont);

            // Preset buttons
            int buttonStartY = y + weaponHeight + nameOffsetY + nameHeight
                + static_cast<int>(bottom * 0.016f); // Add some spacing
            int buttonStartX = x + (weaponWidth - buttonWidth) / 2;

            // Key bindings
            int keyStartX = x + (weaponWidth - keyWidth) / 2;
            int keyStartY = buttonStartY + 3 * (buttonHeight + buttonSpacing)
                + static_cast<int>(bottom * 0.032f);

            RECT keyRect = { keyStartX, keyStartY, keyStartX + keyWidth,
                             keyStartY + keyHeight };

            char keyBindText[16];
            snprintf(keyBindText, sizeof(keyBindText), "Key Bind %d", i + 1);
            DrawText(
                memDC, keyBindText, -1, &keyRect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Enabled status
            char enabledText[16];
            snprintf(
                enabledText, sizeof(enabledText), "Toggled: %s",
                (PrimaryKeyEnabled && i == 0) || (SecondaryKeyEnabled && i == 1)
                        || (TertiaryKeyEnabled && i == 2)
                    ? "Yes"
                    : "No");

            int statusOffsetY = static_cast<int>(keyHeight * 0.67f);
            keyRect.top += statusOffsetY;
            keyRect.bottom += statusOffsetY;
            DrawText(
                memDC, enabledText, -1, &keyRect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Toggle button
            HBRUSH buttonBrush = CreateSolidBrush(LineColour);
            int toggleOffsetY = static_cast<int>(keyHeight * 0.67f);
            RECT buttonRect = { keyStartX,
                                keyStartY + keyHeight + toggleOffsetY,
                                keyStartX + keyWidth,
                                keyStartY + keyHeight + toggleOffsetY
                                    + keyHeight };
            FrameRect(memDC, &buttonRect, buttonBrush);
            DrawText(
                memDC, "Toggle", -1, &buttonRect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(buttonBrush);

            // Draw preset buttons
            for (int p = 0; p < 3; ++p)
            {
                WeaponRecoil recoilData = Files::GetWeaponData(
                    weapons[i], p + 1);

                RECT btnRect = {
                    buttonStartX,
                    buttonStartY + p * (buttonHeight + buttonSpacing),
                    buttonStartX + buttonWidth,
                    buttonStartY + p * (buttonHeight + buttonSpacing)
                        + buttonHeight
                };

                HBRUSH lineBrush = CreateSolidBrush(LineColour);
                FrameRect(memDC, &btnRect, lineBrush);

                char presetText[32];
                snprintf(
                    presetText, sizeof(presetText), "Preset %d: V:%.2f, H:%.2f",
                    p + 1, recoilData.Vertical, recoilData.Horizontal);

                HFONT btnFont = Font::GetMediumFont();
                HFONT oldBtnFont = (HFONT)SelectObject(memDC, btnFont);

                SetTextColor(memDC, TextColour);
                DrawText(
                    memDC, presetText, -1, &btnRect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(memDC, oldBtnFont);

                DeleteObject(lineBrush);
            }
        }

        String::FreeWeaponList(weapons, weaponCount);

        // Note text
        int noteY = static_cast<int>(bottom * 0.92f);
        const char* noteText = "Click a weapon or preset to select recoil "
                               "settings.";
        Font::DrawCenteredText(
            memDC, noteText, 0, noteY, right, Font::GetMediumFont());

        // Back button
        int backBtnWidth = static_cast<int>(right * 0.083f);
        int backBtnHeight = static_cast<int>(bottom * 0.053f);
        int backBtnX = static_cast<int>(right * 0.025f);
        int backBtnY = bottom - backBtnHeight
            - static_cast<int>(bottom * 0.032f);

        RECT backBtn = { backBtnX, backBtnY, backBtnX + backBtnWidth,
                         backBtnY + backBtnHeight };
        DrawText(
            memDC, "Back", -1, &backBtn,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(GRAY_BRUSH));
    }

} // namespace Drawing
