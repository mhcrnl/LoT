#include "sysinfo.h"

static battery _bat;
static proc _cpu;
static memory _mem;
sysstat sys = {
    .battery    = &_bat,
    .cpu        = &_cpu,
    .memory     = &_mem,
};

FILE *fr; //File Pointer

void init_sysstat() {

    _init_battery();
    _init_proc();
    _init_memory();

    return;
}

void update_sysstat() {
    update_battery();
    update_proc();
    update_memory();
}

//Battery
void _init_battery() {

    _bat._bat_dir_present = 0; //Assume absent

    struct stat st;
    stat("/sys/class/power_supply/BAT0", &st);
    if (S_ISDIR(st.st_mode)) {
        _bat._bat_dir_present = 1;
    }

    //First update
    update_battery();

    return;
}

void update_battery() {
    char status[32];
    if ( !_bat._bat_dir_present ) return;

    //Check if battery is present
    fr = fopen("/sys/class/power_supply/BAT0/present", "rt");
    fscanf(fr, "%d", &_bat.present);
    fclose(fr);
    //We rely on this being set for all other settings, so return early
    if (!_bat.present) return;

    //Get total capacity NOTE: This is not the design capacity.
    fr = fopen("/sys/class/power_supply/BAT0/energy_full", "rt");
    fscanf(fr, "%d", &_bat.capacity);
    fclose(fr);

    //Get design capacity
    fr = fopen("/sys/class/power_supply/BAT0/energy_full_design", "rt");
    fscanf(fr, "%d", &_bat.capacity_design);
    fclose(fr);

    //Get remaining power
    fr = fopen("/sys/class/power_supply/BAT0/energy_now", "rt");
    fscanf(fr, "%d", &_bat.remaining);
    fclose(fr);

    //Get status of battery
    fr = fopen("/sys/class/power_supply/BAT0/status", "rt");
    fscanf(fr, "%s", status);
    fclose(fr);

    if ( strcmp(status, "Charging" ) == 0 ) _bat.status = 1;
    else if ( strcmp(status, "Discharging") == 0 ) _bat.status = 2;
    else _bat.status = 0;

    //Get (dis)charge rate
    fr = fopen("/sys/class/power_supply/BAT0/power_now", "rt");
    fscanf(fr, "%d", &_bat.rate);
    fclose(fr);

    return;
}

//Processor
void _init_proc() {
    int i;

    //Assume the module is not loaded
    _cpu._cpufreq_mod_loaded = 0;
    struct stat st;
    //Check if cpufreq_stats is loaded
    stat("/sys/module/cpufreq_stats", &st);
    if (S_ISDIR(st.st_mode))
        _cpu._cpufreq_mod_loaded = 1;

    //Initialize processes array to be 0's
    for (i = 0; i < 10; _cpu.processes[i++] = 0);

    update_proc();

    return;
}
void update_proc() {
    int i;
    int total = 0;
    int idle = 0;

    //We can only get frequency stats if cpufreq_stats is modprobed
    if ( _cpu._cpufreq_mod_loaded ) {
        //Max frequency
        fr = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "rt");
        fscanf(fr, "%d", &_cpu.freq_max);
        fclose(fr);

        //Current frequency
        fr = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "rt");
        fscanf(fr, "%d", &_cpu.freq);
        fclose(fr);
    }

    //Get stuff from /proc/stat
    fr = fopen("/proc/stat", "rt");
    fscanf(fr, "%*s"); //Eats the first column containing cpu
    //There are only 10 columns (max) for proc/stat cpu info
    for ( i = 0; !feof(fr) && !ferror(fr) && i < 10; i++) {
        fscanf(fr, "%d", &_cpu.processes[i]);
        total += _cpu.processes[i];
        if ( i == 3 || i == 4 )
            idle += _cpu.processes[i];
    }
    fclose(fr);

    i = total - _cpu.proc_total; //Use i as a temporary
    _cpu.usage = (double)( i - (idle - _cpu.proc_idle)) / (double)(i);

    //Store total and idle into proc_total and proc_idle for next update
    _cpu.proc_total = total;
    _cpu.proc_idle = idle;

    return;
}

//Memory
void _init_memory() {
    //There should not be anything holding us back from proc/meminfo
    update_memory();
    return;

}
void update_memory() {
    int i;
    //Open up proc/meminfo, it is the only file needed for all mem stats
    fr = fopen("/proc/meminfo", "rt");

    //MemTotal
    fscanf(fr, "%*s %d %*s", &_mem.total);

    //MemFree
    fscanf(fr, "%*s %d %*s", &_mem.free);

    //Buffers
    fscanf(fr, "%*s %d %*s", &_mem.buffers);

    //Cached
    fscanf(fr, "%*s %d %*s", &_mem.cached);

    //Skip 9 lines to get to swap stats
    for (i = 0; i < 9; fscanf(fr, "%*s %*d %*s"), i++);

    //SwapTotal
    fscanf(fr, "%*s %d %*s", &_mem.swap_total);

    //SwapFree
    fscanf(fr, "%*s %d %*s", &_mem.swap_free);

    //Close /proc/meminfo
    fclose(fr);

    return;
}
