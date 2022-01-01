# keyboard-filter-leonardo

This is my first and probably last project involving arduino.
It is used to get the "additional" keys on a Sun keyboard do something on a computer running windows. I use it with a Sun Type 7 keyboard, works also with a Type 6 one, a generic keyboard too (even it is useless in that case since there are no "additional" keys involved).
As long as the keyboard sends a keyboard report when a key is pressed, the sketch can do something with it.

It is heavily based on that project:
https://github.com/gdsports/USBPassThru

However my implementation has only been tested on Arduino Leonardo + Arduino USB Host Shield.

It requires the following libraries:
* HID-Project: I developed with version 2.9.2. It is used to send keyboard keys or consumer keys to the computer, and getting the led statuses. https://github.com/NicoHood/HID
* USB Host Shield 2.0: version 1.6.1. It is used to deal with the physical keyboard. https://github.com/felis/USB_Host_Shield_2.0
Those can be directly installed in the arduino development environment.

# A short story:
A while ago I found a Sun Type 6 USB keyboard. I liked it, but my main computer running Windows would not make use of the eleven keys on the left and the four above they keypad.
So I decided to have a try at it, make a simple kind of keyboard codes filter, between the keyboard and the computer like in the USBPassThru, and it worked.
But this keyboard does not have this key next to the left shift (layout was US and not ISO), so I wouldn't be able to use it without changing some habits. So I bought a Sun Type 7 USB keyboard with an ISO layout, and with my local layout :)
At first the Type 7 didn't work at all. That was because it includes a usb hub.
After a little fix in how usb is initialized in the code (just instantiate a "USBHub" object in the code), both keyboards are working with the same sketch.

# Main features:

* The USB Host shield library keyboard implementation maintains LEDs status (it keeps track of capslock etc. state). I kind of ignored it and decided to reflect the OS state. For example, if there is another keyboard plugged to the commputer, changing numlock/capslock/scrolllock on the other keyboard will make the led immediately change on the keyboard plugged to the arduino.
* whenever "something" is done to the report (i.e. the combination of pressed keys) sent by the physical keyboard, like changing the report contents or sending a consumer report, the "compose" led is flashed.

I made some choices on how keys are remapped, but it is pretty easy to change and adapt to your liking.


The following "mappings" are performed (key pressed: action/key combination sent to computer)
* Help: browser home (or refresh in combination with shift)
* Stop: ctrl+shift+esc (windows task manager shortcut)
* Props: meta-I (windows settings)
* Front: meta-d (display/hide desktop windows shortcut)
* Open: meta-r (execute window shortcut)
* Find: ctrl-f
* Again: ctrl-y
* Undo: ctrl-z
* Copy: ctrl-c
* Paste: ctrl-v
* Cut: ctrl-x
* Mute/degaus : mute consumer action
* vol-/contrast-: vol- consumer action (brightness- if shift)
* vol+/contrast+: vol+ consumer action (broghtness+ if shift)
* sleep: meta-l consumer action (lock session shortcut)

So the keys do not exactly match what it written on them, but that is up to you to map however you want.

# Notes

Another and probably better solution would be for the code to only map to consumer actions known by windows (like volume, play/pause etc.). That way windows "sees" the key as unique, and it becomes easy to change the behaviour of the key directly in windows with software like AHK or power toys.
In my case, when mapping a key ignored by windows to ctrl-c or whatever means I won't be able to do anything else with it.
But on the other hand, if I change the computer on which the keyboard is plugged, everything works immediately.

