#pragma once

#include "Structs.h"

#include <vector>

extern std::vector<const char*> AttackerNames;
extern std::vector<const char*> DefenderNames;

extern std::vector<const char*> AttackerWeapons;
extern std::vector<const char*> DefenderWeapons;

extern WeaponRecoil CurrentRecoil;

extern int ControllerMultiplier;

extern int PresetIndex;

std::pair<float, float> CalculateRecoil(
    float baseX, float baseY, float lookX, float lookY);
