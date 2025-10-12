#pragma once

struct WeaponRecoil
{
    int Vertical;
    int Horizontal;
};

enum class ScopeType
{
    NONE,
    MAGNIFIED,
    NON_MAGNIFIED
};

extern ScopeType SelectedScopeType;
