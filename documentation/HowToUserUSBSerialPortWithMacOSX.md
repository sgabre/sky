# How to use a Max OS X for connecting through the Virtual COM Port with the Nucleo F401RE Board?



You'll need to get a usb-serial adapter and then connect with a console cable. Adapters aren't very expensive.

Connecting to the Console Port with Mac OS X
To connect a Mac OS X system USB port to the console using the built-in OS X Terminal utility, follow these steps:
Step 1 Use the Finder to go to Applications > Utilities > Terminal.

Step 2 Connect the OS X USB port to the router.

Step 3 Enter the following commands to find the OS X USB port number:

macbook:user$ cd /dev
macbook:user$ ls -ltr /dev/*usb*
crw-rw-rw- 1 root wheel 9, 66 Apr 1 16:46 tty.usbmodem1a21
DT-macbook:dev user$
Step 4 Connect to the USB port with the following command followed by the router USB port speed:

macbook:user$ screen /dev/tty.usbmodem1a21 9600
To Disconnect the OS X USB Console from the Terminal Window
Enter Ctrl+A followed by Ctrl+\

Hth,
John


cd /dev
ls -ltr /dev/*usb*

screen /dev/tty.usbserial-AQ02K49L 115200
screen /dev/cu.usbserial-AQ02K49L 115200
screen/dev/tty.usbmodem11103 115200
screen /dev/cu.usbmodem11103 115200


screen /dev/cu.usbserial-AQ02K49L 115200

screen /dev/tty.usbserial-AQ02K49L 115200











 /dev/tty.usbserial-AQ02K49L
/dev/tty.usbmodem11103
/dev/cu.usbserial-AQ02K49L
/dev/cu.usbmodem11103