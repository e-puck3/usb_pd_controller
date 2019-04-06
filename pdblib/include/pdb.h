/*
 * PD Buddy Firmware Library - USB Power Delivery for everyone
 * Copyright 2017-2018 Clayton G. Hobbs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PDB_H
#define PDB_H

#include <pdb_fusb.h>
#include <pdb_dpm.h>
#include <pdb_pe.h>
#include <pdb_prl.h>
#include <pdb_int_n.h>
#include <pdb_msg.h>


/* Version information */
#define PDB_LIB_VERSION "0.1.0"
#define PDB_LIB_MAJOR 0
#define PDB_LIB_MINOR 1
#define PDB_LIB_PATCH 0

/* Status for configuration structures.  EMPTY indicates that the struct is
 * ready to be written, including a status update to VALID.  Once the struct is
 * no longer needed, the status is updated to INVALID.  Erasing the flash page
 * resets all structures to EMPTY. */
#define PDBS_CONFIG_STATUS_INVALID 0x0000
#define PDBS_CONFIG_STATUS_VALID 0xBEEF
#define PDBS_CONFIG_STATUS_EMPTY 0xFFFF

/* Flags for configuration structures. */
/* GiveBack supported */
#define PDBS_CONFIG_FLAGS_GIVEBACK (1 << 0)
/* Variable and battery PDOs preferred (FIXME: not implemented) */
#define PDBS_CONFIG_FLAGS_VAR_BAT (1 << 1)
/* High voltages preferred */
#define PDBS_CONFIG_FLAGS_HV_PREFERRED (1 << 2)
/* Current definition type */
#define PDBS_CONFIG_FLAGS_CURRENT_DEFN_SHIFT 3
#define PDBS_CONFIG_FLAGS_CURRENT_DEFN (0x3 << PDBS_CONFIG_FLAGS_CURRENT_DEFN_SHIFT)
#define PDBS_CONFIG_FLAGS_CURRENT_DEFN_I (0 << PDBS_CONFIG_FLAGS_CURRENT_DEFN_SHIFT)
#define PDBS_CONFIG_FLAGS_CURRENT_DEFN_P (1 << PDBS_CONFIG_FLAGS_CURRENT_DEFN_SHIFT)
#define PDBS_CONFIG_FLAGS_CURRENT_DEFN_R (2 << PDBS_CONFIG_FLAGS_CURRENT_DEFN_SHIFT)

/*
 * PD Buddy Sink configuration structure
 */
struct pdbs_config {
    /* Status halfword, used to indicate which config objects can be written to
     * and which one is valid. */
    uint16_t status;
    /* Flags halfword for miscellaneous small fields. */
    uint16_t flags;
    /* Preferred voltage, in millivolts. */
    uint16_t v;
    /* Union for specifying how much current to request. */
    union {
        /* Required current, in centiamperes. */
        uint16_t i;
        /* Required power, in centiwatts. */
        uint16_t p;
        /* Value of resistive load, in centiohms. */
        uint16_t r;
    };
    /* Lower end of voltage range, in millivolts. */
    uint16_t vmin;
    /* Upper end of voltage range, in millivolts. */
    uint16_t vmax;
    /* Extra bytes reserved for future use. */
    uint16_t _reserved[2];
} __attribute__((packed));

/*
 * Structure for one USB port's PD Buddy firmware library configuration
 *
 * Contains working areas for statically allocated threads, and therefore must
 * be statically allocated!
 */
struct pdb_config {
    /* User-initialized fields */
    /* Configuration information for the FUSB302B* chip */
    struct pdb_fusb_config fusb;
    /* DPM callbacks */
    struct pdb_dpm_callbacks dpm;
    /* Pointer to port-specific DPM data */
    void *dpm_data;

    /* Pointer to PD config */
    struct pdbs_config* pd_config;

    /* Line used to detect VBUS */
    ioline_t vbus_line;

    /* Automatically initialized fields */
    /* Policy Engine thread and related variables */
    struct pdb_pe pe;
    /* Protocol layer threads and related variables */
    struct pdb_prl prl;
    /* INT_N pin thread and related variables */
    struct pdb_int_n int_n;
};


/*
 * Initialize the PD Buddy firmware library, starting all its threads
 *
 * The I2C driver must already be initialized before calling this function.
 */
void pdb_init(struct pdb_config *);


#endif /* PDB_H */
