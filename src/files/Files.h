#pragma once

#include "../recoil/Struct.h"

namespace Files
{

    /**
     * @brief Writes the editable variables to the Config.toml file
     */
    void SaveConfig();

    /**
     * @brief Loads the editable variables from the Config.toml file
     */
    void LoadConfig();

    /**
     * @brief Loads the weapon recoil data from the data.json file
     * @param char* The name of the weapon that recoil value will be fetched
     * @return The weapons's recoil value
     */
    WeaponRecoil GetWeaponData(const char* weaponName);

    /**
     * @brief Constructs the path for the operator bitmap based off the name
     * @param char* The name of the operator that is having the bitmap fetched
     * @return The path to the operator's bitmap
     */
    const char* GetImagePath(const char* name);

} // namespace Files
