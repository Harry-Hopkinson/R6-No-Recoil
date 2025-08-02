#pragma once

enum class UIState
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay
};

extern UIState CurrentUIState;
extern int SelectedOperatorIndex;
