#pragma once

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
	int GetWeaponData(const char* weaponName);
	
} // namespace Files
