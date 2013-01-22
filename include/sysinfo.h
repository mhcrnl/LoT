#include <stdio.h>
#include <stdbool.h>

/* System Power information */
typedef struct battery {
    //All found in /sys/class/power_supply/BAT0
    bool    bat_dir_present;     // does /sys/class/power_supply/BAT0 exist
    bool    present;            // present
    int     capacity;           // energy_full
    int     capacity_design;    // energy_full_design
    int     remaining;          // energy_now
    int     status;             // status: 0 = Full, 1 = Charging,
                                //   2 = Discharging
    int     discharge_rate;     // power_now
} battery;

/* System Load information */
typedef struct proc {
    /* /proc/cpuinfo - whatever useful stuff can be gained from there
       and cpu usage calculated from /proc/stat
    */
    int     freq;               // current running frequency
    int     freq_max;
    double  usage;              // usage percent in range 0 to 1
    struct {
        //Do I need this much info, not really, but why not. :)
        int total;
        int user;
        int niced;
        int system;
        int idle;
        int io;
        int irq;
        int softirq;
        //These are used for guest operating systems (eg. xen)
        int steal;
        //These are virtual cpu under guest
        int guest;
        int guest_nice;
    } pstat;                    // Struct with /proc/stat info
    //Look at adding array of usages for each core
} proc;

/* System Memory information */
typedef struct memory {
    // Effectively reading /proc/meminfo
    int     total;
    int     free;
    int     buffers;
    int     cache;
    int     swap_total;
    int     swap_free;
} memory;

/* Tie it all together */
typedef struct system {
    battery*     battery;
    proc*   cpu;
    memory*      memory;
} system;


//Initilization functions
void init_system();

void _init_battery();
void _init_proc();
void _init_memory();

//Update functions
void update_battery();
void update_proc();
void update_memory();
