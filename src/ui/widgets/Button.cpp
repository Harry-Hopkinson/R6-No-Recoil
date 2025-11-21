#include "Button.h"

#include "../../Globals.h"

namespace
{
    std::vector<Button> ButtonsVector;
}

namespace Buttons
{

    std::vector<Button>& GetButtons()
    {
        return ButtonsVector;
    }

    void ClearButtons()
    {
        ButtonsVector.clear();
    }

    void CreateOperatorSelectionButtons()
    {
        ClearButtons();

        int startX = WINDOW_WIDTH - 200 - (350 / 2);
        int startY = (WINDOW_HEIGHT - 90) / 2;

        ButtonsVector.push_back({ 1, startX + 90, startY, 170, 40, "Toggle Recoil" });
        ButtonsVector.push_back({ 2, startX, startY + 50, 170, 40, "Toggle Key" });

        ButtonsVector.push_back(
            { IsAttackerView ? 4 : 3, startX + 180, startY + 50, 170, 40,
              IsAttackerView ? "Switch to Defenders" : "Switch to Attackers" });

        ButtonsVector.push_back({ 5, startX, startY + 350, 160, 40, "Support the Project" });
        ButtonsVector.push_back({ 6, startX + 180, startY + 350, 160, 40, "Join the Discord" });

        // Vertical Recoil
        ButtonsVector.push_back({ 7, startX + 240, startY - 150, 80, 40, "+" });
        ButtonsVector.push_back({ 8, startX + 150, startY - 150, 80, 40, "-" });

        // Horizontal Recoil
        ButtonsVector.push_back({ 9, startX + 240, startY - 100, 80, 40, "+" });
        ButtonsVector.push_back({ 10, startX + 150, startY - 100, 80, 40, "-" });

        ButtonsVector.push_back({ 11, startX + 100, startY + 100, 140, 40, "Save Config" });
    }

} // namespace Buttons
