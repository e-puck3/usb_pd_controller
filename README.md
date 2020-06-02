usb_pd_controller
=================

This repository is a fork of the [PD Buddy Sink Firmware](https://git.clarahobbs.com/pd-buddy/pd-buddy-firmware.git).
It has been modified to be able to run as a submodule to add to a main project using ChibiOS. 

Only the library files and some functions of the Sink firmware have been kept. The PDBLIB library has been a little modified too in order to be more flexible with hot connections and disconnections of USB PD sources.

More details in the dedicated [readme](platform_dependant_files/readme.md) to know how to add it to your project.