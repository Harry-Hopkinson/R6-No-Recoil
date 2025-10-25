#include "Threads.h"

#include <thread>

#include "../Globals.h"
#include "../inputs/Inputs.h"
#include "../recoil/Recoil.h"

std::pair<int, int> CalculateRecoil(int baseX, int baseY, float lookX, float lookY)
{
    int adjustedX = baseX + static_cast<int>(lookX * ControllerMultiplier);
    int adjustedY = baseY + static_cast<int>(-lookY * ControllerMultiplier);
    return { adjustedX, adjustedY };
}

constexpr int FIRE_DELAY_MS = 5;
constexpr int IDLE_DELAY_MS = 1;

namespace Threads
{

    void ApplyRecoil()
    {
        while (Running)
        {
            bool controllerConnected = EnableController && Inputs::IsControllerConnected();
            XINPUT_STATE state = controllerConnected ? Inputs::GetControllerState() : XINPUT_STATE{};

            bool isADS = Inputs::IsMouseADS() || (EnableController && controllerConnected && Inputs::IsControllerADS(state));

            if (EnableRC && isADS)
            {
                while (Inputs::IsMouseFiring()
                       || (EnableController && controllerConnected && Inputs::IsControllerFiring(state)))
                {

                    int moveX = static_cast<int>(CurrentRecoil.Horizontal * 2.0f);
                    int moveY = static_cast<int>(CurrentRecoil.Vertical * 2.0f);


                    if (EnableController && controllerConnected)
                    {
                        state = Inputs::GetControllerState();
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

            std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_DELAY_MS));
        }
    }

} // namespace Threads
