#pragma once

#include <vector>

struct RecoilPreset
{
    int Vertical;
    int Horizontal;
};

struct WeaponData
{
    const char* name;
    int recoil;
};

extern std::vector<const char*> AttackerNames;
extern std::vector<const char*> DefenderNames;

extern std::vector<const char*> AttackerPrimaryWeapons;
extern std::vector<const char*> DefenderPrimaryWeapons;

extern RecoilPreset RecoilPresets[6];
extern RecoilPreset CurrentRecoil;

extern const char* Modes[6];

extern WeaponData WeaponRecoilData[];

void SetRecoilModeFromWeapon(const char* weaponName);
