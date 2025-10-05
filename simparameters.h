
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

#define PACKET_ARRIVAL_RATE 400 /* default packets per second (legacy) */

/* --- Parameters for Experiment 5: three-switch network --- */
/* Local arrival rates (packets/sec) for Switch 1, 2 and 3 */
#define LAMBDA1 750 /* packets/sec for Switch 1 (experiment 5) */
#define LAMBDA2 500 /* packets/sec for Switch 2 (fixed) */
#define LAMBDA3 500 /* packets/sec for Switch 3 (fixed) */

/* Packet length (bits) for experiment 5 */
#define PACKET_LENGTH 1000 /* bits */

/* Link bit rates (bits per second) for Link1, Link2, Link3 */
#define LINK1_BIT_RATE 2000000.0 /* 2 Mbps */
#define LINK2_BIT_RATE 1000000.0 /* 1 Mbps */
#define LINK3_BIT_RATE 1000000.0 /* 1 Mbps */

/* Probability that a packet transmitted on Link1 is destined for Link2.
	p12 = 1 - p13 */
#define P12 0.5

/* --- End Experiment 5 parameters --- */
#define RUNLENGTH 10e6 /* packets */

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400386351

#define PACKET_XMT_TIME ((double) PACKET_LENGTH/LINK2_BIT_RATE) /* legacy */
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */



