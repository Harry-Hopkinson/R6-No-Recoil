#include "Recoil.hpp"

#include "../Globals.hpp"

RecoilPreset RecoilPresets[5] =
{
    {2, 0},  // LOW
    {3, 0},  // MEDIUM
    {4, 0},  // HIGH
    {5, 0},  // ULTRA
    {6, 0},  // EXTREME
};

RecoilPreset CurrentRecoil = RecoilPresets[SelectedMode];

const char* Modes[5] = {"LOW", "MEDIUM", "HIGH", "ULTRA", "EXTREME"};

std::vector<const char*> AttackerNames =
{
    "Striker",     "Sledge",      "Thatcher",    "Ash",         "Thermite",
    "Twitch",      "Glaz",        "Fuze",        "IQ",          "Buck",
    "Blackbeard",  "Capitao",     "Hibana",      "Jackal",      "Ying",
    "Zofia",       "Dokkaebi",    "Lion",        "Finka",       "Maverick",
    "Nomad",       "Gridlock",    "Nokk",        "Amaru",       "Kali",
    "Iana",        "Ace",         "Zero",        "Flores",      "Osa",
    "Sens",        "Grim",        "Brava",       "Ram",         "Deimos",
    "Rauora"
};

std::vector<const char*> DefenderNames =
{
    "Sentry",      "Smoke",       "Mute",        "Castle",      "Pulse",
    "Doc",         "Rook",        "Kapkan",      "Tachanka",    "Jager",
    "Bandit",      "Frost",       "Valkyrie",    "Caveira",     "Echo",
    "Mira",        "Lesion",      "Ela",         "Vigil",       "Maestro",
    "Alibi",       "Kaid",        "Mozzie",      "Warden",      "Goyo",
    "Wamai",       "Oryx",        "Melusi",      "Aruni",       "Thunderbird",
    "Thorn",       "Azami",       "Solis",       "Fenrir",      "Tubarao",
    "Skopos"
};

std::vector<const char*> AttackerPrimaryWeapons =
{
    "M4, M249",                      // Striker
    "L85A2, M590A1",                 // Sledge
    "L85A2, AR33, M590A1",           // Thatcher
    "R4-C, G36C",                    // Ash
    "556XI, M1014",                  // Thermite
    "F2, 417, SG-CQB",               // Twitch
    "OTs-03",                        // Glaz
    "AK-12, 6P41",                   // Fuze
    "G8A1, 552-Commando, AUG-A2",    // IQ
    "C8-SFW, CAMRS",                 // Buck
    "MK17-CQB, SR-25",               // Blackbeard
    "PARA-308, M249",                // Capitao
    "Type-89, SUPERNOVA",            // Hibana
    "C7E, PDW9, ITA12L",             // Jackal
    "T-95-LSW, SIX12",               // Ying
    "M762, LMG-E",                   // Zofia
    "Mk-14-EBR, BOSG.12.2",          // Dokkaebi
    "V308, 417, SG-CQB",             // Lion
    "Spear-308, 6P41, SASG-12",      // Finka
    "M4, AR-15.50",                  // Maverick
    "AK-74M, ARX200",                // Nomad
    "F90, M249-SAW",                 // Gridlock
    "FMG-9, SIX12-SD",               // Nokk
    "G8A1, SUPERNOVA",               // Amaru
    "CSRX-300",                      // Kali
    "G36C, ARX200",                  // Iana
    "AK-12, M1014",                  // Ace
    "SC3000K, MP7",                  // Zero
    "AR33",                          // Flores
    "556XI",                         // Osa
    "POF-9, 417",                    // Sens
    "552-Commando",                  // Grim
    "PARA-308, CAMRS",               // Brava
    "R4-C, LMG-E",                   // Ram
    "AK-74M, M590A1",                // Deimos
    "417, M249",                     // Rauora
};

std::vector<const char*> DefenderPrimaryWeapons =
{
    "Commando-9, M870",              // Sentry
    "FMG-9, M590A1",                 // Smoke
    "MP5K, M590A1",                  // Mute
    "UMP45, M1014",                  // Castle
    "UMP45, M1014",                  // Pulse
    "MP5, P90, SG-CQB",              // Doc
    "MP5, P90, SG-CQB",              // Rook
    "9x19VSN, SASG-12",              // Kapkan
    "DP27, 9x19VSN",                 // Tachanka
    "M870, 416-C",                   // Jager
    "MP7, M870",                     // Bandit
    "9mm-C1, Super-90",              // Frost
    "MPX, SPAS-12",                  // Valkyrie
    "M12, SPAS-15",                  // Caveira
    "MP5SD, SUPERNOVA",              // Echo
    "Vector, ITA12L",                // Mira
    "T-5, SIX12-SD",                 // Lesion
    "Scorpion-EVO, FO-12",           // Ela
    "K1A, BOSG.12.2",                // Vigil
    "ALDA, ACS12",                   // Maestro
    "Mx4-Storm, ACS12",              // Alibi
    "AUG-A3, TCSG12",                // Kaid
    "Commando-9, P10-RONI",          // Mozzie
    "MPX, M590A1",                   // Warden
    "Vector, TCSG12",                // Goyo
    "AUG-A2, MP5K",                  // Wamai
    "SPAS-12, T-5",                  // Oryx
    "MP5, Super-90",                 // Melusi
    "P10-RONI, Mk-14-EBR",           // Aruni
    "Spear-308, SPAS-15",            // Thunderbird
    "UZK50GI",                       // Thorn
    "9x19VSN, ACS12",                // Azami
    "P90",                           // Solis
    "MP7, SASG-12",                  // Fenrir
    "MPX, M590A1",                   // Tubarao
    "CSRX PDW"                       // Skopos
};

