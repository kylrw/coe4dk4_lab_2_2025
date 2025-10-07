
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
#include "simparameters.h"
#include "main.h"
#include "output.h"

/* External declaration for current data arrival rate */
extern double current_data_arrival_rate;

/******************************************************************************/

/*
 * This function outputs a progress message to the screen to indicate this are
 * working.
 */

void
output_progress_msg_to_screen(Simulation_Run_Ptr simulation_run)
{
  double percentage_done;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  data->blip_counter++;

  if((data->blip_counter >= BLIPRATE)
     ||
     (data->number_of_packets_processed >= RUNLENGTH)) {

    data->blip_counter = 0;

    percentage_done =
      100 * (double) data->number_of_packets_processed/RUNLENGTH;

    // printf("%3.0f%% ", percentage_done);

    // printf("Successfully Xmtted Pkts  = %ld (Arrived Pkts = %ld) \r", 
	  //  data->number_of_packets_processed, data->arrival_count);

    fflush(stdout);
  }

}

/*
 * When a simulation_run run is completed, this function outputs various
 * collected statistics on the screen.
 */

void
output_results(Simulation_Run_Ptr simulation_run)
{
  double xmtted_fraction;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  printf("\n");
  printf("Random Seed = %d \n", data->random_seed);
  printf("Total packet arrival count = %ld \n", data->arrival_count);
  printf("  Voice packet arrivals = %ld \n", data->voice_arrival_count);
  printf("  Data packet arrivals = %ld \n", data->data_arrival_count);

  xmtted_fraction = (double) data->number_of_packets_processed /
    data->arrival_count;

  printf("Total transmitted packet count = %ld (Service Fraction = %.5f)\n",
	 data->number_of_packets_processed, xmtted_fraction);
  printf("  Voice packets transmitted = %ld \n", data->voice_packets_processed);
  printf("  Data packets transmitted = %ld \n", data->data_packets_processed);

  printf("Data arrival rate = %.3f packets/second \n", current_data_arrival_rate);
  printf("Voice arrival rate = %.3f packets/second \n", 1.0/VOICE_INTER_ARRIVAL_TIME);

  printf("Overall Mean Delay (msec) = %.2f \n",
	 1e3*data->accumulated_delay/data->number_of_packets_processed);

  /* Output separate delays for voice and data if packets were processed */
  if(data->voice_packets_processed > 0) {
    printf("Voice Mean Delay (msec) = %.2f \n",
	   1e3*data->voice_accumulated_delay/data->voice_packets_processed);
  }
  
  if(data->data_packets_processed > 0) {
    printf("Data Mean Delay (msec) = %.2f \n",
	   1e3*data->data_accumulated_delay/data->data_packets_processed);
  }

  printf("\n");

  printf("Seed,Data_Arrival_Rate,Voice_Arrival_Rate,Mean_Delay,Voice_Mean_Delay,Data_Mean_Delay\n");
  printf("%d,%.3f,%.3f,%.2f,%.2f,%.2f",  
    data->random_seed, 
    current_data_arrival_rate, 
    1.0/VOICE_INTER_ARRIVAL_TIME,
    1e3*data->accumulated_delay/data->number_of_packets_processed,
    1e3*data->voice_accumulated_delay/data->voice_packets_processed,
    1e3*data->data_accumulated_delay/data->data_packets_processed);



  printf("\n\n");
}



