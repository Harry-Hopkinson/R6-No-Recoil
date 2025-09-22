#pragma once

enum class Scenes
{
    LandingPage,
    OperatorSelection,
    WeaponDisplay,
    InfoScreen
};

Scenes CurrentScene;

int SelectedOperatorIndex = -1;
