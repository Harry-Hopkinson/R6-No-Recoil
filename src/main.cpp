#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>

#include "config/Config.hpp"

#include "recoil/ApplyRecoil.hpp"
#include "recoil/ToggleRecoil.hpp"

#include "ui/Button.hpp"
#include "Globals.hpp"

#include "core/File.hpp"

std::vector<HBITMAP> OperatorBitmaps;
std::vector<const char*> OperatorNames =
{
    "Rauora", "Skopos", "Striker", "Sentry", "Deimos", "Tubarao",
    "Ram", "Fenrir", "Brava", "Solis", "Grim", "Sens",
    "Azami", "Thorn", "Osa", "Thunderbird", "Flores", "Aruni",
    "Zero", "Ace", "Melusi", "Oryx", "Iana", "Wamai",
    "Kali", "Amaru", "Goyo", "Nokk", "Warden", "Mozzie",
    "Gridlock", "Nomad", "Kaid", "Clash", "Maverick",
    "Maestro", "Alibi", "Lion", "Finka", "Vigil",
    "Dokkaebi", "Zofia", "Ela", "Ying", "Lesion",
    "Mira", "Jackal", "Hibana", "Echo", "Caveira",
    "Capitao", "Blackbeard", "Valkyrie", "Buck", "Frost",
    "Mute", "Sledge", "Smoke", "Thatcher", "Ash",
    "Castle", "Pulse", "Thermite", "Montagne", "Twitch",
    "Doc", "Rook", "Jager", "Bandit", "Blitz", "iq",
    "Fuze", "Glaz", "Tachanka", "Kapkan"
};

void DrawCenteredText(HDC hdc, LPCSTR text, int yOffset, int windowWidth)
{
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, static_cast<int>(strlen(text)), &textSize);
    int textX = (windowWidth - textSize.cx) / 2;
    TextOut(hdc, textX, yOffset, text, static_cast<int>(strlen(text)));
}

// Window Procedure for handling events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam)
{
    static HBITMAP kaidImage;
    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

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
        } break;

        case WM_CREATE:
        {
            Buttons.emplace_back(hwnd, 30, 320, 130, 40, "Toggle Recoil", 1);
            Buttons.emplace_back(hwnd, 30 + (130 + 20), 320, 130, 40, "Change Mode",
                           2);
            Buttons.emplace_back(hwnd, 30 + 2 * (130 + 20), 320, 130, 40,
                            "Caps Lock Toggle", 4);

            kaidImage = LoadBitmap("assets\\kaid.bmp");

            for (const auto& name : OperatorNames)
            {
                HBITMAP bmp = LoadBitmap(GetImagePath(name).c_str());
                OperatorBitmaps.push_back(bmp);
            }
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);

            // Bolder text
            SetBkMode(hdc, TRANSPARENT);

            DrawCenteredText(hdc, "Recoil Control", 30, rect.right);
            DrawCenteredText(hdc, "Enable:", 70, rect.right);
            DrawCenteredText(hdc, EnableRC ? "ON" : "OFF", 90, rect.right);
            DrawCenteredText(hdc, "Mode:", 130, rect.right);
            DrawCenteredText(hdc, Modes[SelectedMode], 150, rect.right);
            DrawCenteredText(hdc, ModeDescriptions[SelectedMode], 170, rect.right);

            DrawCenteredText(hdc, "Caps Lock Toggle:", 200, rect.right);
            DrawCenteredText(hdc, UseToggleKey ? "ENABLED" : "DISABLED", 220,
                            rect.right);

            // Display current recoil values
            char recoilInfo[40];
            wsprintfA(recoilInfo, "Vertical: %d  |  Horizontal: %d",
                      CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
            DrawCenteredText(hdc, "Current Recoil Settings:", 260, rect.right);
            DrawCenteredText(hdc, recoilInfo, 280, rect.right);

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, kaidImage);

            GetObject(kaidImage, sizeof(bitmap), &bitmap);
            BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight,
                hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);

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
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
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
