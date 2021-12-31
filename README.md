# keyboard-filter-leonardo

This is my first and probably last project involving arduino.
It is heavily based on that project:
https://github.com/gdsports/USBPassThru

However my implementation has only been tested on Arduino Leonardo + Arduino USB Host Shield.

It requires the following libraries:
* HID-Project: I tested with version 2.9.2. Used to send keyboard or consumer keys to computer and getting the led statuses. https://github.com/NicoHood/HID
* USB Host Shield 2.0: version 1.6.1. Used to deal with the physical keyboard. https://github.com/felis/USB_Host_Shield_2.0


# A short story:
A while ago I found a Sun Type 6 USB keyboard. I liked it, but my main computer running Windows would not make use of the eleven keys on the left and the four above they keypad.
So I decided to have a try at it, and it worked.
But there was not this key next to the left shift, layout was not ISO, and I wouldn't been able to change it to my local layout without changing some habits. So I bought a Sun Type 7 USB keyboard with an ISO layout, and with my local layout :)
But at first the Type 7 didn't work at all, that was because it included a usb hub. After a little fix in how usb is initialized in the code (just instantiate a "USBHub" object), both keyboards are working with the same sketch.

# Main features:

* LED status is not internally maintained, but reflects the OS state. If there is another keyboard plugged to the commputer, changing numlock/capslock/scrolllock on the other keyboard will make the led immediately change.
* whenever "something" is done to the report sent by the physical keyboard before sending something to the computer, the "compose" led is flashed.

I made some choices for key use, but it is pretty easy to change and adapt to your liking.


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

