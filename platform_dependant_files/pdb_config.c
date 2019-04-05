/*
 * PD Buddy Sink Firmware - Smart power jack for USB Power Delivery
 * Copyright (C) 2017-2018 Clayton G. Hobbs <clay@lakeserv.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pdb_config.h"

#include "chprintf.h"

#include <pd.h>


static struct pdbs_config pd_config = {
    .flags    = 0,
    .v        = 9000,
    .i        = 200,
    .vmin     = 12000,
    .vmax     = 19000,
    .status   = PDBS_CONFIG_STATUS_VALID,
};

void pdbs_config_print(BaseSequentialStream *chp, const struct pdbs_config *cfg)
{
    /* Print the status */
    chprintf(chp, "status: ");
    switch (cfg->status) {
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
            chprintf(chp, "%04X\r\n", cfg->status);
            break;
    }

    /* Print the flags */
    chprintf(chp, "flags:");
    if ((cfg->flags & ~PDBS_CONFIG_FLAGS_CURRENT_DEFN) == 0) {
        chprintf(chp, " (none)");
    }
    if (cfg->flags & PDBS_CONFIG_FLAGS_GIVEBACK) {
        chprintf(chp, " GiveBack");
    }
    if (cfg->flags & PDBS_CONFIG_FLAGS_VAR_BAT) {
        chprintf(chp, " Var/Bat");
    }
    if (cfg->flags & PDBS_CONFIG_FLAGS_HV_PREFERRED) {
        chprintf(chp, " HV_Preferred");
    }
    chprintf(chp, "\r\n");

    /* Print voltage */
    chprintf(chp, "v: %d.%03d V\r\n", PD_MV_V(cfg->v), PD_MV_MV(cfg->v));
    /* If either end of the range is non-zero, print the range */
    if (cfg->vmin != 0 || cfg->vmax != 0) {
        chprintf(chp, "vmin: %d.%03d V\r\n", PD_MV_V(cfg->vmin),
                 PD_MV_MV(cfg->vmin));
        chprintf(chp, "vmax: %d.%03d V\r\n", PD_MV_V(cfg->vmax),
                 PD_MV_MV(cfg->vmax));
    }
    /* Print current-deriving setting */
    switch (cfg->flags & PDBS_CONFIG_FLAGS_CURRENT_DEFN) {
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_I:
            chprintf(chp, "i: %d.%02d A\r\n", PD_PDI_A(cfg->i), PD_PDI_CA(cfg->i));
            break;
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_P:
            chprintf(chp, "p: %d.%02d W\r\n", PD_CW_W(cfg->p), PD_CW_CW(cfg->p));
            break;
        case PDBS_CONFIG_FLAGS_CURRENT_DEFN_R:
            chprintf(chp, "r: %d.%02d \316\251\r\n", PD_CO_O(cfg->r), PD_CO_CO(cfg->r));
            break;
    }
}

void pdbs_config_update(const struct pdbs_config *cfg)
{
    

    /* Write the new configuration */
    pd_config.status   = cfg->status;
    pd_config.flags    = cfg->flags;
    pd_config.v        = cfg->v;
    pd_config.i        = cfg->i;
    pd_config.vmin     = cfg->vmin;
    pd_config.vmax     = cfg->vmax;

}

struct pdbs_config *pdbs_config_read(void)
{
    return &pd_config;
}
