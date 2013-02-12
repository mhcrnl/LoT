#include "polar_rings.h"


void add_ring_range(cairo_t* cr, rings* rs, double a1, double a2) {
    rs->radius = rs->radius - (rs->stroke_width + rs->buf_width);   //Update radius to add new ring

    cairo_set_line_width(cr, rs->stroke_width);                     //Set stroke
    cairo_arc_negative(cr, rs->xc, rs->yc, rs->radius, a1, a2);
}

void add_ring_length(cairo_t* cr, rings* rs, double len) {
    add_ring_range(cr, rs, len + TOP, TOP);
}

void add_ring_length_rand(cairo_t* cr, rings* rs, double len) {
    double rnum = (((double)rand() / (double)RAND_MAX) - 0.5) / 2 + TOP;
    add_ring_range(cr, rs, len + rnum, rnum);
}

void draw_ring(cairo_t* cr, rings* rs, double len) {
    double r, rd, g, gd, b, bd;

    r = rs->cur_color.r;
    g = rs->cur_color.g;
    b = rs->cur_color.b;

    rd = rs->color_diff.r;
    gd = rs->color_diff.g;
    bd = rs->color_diff.b;

    cairo_set_source_rgb(cr, r, g, b);
    add_ring_length(cr, rs, len);
    cairo_stroke(cr);

    //Update colors
    rs->cur_color.r += rd;
    rs->cur_color.g += gd;
    rs->cur_color.b += bd;
}

void draw_ring_rand(cairo_t* cr, rings* rs, double len) {
    double r, rd, g, gd, b, bd;

    r = rs->cur_color.r;
    g = rs->cur_color.g;
    b = rs->cur_color.b;

    rd = rs->color_diff.r;
    gd = rs->color_diff.g;
    bd = rs->color_diff.b;

    cairo_set_source_rgb(cr, r, g, b);
    add_ring_length_rand(cr, rs, len);
    cairo_stroke(cr);

    //Update colors
    rs->cur_color.r += rd;
    rs->cur_color.g += gd;
    rs->cur_color.b += bd;
}

void draw_rings(cairo_t* cr, rings* rs, double* set, size_t len) {
    for ( size_t i = 0; i < len; draw_ring(cr, rs, set[i++]));
}

void draw_rings_rand(cairo_t* cr, rings* rs, double* set, size_t len) {
    for ( size_t i = 0; i < len; draw_ring_rand(cr, rs, set[i++]));
}
