#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "battery.h"
#include "clock.h"
#include "cpu.h"
#include "polar_rings.h"
#include "sysinfo.h"

extern sysstat sys;

int size_smallest(GtkWidget* win) { //Returns the size of the window (px) in the smallest orientation
    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    return width < height ? width : height;
}

static void do_draw(cairo_t*, GtkWidget*);

struct {
  gboolean timer;
  gdouble alpha;
  gdouble size;
} glob;

static gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
    cr = gdk_cairo_create(gtk_widget_get_window(widget));
    update_sysstat();
    do_draw(cr, widget);
    cairo_destroy(cr);

    return FALSE;
}


static gboolean time_handler(GtkWidget* widget) {
  if (!glob.timer) return FALSE;

  gtk_widget_queue_draw(widget);

  return TRUE;
}

static void do_draw(cairo_t* cr, GtkWidget* widget) {
    GtkWidget* win = gtk_widget_get_toplevel(widget);

    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    int smallest = size_smallest(win);

    //Clock Look
    rings rs_clock;
    color col_clock = { 1.0, 1.0, 0 };
    color cold_clock = { 0, -0.2, 0 };

    rs_clock.xc             = width/2;
    rs_clock.yc             = height/2;
    rs_clock.radius         = ((0.75 / 2.0) * smallest);
    rs_clock.stroke_width   = 0.1 * rs_clock.radius;
    rs_clock.buf_width      = 0.02 * rs_clock.radius;
    rs_clock.cur_color      = col_clock;
    rs_clock.color_diff     = cold_clock;

    //Battery Look
    rings rs_bat;
    color col_bat = { 1 - bat_remaining(), 1.0 * bat_remaining(), 0 };
    color cold_bat = { 0, -0.5, 0 };

    rs_bat.radius           = ((0.3 / 2.0) * smallest);
    rs_bat.stroke_width     = 0.1 * rs_bat.radius;
    rs_bat.buf_width        = 0.02 * rs_bat.radius;
    rs_bat.xc               = rs_bat.radius + rs_bat.buf_width;
    rs_bat.yc               = rs_bat.xc;
    rs_bat.cur_color        = col_bat;
    rs_bat.color_diff       = cold_bat;

    //CPU Look
    rings rs_cpu;
    color col_cpu = { 0, 0.61, 0.55 };
    color cold_cpu = { 0.1, -0.3, -0.3 };

    rs_cpu.radius           = ((0.3 / 2.0) * smallest);
    rs_cpu.stroke_width     = 0.1 * rs_cpu.radius;
    rs_cpu.buf_width        = 0.02 * rs_cpu.radius;
    rs_cpu.xc               = width - (rs_cpu.radius + rs_cpu.buf_width);
    rs_cpu.yc               = rs_cpu.radius + rs_cpu.buf_width;
    rs_cpu.cur_color        = col_cpu;
    rs_cpu.color_diff       = cold_cpu;

//    cairo_translate(cr, width/2, height/2);     //Move draw point to center

    //Draw Clock background
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_arc(cr, rs_clock.xc, rs_clock.yc, rs_clock.radius, 0, 2 * G_PI);
    cairo_fill(cr);

    //Draw Clock
    draw_clock(cr, &rs_clock);

    //Draw Battery Background
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_arc(cr, rs_bat.xc, rs_bat.yc, rs_bat.radius, 0, 2 * G_PI);
    cairo_fill(cr);

    //Draw Battery
    draw_battery(cr, &rs_bat);

    //Draw CPU Background
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_arc(cr, rs_cpu.xc, rs_cpu.yc, rs_cpu.radius, 0, 2 * G_PI);
    cairo_fill(cr);

    //Draw CPU
    draw_cpu(cr, &rs_cpu);

}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* darea;

    init_sysstat();
    sleep(1);
    update_sysstat();

    srand(time(NULL));

    glob.timer = TRUE;
    glob.alpha = 1.0;
    glob.size = 1.0;

    GdkColor bgColor;
    bgColor.red     = 0;
    bgColor.green   = 0;
    bgColor.blue    = 0;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    g_signal_connect(G_OBJECT(darea), "draw",
            G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_title(GTK_WINDOW(window), "LOT");
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &bgColor);

    g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
