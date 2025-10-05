
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time, void * attachment)
{
  Event event;

  event.description = "Packet Arrival";
  event.function = packet_arrival_event;
  event.attachment = attachment;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;
  int source = 0; /* default source id */

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* If an attachment was provided, it is expected to be a pointer to an int
     containing the source id (0,1,2). Free it after reading. */
  if (ptr != NULL) {
    int *p = (int *) ptr;
    source = *p;
    xfree((void *) p);
  }

  /* Track arrival counts. */
  data->arrival_count++;
  if (source >=0 && source < 3) data->arrival_count_per_switch[source]++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->status = WAITING;
  new_packet->source_id = source;
  new_packet->destination_id = -1;

  /* Put packet into the appropriate switch buffer or start transmission if
     the corresponding link is free. */
  if (server_state(data->links[source]) == BUSY) {
    fifoqueue_put(data->buffers[source], (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->links[source]);
  }

  /* Schedule the next arrival for this same source, using its lambda. */
  double lambda = PACKET_ARRIVAL_RATE; /* fallback */
  if (source == 0) lambda = (double) LAMBDA1;
  else if (source == 1) lambda = (double) LAMBDA2;
  else if (source == 2) lambda = (double) LAMBDA3;

  /* Schedule the next arrival for this same source. Attach the source id so
     the future event knows which stream it belongs to. The attachment is
     freed at the start of packet_arrival_event. */
  {
    int *next_src = (int *) xmalloc(sizeof(int));
    *next_src = source;
    schedule_packet_arrival_event(simulation_run,
            simulation_run_get_time(simulation_run) +
            exponential_generator((double) 1.0/lambda), (void *) next_src);
  }
}



