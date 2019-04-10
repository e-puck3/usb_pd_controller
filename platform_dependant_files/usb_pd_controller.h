/**
 * @file	usb_pd_controller.c
 * @brief  	File containing the high levels functions to use the PD Buddy library
 * 
 * @written by  	Eliot Ferragni
 * @creation date	08.04.2019
 */

#ifndef USB_PD_CONTROLLER_H
#define USB_PD_CONTROLLER_H

/**
 * @brief 	Inits the PD Buddy library (also configures the I2C used).
 */
void usbPDControllerInit(void);

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

#endif /* USB_PD_CONTROLLER_H */