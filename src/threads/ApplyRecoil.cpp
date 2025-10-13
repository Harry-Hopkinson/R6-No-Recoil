#include "Threads.h"

#include <thread>

#include "../Globals.h"
#include "../inputs/Inputs.h"
#include "../recoil/Recoil.h"

std::pair<int, int> CalculateRecoil(int baseX, int baseY, float lookX, float lookY)
{
    int adjustedX = baseX + static_cast<int>(lookX * 12.0f);
    int adjustedY = baseY + static_cast<int>(-lookY * 12.0f);
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
            bool controllerConnected = Inputs::IsControllerConnected();
            XINPUT_STATE state = controllerConnected ? Inputs::GetControllerState() : XINPUT_STATE{};

            bool isADS = Inputs::IsMouseADS() || (controllerConnected && Inputs::IsControllerADS(state));

            if (EnableRC && isADS)
            {
                while (Inputs::IsMouseFiring() || (controllerConnected && Inputs::IsControllerFiring(state)))
                {
                    int moveX = CurrentRecoil.Horizontal;
                    int moveY = CurrentRecoil.Vertical * 2;

                    if (controllerConnected)
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
