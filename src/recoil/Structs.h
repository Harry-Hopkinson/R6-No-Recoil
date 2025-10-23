#pragma once

struct WeaponRecoil
{
    float Vertical;
    float Horizontal;
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
