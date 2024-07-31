# wii-u-upad
Small library to merge VPAD and KPAD for one-input-source Wii U GUI apps

This is a small little class designed to make it so that simple GUI apps on Wii U can take inputs from any Wiimote, Gamepad, Pro controller, or (in the future) HID device connected to the Wii U, without much extra effort on part of the app developer.

Usage is reminiscent of KPAD or VPAD, but now you instantiate UPAD as an class object and call `read()` instead of making a variable, an error out, etc, and then putting those into a read function. UPAD will automatically have the usual variables you're looking for under `upad_var.trigger`, `upad_var.error`, etc. For more in-depth documentation, run a doxygen generator over this project's repo (or look at the in-code comments).

## Usage notes: 
* As it is currently programmed, this will merge all inputs into a single output, meaning (for example) one controller pressing up and another pressing down will return both up *and* down from UPAD.
* Input support is limited to the standard ABXY face buttons, home, the d-pad, (-), and (+); just enough for GUI navigation.
* This library is not tested since I haven't bothered making a test app for it. So, as-is warranty stuff (it's in the license)

# Button mappings

* Button mappings here are assumed relative to the buttons on a Wii U gamepad. 
* Not all buttons on all controller types have mappings due to inconsistencies. 
* If a button is not listed, assume the button naming is how the button is mapped, eg. button A on a Wiimote is button A on a Wii U gamepad. 
* Any "left stick" is mapped to the d-pad.

## Wiimote
* 1 = Y
* 2 = X
## Wiimote + Nunchuk
* C = Y
* Z = X
* 1 and 2 are not mapped to anything, unlike Wiimote standalone
## Classic controller
* Parent Wiimote inputs will not be checked while a classic controller is connected to it
## Pro controller
* No changes, mapped as expected
## GameCube controller
* Start = (+)
* Z = (-)