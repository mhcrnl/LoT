#ifndef __SYSINFO_H__
#define __SYSINFO_H__

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/* System Power information */
typedef struct battery {
    //All found in /sys/class/power_supply/BAT0
    int     _bat_dir_present;     // does /sys/class/power_supply/BAT0 exist
    int     present;            // present
    int     capacity;           // energy_full
    int     capacity_design;    // energy_full_design
    int     remaining;          // energy_now
    int     status;             // status: 0 = Full, 1 = Charging,
                                //   2 = Discharging
    int     rate;     // power_now
} battery;

/* System Load information */
typedef struct proc {
    /* /proc/cpuinfo - whatever useful stuff can be gained from there
       and cpu usage calculated from /proc/stat
    */
    int     _cpufreq_mod_loaded; //Requires cpufreq_stats to be modprobed before hand
    int     freq_max;
    int     freq;               // current running frequency
    double  usage;              // usage percent in range 0 to 1
    int     processes[10];      // Contains info from /proc/stat
    int     proc_total;         // Sum of processes array
    int     proc_idle;          // Sum of idle and io blocked procs
    //Look at adding array of usages for each core
} proc;

/* System Memory information */
typedef struct memory {
    // Effectively reading /proc/meminfo
    int     total;
    int     free;
    int     buffers;
    int     cached;
    int     swap_total;         // I never use swap but I'm including it now
    int     swap_free;          //  so I don't have to later
} memory;

/* Tie it all together */
typedef struct sysstat {
    battery*    battery;
    proc*       cpu;
    memory*     memory;
} sysstat;


//Initilization functions
void init_sysstat();
void update_sysstat();

void _init_battery();
void _init_proc();
void _init_memory();

//Update functions
void update_battery();
void update_proc();
void update_memory();

#endif
