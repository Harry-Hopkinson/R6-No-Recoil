#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <thread>

#include "resource/resource.h"

#include "core/Bitmap.hpp"
#include "core/File.hpp"
#include "core/utils/String.hpp"
#include "core/utils/max.hpp"

#include "ui/Font.hpp"

#include "config/Config.hpp"

#include "recoil/Recoil.hpp"
#include "recoil/threads/ApplyRecoil.hpp"
#include "recoil/threads/ToggleRecoil.hpp"

#include "ui/Button.hpp"
#include "ui/UI.hpp"

// Window Procedure for handling events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            Running = false;
            DestroyWindow(hwnd);
            return 0;
        }
        break;

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case 1: // Enable/Disable Recoil
                    EnableRC = !EnableRC;
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 2: // Mode Selection
                    SelectedMode = (SelectedMode + 1) % 6;
                    CurrentRecoil = RecoilPresets[SelectedMode];
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 3: // Toggle Key
                    UseToggleKey = !UseToggleKey;
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 4: // Attacker Selection
                    IsAttackerView = true;
                    CurrentUIState = UIState::OperatorSelection;
                    for (const auto& button : Buttons) ShowWindow(button.GetHWND(), SW_HIDE);
                    CreateOperatorSelectionButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 5: // Defender Selection
                    IsAttackerView = false;
                    CurrentUIState = UIState::OperatorSelection;
                    for (const auto& button : Buttons) ShowWindow(button.GetHWND(), SW_HIDE);
                    CreateOperatorSelectionButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 6: // Back to Menu
                    CurrentUIState = UIState::LandingPage;
                    for (const auto& button : Buttons) ShowWindow(button.GetHWND(), SW_HIDE);
                    CreateLandingPageButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 7: // Support the Project button
                    system("start https://buymeacoffee.com/harryhopkinson");
                    break;
                case 8: // GitHub button
                    system("start https://github.com/Harry-Hopkinson/R6-No-Recoil");
                    break;
                case 9: // Info Screen button
                    CurrentUIState = UIState::InfoScreen;
                    for (const auto& button : Buttons) ShowWindow(button.GetHWND(), SW_HIDE);
                    CreateInfoScreenButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
        }
        break;

        case WM_CREATE:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            CurrentUIState = UIState::LandingPage;
            CreateLandingPageButtons(hwnd);

            Bitmap::AttackerBitmaps = Bitmap::LoadOperatorBitmaps(AttackerNames, GetImagePath);
            Bitmap::DefenderBitmaps = Bitmap::LoadOperatorBitmaps(DefenderNames, GetImagePath);

            Font::CreateFonts();
        }
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ oldBitmap = SelectObject(memDC, memBitmap);

            FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

            if (CurrentUIState == UIState::LandingPage)
            {
                SetBkMode(memDC, TRANSPARENT);

                int leftColumnWidth = (rect.right / 2) - 340;
                int rightColumnX = (rect.right / 2) + 20;
                int rightColumnWidth = (rect.right / 2) - 60;

                HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
                SetTextColor(memDC, RGB(220, 50, 50));
                RECT titleRect = { 5, 25, rect.right + 5, 75 };
                DrawText(memDC, "R6 NO RECOIL", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectObject(memDC, Font::GetSubtitleFont());
                SetTextColor(memDC, RGB(100, 100, 100));
                RECT subtitleRect = { 5, 80, rect.right + 5, 110 };
                DrawText(
                    memDC, "Advanced Recoil Compensation System", -1, &subtitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Left Column - Features
                SelectObject(memDC, Font::GetSubtitleFont());
                SetTextColor(memDC, RGB(50, 50, 50));
                RECT featuresHeaderRect = { 320, 140, 320 + leftColumnWidth, 170 };
                DrawText(memDC, "Key Features", -1, &featuresHeaderRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                SelectObject(memDC, Font::GetDescFont());
                SetTextColor(memDC, RGB(70, 70, 70));
                int leftCurrentY = 185;

                const char* features[] = { "- All R6 operators supported",
                                           "- Multiple sensitivity presets",
                                           "- Customisable toggle controls",
                                           "- Minimal system impact",
                                           "- Powerful config support" };

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

                const char* steps[] = { "1. Choose Attacker or Defender",
                                        "2. Select your operator",
                                        "3. Pick your primary weapon",
                                        "4. Start playing - it's automatic!",
                                        "5. Use toggle key to enable/disable" };

                const size_t stepCount = sizeof(steps) / sizeof(steps[0]);

                for (size_t i = 0; i < stepCount; i++)
                {
                    RECT stepRect = { rightColumnX + 10, rightCurrentY, rightColumnX + rightColumnWidth, rightCurrentY + 25 };
                    DrawText(memDC, steps[i], -1, &stepRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    rightCurrentY += 28;
                }

                int ctaY = maximum(leftCurrentY, rightCurrentY) + 40;

                RECT ctaBoxRect = { rect.right / 4, ctaY - 10, (rect.right * 3) / 4, ctaY + 70 };
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
                RECT ctaHeaderRect = { 0, ctaY + 5, rect.right, ctaY + 30 };
                DrawText(memDC, "Ready to Get Started?", -1, &ctaHeaderRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectObject(memDC, Font::GetDescFont());
                SetTextColor(memDC, RGB(70, 70, 70));
                RECT ctaTextRect = { 0, ctaY + 32, rect.right, ctaY + 55 };
                DrawText(
                    memDC, "Select your team below to begin configuration", -1, &ctaTextRect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Bottom section - Credits and links
                int bottomY = rect.bottom - 120;

                // Divider line
                HPEN dividerPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                HPEN oldDividerPen = (HPEN)SelectObject(memDC, dividerPen);
                MoveToEx(memDC, 60, bottomY, NULL);
                LineTo(memDC, rect.right - 60, bottomY);
                SelectObject(memDC, oldDividerPen);
                DeleteObject(dividerPen);

                // Footer info in two columns
                SelectObject(memDC, Font::GetDescFont());
                SetTextColor(memDC, RGB(120, 120, 120));

                RECT creatorRect = { 60, bottomY + 15, rect.right / 2, bottomY + 35 };
                DrawText(memDC, "Created by Harry Hopkinson", -1, &creatorRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                RECT versionRect = { rect.right / 2, bottomY + 15, rect.right - 60, bottomY + 35 };
                DrawText(memDC, "Version 2.1 | Open Source", -1, &versionRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

                // GitHub link
                RECT githubRect = { 0, bottomY + 40, rect.right + 15, bottomY + 60 };
                DrawText(
                    memDC, "Visit GitHub for updates and source code", -1, &githubRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectObject(memDC, oldFont);
            }
            else if (CurrentUIState == UIState::OperatorSelection)
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

                // Mode section
                RECT modeRect = { infoBoxRect.left + sectionWidth + 10, infoBoxRect.top + 5,
                                  infoBoxRect.left + 2 * sectionWidth, infoBoxRect.bottom - 5 };
                char modeText[50];
                wsprintfA(modeText, "Mode: %s", Modes[SelectedMode]);
                DrawText(memDC, modeText, -1, &modeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                // Toggle key section
                RECT toggleRect = { infoBoxRect.left + 2 * sectionWidth + 10, infoBoxRect.top + 5,
                                    infoBoxRect.left + 3 * sectionWidth, infoBoxRect.bottom - 5 };
                char toggleText[50];
                wsprintfA(toggleText, "Toggle: %s", UseToggleKey ? "ON" : "OFF");
                DrawText(memDC, toggleText, -1, &toggleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                // Recoil settings section
                RECT recoilRect = { infoBoxRect.left + 3 * sectionWidth + 10, infoBoxRect.top + 5, infoBoxRect.right - 15,
                                    infoBoxRect.bottom - 5 };
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
            else if (CurrentUIState == UIState::WeaponDisplay)
            {
                SetBkMode(memDC, TRANSPARENT);

                const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex]
                                                          : DefenderNames[SelectedOperatorIndex];
                const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                                       : DefenderPrimaryWeapons[SelectedOperatorIndex];

                Font::DrawCenteredText(memDC, operatorName, 0, 260, rect.right, Font::GetLargeFont());

                const char* instruction = "Select a primary weapon:";
                Font::DrawCenteredText(memDC, instruction, 0, 300, rect.right, Font::GetMediumFont());

                const char* weapons[3] = { NULL, NULL, NULL };
                int weaponCount = StringUtils::ParseWeaponList(weaponStr, weapons, 3);

                int imgWidth = 400;
                int imgHeight = 150;

                int totalWidth = weaponCount * imgWidth + (weaponCount - 1);

                int startX = (rect.right - totalWidth) / 2;

                int availableHeight = rect.bottom - 120;
                int contentHeight = imgHeight + 50;
                int startY = 120 + (availableHeight - contentHeight) / 2;

                SetStretchBltMode(memDC, HALFTONE);
                SetBrushOrgEx(memDC, 0, 0, NULL);

                for (int i = 0; i < weaponCount; ++i)
                {
                    int x = startX + i * (imgWidth);
                    int y = startY;

                    HBITMAP weaponBmp = Bitmap::GetWeaponBitmap(weapons[i]);

                    Bitmap::DrawBitmap(memDC, weaponBmp, x, y, imgWidth, imgHeight, true);

                    RECT nameRect = { x, y + imgHeight + 15, x + imgWidth, y + imgHeight + 45 };
                    HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetLargeFont());

                    // Remove background - make text transparent
                    SetBkMode(memDC, TRANSPARENT);
                    SetTextColor(memDC, RGB(0, 0, 0));

                    DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                    SelectObject(memDC, oldFont);
                }

                StringUtils::FreeWeaponList(weapons, weaponCount);

                // Back button at the bottom
                RECT backBtn = { 30, rect.bottom - 80, 130, rect.bottom - 30 };
                DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
            }
            else if (CurrentUIState == UIState::InfoScreen)
            {
                SetBkMode(memDC, TRANSPARENT);

                // Title
                HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
                SetTextColor(memDC, RGB(220, 50, 50));
                RECT infoTitleRect = { 60, 30, rect.right - 60, 80 };
                DrawText(memDC, "IMPORTANT SETUP", -1, &infoTitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Subtitle
                SelectObject(memDC, Font::GetSubtitleFont());
                SetTextColor(memDC, RGB(100, 100, 100));
                RECT infoSubRect = { 60, 80, rect.right - 60, 120 };
                DrawText(
                    memDC, "For best performance and accuracy, follow these steps:", -1, &infoSubRect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Helper lambda for drawing lines
                auto DrawLine = [&](const wchar_t* text, int x, int& y, int right, HFONT font = nullptr,
                                    COLORREF color = RGB(60, 60, 60)) {
                    if (font)
                        SelectObject(memDC, font);
                    SetTextColor(memDC, color);
                    RECT r = { x, y, right, y + 28 };
                    DrawTextW(memDC, text, -1, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    y += 40;
                };

                SelectObject(memDC, Font::GetSubtitleFont());
                int y = 140;
                int left = 80;
                int right = rect.right - 80;

                DrawLine(L"1. Disable Windows mouse acceleration (Enhance Pointer Precision):", left, y, right);

                DrawLine(L"• Open Control Panel > Mouse > Pointer Options.", left + 20, y, right);
                DrawLine(L"• Uncheck 'Enhance pointer precision'. Click Apply.", left + 20, y, right);

                y += 10;
                DrawLine(L"2. Enable raw mouse input in Rainbow Six Siege:", left, y, right);

                DrawLine(L"• Close the game.", left + 20, y, right);
                DrawLine(
                    L"• Open GameSettings.ini in Documents\\My Games\\Rainbow Six - Siege\\RandomNumber", left + 20, y, right);
                DrawLine(L"• Set RawInputMouseKeyboard=1 and save.", left + 20, y, right);
                DrawLine(L"• Restart the game.", left + 20, y, right);

                y += 100;
                DrawLine(
                    L"These steps are required for consistent recoil compensation.", rect.right / 2 - 350, y, right,
                    Font::GetSubtitleFont(), RGB(220, 50, 50));

                SelectObject(memDC, oldFont);
            }

            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

            // Clean up
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            int mouseX = GET_X_LPARAM(lParam);
            int mouseY = GET_Y_LPARAM(lParam);
            RECT rect;
            GetClientRect(hwnd, &rect);

            if (CurrentUIState == UIState::OperatorSelection)
            {
                const auto& bitmaps = Bitmap::GetCurrentBitmapList();
                for (size_t i = 0; i < bitmaps.size(); ++i)
                {
                    int x = 30 + (i % 6) * (110 + 10);
                    int y = 50 + (int)(i / 6) * (110 + 10);

                    if (mouseX >= x && mouseX <= x + 110 && mouseY >= y && mouseY <= y + 110)
                    {
                        SelectedOperatorIndex = static_cast<int>(i);
                        CurrentUIState = UIState::WeaponDisplay;
                        for (const auto& button : Buttons) ShowWindow(button.GetHWND(), SW_HIDE);
                        InvalidateRect(hwnd, NULL, TRUE);
                        return 0;
                    }
                }
            }
            else if (CurrentUIState == UIState::WeaponDisplay)
            {
                const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                                       : DefenderPrimaryWeapons[SelectedOperatorIndex];

                const char* weapons[3] = { NULL, NULL, NULL };
                int weaponCount = StringUtils::ParseWeaponList(weaponStr, weapons, 3);

                int imgWidth = 400;
                int imgHeight = 150;
                int spacing = 60;

                int totalWidth = weaponCount * imgWidth + (weaponCount - 1) * spacing;
                int startX = (rect.right - totalWidth) / 2;

                int availableHeight = rect.bottom - 120;
                int contentHeight = imgHeight + 50;
                int startY = 120 + (availableHeight - contentHeight) / 2;

                for (int i = 0; i < weaponCount; ++i)
                {
                    int x = startX + i * (imgWidth + spacing);
                    int y = startY;

                    RECT clickRect = { x, y, x + imgWidth, y + imgHeight + 45 };

                    if (mouseX >= clickRect.left && mouseX <= clickRect.right && mouseY >= clickRect.top
                        && mouseY <= clickRect.bottom)
                    {
                        SetRecoilModeFromWeapon(weapons[i]);
                        SaveConfig();

                        CurrentUIState = UIState::OperatorSelection;
                        CreateOperatorSelectionButtons(hwnd);
                        InvalidateRect(hwnd, NULL, TRUE);

                        StringUtils::FreeWeaponList(weapons, weaponCount);
                        return 0;
                    }
                }

                StringUtils::FreeWeaponList(weapons, weaponCount);

                // Back button detection
                if (mouseX >= 30 && mouseX <= 130 && mouseY >= rect.bottom - 80 && mouseY <= rect.bottom - 30)
                {
                    CurrentUIState = UIState::OperatorSelection;
                    CreateOperatorSelectionButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
            }
        }
        break;

        case WM_DESTROY:
        {
            Bitmap::CleanupBitmaps(Bitmap::AttackerBitmaps);
            Bitmap::CleanupBitmaps(Bitmap::DefenderBitmaps);
            Bitmap::CleanupWeaponBitmaps();

            Font::Cleanup();

            Buttons.clear();
            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "NoRecoilWindow";
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClass(&wc);

    LoadConfig();

    HWND hwnd = CreateWindowEx(
        0, wc.lpszClassName, "R6 No Recoil", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
        CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    std::thread recoilThread(ApplyRecoil);
    std::thread toggleThread(ToggleRecoilListener);

    MSG msg = {};
    while (Running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                PostMessage(hwnd, WM_CLOSE, 0, 0);

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (recoilThread.joinable())
        recoilThread.join();
    if (toggleThread.joinable())
        toggleThread.join();
    return 0;
}
