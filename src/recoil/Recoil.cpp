#include "Recoil.h"

WeaponRecoil CurrentRecoil = { 3.0f, 0.0f };

std::vector<const char*> AttackerNames = { "Striker", "Sledge",   "Thatcher", "Ash",      "Thermite",   "Twitch",
                                           "Glaz",    "Fuze",     "IQ",       "Buck",     "Blackbeard", "Capitao",
                                           "Hibana",  "Jackal",   "Ying",     "Zofia",    "Dokkaebi",   "Lion",
                                           "Finka",   "Maverick", "Nomad",    "Gridlock", "Nokk",       "Amaru",
                                           "Kali",    "Iana",     "Ace",      "Zero",     "Flores",     "Osa",
                                           "Sens",    "Grim",     "Brava",    "Ram",      "Deimos",     "Rauora" };

std::vector<const char*> DefenderNames = { "Sentry", "Smoke",       "Mute",   "Castle", "Pulse", "Doc",      "Rook",
                                           "Kapkan", "Tachanka",    "Jager",  "Bandit", "Frost", "Valkyrie", "Caveira",
                                           "Echo",   "Mira",        "Lesion", "Ela",    "Vigil", "Maestro",  "Alibi",
                                           "Kaid",   "Mozzie",      "Warden", "Goyo",   "Wamai", "Oryx",     "Melusi",
                                           "Aruni",  "Thunderbird", "Thorn",  "Azami",  "Solis", "Fenrir",   "Tubarao",
                                           "Skopos", "Denari" };

std::vector<const char*> AttackerWeapons =
{
    "M4, M249, SR-25",            // Striker
    "L85A2",                      // Sledge
    "L85A2, AR33",                // Thatcher
    "R4-C, G36C",                 // Ash
    "556XI",                      // Thermite
    "F2, 417",                    // Twitch
    "OTs-03, Bearing-9",          // Glaz
    "AK-12, 6P41",                // Fuze
    "G8A1, 552-Commando, AUG-A2", // IQ
    "C8-SFW, CAMRS",              // Buck
    "MK17-CQB, SR-25",            // Blackbeard
    "PARA-308, M249",             // Capitao
    "Type-89, Bearing-9",         // Hibana
    "C7E, PDW9",                  // Jackal
    "T-95-LSW",                   // Ying
    "M762, LMG-E",                // Zofia
    "Mk-14-EBR, SMG-12, C75",     // Dokkaebi
    "V308, 417",                  // Lion
    "Spear-308, 6P41",            // Finka
    "M4, AR-15.50",               // Maverick
    "AK-74M, ARX200",             // Nomad
    "F90, M249-SAW",              // Gridlock
    "FMG-9",                      // Nokk
    "G8A1, SMG-11",               // Amaru
    "C75, SPSMG9",                // Kali
    "G36C, ARX200",               // Iana
    "AK-12",                      // Ace
    "SC3000K, MP7",               // Zero
    "AR33, SR-25",                // Flores
    "556XI, PDW9",                // Osa
    "POF-9, 417",                 // Sens
    "552-Commando",               // Grim
    "PARA-308, CAMRS",            // Brava
    "R4-C, LMG-E",                // Ram
    "AK-74M",                     // Deimos
    "417, M249",                  // Rauora
};

std::vector<const char*> DefenderWeapons =
{
    "Commando-9, TCSG12",       // Sentry
    "FMG-9, SMG-11",            // Smoke
    "MP5K, SMG-11",             // Mute
    "UMP45",                    // Castle
    "UMP45",                    // Pulse
    "P90, MP5",                 // Doc
    "MP5, P90",                 // Rook
    "9x19VSN",                  // Kapkan
    "DP27, 9x19VSN, Bearing-9", // Tachanka
    "416-carbine",              // Jager
    "MP7",                      // Bandit
    "9mm-C1",                   // Frost
    "MPX",                      // Valkyrie
    "M12",                      // Caveira
    "MP5SD, Bearing-9",         // Echo
    "Vector",                   // Mira
    "T-5",                      // Lesion
    "Scorpion-EVO",             // Ela
    "K1A, SMG-12, C75",         // Vigil
    "ALDA, ACS12",              // Maestro
    "Mx4-Storm, ACS12",         // Alibi
    "AUG-A3, TCSG12",           // Kaid
    "Commando-9, P10-RONI",     // Mozzie
    "MPX, SMG-12",              // Warden
    "Vector, TCSG12",           // Goyo
    "AUG-A2, MP5K",             // Wamai
    "T-5",                      // Oryx
    "MP5",                      // Melusi
    "P10-RONI, Mk-14-EBR",      // Aruni
    "Spear-308, Bearing-9",     // Thunderbird
    "UZK50GI, C75",             // Thorn
    "9x19VSN, ACS12",           // Azami
    "P90, SMG-11",              // Solis
    "MP7",                      // Fenrir
    "MPX, AR-15.50",            // Tubarao
    "PCX-33",                   // Skopos
    "Scorpion-EVO, FMG-9"       // Denari
};

int ControllerMultiplier = 10;

int PresetIndex = 0;
