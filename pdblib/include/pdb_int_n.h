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

/**
 * @modified by         Eliot Ferragni
 * @modification date   08.04.2019
 */

#ifndef PDB_INT_N_H
#define PDB_INT_N_H

#include <ch.h>

#include "pdb_conf.h"


/*
 * Structure for the INT_N thread
 */
struct pdb_int_n {
    /* INT_N thread */
    thread_t *thread;
};


#endif /* PDB_INT_N_H */
