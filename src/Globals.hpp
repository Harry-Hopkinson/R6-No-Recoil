#pragma once

#include <vector>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern std::vector<const char*> AttackerNames;
extern std::vector<const char*> DefenderNames;

extern std::vector<const char*> AttackerPrimaryWeapons;
extern std::vector<const char*> DefenderPrimaryWeapons;

struct RecoilPreset
{
    int Vertical;
    int Horizontal;
};

extern bool EnableRC;
extern bool UseToggleKey;
extern bool Running;
extern bool IsAttackerView;

extern int SelectedMode;
extern int ToggleKey;

extern RecoilPreset RecoilPresets[4];
extern RecoilPreset CurrentRecoil;

extern const char* Modes[4];
