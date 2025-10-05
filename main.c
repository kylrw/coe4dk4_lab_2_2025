
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 *
 * Simulation_Run of A Single Server Queueing System (with Experiment 5 sweep)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/* runtime P12 so packet_transmission can use it without recompiling */
double P12_global = P12;

int
main(int argc, char *argv[])
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  int j = 0;

  /* Parse optional sweep argument: --sweep <steps>
     If provided, sweep p12 from 0..1 in <steps> steps and print one summary line per step. */
  int sweep_steps = 0;
  if (argc >= 3 && strcmp(argv[1], "--sweep") == 0) {
    sweep_steps = atoi(argv[2]);
    if (sweep_steps < 1) sweep_steps = 1;
  }

  if (sweep_steps > 0) {
    printf("# Sweeping p12 from 0.0 to 1.0 in %d steps\n", sweep_steps);
    printf("# Columns: p12 mean_ms_S1 mean_ms_S2 mean_ms_S3 proc_S1 proc_S2 proc_S3\n");

    for (int step = 0; step <= sweep_steps; step++) {
      P12_global = (double) step / (double) sweep_steps;

      long total_processed[3] = {0,0,0};
      double total_accum_delay[3] = {0.0,0.0,0.0};

      j = 0;
      while ((random_seed = RANDOM_SEEDS[j++]) != 0) {
        /* Create and initialize a simulation run */
        simulation_run = simulation_run_new();
        simulation_run_attach_data(simulation_run, (void *) &data);

        data.blip_counter = 0;
        data.arrival_count = 0;
        data.number_of_packets_processed = 0;
        data.accumulated_delay = 0.0;
        data.random_seed = random_seed;

        data.buffer = fifoqueue_new();
        data.link = server_new();
        for (int s = 0; s < 3; s++) {
          data.buffers[s] = fifoqueue_new();
          data.links[s] = server_new();
          data.arrival_count_per_switch[s] = 0;
          data.number_of_packets_processed_per_switch[s] = 0;
          data.accumulated_delay_per_switch[s] = 0.0;
        }

        random_generator_initialize(random_seed);

        /* schedule initial arrivals for each switch */
        for (int s = 0; s < 3; s++) {
          int *src = (int *) xmalloc(sizeof(int));
          *src = s;
          schedule_packet_arrival_event(simulation_run, simulation_run_get_time(simulation_run), (void *) src);
        }

        while (data.number_of_packets_processed < RUNLENGTH) {
          simulation_run_execute_event(simulation_run);
        }

        /* accumulate per-seed results */
        for (int s = 0; s < 3; s++) {
          total_processed[s] += data.number_of_packets_processed_per_switch[s];
          total_accum_delay[s] += data.accumulated_delay_per_switch[s];
        }

        /* print per-seed outputs if desired (kept minimal) */
        output_results(simulation_run);

        cleanup_memory(simulation_run);
      }

      /* compute means across seeds */
      double mean_ms[3] = {0.0,0.0,0.0};
      for (int s = 0; s < 3; s++) {
        if (total_processed[s] > 0) mean_ms[s] = 1e3 * total_accum_delay[s] / (double) total_processed[s];
      }

      /* print one concise line for easy grepping */
      printf("%.6f %.6f %.6f %.6f %ld %ld %ld\n",
             P12_global, mean_ms[0], mean_ms[1], mean_ms[2],
             total_processed[0], total_processed[1], total_processed[2]);
    }

    return 0;
  }

  /* Legacy single-run behaviour: loop over RANDOM_SEEDS and run the sim for each */
  j = 0;
  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {
    simulation_run = simulation_run_new(); /* Create a new simulation run. */

    simulation_run_attach_data(simulation_run, (void *) &data);

    data.blip_counter = 0;
    data.arrival_count = 0;
    data.number_of_packets_processed = 0;
    data.accumulated_delay = 0.0;
    data.random_seed = random_seed;
 
    data.buffer = fifoqueue_new();
    data.link   = server_new();

    for (int s = 0; s < 3; s++) {
      data.buffers[s] = fifoqueue_new();
      data.links[s] = server_new();
      data.arrival_count_per_switch[s] = 0;
      data.number_of_packets_processed_per_switch[s] = 0;
      data.accumulated_delay_per_switch[s] = 0.0;
    }

    random_generator_initialize(random_seed);

    for (int s = 0; s < 3; s++) {
      int *src = (int *) xmalloc(sizeof(int));
      *src = s; /* source id: 0 -> Switch1, 1 -> Switch2, 2 -> Switch3 */
      schedule_packet_arrival_event(simulation_run, simulation_run_get_time(simulation_run), (void *) src);
    }

    while(data.number_of_packets_processed < RUNLENGTH) {
      simulation_run_execute_event(simulation_run);
    }

    output_results(simulation_run);
    cleanup_memory(simulation_run);
  }

  return 0;
}












