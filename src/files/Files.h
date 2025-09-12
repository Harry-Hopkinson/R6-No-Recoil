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
     */
	void LoadRecoilValues();
	
} // namespace Files
