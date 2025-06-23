#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

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

HFONT FontMedium = nullptr;
HFONT FontLarge = nullptr;

enum class UIState
{
    OperatorSelection,
    WeaponDisplay
};

UIState CurrentUIState = UIState::OperatorSelection;
int SelectedOperatorIndex = -1;

// Window Procedure for handling events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            Running = false;
            DestroyWindow(hwnd);
            return 0;

        case WM_COMMAND:
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
            break;

        case WM_CREATE:
        {
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 525, 570, 150, 40, "Toggle Recoil", 1);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 355, 570, 150, 40, "Change Mode", 2);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 180, 570, 150, 40, "Caps Lock Toggle", 3);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 450, 620, 150, 40, "Attackers", 4);
            Buttons.emplace_back(hwnd, WINDOW_WIDTH - 275, 620, 150, 40, "Defenders", 5);

            for (const auto& name : AttackerNames)
                AttackerBitmaps.push_back(LoadBitmap(GetImagePath(name)));

            for (const auto& name : DefenderNames)
                DefenderBitmaps.push_back(LoadBitmap(GetImagePath(name)));

            FontLarge = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            FontMedium = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
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

            auto DrawRightAlignedText = [&](LPCSTR text, int yOffset, int fontSize = 20)
            {
                HFONT selectedFont = (fontSize >= 28) ? FontLarge : FontMedium;
                HFONT oldFont = (HFONT)SelectObject(memDC, selectedFont);

                SIZE textSize;
                GetTextExtentPoint32(memDC, text, static_cast<int>(strlen(text)), &textSize);
                int textX = rect.right - 475 + (400 - textSize.cx) / 2;

                TextOut(memDC, textX, yOffset, text, static_cast<int>(strlen(text)));
                SelectObject(memDC, oldFont);
            };

            if (CurrentUIState == UIState::OperatorSelection)
            {
                HDC hdcMem = CreateCompatibleDC(memDC);
                const auto& bitmaps = GetCurrentBitmapList();
                for (size_t i = 0; i < bitmaps.size(); ++i)
                {
                    HBITMAP bmp = bitmaps[i];
                    if (!bmp) continue;

                    HGDIOBJ oldBmp = SelectObject(hdcMem, bmp);
                    BITMAP bm;
                    GetObject(bmp, sizeof(bm), &bm);

                    int x = 30 + (i % 6) * (145 + 1);
                    int y = (int)(i / 6) * (145 + 1);

                    StretchBlt(memDC, x, y, 145, 145, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                    SelectObject(hdcMem, oldBmp);
                }
                DeleteDC(hdcMem);

                SetBkMode(memDC, TRANSPARENT);
                DrawRightAlignedText("Recoil Control", 280, 28);
                DrawRightAlignedText("Enable:", 320);
                DrawRightAlignedText(EnableRC ? "ON" : "OFF", 342);
                DrawRightAlignedText("Mode:", 380);
                DrawRightAlignedText(Modes[SelectedMode], 402);
                DrawRightAlignedText("Caps Lock Toggle:", 440);
                DrawRightAlignedText(UseToggleKey ? "ENABLED" : "DISABLED", 462);

                char recoilInfo[40];
                wsprintfA(recoilInfo, "Vertical: %d  |  Horizontal: %d", CurrentRecoil.Vertical, CurrentRecoil.Horizontal);
                DrawRightAlignedText("Current Recoil Settings:", 500);
                DrawRightAlignedText(recoilInfo, 522);
            }
            else if (CurrentUIState == UIState::WeaponDisplay)
            {
                SetBkMode(memDC, TRANSPARENT);

                const char* operatorName = IsAttackerView ? AttackerNames[SelectedOperatorIndex] : DefenderNames[SelectedOperatorIndex];
                const char* weaponStr = IsAttackerView ? AttackerPrimaryWeapons[SelectedOperatorIndex] : DefenderPrimaryWeapons[SelectedOperatorIndex];

                DrawRightAlignedText(operatorName, 100, 28);
                DrawRightAlignedText("Select a weapon:", 150);

                std::vector<std::string> weapons;
                const char* ptr = weaponStr;
                while (*ptr)
                {
                    // Skip leading spaces
                    while (*ptr == ' ') ++ptr;

                    const char* start = ptr;
                    while (*ptr && *ptr != ',') ++ptr;

                    std::string weapon(start, ptr);
                    while (!weapon.empty() && weapon.back() == ' ')
                        weapon.pop_back();

                    if (!weapon.empty())
                        weapons.push_back(weapon);

                    if (*ptr == ',') ++ptr; // Skip the comma
                }

                int centerX = rect.right / 2;
                int y = 400;
                int width = 300, height = 60;

                RECT weaponRects[3] = {};
                for (size_t i = 0; i < weapons.size() && i < 3; ++i)
                {
                    RECT buttonRect = {0};
                    if (weapons.size() == 1)
                        buttonRect = { centerX - width / 2, y, centerX + width / 2, y + height };
                    else if (weapons.size() == 2)
                        buttonRect = { centerX + (i == 0 ? -width - 10 : 10), y, centerX + (i == 0 ? -10 : width + 10), y + height };
                    else if (weapons.size() == 3)
                        buttonRect = { centerX - width + static_cast<int>(i) * (width + 10), y, centerX - width + static_cast<int>(i) * (width + 10) + width, y + height };

                    DrawText(memDC, weapons[i].c_str(), -1, &buttonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    FrameRect(memDC, &buttonRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                    weaponRects[i] = buttonRect;
                }

                RECT backBtn = { 30, rect.bottom - 80, 130, rect.bottom - 30 };
                DrawText(memDC, "Back", -1, &backBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                FrameRect(memDC, &backBtn, (HBRUSH)GetStockObject(BLACK_BRUSH));
            }

            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

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
                const auto& bitmaps = GetCurrentBitmapList();
                for (size_t i = 0; i < bitmaps.size(); ++i)
                {
                    int x = 30 + (i % 6) * (145 + 1);
                    int y = (int)(i / 6) * (145 + 1);

                    if (mouseX >= x && mouseX <= x + 145 &&
                        mouseY >= y && mouseY <= y + 145)
                    {
                        SelectedOperatorIndex = static_cast<int>(i);
                        CurrentUIState = UIState::WeaponDisplay;
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

                std::vector<std::string> weapons;
                const char* ptr = weaponStr;
                while (*ptr)
                {
                        // Skip leading spaces
                        while (*ptr == ' ') ++ptr;

                        const char* start = ptr;
                        while (*ptr && *ptr != ',') ++ptr;

                        std::string weapon(start, ptr);
                        while (!weapon.empty() && weapon.back() == ' ')
                            weapon.pop_back();

                        if (!weapon.empty())
                            weapons.push_back(weapon);

                        if (*ptr == ',') ++ptr; // Skip the comma
                }

                int centerX = rect.right / 2;
                int y = 400;
                int width = 300, height = 60;

                for (size_t i = 0; i < weapons.size(); ++i)
                {
                    RECT r;
                    if (weapons.size() == 1)
                        r = { centerX - width / 2, y, centerX + width / 2, y + height };
                    else if (weapons.size() == 2)
                        r = { centerX + (i == 0 ? -width - 10 : 10), y, centerX + (i == 0 ? -10 : width + 10), y + height };
                    else
                        r = { centerX - width + static_cast<int>(i) * (width + 10), y,
                              centerX - width + static_cast<int>(i) * (width + 10) + width, y + height };

                    if (mouseX >= r.left && mouseX <= r.right && mouseY >= r.top && mouseY <= r.bottom)
                    {
                        CurrentUIState = UIState::OperatorSelection;
                        InvalidateRect(hwnd, NULL, TRUE);
                        return 0;
                    }
                }

                // Back button detection using client rect
                if (mouseX >= 30 && mouseX <= 130 &&
                    mouseY >= rect.bottom - 80 && mouseY <= rect.bottom - 30)
                {
                    CurrentUIState = UIState::OperatorSelection;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
            }
        }
        break;

        case WM_DESTROY:
        {
            for (HBITMAP bmp : AttackerBitmaps) if (bmp) DeleteObject(bmp);
            for (HBITMAP bmp : DefenderBitmaps) if (bmp) DeleteObject(bmp);
            AttackerBitmaps.clear();
            DefenderBitmaps.clear();

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
