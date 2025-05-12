#include <thread>

#include "core/Config.hpp"
#include "core/utils/random.hpp"
#include "core/EventHandler.hpp"

void ApplyRecoil()
{
    bool horizontalFlip = false;

    while (Running)
    {
        if (EnableRC && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))  // ADS
        {
            while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)  // Firing
            {
                int baseX = CurrentRecoil.Horizontal;
                int baseY = CurrentRecoil.Vertical * 2;

                int finalX = baseX;
                int finalY = baseY + GetRandomInt(-1, 1);

                if (baseX != 0)
                {
                    finalX += GetRandomInt(-1, 1);
                    finalX *= (horizontalFlip ? -1 : 1);
                    horizontalFlip = !horizontalFlip;
                }

                mouse_event(MOUSEEVENTF_MOVE, finalX, finalY, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ToggleRecoilListener()
{
    while (Running)
    {
        if (UseToggleKey && (GetAsyncKeyState(ToggleKey) & 0x8000))
        {
            EnableRC = !EnableRC;
            InvalidateRect(FindWindow(NULL, "R6 No Recoil"), NULL, TRUE);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Register Window Class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "NoRecoilWindow";
    RegisterClass(&wc);

    LoadConfig();

    // Create Window
    HWND hwnd =
        CreateWindowEx(0, "NoRecoilWindow", "R6 No Recoil",
                       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                       CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
                       nullptr, nullptr, hInstance, nullptr);

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
