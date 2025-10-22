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

enum class GripType
{
    NONE,
    ANGLED,
    HORIZONTAL,
    VERTICAL
};

extern ScopeType SelectedScopeType;
extern GripType SelectedGripType;
