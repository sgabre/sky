# Raspberry Version

Raspberry Pi 4 Model B 8GB RAM
SD Card Sizes: 32 GByte
Mac Address: e4:5f:01:fa:c5:09

# How to find a raspberry Pi on the local network ?

 arp -a | grep raspberry

# How to connect to the raspberry via ssh ?

ssh pi@192.168.1.22

# how to get the RAM Size ?

pi@raspberrypi:~ $ free -h
               total        used        free      shared  buff/cache   available
Mem:           7.6Gi       192Mi       7.4Gi       1.1Mi       133Mi       7.4Gi
Swap:           99Mi          0B        99Mi


free -ght
               total        used        free      shared  buff/cache   available
Mem:           7.6Gi       192Mi       7.4Gi       1.1Mi       133Mi       7.4Gi
Swap:           99Mi          0B        99Mi
Total:         7.7Gi       192Mi       7.5Gi

# how to get the SD Card Size ?

df -Bm

Filesystem     1M-blocks  Used Available Use% Mounted on
udev               3642M    0M     3642M   0% /dev
tmpfs               782M    2M      780M   1% /run
/dev/mmcblk0p2    28784M 1716M    25587M   7% /
tmpfs              3906M    0M     3906M   0% /dev/shm
tmpfs                 5M    1M        5M   1% /run/lock
/dev/mmcblk0p1      510M   63M      447M  13% /boot/firmware
tmpfs               782M    0M      782M   0% /run/user/1000

# Howto: Use AirDrop on a Raspberry Pi 3 
[Link](https://owlink.org/2019/05/16/howto-use-airdrop-on-raspberry-pi-3.html)


# 3D Printer

Where the gcode file are store inside the raspberry:
/home/pi/.octoprint/uploads


# Process

i-File: xyzCalibration_cube.stl
o-File: XYZ 20mm Calibration Cube.gcode


