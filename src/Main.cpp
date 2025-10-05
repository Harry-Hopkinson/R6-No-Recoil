#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include "resource/resource.h"

#include <thread>

#include "detection/ClickDetection.h"

#include "files/Files.h"

#include "recoil/Recoil.h"
#include "scenes/Scenes.h"
#include "threads/Threads.h"

#include "ui/Bitmap.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Font.h"

#define maximum(a, b) ((a) > (b) ? (a) : (b))

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
                case 2: // Toggle Key
                    UseToggleKey = !UseToggleKey;
                    Files::SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 3: // Attacker Selection
                    IsAttackerView = true;
                    Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
                    for (const auto& button : Buttons::GetButtons())
                        ShowWindow(button.GetHWND(), SW_HIDE);
                    Buttons::CreateOperatorSelectionButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 4: // Defender Selection
                    IsAttackerView = false;
                    Scenes::ChangeCurrentScene(SceneType::OperatorSelection);
                    for (const auto& button : Buttons::GetButtons())
                        ShowWindow(button.GetHWND(), SW_HIDE);
                    Buttons::CreateOperatorSelectionButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 5: // Back to Menu
                    Scenes::ChangeCurrentScene(SceneType::LandingPage);
                    for (const auto& button : Buttons::GetButtons())
                        ShowWindow(button.GetHWND(), SW_HIDE);
                    Buttons::CreateLandingPageButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 6: // Support the Project button
                    system("start https://buymeacoffee.com/harryhopkinson");
                    break;
                case 7: // GitHub button
                    system("start https://github.com/Harry-Hopkinson/R6-No-Recoil");
                    break;
                case 8: // Info Screen button
                    Scenes::ChangeCurrentScene(SceneType::InfoScreen);
                    for (const auto& button : Buttons::GetButtons())
                        ShowWindow(button.GetHWND(), SW_HIDE);
                    Buttons::CreateInfoScreenButtons(hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 9: // "+" button
                    CurrentRecoil.Vertical++;
                    Files::SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case 10: // "-" button
                    CurrentRecoil.Vertical = maximum(CurrentRecoil.Vertical - 1, 0);
                    Files::SaveConfig();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
        }
        break;

        case WM_CREATE:
        {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            Buttons::CreateLandingPageButtons(hwnd);

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

            switch (Scenes::GetCurrentScene())
            {
                case SceneType::LandingPage:
                    Scenes::DrawLandingPage(memDC, rect.right, rect.bottom);
                    break;

                case SceneType::OperatorSelection:
                    Scenes::DrawOperatorSelection(memDC, rect.right, rect.bottom);
                    break;

                case SceneType::WeaponDisplay:
                    Scenes::DrawWeaponDisplay(memDC, rect.right, rect.bottom);
                    break;
					
				case SceneType::AttachmentDisplay:
					Scenes::DrawAttachmentDisplay(memDC, rect.right, rect.bottom);
					break;

                case SceneType::InfoScreen:
                    Scenes::DrawInfoScreen(memDC, rect.right);
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

            if (Scenes::GetCurrentScene() == SceneType::OperatorSelection) ClickDetection::OperatorSelection(hwnd, mouseX, mouseY);
            else if (Scenes::GetCurrentScene() == SceneType::WeaponDisplay) ClickDetection::WeaponDisplay(hwnd, rect.right, rect.bottom,
                                                                                                          mouseX, mouseY);
        }
        break;

        case WM_DESTROY:
        {
            Bitmap::CleanupBitmaps(Bitmap::AttackerBitmaps);
            Bitmap::CleanupBitmaps(Bitmap::DefenderBitmaps);
            Bitmap::CleanupWeaponBitmaps();

            Font::Cleanup();

            Buttons::GetButtons().clear();
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
