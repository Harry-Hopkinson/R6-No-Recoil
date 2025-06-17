#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>

#include "core/File.hpp"
#include "Globals.hpp"

#include "config/Config.hpp"

#include "recoil/ApplyRecoil.hpp"
#include "recoil/ToggleRecoil.hpp"

#include "ui/Button.hpp"

std::vector<HBITMAP> AttackerBitmaps;
std::vector<HBITMAP> DefenderBitmaps;
std::vector<HBITMAP>& GetCurrentBitmapList() { return IsAttackerView ? AttackerBitmaps : DefenderBitmaps; }

HFONT FontLarge = nullptr;
HFONT FontMedium = nullptr;

// Window Procedure for handling events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            Running = false;
            DestroyWindow(hwnd);
            return 0;
        } break;

        case WM_COMMAND:
        {
            if (LOWORD(wParam) == 1)  // Toggle Recoil Button
            {
                EnableRC = !EnableRC;
                SaveConfig();
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (LOWORD(wParam) == 2)  // Change Mode Button
            {
                SelectedMode = (SelectedMode + 1) % 4;
                CurrentRecoil = RecoilPresets[SelectedMode];
                SaveConfig();
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (LOWORD(wParam) == 3)  // Toggle Caps Lock Feature Button
            {
                UseToggleKey = !UseToggleKey;
                SaveConfig();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (LOWORD(wParam) == 4)  // Attackers Button
            {
                IsAttackerView = true;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (LOWORD(wParam) == 5)  // Defenders Button
            {
                IsAttackerView = false;
                InvalidateRect(hwnd, NULL, TRUE);
            }
        } break;

        case WM_CREATE:
        {
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 525, 570, 150, 40, "Toggle Recoil", 1);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 355, 570, 150, 40, "Change Mode", 2);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 180, 570, 150, 40, "Caps Lock Toggle", 3);
            Buttons.emplace_back(hwnd, 30, 550, 130, 40, "Attackers", 4);
            Buttons.emplace_back(hwnd, 180, 550, 130, 40, "Defenders", 5);

            // Load attacker bitmaps
            for (const auto& name : AttackerNames)
            {
                HBITMAP bmp = LoadBitmap(GetImagePath(name).c_str());
                AttackerBitmaps.push_back(bmp);
            }

            // Load defender bitmaps
            for (const auto& name : DefenderNames)
            {
                HBITMAP bmp = LoadBitmap(GetImagePath(name).c_str());
                DefenderBitmaps.push_back(bmp);
            }

            FontLarge = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            FontMedium = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);

            // Create a memory DC compatible with the screen
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ oldBitmap = SelectObject(memDC, memBitmap);

            // Fill the background
            FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw bitmaps
            HDC hdcMem = CreateCompatibleDC(memDC);
            HGDIOBJ oldBmp = nullptr;

            const auto& bitmaps = GetCurrentBitmapList();
            for (size_t i = 0; i < bitmaps.size(); ++i)
            {
                HBITMAP bmp = bitmaps[i];
                if (!bmp) continue;

                oldBmp = SelectObject(hdcMem, bmp);

                BITMAP bm;
                GetObject(bmp, sizeof(bm), &bm);

                int x = 30 + (i % 6) * (145 + 1);
                int y = (int)(i / 6) * (145 + 1);

                StretchBlt(memDC, x, y, 145, 145, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                SelectObject(hdcMem, oldBmp);
            }

            DeleteDC(hdcMem);

            SetBkMode(memDC, TRANSPARENT);
            auto DrawRightAlignedText = [&](LPCSTR text, int yOffset, int fontSize = 20)
            {
                HFONT selectedFont = FontMedium;
                if (fontSize >= 28) selectedFont = FontLarge;

                HFONT oldFont = (HFONT)SelectObject(memDC, selectedFont);

                SIZE textSize;
                GetTextExtentPoint32(memDC, text, static_cast<int>(strlen(text)), &textSize);
                int textX = rect.right - 475 + (400 - textSize.cx) / 2;

                TextOut(memDC, textX, yOffset, text, static_cast<int>(strlen(text)));

                SelectObject(memDC, oldFont);
            };

            DrawRightAlignedText("Recoil Control", 280, 28);
            DrawRightAlignedText("Enable:", 320, 20);
            DrawRightAlignedText(EnableRC ? "ON" : "OFF", 342, 20);
            DrawRightAlignedText("Mode:", 380, 20);
            DrawRightAlignedText(Modes[SelectedMode], 402, 20);
            DrawRightAlignedText("Caps Lock Toggle:", 440, 20);
            DrawRightAlignedText(UseToggleKey ? "ENABLED" : "DISABLED", 462, 20);

            char recoilInfo[40];
            wsprintfA(recoilInfo, "Vertical: %d  |  Horizontal: %d", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
            DrawRightAlignedText("Current Recoil Settings:", 500, 20);
            DrawRightAlignedText(recoilInfo, 522, 20);

            // Blit the entire memory DC to the screen at once
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

            // Cleanup
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
        } break;

        case WM_DESTROY:
        {
            // Delete loaded attacker bitmaps
            for (HBITMAP bmp : AttackerBitmaps)
            {
                if (bmp) DeleteObject(bmp);
            }
            AttackerBitmaps.clear();

            // Delete loaded defender bitmaps
            for (HBITMAP bmp : DefenderBitmaps)
            {
                if (bmp) DeleteObject(bmp);
            }
            DefenderBitmaps.clear();

            // Delete fonts
            if (FontLarge) DeleteObject(FontLarge);
            if (FontMedium) DeleteObject(FontMedium);
            FontLarge = FontMedium = nullptr;

            Buttons.clear();

            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
                   LPSTR, int nCmdShow)
{
    // Register Window Class
    WNDCLASS wc = {0};

    wc.style         =  CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = "NoRecoilWindow";
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = WindowProc;
    RegisterClass(&wc);

    LoadConfig();

    // Create Window
    HWND hwnd =
        CreateWindowEx(0, wc.lpszClassName, "R6 No Recoil",
                       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                       CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
                       WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    std::thread recoilThread(ApplyRecoil);
    std::thread toggleThread(ToggleRecoilListener);

    // Message Loop
    MSG msg = {};
    while (Running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
            {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                continue;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (recoilThread.joinable()) recoilThread.join();
    if (toggleThread.joinable()) toggleThread.join();
    return 0;
}
