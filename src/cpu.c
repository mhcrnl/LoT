#include "cpu.h"

extern sysstat sys;

double cpu_freq_percent() {
    int current = sys.cpu->freq;
    int max = sys.cpu->freq_max;

    return (double)current / (double)max;
}

void draw_cpu(cairo_t* cr, rings* rs) {
    double segments[1];

    segments[0] = sys.cpu->usage * 2 * G_PI;

    draw_rings(cr, rs, segments, 2);
}
