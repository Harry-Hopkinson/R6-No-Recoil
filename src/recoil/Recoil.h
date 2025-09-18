#pragma once

#include "Struct.h"

#include <vector>

struct WeaponData
{
    const char* name;
    int recoil;
};

extern std::vector<const char*> AttackerNames;
extern std::vector<const char*> DefenderNames;

extern std::vector<const char*> AttackerPrimaryWeapons;
extern std::vector<const char*> DefenderPrimaryWeapons;

extern WeaponRecoil CurrentRecoil;

extern WeaponData WeaponRecoilData[];

void SetRecoilModeFromWeapon(const char* weaponName);
