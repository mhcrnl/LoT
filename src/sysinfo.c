#include <sysinfo.h>

extern system sys;

system* init() {
    battery*    bat = init_battery();
    cpu*        cpu = init_cpu();
    memory*     mem = init_memory();

    //Call first update
    update_battery(bat);
    update_cpu(cpu);
    update_memory(mem);

    sys.battery = bat;
    sys.cpu     = cpu;
    sys.memory  = mem;

    return &sys;
}
