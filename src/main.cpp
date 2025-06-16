#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>

#include "config/Config.hpp"
#include "recoil/ApplyRecoil.hpp"
#include "recoil/ToggleRecoil.hpp"
#include "ui/Button.hpp"
#include "Globals.hpp"
#include "core/File.hpp"

bool IsAttackerView = true;
std::vector<HBITMAP> AttackerBitmaps;
std::vector<HBITMAP> DefenderBitmaps;
std::vector<const char*> AttackerNames =
{
    "Striker",     "Sledge",   "Thatcher",   "Ash",      "Thermite",
    "Twitch",      "Montagne", "Glaz",       "Fuze",     "Blitz",
    "IQ",          "Buck",     "Blackbeard", "Capitao",  "Hibana",
    "Jackal",      "Ying",     "Zofia",      "Dokkaebi", "Lion",
    "Finka",       "Maverick", "Nomad",      "Gridlock", "Nokk",
    "Amaru",       "Kali",     "Iana",       "Ace",      "Zero",
    "Flores",      "Osa",      "Sens",       "Grim",     "Brava",
    "Ram",         "Deimos",   "Rauora"
};
std::vector<const char*> DefenderNames =
{
    "Sentry",      "Smoke",    "Mute",       "Castle",   "Pulse",
    "Doc",         "Rook",     "Kapkan",     "Tachanka", "Jager",
    "Bandit",      "Frost",    "Valkyrie",   "Caveira",  "Echo",
    "Mira",        "Lesion",   "Ela",        "Vigil",    "Maestro",
    "Alibi",       "Clash",    "Kaid",       "Mozzie",   "Warden",
    "Goyo",        "Wamai",    "Oryx",       "Melusi",   "Aruni",
    "Thunderbird", "Thorn",    "Azami",      "Solis",    "Fenrir",
    "Tubarao",     "Skopos"
};
std::vector<HBITMAP>& GetCurrentBitmapList() { return IsAttackerView ? AttackerBitmaps : DefenderBitmaps; }

// Window Procedure for handling events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            Running = false;
            PostQuitMessage(0);
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
            Buttons.emplace_back(hwnd, GetSystemMetrics(SM_CXSCREEN) - 400 - 200, 320, 130, 40, "Toggle Recoil", 1);
            Buttons.emplace_back(hwnd, GetSystemMetrics(SM_CXSCREEN) - 600 + 200, 320, 130, 40, "Change Mode", 2);
            Buttons.emplace_back(hwnd, GetSystemMetrics(SM_CXSCREEN) - 800 + 400, 320, 130, 40, "Caps Lock Toggle", 3);
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

                int x = 30 + (i % 11) * (110 + 1);
                int y = 30 + (i / 11) * (110 + 1);

                StretchBlt(memDC, x, y, 110, 110, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                SelectObject(hdcMem, oldBmp);
            }

            DeleteDC(hdcMem);

            SetBkMode(memDC, TRANSPARENT);
            auto DrawRightAlignedText = [&](LPCSTR text, int yOffset)
            {
                SIZE textSize;
                GetTextExtentPoint32(memDC, text, static_cast<int>(strlen(text)), &textSize);
                int textX = rect.right - 400 + (400 - textSize.cx) / 2;
                TextOut(memDC, textX, yOffset, text, static_cast<int>(strlen(text)));
            };

            DrawRightAlignedText("Recoil Control", 30);
            DrawRightAlignedText("Enable:", 70);
            DrawRightAlignedText(EnableRC ? "ON" : "OFF", 90);
            DrawRightAlignedText("Mode:", 130);
            DrawRightAlignedText(Modes[SelectedMode], 150);
            DrawRightAlignedText(ModeDescriptions[SelectedMode], 170);
            DrawRightAlignedText("Caps Lock Toggle:", 200);
            DrawRightAlignedText(UseToggleKey ? "ENABLED" : "DISABLED", 220);

            char recoilInfo[40];
            wsprintfA(recoilInfo, "Vertical: %d  |  Horizontal: %d", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
            DrawRightAlignedText("Current Recoil Settings:", 260);
            DrawRightAlignedText(recoilInfo, 280);

            // Blit the entire memory DC to the screen at once
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

            // Cleanup
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
        } break;


        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);  // Close the window
        } break;

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
                       CW_USEDEFAULT, CW_USEDEFAULT, GetSystemMetrics(SM_CXSCREEN) - 200,
                       GetSystemMetrics(SM_CYSCREEN) - 200, nullptr, nullptr, hInstance, nullptr);

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
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (recoilThread.joinable()) recoilThread.join();
    if (toggleThread.joinable()) toggleThread.join();
    return 0;
}
