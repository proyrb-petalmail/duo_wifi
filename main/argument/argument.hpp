#pragma once

/* fbdev */
#define Argument_Fbdev         "fbdev"
#define Argument_Fbdev_Short   'f'
#define Argument_Fbdev_Refer   "specify fbdev"
#define Argument_Fbdev_Need    false
#define Argument_Fbdev_Default "/dev/fb0"

/* evdev */
#define Argument_Evdev         "evdev"
#define Argument_Evdev_Short   'e'
#define Argument_Evdev_Refer   "specify evdev"
#define Argument_Evdev_Need    false
#define Argument_Evdev_Default "/dev/input/event0"

/* gui */
#define Argument_Gui           "gui"
#define Argument_Gui_Short     'g'
#define Argument_Gui_Refer     "specify gui configuration file"
#define Argument_Gui_Need      false
#define Argument_Gui_Default   "gui.json"
