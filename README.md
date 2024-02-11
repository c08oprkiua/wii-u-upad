# wii-u-upad
Small library to merge VPAD and KPAD for one-input-source Wii U GUI apps

This is a small little class designed to make it so that simple GUI apps on Wii U can take inputs from any Wiimote, Gamepad, Pro controller, or (in the future) HID device connected to the Wii U, without much extra effort on part of the app developer.

Usage is reminiscent of KPAD or VPAD, but now you instantiate UPAD as an class object and call `read()` instead of making a variable, an error out, etc, and then putting those into a read function. UPAD will automatically have the usual variables you're looking for under `upad_var.trigger`, `upad_var.error`, etc. For more in-depth documentation, run a doxygen generator over this project's repo.

## Usage notes: 
* As it is currently programmed, this will merge all inputs into a single output, meaning (for example) one controller pressing up and another pressing down will return both up *and* down from UPAD.
* Input support is limited to the standard ABXY face buttons, home, the d-pad, (-), and (+); just enough for GUI navigation.

# Button mappings

Button mappings here are assumed relative to the buttons on a Wii U gamepad, though not all buttons on all controller types have mappings due to inconsistencies. If a button is not listed, assume the button naming is how the button is mapped, eg. button A on a Wiimote is button A on a Wii U gamepad.

## Wiimote
* 1 = X
* 2 = Y
## Wiimote + Nunchuk
* nunchuk joystick = d-pad
* C = X
* Z = Y
* 1 and 2 are not mapped to anything, unlike Wiimote standalone
## Classic controller
* Left stick = d-pad
* Parent Wiimote inputs will not be checked while a classic controller is connected to it
## Pro controller
* Left stick = d-pad
* No changes, mapped as expected