#pragma once


extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

struct RecoilPreset
{
    int Vertical;
    int Horizontal;
};

extern bool EnableRC;
extern bool UseToggleKey;
extern bool Running;

extern int SelectedMode;
extern int ToggleKey;

extern RecoilPreset RecoilPresets[4];
extern RecoilPreset CurrentRecoil;

extern const char* Modes[4];
extern const char* ModeDescriptions[4];
