#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>

#include "polar_rings.h"

//Time functions
double time_circle_month();
double time_circle_dom();
double time_circle_hours();
double time_circle_minutes();
double time_circle_seconds();

//Drawing functions
void draw_clock(cairo_t*, rings*);

#endif
