#include <thread>

#include "core/Config.hpp"
#include "core/EventHandler.hpp"

#include "recoil/ApplyRecoil.hpp"
#include "recoil/ToggleRecoil.hpp"

#include "Globals.hpp"
#include "ui/UI.hpp"

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
