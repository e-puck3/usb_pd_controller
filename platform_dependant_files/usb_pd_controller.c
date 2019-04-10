/**
 * @file	usb_pd_controller.c
 * @brief  	File containing the high levels functions to use the PD Buddy library
 * 
 * @written by  	Eliot Ferragni
 * @creation date	08.04.2019
 */
#include <ch.h>
#include <hal.h>

#include <pdb.h>
#include <pd.h>
#include <device_policy_manager.h>

#include <usb_pd_controller.h>
#include "chprintf.h"


/********************            CONFIGURATION VARIABLES           ********************/

/*
 * I2C configuration object.
 * I2C2_TIMINGR: 1000 kHz with I2CCLK = 54 MHz, rise time = 100 ns,
 *               fall time = 10 ns (0x0080091C)
 */
static const I2CConfig i2c2config = {
    0x0080091C,
    0,
    0
};

/*
 * PD profile config
 */
static struct pdbs_config pd_config = {
    .flags    = 0,
    .v        = 0,
    .i        = 200,
    .vmin     = 12000,
    .vmax     = 19000,
    .status   = PDBS_CONFIG_STATUS_VALID,
};

/*
 * PD Buddy Sink DPM data
 */
static struct pdbs_dpm_data dpm_data = {
    NULL,
    fusb_tcc_none,
    true,
    true,
    true,
    ._present_voltage = 5000
};

/*
 * PD Buddy firmware library configuration object
 */
static struct pdb_config pdb_config = {
    .fusb = {
        &I2CD2,
        FUSB302B_ADDR,
        LINE_INT_PD_CTRL_n
    },
    .dpm = {
    	pdbs_dpm_init,
        pdbs_dpm_evaluate_capability,
        pdbs_dpm_get_sink_capability,
        pdbs_dpm_giveback_enabled,
        pdbs_dpm_evaluate_typec_current,
        pdbs_dpm_check_vbus,
        pdbs_dpm_wait_vbus,
        pdbs_dpm_pd_start,
        pdbs_dpm_transition_default,
        pdbs_dpm_transition_min,
        pdbs_dpm_transition_standby,
        pdbs_dpm_transition_requested,
        pdbs_dpm_transition_typec,
        NULL /* not_supported_received */
    },
    .dpm_data = &dpm_data,
    .pd_config = &pd_config,
    .vbus_line = LINE_PWR_PP_STATE,
};

/********************               PRIVATE FUNCTIONS              ********************/

/*
 * Helper function for printing PDOs
 * 
 * Copyright (C) 2017-2018 Clayton G. Hobbs <clay@lakeserv.net>
 */
static void print_src_fixed_pdo(BaseSequentialStream *chp, uint32_t pdo)
{
    int tmp;

    chprintf(chp, "fixed\r\n");

    /* Dual-role power */
    tmp = (pdo & PD_PDO_SRC_FIXED_DUAL_ROLE_PWR) >> PD_PDO_SRC_FIXED_DUAL_ROLE_PWR_SHIFT;
    if (tmp) {
        chprintf(chp, "\tdual_role_pwr: %d\r\n", tmp);
    }

    /* USB Suspend Supported */
    tmp = (pdo & PD_PDO_SRC_FIXED_USB_SUSPEND) >> PD_PDO_SRC_FIXED_USB_SUSPEND_SHIFT;
    if (tmp) {
        chprintf(chp, "\tusb_suspend: %d\r\n", tmp);
    }

    /* Unconstrained Power */
    tmp = (pdo & PD_PDO_SRC_FIXED_UNCONSTRAINED) >> PD_PDO_SRC_FIXED_UNCONSTRAINED_SHIFT;
    if (tmp) {
        chprintf(chp, "\tunconstrained_pwr: %d\r\n", tmp);
    }

    /* USB Communications Capable */
    tmp = (pdo & PD_PDO_SRC_FIXED_USB_COMMS) >> PD_PDO_SRC_FIXED_USB_COMMS_SHIFT;
    if (tmp) {
        chprintf(chp, "\tusb_comms: %d\r\n", tmp);
    }

    /* Dual-Role Data */
    tmp = (pdo & PD_PDO_SRC_FIXED_DUAL_ROLE_DATA) >> PD_PDO_SRC_FIXED_DUAL_ROLE_DATA_SHIFT;
    if (tmp) {
        chprintf(chp, "\tdual_role_data: %d\r\n", tmp);
    }

    /* Unchunked Extended Messages Supported */
    tmp = (pdo & PD_PDO_SRC_FIXED_UNCHUNKED_EXT_MSG) >> PD_PDO_SRC_FIXED_UNCHUNKED_EXT_MSG_SHIFT;
    if (tmp) {
        chprintf(chp, "\tunchunked_ext_msg: %d\r\n", tmp);
    }

    /* Peak Current */
    tmp = (pdo & PD_PDO_SRC_FIXED_PEAK_CURRENT) >> PD_PDO_SRC_FIXED_PEAK_CURRENT_SHIFT;
    if (tmp) {
        chprintf(chp, "\tpeak_i: %d\r\n", tmp);
    }

    /* Voltage */
    tmp = (pdo & PD_PDO_SRC_FIXED_VOLTAGE) >> PD_PDO_SRC_FIXED_VOLTAGE_SHIFT;
    chprintf(chp, "\tv: %d.%02d V\r\n", PD_PDV_V(tmp), PD_PDV_CV(tmp));

    /* Maximum Current */
    tmp = (pdo & PD_PDO_SRC_FIXED_CURRENT) >> PD_PDO_SRC_FIXED_CURRENT_SHIFT;
    chprintf(chp, "\ti: %d.%02d A\r\n", PD_PDI_A(tmp), PD_PDI_CA(tmp));
}

