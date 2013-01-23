#include <stdio.h>
#include <unistd.h>

#include <sysinfo.h>

extern sysstat sys;

int main(int argc, char* argv[]) {
    int i;
    init_sysstat();

    printf("\nBattery Stats\n");
    printf("Dir Present: %d\n", sys.battery->_bat_dir_present);
    printf("Bat Present: %d\n", sys.battery->present);
    printf("Energy Full: %d\n", sys.battery->capacity);
    printf("Energy Full Design: %d\n", sys.battery->capacity_design);
    printf("Energy Now: %d\n", sys.battery->remaining);
    printf("Status: %d\n", sys.battery->status);
    printf("Discharge Rate: %d\n", sys.battery->rate);

    printf("\nMem Info\n");
    printf("Total: %d\n", sys.memory->total);
    printf("Free: %d\n", sys.memory->free);
    printf("Buffers: %d\n", sys.memory->buffers);
    printf("Cached: %d\n", sys.memory->cached);
    printf("SwapTotal: %d\n", sys.memory->swap_total);
    printf("SwapFree: %d\n", sys.memory->swap_free);

    printf("\nGathering data...\n");
    sleep(1);
    update_proc();
    printf("\nProc\n");
    printf("cpufreq_stats module: %d\n", sys.cpu->_cpufreq_mod_loaded);
    printf("Max Freq: %d\n", sys.cpu->freq_max);
    printf("Cur Freq: %d\n", sys.cpu->freq);
    printf("/proc/stat cpu line\n");
    for (i = 0; i < 10; printf("%d\n", sys.cpu->processes[i++]));
    printf("Total proc: %d\n", sys.cpu->proc_total);
    printf("Idle proc: %d\n", sys.cpu->proc_idle);
    printf("Usage: %lf\n", sys.cpu->usage);

    return 0;
}
