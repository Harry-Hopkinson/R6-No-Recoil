#include "Globals.hpp"

const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 1050;

std::vector<const char*> AttackerNames =
{
    "Striker",     "Sledge",   "Thatcher",   "Ash",      "Thermite",
    "Twitch",      "Montagne", "Glaz",       "Fuze",     "Blitz",
    "IQ",          "Buck",     "Blackbeard", "Capitao",  "Hibana",
    "Jackal",      "Ying",     "Zofia",      "Dokkaebi", "Lion",
    "Finka",       "Maverick", "Nomad",      "Gridlock", "Nokk",
    "Amaru",       "Kali",     "Iana",       "Ace",      "Zero",
    "Flores",      "Osa",      "Sens",       "Grim",     "Brava",
    "Ram",         "Deimos",   "Rauora"
};
std::vector<const char*> DefenderNames =
{
    "Sentry",      "Smoke",    "Mute",       "Castle",   "Pulse",
    "Doc",         "Rook",     "Kapkan",     "Tachanka", "Jager",
    "Bandit",      "Frost",    "Valkyrie",   "Caveira",  "Echo",
    "Mira",        "Lesion",   "Ela",        "Vigil",    "Maestro",
    "Alibi",       "Clash",    "Kaid",       "Mozzie",   "Warden",
    "Goyo",        "Wamai",    "Oryx",       "Melusi",   "Aruni",
    "Thunderbird", "Thorn",    "Azami",      "Solis",    "Fenrir",
    "Tubarao",     "Skopos"
};

std::vector<const char*> AttackerPrimaryWeapons =
{
    "M4, M249",                      // Striker
    "L85A2, M590A1",                 // Sledge
    "AR33, L85A2, M590A1",           // Thatcher
    "G36C, R4-C",                    // Ash
    "556XI, M1014",                  // Thermite
    "F2, 417, SG-CQB",               // Twitch
    "Ballistic Shield",              // Montagne
    "OTs-03",                        // Glaz
    "AK-12, 6P41, Ballistic Shield", // Fuze
    "G52-Tactical Shield",           // Blitz
    "AUG A2, 552 Commando, G8A1",    // IQ
    "C8-SFW, CAMRS",                 // Buck
    "MK17 CQB, SR-25",               // Blackbeard
    "PARA-308, M249",                // Capitao
    "Type-89, SUPERNOVA",            // Hibana
    "C7E, PDW9, ITA12L",             // Jackal
    "T-95 LSW, SIX12",               // Ying
    "LMG-E, M762",                   // Zofia
    "Mk 14 EBR, BOSG.12.2",          // Dokkaebi
    "V308, 417, SG-CQB",             // Lion
    "Spear .308, 6P41, SASG-12",     // Finka
    "M4, AR-15.50",                  // Maverick
    "AK-74M, ARX200",                // Nomad
    "F90, M249 SAW",                 // Gridlock
    "FMG-9, SIX12 SD",               // Nokk
    "G8A1, SUPERNOVA",               // Amaru
    "CSRX 300",                      // Kali
    "ARX200, G36C",                  // Iana
    "AK-12, M1014",                  // Ace
    "SC3000K, MP7",                  // Zero
    "AR33",                          // Flores
    "556XI",                         // Osa
    "POF-9, 417",                    // Sens
    "552 Commando",                  // Grim
    "PARA-308",                      // Brava
    "R4-C, LMG-E",                   // Ram
    "AK-74M, M590A1",                // Deimos
    "AR-W"                           // Rauora
};

std::vector<const char*> DefenderPrimaryWeapons =
{
    "MPX-SD",                        // Sentry
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
    "9mm C1, Super 90",              // Frost
    "MPX, SPAS-12",                  // Valkyrie
    "M12, SPAS-15",                  // Caveira
    "MP5SD, SUPERNOVA",              // Echo
    "Vector .45 ACP, ITA12L",        // Mira
    "T-5 SMG, SIX12 SD",             // Lesion
    "Scorpion EVO 3 A1, FO-12",      // Ela
    "K1A, BOSG.12.2",                // Vigil
    "ALDA 5.56, ACS12",              // Maestro
    "Mx4 Storm, ACS12",              // Alibi
    "CCE Shield",                    // Clash
    "AUG A3, TCSG12",                // Kaid
    "Commando 9, P10 RONI",          // Mozzie
    "MPX, M590A1",                   // Warden
    "Vector .45 ACP, TCSG12",        // Goyo
    "AUG A2, MP5K",                  // Wamai
    "SPAS-12, T-5 SMG",              // Oryx
    "MP5, Super 90",                 // Melusi
    "P10 RONI, Mk 14 EBR",           // Aruni
    "Spear .308, SPAS-15",           // Thunderbird
    "UZK50GI",                       // Thorn
    "9x19VSN, ACS12",                // Azami
    "P90",                           // Solis
    "MP7, SASG-12",                  // Fenrir
    "MPX, M590A1",                   // Tubarao
    "CSRX PDW"                       // Skopos
};

bool EnableRC = true;
bool UseToggleKey = false;
bool Running = true;
bool IsAttackerView = true;

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
