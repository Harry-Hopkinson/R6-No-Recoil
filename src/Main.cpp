#include <windows.h>
#include <windowsx.h>

#include "resource/resource.h"

#include <thread>

#include "Globals.h"

#include "detection/ClickDetection.h"
#include "drawing/Drawing.h"

#include "files/Files.h"

#include "recoil/Recoil.h"
#include "scenes/Scenes.h"
#include "threads/Threads.h"

#include "ui/Bitmap.h"
#include "ui/Themes.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Font.h"

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

        case WM_CREATE:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            Buttons::CreateOperatorSelectionButtons();

            Bitmap::InitialiseOperatorBitmaps(AttackerNames, DefenderNames);

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

            HBRUSH bgBrush = CreateSolidBrush(BackgroundColour);
            FillRect(memDC, &rect, bgBrush);
            DeleteObject(bgBrush);

            switch (Scenes::GetCurrentScene())
            {
                case SceneType::OperatorSelection:
                    Drawing::DrawOperatorSelection(memDC, rect.right, rect.bottom);
                    break;

                case SceneType::WeaponDisplay:
                    Drawing::DrawWeaponDisplay(memDC, rect.right, rect.bottom);
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

            switch (Scenes::GetCurrentScene())
            {
                case SceneType::OperatorSelection:
                    ClickDetection::OperatorSelection(hwnd, mouseX, mouseY);
                    break;

                case SceneType::WeaponDisplay:
                    ClickDetection::WeaponDisplay(hwnd, rect.right, rect.bottom, mouseX, mouseY);
                    break;

                default:
                    break;
            }
        }
        break;

        case WM_DESTROY:
        {
            Buttons::ClearButtons();

            Bitmap::CleanupOperatorBitmaps();
            Bitmap::CleanupWeaponBitmaps();

            Font::Cleanup();

            PostQuitMessage(0);
            return 0;
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
    wc.lpszClassName = "R6NoRecoil";
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClass(&wc);

    Files::LoadConfig();

    HWND hwnd = CreateWindowEx(
        0, wc.lpszClassName, "R6 No Recoil", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    std::thread recoilThread(Threads::ApplyRecoil);
    std::thread toggleThread(Threads::ToggleRecoil);

    MSG msg = {};
    while (Running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    if (recoilThread.joinable()) recoilThread.join();
    if (toggleThread.joinable()) toggleThread.join();

    return 0;
}
