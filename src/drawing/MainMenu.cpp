#include "../core/macros.h"
#include "../ui/widgets/Font.h"
#include "../utils/GdiHelpers.h"

namespace Drawing
{

    void DrawMainMenu(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        int leftColumnWidth = (right / 2) - 340;
        int rightColumnX = (right / 2) + 20;
        int rightColumnWidth = (right / 2) - 60;

        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
        SetTextColor(memDC, RGB(220, 50, 50));

        // Title
        RECT textRect = { 5, 25, right + 5, 75 };
        DrawText(memDC, "R6 NO RECOIL", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Subtitle
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(100, 100, 100));
        textRect = { 5, 80, right + 5, 110 };
        DrawText(memDC, "Advanced Recoil Compensation System", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Left Column - Features
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(50, 50, 50));
        textRect = { 320, 140, 320 + leftColumnWidth, 170 };
        DrawText(memDC, "Key Features", -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));

        int leftCurrentY = 185;
        const char* features[] = { "- All R6 operators supported",   "- Multiple sensitivity presets",
                                   "- Customisable toggle controls", "- Minimal system impact",
                                   "- Powerful config support",      "- Mouse and Controller Support" };
        size_t featureCount = sizeof(features) / sizeof(features[0]);

        for (size_t i = 0; i < featureCount; i++)
        {
            textRect.left = 330;
            textRect.top = leftCurrentY;
            textRect.right = 320 + leftColumnWidth;
            textRect.bottom = leftCurrentY + 25;
            DrawText(memDC, features[i], -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            leftCurrentY += 28;
        }

        // Right Column - How to Use
        int rightStartY = 140;
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(50, 50, 50));
        textRect = { rightColumnX, rightStartY, rightColumnX + rightColumnWidth, rightStartY + 30 };
        DrawText(memDC, "Quick Start Guide", -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));

        int rightCurrentY = rightStartY + 45;
        const char* steps[] = { "1. Choose Attacker or Defender",     "2. Select your operator",
                                "3. Pick your primary weapon",        "4. Choose your weapon preset for that gun",
                                "5. Start playing - it's automatic!", "6. Use toggle key to enable/disable" };
        size_t stepCount = sizeof(steps) / sizeof(steps[0]);

        for (size_t i = 0; i < stepCount; i++)
        {
            textRect.left = rightColumnX + 10;
            textRect.top = rightCurrentY;
            textRect.right = rightColumnX + rightColumnWidth;
            textRect.bottom = rightCurrentY + 25;
            DrawText(memDC, steps[i], -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            rightCurrentY += 28;
        }

        // Call to Action Box
        int ctaY = maximum(leftCurrentY, rightCurrentY) + 80;
        textRect = { right / 4, ctaY - 10, (right * 3) / 4, ctaY + 70 };
        GdiHelpers::FillRectColor(memDC, textRect, RGB(245, 245, 245));
        GdiHelpers::DrawRectBorder(memDC, textRect, RGB(220, 50, 50), 2);

        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(220, 50, 50));
        textRect = { 0, ctaY + 5, right, ctaY + 30 };
        DrawText(memDC, "Ready to Get Started?", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));
        textRect = { 0, ctaY + 32, right, ctaY + 55 };
        DrawText(memDC, "Select your team below to begin configuration", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Footer
        int bottomY = bottom - 120;

        GdiHelpers::ScopedPen dividerPen(PS_SOLID, 1, RGB(200, 200, 200));
        GdiHelpers::ScopedSelectObject select(memDC, dividerPen);
        MoveToEx(memDC, 60, bottomY, nullptr);
        LineTo(memDC, right - 60, bottomY);

        SetTextColor(memDC, RGB(120, 120, 120));

        textRect = { 60, bottomY + 15, right / 2, bottomY + 35 };
        DrawText(memDC, "Created by Harry Hopkinson", -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        textRect = { right / 2, bottomY + 15, right - 60, bottomY + 35 };
        DrawText(memDC, "Version 2.7 | Open Source", -1, &textRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

        textRect = { 0, bottomY + 40, right + 15, bottomY + 60 };
        DrawText(memDC, "Visit GitHub for updates and source code", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, oldFont);
    }

} // namespace Drawing
