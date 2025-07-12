#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <thread>
#include <vector>

#include "core/File.hpp"
#include "Globals.hpp"

#include "config/Config.hpp"

#include "recoil/ApplyRecoil.hpp"
#include "recoil/ToggleRecoil.hpp"

#include "ui/Button.hpp"

std::vector<HBITMAP> AttackerBitmaps;
std::vector<HBITMAP> DefenderBitmaps;
std::vector<HBITMAP>& GetCurrentBitmapList() { return IsAttackerView ? AttackerBitmaps : DefenderBitmaps; }

struct WeaponBitmapEntry
{
    const char* name;
    HBITMAP bitmap;
};
std::vector<WeaponBitmapEntry> WeaponBitmaps;

char* RemoveSpaces(const char* input)
{
    if (!input) return nullptr;

    int len = strlen(input);
    char* result = new char[len + 1]; // +1 for null terminator
    int j = 0;

    for (int i = 0; i < len; i++)
    {
        if (input[i] != ' ')
        {
            result[j++] = input[i];
        }
    }
    result[j] = '\0'; // Null terminate

    return result;
}

// Concatenate path and filename
char* BuildPath(const char* dir, const char* filename)
{
    int len1 = strlen(dir);
    int len2 = strlen(filename);
    int len3 = strlen(".bmp");

    char* result = new char[len1 + len2 + len3 + 1]; // +1 for null terminator
    strcpy_s(result, len1 + len2 + len3 + 1, dir);
    strcat_s(result, len1 + len2 + len3 + 1, filename);
    strcat_s(result, len1 + len2 + len3 + 1, ".bmp");

    return result;
}

HBITMAP LoadWeaponBitmap(const char* weaponName)
{
    char* cleanName = RemoveSpaces(weaponName);
    if (!cleanName) return nullptr;

    char* path = BuildPath("assets/weapons/", cleanName);
    HBITMAP bitmap = LoadBitmap(path);

    if (!bitmap)
    {
        char debugMsg[256];
        sprintf_s(debugMsg, "Failed to load weapon bitmap: %s", path);
        OutputDebugStringA(debugMsg);
    }

    // Free the allocated strings
    delete[] cleanName;
    delete[] path;

    return bitmap;
}

HBITMAP GetWeaponBitmap(const char* weaponName)
{
    // Search the cache first
    for (const auto& entry : WeaponBitmaps)
    {
        if (strcmp(entry.name, weaponName) == 0)
            return entry.bitmap;
    }

    // Not found in cache, load the bitmap
    HBITMAP bmp = LoadWeaponBitmap(weaponName);
    if (bmp)
    {
        // Make a copy of the name to store in our cache
        int len = strlen(weaponName);
        char* nameCopy = new char[len + 1];
        strcpy_s(nameCopy, len + 1, weaponName);

        WeaponBitmapEntry entry;
        entry.name = nameCopy;
        entry.bitmap = bmp;
        WeaponBitmaps.push_back(entry);
    }
    return bmp;
}

