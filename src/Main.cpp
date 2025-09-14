#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include "resource/resource.h"

#include "core/String.h"
#include "core/max.h"

#include "ui/widgets/Font.h"

#include "files/Files.h"

#include "recoil/Recoil.h"
#include "recoil/Threads.h"

#include "ui/Bitmap.h"
#include "ui/UI.h"
#include "ui/views/Views.h"
#include "ui/widgets/Button.h"

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
                    Files::SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 2: // Mode Selection
                    SelectedMode = (SelectedMode + 1) % 6;
                    CurrentRecoil = RecoilPresets[SelectedMode];
                    Files::SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 3: // Toggle Key
                    UseToggleKey = !UseToggleKey;
                    Files::SaveConfig();
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

            Bitmap::AttackerBitmaps = Bitmap::LoadOperatorBitmaps(AttackerNames);
            Bitmap::DefenderBitmaps = Bitmap::LoadOperatorBitmaps(DefenderNames);

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

            switch (CurrentUIState)
            {
                case UIState::LandingPage:
                    Views::DrawLandingPage(memDC, rect);
                    break;
                case UIState::OperatorSelection:
                    Views::DrawOperatorSelection(memDC, rect);
                    break;
                case UIState::WeaponDisplay:
                    Views::DrawWeaponDisplay(memDC, rect);
                    break;
                case UIState::InfoScreen:
                    Views::DrawInfoScreen(memDC, rect);
                    break;
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
                int weaponCount = String::ParseWeaponList(weaponStr, weapons, 3);

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
                        Files::SaveConfig();

                        CurrentUIState = UIState::OperatorSelection;
                        CreateOperatorSelectionButtons(hwnd);
                        InvalidateRect(hwnd, NULL, TRUE);

                        String::FreeWeaponList(weapons, weaponCount);
                        return 0;
                    }
                }

                String::FreeWeaponList(weapons, weaponCount);

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

    Files::LoadConfig();

    HWND hwnd = CreateWindowEx(
        0, wc.lpszClassName, "R6 No Recoil", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
        CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    Threads::StartThreads();

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

        Threads::Update();
    }

    Threads::StopThreads();
    return 0;
}
