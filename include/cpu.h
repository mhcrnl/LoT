#ifndef __CPU_H__
#define __CPU_H__

#include <cairo.h>
#include <gtk/gtk.h>

#include "polar_rings.h"
#include "sysinfo.h"

double cpu_freq_percent();

void draw_cpu(cairo_t*, rings*);

#endif
