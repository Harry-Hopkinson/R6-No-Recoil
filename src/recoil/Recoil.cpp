#include "Recoil.h"

#include "../files/Files.h"

int CurrentRecoil = 3;

std::vector<const char*> AttackerNames = { "Striker", "Sledge",   "Thatcher", "Ash",      "Thermite",   "Twitch",
                                           "Glaz",    "Fuze",     "IQ",       "Buck",     "Blackbeard", "Capitao",
                                           "Hibana",  "Jackal",   "Ying",     "Zofia",    "Dokkaebi",   "Lion",
                                           "Finka",   "Maverick", "Nomad",    "Gridlock", "Nokk",       "Amaru",
                                           "Kali",    "Iana",     "Ace",      "Zero",     "Flores",     "Osa",
                                           "Sens",    "Grim",     "Brava",    "Ram",      "Deimos",     "Rauora"
					                     };

std::vector<const char*> DefenderNames = { "Sentry",   "Smoke",   "Mute",     "Castle", "Pulse",   "Doc",
                                           "Rook",     "Kapkan",  "Tachanka", "Jager",  "Bandit",  "Frost",
                                           "Valkyrie", "Caveira", "Echo",     "Mira",   "Lesion",  "Ela",
                                           "Vigil",    "Maestro", "Alibi",    "Kaid",   "Mozzie",  "Warden",
                                           "Goyo",     "Wamai",   "Oryx",     "Melusi", "Aruni",   "Thunderbird",
                                           "Thorn",    "Azami",   "Solis",    "Fenrir", "Tubarao", "Skopos",
                                           "Denari"
					                     };

std::vector<const char*> AttackerPrimaryWeapons =
{
    "M4, M249, SR-25",                 // Striker
    "L85A2, M590A1",                   // Sledge
    "L85A2, AR33, M590A1",             // Thatcher
    "R4-C, G36C",                      // Ash
    "556XI, M1014",                    // Thermite
    "F2, 417, SG-CQB",                 // Twitch
    "OTs-03",                          // Glaz
    "AK-12, 6P41",                     // Fuze
    "G8A1, 552-Commando, AUG-A2-Acog", // IQ
    "C8-SFW, CAMRS",                   // Buck
    "MK17-CQB, SR-25",                 // Blackbeard
    "PARA-308, M249",                  // Capitao
    "Type-89, SUPERNOVA",              // Hibana
    "C7E, PDW9, ITA12L",               // Jackal
    "T-95-LSW, SIX12",                 // Ying
    "M762, LMG-E",                     // Zofia
    "Mk-14-EBR, BOSG.12.2",            // Dokkaebi
    "V308, 417, SG-CQB",               // Lion
    "Spear-308-Acog, 6P41, SASG-12",   // Finka
    "M4, AR-15.50",                    // Maverick
    "AK-74M, ARX200",                  // Nomad
    "F90, M249-SAW",                   // Gridlock
    "FMG-9, SIX12-SD",                 // Nokk
    "G8A1, SUPERNOVA",                 // Amaru
    "CSRX-300",                        // Kali
    "G36C, ARX200",                    // Iana
    "AK-12, M1014",                    // Ace
    "SC3000K, MP7",                    // Zero
    "AR33, SR-25",                     // Flores
    "556XI, PDW9",                     // Osa
    "POF-9, 417",                      // Sens
    "552-Commando, SG-CQB",            // Grim
    "PARA-308, CAMRS",                 // Brava
    "R4-C, LMG-E",                     // Ram
    "AK-74M, M590A1",                  // Deimos
    "417, M249",                       // Rauora
};

std::vector<const char*> DefenderPrimaryWeapons =
{
    "Commando-9, M870, TCSG12", // Sentry
    "FMG-9, M590A1",            // Smoke
    "M590A1, MP5K",             // Mute
    "UMP45, M1014",             // Castle
    "UMP45, M1014",             // Pulse
    "P90, MP5, SG-CQB",         // Doc
    "MP5, P90, SG-CQB",         // Rook
    "9x19VSN, SASG-12",         // Kapkan
    "DP27, 9x19VSN",            // Tachanka
    "416-carbine, M870",        // Jager
    "MP7, M870",                // Bandit
    "9mm-C1, Super-90",         // Frost
    "MPX, SPAS-12",             // Valkyrie
    "M12, SPAS-15",             // Caveira
    "MP5SD, SUPERNOVA",         // Echo
    "Vector, ITA12L",           // Mira
    "T-5, SIX12-SD",            // Lesion
    "Scorpion-EVO, FO-12",      // Ela
    "K1A, BOSG.12.2",           // Vigil
    "ALDA, ACS12",              // Maestro
    "Mx4-Storm, ACS12",         // Alibi
    "AUG-A3, TCSG12",           // Kaid
    "Commando-9, P10-RONI",     // Mozzie
    "MPX, M590A1",              // Warden
    "Vector, TCSG12",           // Goyo
    "AUG-A2, MP5K",             // Wamai
    "T-5, SPAS-12",             // Oryx
    "MP5, Super-90",            // Melusi
    "P10-RONI, Mk-14-EBR",      // Aruni
    "Spear-308, SPAS-15",       // Thunderbird
    "UZK50GI, M870",            // Thorn
    "9x19VSN, ACS12",           // Azami
    "P90, ITA12L",              // Solis
    "MP7, SASG-12",             // Fenrir
    "MPX, AR-15.50",            // Tubarao
    "PCX-33",                   // Skopos
    "Scorpion-EVO, FMG-9"       // Denari
};

void SetRecoilModeFromWeapon(const char* weaponName)
{
    if (!weaponName)
        return;

    CurrentRecoil = Files::GetWeaponData(weaponName);;
}
