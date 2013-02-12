#ifndef __BATTERY_H__
#define __BATTERY_H__

#include <cairo.h>
#include <gtk/gtk.h>

#include "polar_rings.h"
#include "sysinfo.h"

double bat_remaining();

void draw_battery(cairo_t*, rings*);

#endif
