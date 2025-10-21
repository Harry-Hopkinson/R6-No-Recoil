# 🎯 R6 No Recoil

![Preview](website/public/image.png)

<div align="center">
    <a href="https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest">
        <img src="https://raw.githubusercontent.com/Harry-Hopkinson/R6-No-Recoil/master/website/public/app.png" alt="App Preview" width="750px">
    </a>
    <hr>
    <a href="http://discord.gg/68rUtWqEBy">
        <img src="https://img.shields.io/badge/Join%20Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white" alt="Join Discord">
    </a>

| Release Build | Development Build |
|---------------|-------------------|
| [![Release](https://img.shields.io/badge/Release%20Build-Download-blue?style=for-the-badge&logo=github)](https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest/download/R6NoRecoil.zip) | [![Development](https://img.shields.io/badge/Development%20Build-Download-yellow?style=for-the-badge&logo=github)](https://nightly.link/Harry-Hopkinson/R6-No-Recoil/workflows/CI/main/R6NoRecoil.zip) |
</div>

A lightweight, efficient **no-recoil tool** for Rainbow Six Siege. Designed for smooth performance and ease of use.

## 🧠 Why I Made This

Most recoil tools like Logitech G Hub (now detectable) are bloated, heavy, and come with a ton of unnecessary overhead just to perform a simple task. I built this tool to be **clean**, **compact**, and **laser-focused** on one thing: **removing recoil with minimal system impact**. No drivers, no background bloat—just raw utility.

## 🚀 Why Use This Tool?

- **📌 No Recoil** – Reduces weapon recoil for better accuracy.
- **⚡ Lightweight & Fast** – Optimised for minimal CPU usage.
- **🔧 No Installation Required** – Just run the `.exe`.
- **⚙️ Powerful Config Support** – Modify the Config.toml file.
- **🖥️ Works on Any Setup** – Supports Windows 10/11.
- **🛠️ Open Source & Customisable** – Modify as needed.

## 🎮 How to Use

1. **Download** `R6NoRecoil` from the [Github Release Page](https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest).
2. **Run the Program** (double-click `R6NoRecoil.exe`).
3. **Choose whether attacking or defending**.
4. **Choose your operator and their primary weapon**.
5. **Controls:**
   - **Press `Toggle Recoil`** → Toggle the no-recoil effect on/off.
   - **Press `Change Mode`** → Adjust the strength of the recoil compensation.
   - **Press `Toggle Key`** -> Enable/Disable toggle key activating/deactivating recoil. Default CapsLock.
   - **Switch to Defenders/Attackers** -> Change operator type.
6. **Enjoy Smoother Aim!**

## ⚠️ Important Setup Note

To ensure the recoil compensation behaves consistently across all systems, you must:

1. **Disable Windows mouse acceleration** (Enhance Pointer Precision).
2. **Enable raw mouse input in Rainbow Six Siege**.

### 🔧 How to disable **Enhance Pointer Precision**

#### Method 1: Using Control Panel
1. **Open the Control Panel.**
   - Press **Windows + R**, type `control`, and press **Enter**.

2. **Go to Mouse → Pointer Options.**
   - In Control Panel, set **View by** to **Large icons** or **Small icons**.
   - Click **Mouse**.
   - Select the **Pointer Options** tab.

3. **Uncheck the box for "Enhance pointer precision".**

4. **Click Apply and OK.**

#### Method 2: Using Windows Settings
1. **Open Settings.**
   - Press **Windows + I** to open the Settings app.

2. **Go to Devices → Mouse.**
   - In the left sidebar, select **Mouse**.
   - Click **Additional mouse options** on the right.

3. **Disable Enhance pointer precision.**
   - In the **Pointer Options** tab, uncheck **Enhance pointer precision**.

4. **Click Apply and OK.**

### 🎯 How to Enable Raw Mouse Input in Rainbow Six Siege

1. **Close Rainbow Six Siege** completely.
2. **Locate your GameSettings.ini file**:
    ```plaintext
    Documents\My Games\Rainbow Six - Siege\<RandomNumbersFolder>\GameSettings.ini
    ```
3. **Open the file in Notepad (or any text editor)**.
4. **Find the line**:
    ```plaintext
    RawInputMouseKeyboard=0
    ```
5. **Change it to**:
    ```plaintext
    RawInputMouseKeyboard=1
    ```
6. **Save the file and close the editor**.
7. **Restart the game** — raw mouse input will now be active.

## ⚙️ Config Explained

The app automatically creates a `Config.toml` file on first run. You can manually edit it to customise the experience.

### Example:

```toml
[RecoilPresets]
VerticalRecoil = 3
HorizontalRecoil = 0
Enabled = true

# Use https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
# And then convert the key code to decimal
# Toggle Key (e.g. 20 = CAPS LOCK)
ToggleKey = 20
```

## 🔫 WeaponData.json Explained

The app uses a `WeaponData.json` file to store **recoil compensation values** for each weapon. This allows you to fine-tune how much recoil reduction is applied for every primary gun in Rainbow Six Siege.

### Example Structure

```json
{
    "name": "R4-C",
    "recoil": {
        "non_magnified": { "vertical": 3, "horizontal": 0 },
        "magnified": { "vertical": 6, "horizontal": 0 }
    }
},
```

### Fields Explained

- **name:** The exact name of the weapon as recognised by the app.
- **non_magnified and magnified** Specifies whether recoil is for Non-Magnified and Magnified scopes respectively.
- **vertical:** The amount of vertical recoil compensation applied.
  - **Higher values = stronger recoil reduction**
  - **Lower values = lighter recoil reduction**
  - **Typical range:** 1 (low recoil SMGs) to 6 (high recoil assault rifles), but you can experiment.
- **horizontal:** The amount of horizontal recoil compensation applied.
  - **Typical range:** Between 1 and 2.

## 💸 Support Development

Any form of donation is greatly appreciated and will help keep me motivated to keep working on this project and developing it further with new features and improvements. If you become a member you get:

- Early access to new releases
- Requests for custom builds of the program
- Custom discord roles
- Help support this project

[![Buy Me a Coffee](https://img.shields.io/badge/buymeacoffee-donate-yellow)](https://www.buymeacoffee.com/HarryHopkinson)

## ⚠️ Disclaimer

This tool is for **educational purposes only**. Use it at your own risk. I do not advocate cheating.
Windows Defender may falsely flag this program as a virus. Just allow the app to run anyway. If you are unsure, use **VirusTotal** to check the program.

---

**Got questions?** Feel free to open an issue!