/*
 * Helper function for printing PDOs
 * 
 * Copyright (C) 2017-2018 Clayton G. Hobbs <clay@lakeserv.net>
 */
static void print_src_pps_apdo(BaseSequentialStream *chp, uint32_t pdo)
{
    int tmp;

    chprintf(chp, "pps\r\n");

    /* Minimum voltage */
    tmp = (pdo & PD_APDO_PPS_MIN_VOLTAGE) >> PD_APDO_PPS_MIN_VOLTAGE_SHIFT;
    chprintf(chp, "\tvmin: %d.%02d V\r\n", PD_PAV_V(tmp), PD_PAV_CV(tmp));

    /* Maximum voltage */
    tmp = (pdo & PD_APDO_PPS_MAX_VOLTAGE) >> PD_APDO_PPS_MAX_VOLTAGE_SHIFT;
    chprintf(chp, "\tvmax: %d.%02d V\r\n", PD_PAV_V(tmp), PD_PAV_CV(tmp));

    /* Maximum current */
    tmp = (pdo & PD_APDO_PPS_CURRENT) >> PD_APDO_PPS_CURRENT_SHIFT;
    chprintf(chp, "\ti: %d.%02d A\r\n", PD_PAI_A(tmp), PD_PAI_CA(tmp));
}

/*
 * Helper function for printing PDOs
 * 
 * Copyright (C) 2017-2018 Clayton G. Hobbs <clay@lakeserv.net>
 */
static void print_src_pdo(BaseSequentialStream *chp, uint32_t pdo, uint8_t index)
{
    /* If we have a positive index, print a label for the PDO */
    if (index) {
        chprintf(chp, "PDO %d: ", index);
    }

    /* Select the appropriate method for printing the PDO itself */
    if ((pdo & PD_PDO_TYPE) == PD_PDO_TYPE_FIXED) {
        print_src_fixed_pdo(chp, pdo);
    } else if ((pdo & PD_PDO_TYPE) == PD_PDO_TYPE_AUGMENTED
            && (pdo & PD_APDO_TYPE) == PD_APDO_TYPE_PPS) {
        print_src_pps_apdo(chp, pdo);
    } else {
        /* Unknown PDO, just print it as hex */
        chprintf(chp, "%08X\r\n", pdo);
    }
}

/********************                PUBLIC FUNCTIONS              ********************/

void usbPDControllerInit(void){
	i2cStart(pdb_config.fusb.i2cp, &i2c2config);
	pdb_init(&pdb_config);
}

bool usbPDControllerIsPowerReady(void){

    return pdb_config.pe._explicit_contract;
}

bool usbPDControllerIsContract(void){
    if(pdb_config.pe._explicit_contract){
        return dpm_data._capability_match;
    }else{
        return false;
    }
}

uint16_t usbPDControllerGetNegociatedVoltage(void){

    return dpm_data._requested_voltage;
}

bool usbPDControllerSetFixedVoltage(uint16_t voltage){
    if (voltage <= PD_MV_MAX) {
        pd_config.v = voltage;
        if(usbPDControllerIsPowerReady()){
            chEvtSignal(pdb_config.pe.thread, PDB_EVT_PE_NEW_POWER);
        }
        return true;
    }else{
        return false;
    }
}

bool usbPDControllerSetRangeVoltage(uint16_t vmin, uint16_t vmax){
    if (vmin <= PD_MV_MAX && vmax <= PD_MV_MAX &&
        vmin < vmax) {
        pd_config.vmin = vmin;
        pd_config.vmax = vmax;
        if(usbPDControllerIsPowerReady()){
            chEvtSignal(pdb_config.pe.thread, PDB_EVT_PE_NEW_POWER);
        }
        return true;
    }else{
        return false;
    }
}

void usbPDControllerSetHVPrefered(bool enable){
    if(enable){
        pd_config.flags |= PDBS_CONFIG_FLAGS_HV_PREFERRED;
    }else{
        pd_config.flags &= ~PDBS_CONFIG_FLAGS_HV_PREFERRED;
    }

    if(usbPDControllerIsPowerReady()){
        chEvtSignal(pdb_config.pe.thread, PDB_EVT_PE_NEW_POWER);
    }
}

