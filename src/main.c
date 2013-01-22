#include <stdio.h>

#include <sysinfo.h>

extern system sys;

int main(int argc, char* argv[]) {
    init_system();

    printf("%d", sys.battery->capacity);

    return 0;
}
