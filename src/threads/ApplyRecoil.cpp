#include "Threads.h"

#include <thread>

#include "../Globals.h"
#include "../inputs/Inputs.h"
#include "../recoil/Recoil.h"

inline constexpr int FIRE_DELAY_MS = 5;
inline constexpr int IDLE_DELAY_MS = 5;

static std::pair<int, int> CalculateRecoil(int baseX, int baseY, float lookX, float lookY)
{
    int adjustedX = baseX + static_cast<int>(lookX * ControllerMultiplier);
    int adjustedY = baseY + static_cast<int>(-lookY * ControllerMultiplier);
    return { adjustedX, adjustedY };
}

namespace Threads
{

    void ApplyRecoil()
    {
        while (Running)
        {
            const bool controllerEnabled = EnableController;
            const bool controllerConnected = controllerEnabled && Inputs::IsControllerConnected();
            const bool isADS = Inputs::IsMouseADS()
                || (controllerConnected && Inputs::IsControllerADS(Inputs::GetControllerState()));

            if (!EnableRC || !isADS)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_DELAY_MS));
                continue;
            }

            const bool firingMouse = Inputs::IsMouseFiring();
            const bool firingController = controllerConnected && Inputs::IsControllerFiring(Inputs::GetControllerState());

            if (!firingMouse && !firingController)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_DELAY_MS));
                continue;
            }

            int moveX = static_cast<int>(CurrentRecoil.Horizontal * 2.0f);
            int moveY = static_cast<int>(CurrentRecoil.Vertical * 2.0f);

            if (controllerConnected)
            {
                XINPUT_STATE state = Inputs::GetControllerState();
                float lookX, lookY;
                Inputs::GetControllerStickInput(state, lookX, lookY);

                auto [rx, ry] = CalculateRecoil(moveX, moveY, lookX, lookY);
                moveX = rx;
                moveY = ry;
            }

            Inputs::MoveMouseRaw(moveX, moveY);

            std::this_thread::sleep_for(std::chrono::milliseconds(FIRE_DELAY_MS));
        }
    }

} // namespace Threads
