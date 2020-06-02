The modifications made to the pd-buddy library let people use it as a submodule for their project using ChibiOS.
In order to use it, you need to :

1) Add these lines somewhere in the makefile in the same manner you would do with components of ChibiOS. The path should link to the pdblib folder and is referenced from your makefile

PDBLIB = path_to_pdblib
include $(PDBLIB)/pdblib.mk

2) Copy the device_policy_manager.c/.h and usb_pd_controller.c/.h to your project and don't forget to include them to your makefile. These files are platform dependant so you will have to change some settings. The high level functions are located in the usb_pd_controller files.