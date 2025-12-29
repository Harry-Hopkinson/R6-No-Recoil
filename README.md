<div align="center">
    <h1>🎯 R6 No Recoil</h1>
    <a href="https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest">
        <img src="https://raw.githubusercontent.com/Harry-Hopkinson/R6-No-Recoil/master/website/public/app.png" alt="App Preview" width="750px">
    </a>
    <hr>
    <a href="http://discord.gg/KhdXPqQxK8">
        <img src="https://img.shields.io/badge/Join%20Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white" alt="Join Discord">
    </a>
    <a href="https://ko-fi.com/harryhopkinson">
        <img src="https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white" alt="Support the Project">
    </a>

| Release Build                                                                                                                                                                                     | Development Build                                                                                                                                                                                      |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [![Release](https://img.shields.io/badge/Release%20Build-Download-blue?style=for-the-badge&logo=github)](https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest/download/R6NoRecoil.zip) | [![Development](https://img.shields.io/badge/Development%20Build-Download-yellow?style=for-the-badge&logo=github)](https://nightly.link/Harry-Hopkinson/R6-No-Recoil/workflows/CI/main/R6NoRecoil.zip) |

A lightweight, efficient **no-recoil tool** for Rainbow Six Siege. Designed for smooth performance and ease of use, whilst having deep customisability without all the bloat.

</div>

## 🧠 Why I Made This

Most recoil control tools are packed with features you'll never use, slowing down your system. Tools like Logitech G Hub (now detectable) were too large and bloated just to handle simple recoil control. I built this tool to be **clean**, **compact**, and **laser-focused** on one thing: **removing recoil with minimal system impact**. No drivers, no background bloat—just raw utility.

## 🚀 Why Use This Tool?

- **⚡ Lightning Fast** – Optimised for minimal CPU usage and instant response. No lag, no delays — meaning no missed frames in game.
- **🎨 Powerful Configs** – Extensive configuration options for every operator and weapon. Fine-tune your recoil control to perfection on top of custom keybinds and theme customisation.
- **🔓 Open Source** – Fully transparent and customisable. Inspect the code, suggest improvements, or modify it to your needs.
- **💻 No Installation** – Just download, extract the zip folder and run the .exe file. No bloated installers, no unnecessary dependencies — it just works.
- **🖥️ Works Seamlessly** – Supports Windows 10 and 11. Fully open source and customisable to fit your playstyle.

## 🎮 How to Use

1. **Download** `R6NoRecoil.exe` from the [Website](https://r6norecoil.com/download) and extract the zip folder.
2. **Run the Program** (double-click `R6NoRecoil.exe`). No installation needed.
3. **Choose whether you're Attacking or Defending** (defaults to Attacking Operators).
4. **Select your operator and primary weapon** by clicking on the icons.
5. **Use the Toggle Recoil Button** to enable/disable the recoil control.
6. **Use CapsLock** (default) to toggle on/off during gameplay. Enable by clicking on the Toggle Key button.
7. **Modify toggle keys, themes, controller support and more** in the Config.toml file.
8. **Enjoy improved accuracy and reduced recoil** in Rainbow Six Siege!

## 💸 Support Development

Any form of donation is greatly appreciated and will help keep me motivated to keep working on this project and developing it further with new features and improvements. If you become a member you get:

- Early access to new releases.
- Requests for custom builds of the program.
- Custom discord roles.
- Help support me develop bigger and better features for this project.

[![Buy Me a Coffee](https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/harryhopkinson)

## ⚙️ Config Explained

The app automatically creates a `Config.toml` file on first run. You can manually edit it to customise the experience.

### Example:

```ini
[General]
Enabled = true
VerticalRecoil = 3.00
HorizontalRecoil = 0.00
ToggleADS = false

[Controller]
Enabled = false
Multiplier = 10

[Keys]
# Use https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
# And then convert the key code to decimal
# Toggle Key (e.g. 20 = CAPS LOCK)
ToggleRecoilKey = 20
ToggleRecoilEnabled = false
PrimaryKey = 49
SecondaryKey = 50
TertiaryKey = 51

[Theme]
BackgroundColour = RGB(30, 30, 30)
TextColour = RGB(230, 230, 230)
ButtonColour = RGB(70, 70, 70)
LineColour = RGB(180, 180, 180)
```

### Fields Explained

#### [General]

- **Enabled:** Enable or disable recoil adjustment.
- **VerticalRecoil:** The vertical recoil compensation value.
- **HorizontalRecoil:** The horizontal recoil compensation value (Positive goes right, negative goes left).
- **ToggleADS:** Enable if you use toggle for ADS (aim down sights).

#### [Controller]

- **Enabled:** Enable or disable recoil adjustment with a controller.
- **Multiplier:** The multiplier for controller adjustment when firing.

#### [Keys]

- **ToggleRecoilKey:** The key code for toggling recoil adjustment.
- **ToggleRecoilEnabled:** Enable or disable recoil adjustment with a toggle key.
- **PrimaryKey:** The key code for selecting the first weapon of a selected operator.
- **SecondaryKey:** The key code for selecting the second weapon of a selected operator.
- **TertiaryKey:** The key code for selecting the third weapon of a selected operator.

#### [Theme]

- **BackgroundColour:** RGB values for the background colour of the app.
- **TextColour:** RGB values for the text colour of the app.
- **ButtonColour:** RGB values for the button colour of the app.
- **LineColour:** RGB values for the line colour of the app.

## 🔫 WeaponData.json Explained

The app uses a `WeaponData.json` file to store **recoil compensation values** for each weapon. This allows you to fine-tune how much recoil reduction is applied for every primary gun in Rainbow Six Siege.

### Example Structure

```json
{
    "name": "R4-C",
    "preset-1": { "vertical": 7.5, "horizontal": 0 },
    "preset-2": { "vertical": 3.0, "horizontal": 0 },
    "preset-3": { "vertical": 0.0, "horizontal": 0 }
},
```

### Fields Explained

- **name:** The exact name of the weapon as recognised by the app.
- **preset-1/2/3** Specifies which preset to use for the weapon.
- **vertical:** The amount of vertical recoil compensation applied.
    - **Higher values (Vertical) = stronger recoil reduction**
    - **Lower values (Vertical) = lighter recoil reduction**
- **horizontal:** The amount of horizontal recoil compensation applied.
    - **Positive Values (Horizontal) = movement to the right**
    - **Negative Values (Horizontal) = movement to the left**

## ⚠️ Disclaimer

**This tool is for educational purposes only.** Use it at your own risk. I do not advocate cheating.

Windows Defender may flag it as a false positive due to the nature of input simulation. You can verify the safety by checking the file on VirusTotal. Always download from the official GitHub repository or the website to ensure you have the real version.

---

**Got questions?** Feel free to open an issue or join our [Discord](http://discord.gg/KhdXPqQxK8)!