bool usbPDControllerGetHVPrefered(void){
    
    return (pd_config.flags & PDBS_CONFIG_FLAGS_HV_PREFERRED);
}

bool usbPDControllerSetFixedCurrent(uint16_t current){
    if (current > PD_MA_MIN && current <= PD_MA_MAX) {
        /* Convert mA to the unit used by USB PD */
        pd_config.i = PD_MA2PDI(current);
        /* Set the flags to say we're storing a current */
        pd_config.flags &= ~PDBS_CONFIG_FLAGS_CURRENT_DEFN;
        pd_config.flags |= PDBS_CONFIG_FLAGS_CURRENT_DEFN_I;
        if(usbPDControllerIsPowerReady()){
            chEvtSignal(pdb_config.pe.thread, PDB_EVT_PE_NEW_POWER);
        }
        return true;
    }else{
        return false;
    }
}

void usbPDControllerPrintSrcPDO(BaseSequentialStream *chp){
    /* If we haven't seen any Source_Capabilities */
    if (dpm_data.capabilities == NULL) {
        /* Have we started reading Type-C Current advertisements? */
        if (dpm_data.typec_current != fusb_tcc_none) {
            /* Type-C Current is available, so report it */
            chprintf(chp, "PDO 1: typec_virtual\r\n");
            if (dpm_data.typec_current == fusb_tcc_default) {
                chprintf(chp, "\ti: 0.50 A\r\n");
            } else if (dpm_data.typec_current == fusb_tcc_1_5) {
                chprintf(chp, "\ti: 1.50 A\r\n");
            } else if (dpm_data.typec_current == fusb_tcc_3_0) {
                chprintf(chp, "\ti: 3.00 A\r\n");
            }
            return;
        } else {
            /* No Type-C Current, so report no capabilities */
            chprintf(chp, "No Source_Capabilities\r\n");

            return;
        }
    }else if(!usbPDControllerIsPowerReady()){
        //we have capabilities but they are out dated
        chprintf(chp, "No power source present\r\n");
        return;
    }

    /* Print all the PDOs */
    uint8_t numobj = PD_NUMOBJ_GET(dpm_data.capabilities);
    for (uint8_t i = 0; i < numobj; i++) {
        print_src_pdo(chp, dpm_data.capabilities->obj[i], i+1);
    }
}

void usbPDControllerPrintConfig(BaseSequentialStream *chp)
{
    /* Print the status */
    chprintf(chp, "status: ");
    switch (pd_config.status) {
        case PDBS_CONFIG_STATUS_INVALID:
            chprintf(chp, "in");
            /* fall-through */
        case PDBS_CONFIG_STATUS_VALID:
            chprintf(chp, "valid\r\n");
            break;
        case PDBS_CONFIG_STATUS_EMPTY:
            chprintf(chp, "empty\r\n");
            /* Stop early because the rest of the information is meaningless in
             * this case. */
            return;
        default:
            chprintf(chp, "%04X\r\n", pd_config.status);
            break;
    }

    /* Print the flags */
    chprintf(chp, "flags:");
    if ((pd_config.flags & ~PDBS_CONFIG_FLAGS_CURRENT_DEFN) == 0) {
        chprintf(chp, " (none)");
    }
    if (pd_config.flags & PDBS_CONFIG_FLAGS_GIVEBACK) {
        chprintf(chp, " GiveBack");
    }
    if (pd_config.flags & PDBS_CONFIG_FLAGS_VAR_BAT) {
        chprintf(chp, " Var/Bat");
    }
    if (pd_config.flags & PDBS_CONFIG_FLAGS_HV_PREFERRED) {
        chprintf(chp, " HV_Preferred");
    }
    chprintf(chp, "\r\n");

    /* Print voltage */
    chprintf(chp, "v: %d.%03d V\r\n", PD_MV_V(pd_config.v), PD_MV_MV(pd_config.v));
    /* If either end of the range is non-zero, print the range */
    if (pd_config.vmin != 0 || pd_config.vmax != 0) {
        chprintf(chp, "vmin: %d.%03d V\r\n", PD_MV_V(pd_config.vmin),
                 PD_MV_MV(pd_config.vmin));
        chprintf(chp, "vmax: %d.%03d V\r\n", PD_MV_V(pd_config.vmax),
                 PD_MV_MV(pd_config.vmax));
    }
    /* Print current-deriving setting */
    switch (pd_config.flags & PDBS_CONFIG_FLAGS_CURRENT_DEFN) {
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_I:
            chprintf(chp, "i: %d.%02d A\r\n", PD_PDI_A(pd_config.i), PD_PDI_CA(pd_config.i));
            break;
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_P:
            chprintf(chp, "p: %d.%02d W\r\n", PD_CW_W(pd_config.p), PD_CW_CW(pd_config.p));
            break;
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_R:
            chprintf(chp, "r: %d.%02d \316\251\r\n", PD_CO_O(pd_config.r), PD_CO_CO(pd_config.r));
            break;
    }
}

