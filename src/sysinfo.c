#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <sysinfo.h>

static battery _bat;
static proc _cpu;
static memory _mem;
static system sys = {
    .battery    = &_bat,
    .cpu        = &_cpu,
    .memory     = &_mem,
};

FILE *fr; //File Pointer

void init_system() {

    _init_battery();
    _init_proc();
    _init_memory();

    return;
}

//Battery
void _init_battery() {
    struct stat st;

    if ( !stat("/sys/class/power_supply/BAT0",&st) == 0 ) {
        _bat.bat_dir_present = false;
        return;
    }

    //First update
    update_battery();

    return;
}

void update_battery() {
    int tmp;

    //Check if battery is present
    fr = fopen("/sys/class/power_supply/BAT0/present", "rt");
    fscanf(fr, "%d", &tmp);
    fclose(fr);
    //We rely on this being set for all other settings, so return early
    //  if it is not set.
    if (tmp == 0) {
        _bat.present = false;
        return;
    }

    //Get total capacity NOTE: This is not the design capacity.
    fr = fopen("/sys/class/power_supply/BAT0/energy_full", "rt");
    fscanf(fr, "%d", &_bat.capacity);
    fclose(fr);

    //Get design capacity
    fr = fopen("/sys/class/power_supply/BAT0/energy_full", "rt");
    fscanf(fr, "%d", &_bat.capacity_design);
    fclose(fr);

    //Get remaining power
    fr = fopen("/sys/class/power_supply/BAT0/energy_now", "rt");
    fscanf(fr, "%d", &_bat.remaining);
    fclose(fr);

    //Get status of battery
    fr = fopen("/sys/class/power_supply/BAT0/status", "rt");
    fscanf(fr, "%d", &_bat.status);
    fclose(fr);

    //Get (dis)charge rate
    fr = fopen("/sys/class/power_supply/BAT0/power_now", "rt");
    fscanf(fr, "%d", &_bat.status);
    fclose(fr);

    return;
}

//Processor
void _init_proc() {}
void update_proc() {}


//Memory
void update_memory() {}
void _init_memory() {}
