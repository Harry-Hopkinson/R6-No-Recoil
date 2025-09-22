#pragma once

namespace Threads
{

    /**
     * @brief Thread for applying recoil control
     */
    void ApplyRecoil();

    /**
     * @brief Thread that detects if toggle key is pressed then enables/disables recoil control
     */
    void ToggleRecoil();

} // namespace Threads
