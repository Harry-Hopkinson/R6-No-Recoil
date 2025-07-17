#pragma once

enum class UIState
{
    OperatorSelection,
    WeaponDisplay
};

extern UIState CurrentUIState;
extern int SelectedOperatorIndex;
