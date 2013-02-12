#ifndef __POLAR_RINGS_H__
#define __POLAR_RINGS_H__

#include <cairo.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define TOP (-0.5 * G_PI)

typedef struct color {
    double r, g, b;
} color;

typedef struct rings {
    double      xc;
    double      yc;
    double      radius;
    int         stroke_width;
    int         buf_width;
    color       cur_color;
    color       color_diff; // Values represent change in each color to apply
} rings;


void add_ring_range(cairo_t*, rings*, double, double);

void add_ring_length(cairo_t*, rings*, double);
void add_ring_length_rand(cairo_t*, rings*, double);

void draw_ring(cairo_t*, rings*, double);
void draw_ring_rand(cairo_t*, rings*, double);

void draw_rings(cairo_t*, rings*, double*, size_t);
void draw_rings_rand(cairo_t*, rings*, double*, size_t);

#endif
