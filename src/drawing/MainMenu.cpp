#include <windows.h>

#include "../ui/widgets/Font.h"

#define maximum(a, b) ((a) > (b) ? (a) : (b))

namespace Drawing
{

    void DrawMainMenu(HDC memDC, int right, int bottom)
    {
        SetBkMode(memDC, TRANSPARENT);

        int leftColumnWidth = (right / 2) - 340;
        int rightColumnX = (right / 2) + 20;
        int rightColumnWidth = (right / 2) - 60;

        // Title
        HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
        SetTextColor(memDC, RGB(220, 50, 50));
        RECT titleRect = { 5, 25, right + 5, 75 };
        DrawText(memDC, "R6 NO RECOIL", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Subtitle
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(100, 100, 100));
        RECT subtitleRect = { 5, 80, right + 5, 110 };
        DrawText(memDC, "Advanced Recoil Compensation System", -1, &subtitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Left Column - Features
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(50, 50, 50));
        RECT featuresHeaderRect = { 320, 140, 320 + leftColumnWidth, 170 };
        DrawText(memDC, "Key Features", -1, &featuresHeaderRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));
        int leftCurrentY = 185;

        const char* features[] = { "- All R6 operators supported", "- Multiple sensitivity presets",
                                   "- Customisable toggle controls", "- Minimal system impact", "- Powerful config support" };
        const size_t featureCount = sizeof(features) / sizeof(features[0]);

        for (size_t i = 0; i < featureCount; i++)
        {
            RECT featureRect = { 330, leftCurrentY, 320 + leftColumnWidth, leftCurrentY + 25 };
            DrawText(memDC, features[i], -1, &featureRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            leftCurrentY += 28;
        }

        // Right Column - How to Use
        int rightStartY = 140;
        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(50, 50, 50));
        RECT stepsHeaderRect = { rightColumnX, rightStartY, rightColumnX + rightColumnWidth, rightStartY + 30 };
        DrawText(memDC, "Quick Start Guide", -1, &stepsHeaderRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));
        int rightCurrentY = rightStartY + 45;

        const char* steps[] = { "1. Choose Attacker or Defender", "2. Select your operator", "3. Pick your primary weapon",
                                "4. Start playing - it's automatic!", "5. Use toggle key to enable/disable" };
        const size_t stepCount = sizeof(steps) / sizeof(steps[0]);

        for (size_t i = 0; i < stepCount; i++)
        {
            RECT stepRect = { rightColumnX + 10, rightCurrentY, rightColumnX + rightColumnWidth, rightCurrentY + 25 };
            DrawText(memDC, steps[i], -1, &stepRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            rightCurrentY += 28;
        }

        int ctaY = maximum(leftCurrentY, rightCurrentY) + 80;

        // CTA Box
        RECT ctaBoxRect = { right / 4, ctaY - 10, (right * 3) / 4, ctaY + 70 };
        HBRUSH ctaBrush = CreateSolidBrush(RGB(245, 245, 245));
        FillRect(memDC, &ctaBoxRect, ctaBrush);
        DeleteObject(ctaBrush);

        HPEN ctaPen = CreatePen(PS_SOLID, 2, RGB(220, 50, 50));
        HPEN oldPen = (HPEN)SelectObject(memDC, ctaPen);
        Rectangle(memDC, ctaBoxRect.left, ctaBoxRect.top, ctaBoxRect.right, ctaBoxRect.bottom);
        SelectObject(memDC, oldPen);
        DeleteObject(ctaPen);

        SelectObject(memDC, Font::GetSubtitleFont());
        SetTextColor(memDC, RGB(220, 50, 50));
        RECT ctaHeaderRect = { 0, ctaY + 5, right, ctaY + 30 };
        DrawText(memDC, "Ready to Get Started?", -1, &ctaHeaderRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(70, 70, 70));
        RECT ctaTextRect = { 0, ctaY + 32, right, ctaY + 55 };
        DrawText(
            memDC, "Select your team below to begin configuration", -1, &ctaTextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // Footer
        int bottomY = bottom - 120;

        HPEN dividerPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        HPEN oldDividerPen = (HPEN)SelectObject(memDC, dividerPen);
        MoveToEx(memDC, 60, bottomY, NULL);
        LineTo(memDC, right - 60, bottomY);
        SelectObject(memDC, oldDividerPen);
        DeleteObject(dividerPen);

        SelectObject(memDC, Font::GetDescFont());
        SetTextColor(memDC, RGB(120, 120, 120));

        RECT creatorRect = { 60, bottomY + 15, right / 2, bottomY + 35 };
        DrawText(memDC, "Created by Harry Hopkinson", -1, &creatorRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        RECT versionRect = { right / 2, bottomY + 15, right - 60, bottomY + 35 };
        DrawText(memDC, "Version 2.5 | Open Source", -1, &versionRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

        RECT githubRect = { 0, bottomY + 40, right + 15, bottomY + 60 };
        DrawText(memDC, "Visit GitHub for updates and source code", -1, &githubRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(memDC, oldFont);
    }

} // namespace Drawing
