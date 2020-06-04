/**
 * @file	usb_pd_controller.c
 * @brief  	File containing the high levels functions to use the PD Buddy library
 * 
 * @written by  	Eliot Ferragni
 * @creation date	08.04.2019
 */

#ifndef USB_PD_CONTROLLER_H
#define USB_PD_CONTROLLER_H

/********************                PUBLIC FUNCTIONS              ********************/

/**
 * @brief 	Inits the PD Buddy library (also configures the I2C used).
 */
void usbPDControllerStart(void);

/**
 * @brief 	Tells if we have an USB PD power conencted and ready.
 * @return 	True if connected, false otherwise.
 */
bool usbPDControllerIsPowerReady(void);

/**
 * @brief 	Tells if we have a contract with the power source.
 * 			Example :	If we ask a pair of voltage/current not available, then 
 * 						we don't have a contract.
 * 
 * @return 	True if we have a contract, false otherwise.
 */
bool usbPDControllerIsContract(void);

/**
 * @brief 	Gets the voltage negociated with the source. 
 * 			Will return 5V if we don't have a contract since it
 * 			is the default USB voltage. 
 * @return 	The voltage in mV.
 */
uint16_t usbPDControllerGetNegociatedVoltage(void);

/**
 * @brief 	Sets the fixed voltage we want.
 * 			Note : 	This will launch a new negociation with the source
 * 					if connected.
 * 
 * @param 	voltage	Desired voltage in mV. Should be in the range [PD_MV_MIN : PD_MV_MAX].
 * @return 	True if the specified voltage is valid, false otherwise.
 */
bool usbPDControllerSetFixedVoltage(uint16_t voltage);

/**
 * @brief 	Sets the voltage range we want. Used in case the fixed voltage isn't available.
 * 
 * @param 	vmin Desired min voltage in mV. Should be in the range [PD_MV_MIN : PD_MV_MAX].
 * @param 	vmax Desired max voltage in mV. Should be in the range [PD_MV_MIN : PD_MV_MAX].
 * 
 * @return 	True if the specified voltages are valids, false otherwise.
 */
bool usbPDControllerSetRangeVoltage(uint16_t vmin, uint16_t vmax);

/**
 * @brief 	Sets the voltage range we want. Used in case the fixed voltage isn't available.
 * 			When enabled, wduring the selection of a voltage in the voltage range set,
 * 			it will choose the highest one, otherwise the lowest.
 * 			Note : 	This will launch a new negociation with the source
 * 					if connected.
 * 
 * @param 	enable Desired state of the HV prefered setting.
 */
void usbPDControllerSetHVPrefered(bool enable);

/**
 * @brief 	Gets the HV prefered setting.
 * 
 * @return 	True if enabled, false otherwise.
 */
bool usbPDControllerGetHVPrefered(void);

/**
 * @brief 	Sets the fixed current we want.
 * 			Note : 	This will launch a new negociation with the source
 * 					if connected.
 * 
 * @param 	current	Desired current in mA. Should be in the range [PD_MA_MIN : PD_MA_MAX].
 * @return 	True if the specified current is valid, false otherwise.
 */
bool usbPDControllerSetFixedCurrent(uint16_t current);

/**
 * @brief 	Prints the capabilities of the source.
 * 
 * @param 	The stream to which we want to write.s
 */
void usbPDControllerPrintSrcPDO(BaseSequentialStream *chp);

/**
 * @brief 	Prints the PD config we use with a source.
 * 
 * @param 	The stream to which we want to write.
 */
void usbPDControllerPrintConfig(BaseSequentialStream *chp);

/********************                SHELL FUNCTIONS               ********************/

/**     
 * @brief 			Shell command to print the source capabilities
 * 					Calls usbPDControllerPrintSrcPDO()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_get_source_cap(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to print the actual config
 * 					Calls usbPDControllerPrintConfig()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_get_cfg(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to set the wanted voltage
 * 					Calls usbPDControllerSetFixedVoltage()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_set_v(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to set the wanted voltage range
 * 					Calls usbPDControllerSetRangeVoltage()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_set_vrange(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to set the current wanted
 * 					Calls usbPDControllerSetFixedCurrent()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_set_i(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to set the hv_prefered setting
 * 					Calls usbPDControllerSetHVPrefered()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_hv_prefered(BaseSequentialStream *chp, int argc, char *argv[]);
/**     
 * @brief 			Shell command to print if a contract is made and the actual voltage
 * 					Calls usbPDControllerIsContract() and usbPDControllerGetNegociatedVoltage()
 * 	
 * @param chp 		Pointer to the BaseSequentialStream stream to write to
 * @param argc 		Number of arguments given when calling this shell command
 * @param argv 		Array of the arguments given when calling thos shell command
 */	
void cmd_pd_get_contract(BaseSequentialStream *chp, int argc, char *argv[]);

#define USB_PD_CONTROLLER_SHELL_CMD					\
	{"pd_get_source_cap", cmd_pd_get_source_cap},	\
	{"pd_get_cfg", cmd_pd_get_cfg},					\
	{"pd_set_v", cmd_pd_set_v},						\
	{"pd_set_vrange", cmd_pd_set_vrange},			\
	{"pd_set_i", cmd_pd_set_i},						\
	{"pd_hv_prefered", cmd_pd_hv_prefered},			\
	{"pd_get_contract", cmd_pd_get_contract},		\

#endif /* USB_PD_CONTROLLER_H */