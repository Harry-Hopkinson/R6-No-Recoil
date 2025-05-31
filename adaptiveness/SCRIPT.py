import ctypes
import pyperclip
from pynput import mouse
from pynput.keyboard import Listener, KeyCode
import time
import threading

pyperclip.copy("0,0,0")

# Variables (updated via clipboard)
variable1 = 0.035  # recoil_speed
variable2 = 4      # recoil_strength_x
variable3 = 4      # recoil_strength_y

holding = False       # Whether the left mouse button is held
paused = False        # Whether the recoil movement is paused
pause_lock = threading.Lock()

# Windows API for moving the mouse
def move_mouse(dx, dy):
    ctypes.windll.user32.mouse_event(0x0001, int(dx), int(dy), 0, 0)

def recoil_control():
    global paused
    while True:
        with pause_lock:
            if holding and not paused:
                move_mouse(variable2, variable3)

        time.sleep(variable1)

def on_click(x, y, button, pressed):
    global holding
    if button == mouse.Button.left:
        holding = pressed

def on_release(key):
    global paused
    if key == KeyCode.from_char('p'):
        with pause_lock:
            paused = not paused
            print("Paused" if paused else "Resumed")

def wait_for_clipboard_change():
    global variable1, variable2, variable3
    previous_clipboard = ""
    while True:
        clipboard_content = pyperclip.paste()
        if clipboard_content != previous_clipboard and clipboard_content:
            try:
                variable1, variable2, variable3 = map(float, clipboard_content.split(','))
                print(f"Updated Variables -> Speed: {variable1}, X: {variable2}, Y: {variable3}")
                paused = True
            except ValueError:
                print("Invalid clipboard format.")
            previous_clipboard = clipboard_content
        time.sleep(0.1)

# -------- WAIT FOR "start1" BEFORE STARTING MAIN LOGIC --------
print("Waiting for 'start1' in clipboard...")
while True:
    if pyperclip.paste().strip() == "start1":
        print("'start1' detected. Proceeding with startup.")
        pyperclip.copy("start2")
        time.sleep(2)
        paused = False 
        pyperclip.copy("0,0,0")
        break
    time.sleep(0.1)

# Start threads
threading.Thread(target=recoil_control, daemon=True).start()
threading.Thread(target=wait_for_clipboard_change, daemon=True).start()

# Start mouse listener
mouse.Listener(on_click=on_click).start()

# Start keyboard listener
with Listener(on_release=on_release) as keyboard_listener:
    keyboard_listener.join()
