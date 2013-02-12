#include "battery.h"

extern sysstat sys;

double bat_remaining() {
    int cap_design = sys.battery->capacity_design;
    int remain = sys.battery->remaining;

    return (double)remain / (double)cap_design;
}

void draw_battery(cairo_t* cr, rings* rs) {
    double segments[1];

    segments[0] = bat_remaining() * 2 * G_PI;

    draw_rings(cr, rs, segments, 1);
}
