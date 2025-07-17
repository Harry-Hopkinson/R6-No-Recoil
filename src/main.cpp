#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <thread>

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
                case 1:
                    EnableRC = !EnableRC;
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 2:
                    SelectedMode = (SelectedMode + 1) % 4;
                    CurrentRecoil = RecoilPresets[SelectedMode];
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 3:
                    UseToggleKey = !UseToggleKey;
                    SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 4:
                    IsAttackerView = true;
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 5:
                    IsAttackerView = false;
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
        }
        break;

        case WM_CREATE:
        {
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 525, 570, 150, 40, "Toggle Recoil", 1);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 355, 570, 150, 40, "Change Mode", 2);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 180, 570, 150, 40, "Caps Lock Toggle", 3);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 450, 620, 150, 40, "Attackers", 4);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 275, 620, 150, 40, "Defenders", 5);

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

            if (CurrentUIState == UIState::OperatorSelection)
            {
                const auto& bitmaps = Bitmap::GetCurrentBitmapList();
                for (size_t i = 0; i < bitmaps.size(); ++i)
                {
                    int x = 30 + (i % 6) * (160 + 1);
                    int y = (int)(i / 6) * (160 + 1);

                    Bitmap::DrawBitmap(memDC, bitmaps[i], x, y, 160, 160, true);
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

                Font::DrawCenteredText(memDC, operatorName, 0, 360, rect.right, Font::GetLargeFont());

                const char* instruction = "Select a primary weapon:";
                Font::DrawCenteredText(memDC, instruction, 0, 400, rect.right, Font::GetMediumFont());

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

                SetStretchBltMode(memDC, HALFTONE);
                SetBrushOrgEx(memDC, 0, 0, NULL);

                for (int i = 0; i < weaponCount; ++i)
                {
                    int x = startX + i * (imgWidth + spacing);
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

                // Free weapon name memory
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
                    int x = 30 + (i % 6) * (160 + 1);
                    int y = (int)(i / 6) * (160 + 1);

                    if (mouseX >= x && mouseX <= x + 160 &&
                        mouseY >= y && mouseY <= y + 160)
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
                        for (const auto& button : Buttons)
                        {
                            ShowWindow(button.GetHWND(), SW_SHOW);
                        }
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
                    for (const auto& button : Buttons)
                    {
                        ShowWindow(button.GetHWND(), SW_SHOW);
                    }
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
