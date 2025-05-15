#include "Globals.hpp"

const int WINDOW_WIDTH = 650;
const int WINDOW_HEIGHT = 400;

bool EnableRC = true;
bool UseToggleKey = false;
bool Running = true;
int SelectedMode = 1;
int ToggleKey = 20;

RecoilPreset RecoilPresets[4] =
{
    {2, 0},  // LOW
    {3, 0},  // MEDIUM
    {4, 0},  // HIGH
    {5, 0}   // ULTRA
};

RecoilPreset CurrentRecoil = RecoilPresets[SelectedMode];

const char* Modes[4] = {"LOW", "MEDIUM", "HIGH", "ULTRA"};

const char* ModeDescriptions[4] =
{
    "LOW: Best for Low Recoil SMGs and Pistols",
    "MEDIUM: Best for Medium Recoil Assault Rifles (e.g 556XI)",
    "HIGH: Best for High Recoil Rifles (e.g R4C)",
    "ULTRA: Best for Twitch's F2"
};

bool DarkTheme = true;
