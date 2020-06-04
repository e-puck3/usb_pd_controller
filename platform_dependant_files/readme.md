USB PD Controller for ChibiOS
=============================

Files description
-----------------
Here is a list of the important files contained in the **platform_dependant_files** folder :

- **device_policy_manager.c/.h** --Files containing part of the machine state of the pd controller. It's mainly here that its behavior can be modified if needed.
- **usb_pd_controller.c** --The top of this file contains the configuration of the various parts of the pd controller. The user has check and modify some settings like the GPIO lines used or the I2C configuration for example
- **usb_pd_controller.h** --Contains the declarations of the function meant to be used by the user. All the others functions findables in other files are meant to be used internally.


Build instructions
------------------
The modifications made to the pd-buddy library let people use it as a submodule for their project using ChibiOS.
In order to use it, you need to :

- Add the specific variables below to your makefile before the **CSRC** definition
  - ``PDBLIB`` : Path to the **pdblib** folder (relative to the ChibiOS makefile)
  - ``include $(PDBLIB)/pdblib.mk``
- Add these variables in your makefile if it's not already the case
	-	``$(ALLCSRC)`` 	to 	``CSRC``
	-	``$(ALLINC)`` 	to 	``INCDIR``
- Copy the **device_policy_manager.c/.h** and **usb_pd_controller.c/.h** files to your project and don't forget to include them in your makefile. These files are platform dependant so you will have to change some settings. The high level functions are located in the **usb_pd_controller.c/.h** files.
- Finally, include **usb_pd_controller.h** in your C code in order to use the library.

Shell commands
--------------

###Hot to add the commands to the shell

Add ``USB_PD_CONTROLLER_SHELL_CMD`` inside the ``ShellCommand`` array.

For example the following ``ShellCommand`` array : 

```c
...
static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"write", cmd_write},
  {NULL, NULL}
};
...
```
Would become :

```c
...
static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"write", cmd_write},
  USB_PD_CONTROLLER_SHELL_CMD
  {NULL, NULL}
};
...
```
### Command list
The following commands are provided to the shell with ``USB_PD_CONTROLLER_SHELL_CMD`` :

- ``pd_get_source_cap`` : Prints the source capabilities
- ``pd_get_cfg`` : Prints the actual config
- ``pd_set_v`` : Sets the wanted voltage
- ``pd_set_vrange`` : Sets the wanted voltage range
- ``pd_set_i`` : Sets the current wanted
- ``pd_hv_prefered`` : Sets the hv_prefered setting
- ``pd_get_contract`` : Prints if a contract is made and the actual voltage