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
