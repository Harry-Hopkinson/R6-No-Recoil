#pragma once

enum class UIState
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    InfoScreen
};

UIState CurrentUIState;
int SelectedOperatorIndex = -1;
