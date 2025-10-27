#pragma once

#include "Structs.h"

#include <vector>

struct WeaponData
{
    const char* name;
    int recoil;
};

extern WeaponData WeaponRecoilData[];

extern std::vector<const char*> AttackerNames;
extern std::vector<const char*> DefenderNames;

extern std::vector<const char*> AttackerWeapons;
extern std::vector<const char*> DefenderWeapons;

extern WeaponRecoil CurrentRecoil;

void SetRecoilModeFromWeapon(const char* weaponName);

extern int SelectedWeaponIndex;

extern int ControllerMultiplier;

extern int PresetIndex;
