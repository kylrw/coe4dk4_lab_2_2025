
/*
 * 
 * Simulation_Run of A Single Server Queueing System
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

#ifndef _MAIN_H_
#define _MAIN_H_

/******************************************************************************/

#include "simlib.h"
#include "simparameters.h"

/******************************************************************************/

typedef struct _simulation_run_data_ 
{
  Fifoqueue_Ptr buffer;
  Server_Ptr link;
  /* For experiment 5, support up to 3 switches. We'll keep legacy single
     server fields for backward compatibility and also add arrays for the
     multi-switch experiment. */
  Fifoqueue_Ptr buffers[3];
  Server_Ptr links[3];
  long int arrival_count_per_switch[3];
  long int number_of_packets_processed_per_switch[3];
  double accumulated_delay_per_switch[3];
  long int blip_counter;
  long int arrival_count;
  long int number_of_packets_processed;
  double accumulated_delay;
  unsigned random_seed;
} Simulation_Run_Data, * Simulation_Run_Data_Ptr;

/* Runtime configurable p12 used by packet_transmission to forward packets.
  Defined in main.c as a global so the program can sweep it without recompiling. */
extern double P12_global;

typedef enum {XMTTING, WAITING} Packet_Status;

typedef struct _packet_ 
{
  double arrive_time;
  double service_time;
  int source_id;
  int destination_id;
  Packet_Status status;
} Packet, * Packet_Ptr;

/*
 * Function prototypes
 */

int
main(int argc, char *argv[]);

/******************************************************************************/

#endif /* main.h */


