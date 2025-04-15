#include <thread>

#include "../include/RecoilPresets.hpp"
#include "../include/UI.hpp"
#include "../include/Utils.hpp"

void LoadConfig()
{
  HANDLE file = CreateFileA("Config.toml", GENERIC_READ, FILE_SHARE_READ, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file == INVALID_HANDLE_VALUE)
  {
    EnableRC = true;
    DarkTheme = true;
    return;
  }

  DWORD fileSize = GetFileSize(file, NULL);
  if (fileSize == INVALID_FILE_SIZE || fileSize == 0)
  {
    CloseHandle(file);
    return;
  }

  char* buffer = new char[fileSize + 1];
  DWORD bytesRead;
  if (!ReadFile(file, buffer, fileSize, &bytesRead, NULL))
  {
    delete[] buffer;
    CloseHandle(file);
    return;
  }

  buffer[bytesRead] = '\0';  // Null-terminate
  CloseHandle(file);

  std::string section;
  char* line = strtok(buffer, "\r\n");
  while (line)
  {
    while (*line == ' ' || *line == '\t') ++line;  // Trim left
    if (*line == '\0' || *line == '#')
    {
      line = strtok(NULL, "\r\n");
      continue;
    }

    if (line[0] == '[')
    {
      char* end = strchr(line, ']');
      if (end) section = std::string(line + 1, end);
      line = strtok(NULL, "\r\n");
      continue;
    }

    char* equal = strchr(line, '=');
    if (!equal)
    {
      line = strtok(NULL, "\r\n");
      continue;
    }

    std::string key(line, equal);
    std::string value(equal + 1);

    // Trim both
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    if (section == "RecoilPresets")
    {
      if (key == "Mode")
        SelectedMode = atoi(value.c_str());
      else if (key == "Enabled")
        EnableRC = (value == "true" || value == "1");
      else if (key == "Vertical")
        CurrentRecoil.Vertical = atoi(value.c_str());
      else if (key == "ToggleKey")
        ToggleKey = atoi(value.c_str());
    } else if (section == "UI")
    {
      if (key == "DarkTheme") DarkTheme = (value == "true" || value == "1");
    }
    line = strtok(NULL, "\r\n");
  }

  delete[] buffer;

  SelectedMode = clamp(SelectedMode, 0, 3);
  CurrentRecoil = RecoilPresets[SelectedMode];
}

void SaveConfig()
{
  HANDLE file = CreateFileA("Config.toml", GENERIC_WRITE, 0, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file == INVALID_HANDLE_VALUE) return;

  char buffer[512];
  int len = 0;

  len += wsprintfA(buffer + len, "[RecoilPresets]\r\n");
  len += wsprintfA(buffer + len,
                   "# 0 = Low, 1 = Medium, 2 = High, 3 = Custom\r\n");
  len += wsprintfA(buffer + len, "Mode = %d\r\n", SelectedMode);
  len +=
      wsprintfA(buffer + len, "Enabled = %s\r\n", EnableRC ? "true" : "false");
  len += wsprintfA(buffer + len, "Vertical = %d\r\n", CurrentRecoil.Vertical);
  len += wsprintfA(buffer + len, "ToggleKey = %d\r\n\r\n", ToggleKey);

  len += wsprintfA(buffer + len, "[UI]\r\n");
  len += wsprintfA(buffer + len, "DarkTheme = %s\r\n",
                   DarkTheme ? "true" : "false");

  DWORD written;
  WriteFile(file, buffer, len, &written, NULL);
  CloseHandle(file);
}

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
      } else if (LOWORD(wParam) == 3)  // Toggle Theme Button
      {
        ToggleTheme();
        SaveConfig();
        InvalidateRect(hwnd, NULL, TRUE);
      } else if (LOWORD(wParam) == 4)  // Toggle Caps Lock Feature Button
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
                           "Toggle Theme", 3);
      Buttons.emplace_back(hwnd, 30 + 3 * (130 + 20), 320, 130, 40,
                           "Caps Lock Toggle", 4);
    } break;

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      RECT rect;
      GetClientRect(hwnd, &rect);

      // Set colors based on theme
      COLORREF bgColor = DarkTheme ? RGB(0, 0, 0) : RGB(255, 255, 255);
      COLORREF textColor = DarkTheme ? RGB(255, 255, 255) : RGB(0, 0, 0);

      // Set background
      HBRUSH hBrush = CreateSolidBrush(bgColor);
      FillRect(hdc, &rect, hBrush);
      DeleteObject(hBrush);

      // Set text color
      SetTextColor(hdc, textColor);
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

      EndPaint(hwnd, &ps);
    } break;

    case WM_KEYDOWN:
    {
      if (wParam == VK_ESCAPE)
      {
        PostMessage(hwnd, WM_CLOSE, 0, 0);  // Close the window
      }
    } break;

    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

void ApplyRecoil()
{
  while (Running)
  {
    if (EnableRC &&
        (GetAsyncKeyState(VK_RBUTTON) & 0x8000))  // Right Mouse Button (ADS)
    {
      while (GetAsyncKeyState(VK_LBUTTON) &
             0x8000)  // Left Mouse Button (Firing)
      {
        mouse_event(MOUSEEVENTF_MOVE, CurrentRecoil.Horizontal * 2,
                    CurrentRecoil.Vertical * 2, 0, 0);
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
