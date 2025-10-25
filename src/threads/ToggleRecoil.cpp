#include "Threads.h"

#include <windows.h>
#include <thread>

#include "../Globals.h"
#include "../files/Files.h"

constexpr int TOGGLE_DELAY_MS = 300;
constexpr int POLL_INTERVAL_MS = 10;

namespace Threads
{

    void ToggleRecoil()
    {

        while (Running)
        {
            if (UseToggleKey && (GetAsyncKeyState(ToggleKey) & 0x8000))
            {
                EnableRC = !EnableRC;
                Files::SaveConfig();

                InvalidateRect(FindWindow(NULL, "R6 No Recoil"), NULL, TRUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(TOGGLE_DELAY_MS));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
        }
    }
} // namespace Threads
