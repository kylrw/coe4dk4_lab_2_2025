
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

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* Packet transmission is finished. Take the packet off the link. */
  this_packet = (Packet_Ptr) server_get(link);

  /* Determine which link just finished by comparing pointers in data->links. */
  int link_index = -1;
  for (int i = 0; i < 3; i++) {
    if (data->links[i] == (Server_Ptr) link) { link_index = i; break; }
  }

  if (link_index == 0) {
    /* Finished transmission on Link1. Decide destination: 1 (Link2) or 2 (Link3). */
  double r = uniform_generator();
  extern double P12_global; /* declared in main.h */
  int dest = (r <= P12_global) ? 1 : 2; /* dest is switch index */
    this_packet->destination_id = dest;

    /* Place packet into destination switch buffer or start transmission immediately if link free. */
    if (server_state(data->links[dest]) == BUSY) {
      fifoqueue_put(data->buffers[dest], (void*) this_packet);
    } else {
      start_transmission_on_link(simulation_run, this_packet, data->links[dest]);
    }

    /* After removing packet from Link1, check if there is another packet waiting at Switch1. */
    if (fifoqueue_size(data->buffers[0]) > 0) {
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffers[0]);
      start_transmission_on_link(simulation_run, next_packet, data->links[0]);
    }
  }
  else if (link_index == 1 || link_index == 2) {
    /* Final transmission finished on Link2 or Link3: collect stats. */
    int origin = this_packet->source_id;
    if (origin >= 0 && origin < 3) {
      data->number_of_packets_processed_per_switch[origin]++;
      data->accumulated_delay_per_switch[origin] += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
    }

    /* Also update legacy/global counters. */
    data->number_of_packets_processed++;
    data->accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;

    /* Output activity blip every so often. */
    output_progress_msg_to_screen(simulation_run);

    /* Free packet memory. */
    xfree((void *) this_packet);

    /* If there are waiting packets at this switch, start next transmission. */
    if (fifoqueue_size(data->buffers[link_index]) > 0) {
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffers[link_index]);
      start_transmission_on_link(simulation_run, next_packet, data->links[link_index]);
    }
  }
  else {
    /* Unknown link: fallback to legacy behavior for single-link setups. */
    data->number_of_packets_processed++;
    data->accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
    output_progress_msg_to_screen(simulation_run);
    xfree((void *) this_packet);
    if(fifoqueue_size(data->buffer) > 0) {
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
      start_transmission_on_link(simulation_run, next_packet, link);
    }
  }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Determine service time based on which link this is (Link1, Link2 or Link3). */
  Simulation_Run_Data_Ptr data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  double service_time = PACKET_XMT_TIME; /* fallback */
  if (data != NULL) {
    for (int i = 0; i < 3; i++) {
      if (data->links[i] == link) {
        if (i == 0) service_time = (double) PACKET_LENGTH / LINK1_BIT_RATE;
        else if (i == 1) service_time = (double) PACKET_LENGTH / LINK2_BIT_RATE;
        else if (i == 2) service_time = (double) PACKET_LENGTH / LINK3_BIT_RATE;
        break;
      }
    }
  }
  this_packet->service_time = service_time;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(void)
{
  return ((double) PACKET_XMT_TIME);
}


