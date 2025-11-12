#pragma once

#include <Xinput.h>

namespace Inputs
{

    /*
     * Returns true if the mouse is in ADS mode (Right Mouse Button).
     */
    bool IsMouseADS();

    /*
     * Returns true if the mouse is firing (Left Mouse Button).
     */
    bool IsMouseFiring();

    /*
     * Moves the mouse cursor by the specified amount.
     */
    void MoveMouseRaw(int dx, int dy);

    /*
     * Returns true if a controller is connected.
     */
    bool IsControllerConnected();

    /*
     * Returns the current state of the controller.
     */
    XINPUT_STATE GetControllerState();

    /*
     * Returns the controller's stick input.
     */
    void GetControllerStickInput(const XINPUT_STATE& state, float& outX, float& outY);

    /*
     * Returns true if the controller is in ADS mode (Left Trigger).
     */
    bool IsControllerADS(const XINPUT_STATE& state);

    /*
     * Returns true if the controller is firing (Right Trigger).
     */
    bool IsControllerFiring(const XINPUT_STATE& state);

} // namespace Inputs
