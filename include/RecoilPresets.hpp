#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool EnableRC;
const char* Modes[] = {"LOW", "MEDIUM", "HIGH", "ULTRA"};
int SelectedMode = 1;
const char* ModeDescriptions[] =
{
    "LOW: Best for Low Recoil SMGs and Pistols",
    "MEDIUM: Best for Medium Recoil Assault Rifles (e.g 556XI)",
    "HIGH: Best for High Recoil Rifles (e.g R4C)",
    "ULTRA: Best for Twitch's F2"
};

struct RecoilPreset
{
    int Vertical;
    int Horizontal;
};

// Default values will be overwritten by Config.toml if found
RecoilPreset RecoilPresets[] =
{
    {2, 0},  // LOW
    {3, 0}, // MEDIUM
    {4, 0}, // HIGH
    {5, 0}  // ULTRA
};

RecoilPreset CurrentRecoil = RecoilPresets[SelectedMode];
bool Running = true;

int ToggleKey = VK_CAPITAL;
bool UseToggleKey = false;
