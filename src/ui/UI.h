#pragma once

enum class UIState
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    InfoScreen
};

extern UIState CurrentUIState;
extern int SelectedOperatorIndex;
