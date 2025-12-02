#pragma once

typedef struct HWND__* HWND;

namespace ButtonHandler
{

    void HandleToggleRecoil(HWND hwnd);
    void HandleToggleKey(HWND hwnd);
    void HandleSwitchToAttackers(HWND hwnd);
    void HandleSwitchToDefenders(HWND hwnd);
    void HandleSupportProject(HWND hwnd);
    void HandleJoinDiscord(HWND hwnd);
    void HandleVerticalRecoilPlus(HWND hwnd);
    void HandleVerticalRecoilMinus(HWND hwnd);
    void HandleHorizontalRecoilPlus(HWND hwnd);
    void HandleHorizontalRecoilMinus(HWND hwnd);
    void HandleSavePreset(HWND hwnd);
    void HandleUpdateTool(HWND hwnd);

} // namespace ButtonHandlers
