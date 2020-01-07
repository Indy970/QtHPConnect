# QtHPConnect
Qt Based Interface to a HP Prime Calculator

Originally intended as a Qt based GUI to libhpcalcs but it looks like much of the
HP Prime interface has changed.

The code currently includes hpcalcs as a reference but this will come out in time.

Using libusb to communicate directly with the HP Prime. This requires reverse engineering of the interface.

The sync version of libusb is applied, but ultimately the async routines will be used.

If anyone wants to help, or has a description of the protocol, you are very welcome to get involved

Depends libusb-dev
Depends libhidapi-libusb

Note : udev/rules.d
SUBSYSTEM=="usb", ATTRS{idVendor}=="03f0", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="03f0", MODE="0666"
