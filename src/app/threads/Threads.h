#pragma once

namespace Threads
{

    /**
     * @brief Thread for applying recoil control for mouse or controller
     */
    void ApplyRecoil();

    /**
     * @brief Thread that detects if toggle key is pressed and toggles recoil
     */
    void ToggleRecoil();

} // namespace Threads