int StringCompare(const char* str1, const char* str2)
{
    if (!str1 || !str2) return -1;

    while (*str1 && *str2)
    {
        if (*str1 != *str2) return *str1 - *str2;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

WeaponData WeaponRecoilData[] =
{
    // SMGs (Low recoil)
    {"P90", 2},
    {"MP5", 2},
    {"MP5K", 2},
    {"MP5SD", 2},
    {"UMP45", 2},
    {"MP7", 2},
    {"9x19VSN", 2},
    {"FMG-9", 5},
    {"Mx4-Storm", 2},
    {"MPX", 2},
    {"MPX-SD", 2},
    {"T-5", 2},
    {"Scorpion-EVO", 2},
    {"K1A", 2},
    {"Vector", 2},
    {"P10-RONI", 2},
    {"Commando-9", 2},
    {"9mm-C1", 2},
    {"M12", 2},
    {"PDW9", 4},
    {"POF-9", 4},
    {"SC3000K", 5},
    {"UZK50GI", 2},
    {"CSRX PDW", 2},

    // Assault Rifles (High recoil)
    {"AK-12", 5},
    {"F2", 6},
    {"AR33", 4},
    {"556XI", 3},
    {"G36C", 5},
    {"R4-C", 6},
    {"C8-SFW", 5},
    {"MK17-CQB", 2},
    {"PARA-308", 4},
    {"Type-89", 5},
    {"C7E", 5},
    {"M762", 5},
    {"AK-74M", 3},
    {"ARX200", 4},
    {"F90", 4},
    {"AUG-A2", 5},
    {"AUG-A3", 4},
    {"552-Commando", 4},
    {"L85A2", 3},
    {"V308", 4},
    {"Spear-308", 5},
    {"416-C", 4},
    {"M4", 5},
    {"AR-W", 4},
    {"DP27", 4},

    // LMGs (Low recoil)
    {"6P41", 5},
    {"G8A1", 4},
    {"ALDA", 2},
    {"T-95-LSW", 4},
    {"LMG-E", 5},
    {"M249", 3},
    {"M249-SAW", 4},

    // Marksman Rifles (Medium recoil)
    {"OTs-03", 4},
    {"417", 2},
    {"SR-25", 3},
    {"Mk-14-EBR", 4},
    {"CAMRS", 2},
    {"AR-15.50", 3},
    {"CSRX-300", 3},

    // Shotguns (Low recoil)
    {"M590A1", 2},
    {"M1014", 2},
    {"SG-CQB", 2},
    {"SASG-12", 6},
    {"M870", 2},
    {"Super-90", 2},
    {"SPAS-12", 2},
    {"SPAS-15", 2},
    {"SIX12", 2},
    {"SIX12-SD", 2},
    {"TCSG12", 2},
    {"FO-12", 2},
    {"ACS12", 2},
    {"BOSG.12.2", 2},
    {"ITA12L", 2},
    {"SUPERNOVA", 2},
};

const int WEAPON_COUNT = sizeof(WeaponRecoilData) / sizeof(WeaponData);

int GetWeaponRecoil(const char* weaponName)
{
    if (!weaponName) return 3; // Default medium recoil

    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        if (StringCompare(WeaponRecoilData[i].name, weaponName) == 0)
        {
            return WeaponRecoilData[i].recoil;
        }
    }

    // Default medium recoil if weapon not found
    return 3;
}

void SetRecoilModeFromWeapon(const char* weaponName)
{
    if (!weaponName) return;

    int weaponRecoil = GetWeaponRecoil(weaponName);

    switch (weaponRecoil)
    {
        case 2:
            SelectedMode = 0; // LOW
            break;
        case 3:
            SelectedMode = 1; // MEDIUM
            break;
        case 4:
            SelectedMode = 2; // HIGH
            break;
        case 5:
            SelectedMode = 3; // ULTRA
            break;
        case 6:
            SelectedMode = 4; // EXTREME
            break;
        default:
            SelectedMode = 1; // Default to MEDIUM
            break;
    }

    CurrentRecoil = RecoilPresets[SelectedMode];
}
