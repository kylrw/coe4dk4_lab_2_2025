
/*
 * 
 * Simulation of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/******************************************************************************/

#define DATA_PACKET_LENGTH 500 /* bits */
#define LINK_BIT_RATE 1e6 /* bits per second */
#define RUNLENGTH 1e6 /* packets */

/* Voice traffic parameters */
#define VOICE_INTER_ARRIVAL_TIME 0.020 /* 20 ms */
#define VOICE_PAYLOAD_BITS (64000 * 0.020) /* 64 Kbps * 20 ms = 1280 bits */
#define VOICE_HEADER_BITS (62 * 8) /* 62 bytes * 8 bits/byte = 496 bits */
#define VOICE_PACKET_LENGTH (VOICE_PAYLOAD_BITS + VOICE_HEADER_BITS) /* total bits */

/* Service time parameters */
#define DATA_SERVICE_TIME 0.040 /* 40 ms mean service time for data packets */

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 333333

#define DATA_XMT_TIME ((double) DATA_PACKET_LENGTH/LINK_BIT_RATE)
#define VOICE_XMT_TIME ((double) VOICE_PACKET_LENGTH/LINK_BIT_RATE)
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */



