#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <thread>

#include "resource/resource.h"

#include "core/Bitmap.hpp"
#include "core/File.hpp"
#include "core/utils/String.hpp"
#include "ui/Font.hpp"

#include "Globals.hpp"

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
                    SelectedMode = (SelectedMode + 1) % 4;
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
                case 7: // GitHub button
                    system("start https://github.com/Harry-Hopkinson/R6-No-Recoil");
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
                SetTextColor(memDC, RGB(30, 30, 30));

                // Main title - centered horizontally, positioned relative to window height
                HFONT oldFont = (HFONT)SelectObject(memDC, Font::GetTitleFont());
                SetTextColor(memDC, RGB(220, 50, 50)); // R6 Siege red color
                RECT titleRect = {0, rect.bottom / 10, rect.right, rect.bottom / 10 + 80};
                DrawText(memDC, "R6 NO RECOIL", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Subtitle
                SelectObject(memDC, Font::GetSubtitleFont());
                SetTextColor(memDC, RGB(50, 50, 50));

                // Description section - centered with margins
                SelectObject(memDC, Font::GetDescFont());
                SetTextColor(memDC, RGB(70, 70, 70));

                const char* descriptions[] =
                {
                    "Automatically compensates for weapon recoil patterns",
                    "Supports all Rainbow Six Siege operators and weapons",
                    "Customizable recoil modes and sensitivity settings",
                    "Toggle on/off with Caps Lock or mouse click",
                    "Lightweight and efficient background operation",
                    "",
                    "How it works:",
                    "1. Select your operator (Attacker or Defender)",
                    "2. Choose your primary weapon",
                    "3. The system automatically applies recoil compensation",
                    "4. Toggle the feature on/off as needed during gameplay"
                };

                // Calculate starting position relative to window size
                int margin = rect.right / 8;  // 12.5% margin on each side
                int startY = rect.bottom / 4; // Start at 25% down the window
                const size_t descCount = sizeof(descriptions) / sizeof(descriptions[0]);

                for (size_t i = 0; i < descCount; i++)
                {
                    RECT descRect = {margin, startY + (int)(i * 25), rect.right - margin, startY + (int)(i * 25) + 25};
                    DrawText(memDC, descriptions[i], -1, &descRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }

                // Team selection instruction - centered, positioned relative to content
                SelectObject(memDC, Font::GetSubtitleFont());
                SetTextColor(memDC, RGB(30, 30, 30));
                int instructY = startY + (descCount * 25) + 40;
                RECT instructRect = {0, instructY, rect.right, instructY + 40};
                DrawText(memDC, "Select Your Team:", -1, &instructRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                // Footer - centered at bottom
                SelectObject(memDC, Font::GetDescFont());
                SetTextColor(memDC, RGB(100, 100, 100));
                RECT footerRect = {0, rect.bottom - 60, rect.right, rect.bottom - 40};
                DrawText(memDC, "Harry Hopkinson", -1, &footerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                SelectObject(memDC, oldFont);
            }
            else if (CurrentUIState == UIState::OperatorSelection)
            {
                const auto& bitmaps = Bitmap::GetCurrentBitmapList();
                for (size_t i = 0; i < bitmaps.size(); ++i)
                {
                    int x = 30 + (i % 6) * (110 + 10);
                    int y = (int)(i / 6) * (110 + 10);

                    Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, 110, 110, true);
                }

                SetBkMode(memDC, TRANSPARENT);
                Font::DrawRightAlignedText(memDC, "Recoil Control", 280, 28, rect);
                Font::DrawRightAlignedText(memDC, "Enable:", 320, 20, rect);
                Font::DrawRightAlignedText(memDC, EnableRC ? "ON" : "OFF", 342, 20, rect);
                Font::DrawRightAlignedText(memDC, "Mode:", 380, 20, rect);
                Font::DrawRightAlignedText(memDC, Modes[SelectedMode], 402, 20, rect);
                Font::DrawRightAlignedText(memDC, "Caps Lock Toggle:", 440, 20, rect);
                Font::DrawRightAlignedText(memDC, UseToggleKey ? "ENABLED" : "DISABLED", 462, 20, rect);

                char recoilInfo[40];
                wsprintfA(recoilInfo, "Vertical: %d  |  Horizontal: %d", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
                Font::DrawRightAlignedText(memDC, "Current Recoil Settings:", 500, 20, rect);
                Font::DrawRightAlignedText(memDC, recoilInfo, 522, 20, rect);
            }
            else if (CurrentUIState == UIState::WeaponDisplay)
            {
                SetBkMode(memDC, TRANSPARENT);

                const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
                const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex] : DefenderPrimaryWeapons[SelectedOperatorIndex];

                Font::DrawCenteredText(memDC, operatorName, 0, 260, rect.right, Font::GetLargeFont());

                const char* instruction = "Select a primary weapon:";
                Font::DrawCenteredText(memDC, instruction, 0, 300, rect.right, Font::GetMediumFont());

                const char* weapons[3] = {NULL, NULL, NULL};
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

                    RECT nameRect = {x, y + imgHeight + 15, x + imgWidth, y + imgHeight + 45};
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
                    int y = (int)(i / 6) * (110 + 10);

                    if (mouseX >= x && mouseX <= x + 110 &&
                        mouseY >= y && mouseY <= y + 110)
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
                const char* weaponStr = IsAttackerView
                                        ? AttackerPrimaryWeapons[SelectedOperatorIndex]
                                        : DefenderPrimaryWeapons[SelectedOperatorIndex];

                const char* weapons[3] = {NULL, NULL, NULL};
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

                    RECT clickRect = {x, y, x + imgWidth, y + imgHeight + 45};

                    if (mouseX >= clickRect.left && mouseX <= clickRect.right &&
                        mouseY >= clickRect.top && mouseY <= clickRect.bottom)
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
                if (mouseX >= 30 && mouseX <= 130 &&
                    mouseY >= rect.bottom - 80 && mouseY <= rect.bottom - 30)
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

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "R6 No Recoil",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;

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

    if (recoilThread.joinable()) recoilThread.join();
    if (toggleThread.joinable()) toggleThread.join();
    return 0;
}
