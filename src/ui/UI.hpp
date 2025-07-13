#pragma once

enum class UIState
{
    OperatorSelection,
    WeaponDisplay
};

UIState CurrentUIState = UIState::OperatorSelection;
int SelectedOperatorIndex = -1;