void ShowAllButtons(bool show)
{
    for (const auto& button : Buttons)
    {
        ShowWindow(button.GetHWND(), show ? SW_SHOW : SW_HIDE);
    }
}

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

            FontLarge = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
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

                // Display operator name at the top
                HFONT oldFont = (HFONT)SelectObject(memDC, FontLarge);
                SIZE textSize;
                GetTextExtentPoint32(memDC, operatorName, static_cast<int>(strlen(operatorName)), &textSize);
                TextOut(memDC, (rect.right - textSize.cx) / 2, 50, operatorName, static_cast<int>(strlen(operatorName)));
                SelectObject(memDC, oldFont);

                // Display instruction
                oldFont = (HFONT)SelectObject(memDC, FontMedium);
                const char* instruction = "Select a primary weapon:";
                GetTextExtentPoint32(memDC, instruction, static_cast<int>(strlen(instruction)), &textSize);
                TextOut(memDC, (rect.right - textSize.cx) / 2, 100, instruction, static_cast<int>(strlen(instruction)));
                SelectObject(memDC, oldFont);

                // Parse weapon list
                const char* weapons[3] = {NULL, NULL, NULL};
                int weaponCount = 0;

                const char* ptr = weaponStr;
                while (*ptr && weaponCount < 3)
                {
                    // Skip leading spaces
                    while (*ptr == ' ') ++ptr;

                    // Save start of this weapon name
                    const char* start = ptr;

                    // Find end of weapon name (comma or end of string)
                    while (*ptr && *ptr != ',') ++ptr;

                    // Calculate length of this weapon name
                    int len = (int)(ptr - start);

                    // If we found a non-empty name, store it
                    if (len > 0)
                    {
                        // Allocate memory for the weapon name
                        char* weaponName = new char[len + 1];
                        strncpy_s(weaponName, len + 1, start, len);

                        // Trim trailing spaces
                        while (len > 0 && weaponName[len - 1] == ' ')
                            weaponName[--len] = '\0';

                        // Store in our array if it's not empty
                        if (len > 0)
                            weapons[weaponCount++] = weaponName;
                        else
                            delete[] weaponName;
                    }

                    // Skip comma if present
                    if (*ptr == ',') ++ptr;
                }

                int y = 150;

                // Create weapon bitmap display
                HDC hdcMem = CreateCompatibleDC(memDC);

                int imgWidth = 500;
                int imgHeight = 160;

                int spacing = 40;
                int totalWidth = weaponCount * (imgWidth + spacing) - spacing;
                int startX = (rect.right - totalWidth) / 2;

                for (int i = 0; i < weaponCount; ++i)
                {
                    int x = startX + i * (imgWidth + spacing);

                    // Load and display weapon image first
                    HBITMAP weaponBmp = GetWeaponBitmap(weapons[i]);
                    if (weaponBmp)
                    {
                        HGDIOBJ oldBmp = SelectObject(hdcMem, weaponBmp);
                        BITMAP bm;
                        GetObject(weaponBmp, sizeof(bm), &bm);

                        // Draw a border around the image area
                        RECT imgRect = {x, y, x + imgWidth, y + imgHeight};
                        FrameRect(memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

                        // Display the image
                        SetStretchBltMode(memDC, HALFTONE);  // Use HALFTONE for better quality
                        SetBrushOrgEx(memDC, 0, 0, NULL);

                        // Display the image
                        StretchBlt(memDC, x, y, imgWidth, imgHeight,
                                  hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                        SelectObject(hdcMem, oldBmp);
                    }
                    else
                    {
                        // If no image, draw placeholder
                        RECT imgRect = {x, y, x + imgWidth, y + imgHeight};
                        FrameRect(memDC, &imgRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                        FillRect(memDC, &imgRect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

                        // Display "No Image" text
                        RECT textRect = imgRect;
                        DrawText(memDC, "No Image", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    }

                    // Display weapon name below the image
                    RECT nameRect = {x, y + imgHeight + 10, x + imgWidth, y + imgHeight + 40};
                    oldFont = (HFONT)SelectObject(memDC, FontMedium);
                    DrawText(memDC, weapons[i], -1, &nameRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    SelectObject(memDC, oldFont);
                }

                // Free weapon name memory
                for (int i = 0; i < weaponCount; i++)
                {
                    if (weapons[i]) delete[] (char*)weapons[i];
                }

                DeleteDC(hdcMem);

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

                // Parse weapon list again for click detection
                const char* weapons[3] = {NULL, NULL, NULL};
                int weaponCount = 0;

                const char* ptr = weaponStr;
                while (*ptr && weaponCount < 3)
                {
                    // Skip leading spaces
                    while (*ptr == ' ') ++ptr;

                    // Save start of this weapon name
                    const char* start = ptr;

                    // Find end of weapon name (comma or end of string)
                    while (*ptr && *ptr != ',') ++ptr;

                    // Calculate length of this weapon name
                    int len = (int)(ptr - start);

                    // If we found a non-empty name, store it
                    if (len > 0)
                    {
                        // Allocate memory for the weapon name
                        char* weaponName = new char[len + 1];
                        strncpy_s(weaponName, len + 1, start, len);

                        // Trim trailing spaces
                        while (len > 0 && weaponName[len - 1] == ' ')
                            weaponName[--len] = '\0';

                        // Store in our array if it's not empty
                        if (len > 0)
                            weapons[weaponCount++] = weaponName;
                        else
                            delete[] weaponName;
                    }

                    // Skip comma if present
                    if (*ptr == ',') ++ptr;
                }

                int y = 150;
                int imgWidth = 500;
                int imgHeight = 160;
                int spacing = 40;
                int totalWidth = weaponCount * (imgWidth + spacing) - spacing;
                int startX = (rect.right - totalWidth) / 2;

                for (int i = 0; i < weaponCount; ++i)
                {
                    int x = startX + i * (imgWidth + spacing);
                    RECT clickRect = {x, y, x + imgWidth, y + imgHeight + 40}; // Include name area in clickable region

                    if (mouseX >= clickRect.left && mouseX <= clickRect.right &&
                        mouseY >= clickRect.top && mouseY <= clickRect.bottom)
                    {
                        // Weapon selected - do something with selection if needed
                        CurrentUIState = UIState::OperatorSelection;
                        for (const auto& button : Buttons)
                        {
                            ShowWindow(button.GetHWND(), SW_SHOW);
                        }
                        InvalidateRect(hwnd, NULL, TRUE);

                        // Free weapon name memory before returning
                        for (int j = 0; j < weaponCount; j++)
                        {
                            if (weapons[j]) delete[] (char*)weapons[j];
                        }
                        return 0;
                    }
                }

                // Free weapon name memory
                for (int i = 0; i < weaponCount; i++)
                {
                    if (weapons[i]) delete[] (char*)weapons[i];
                }

                // Back button detection
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

            // Clean up weapon bitmaps and allocated names
            for (auto& entry : WeaponBitmaps) {
                if (entry.bitmap) DeleteObject(entry.bitmap);
                if (entry.name) delete[] entry.name;
            }
            WeaponBitmaps.clear();

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
