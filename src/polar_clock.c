#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>

typedef struct rings {
    double      xc;
    double      yc;
    double      radius;
    int         stroke_width;
    int         buf_width;
} rings;

double time_circle_month() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_mon + 1) / (12.0 / 2.0);
}

double time_circle_dom() {
    time_t t = time(NULL);
    int m;
    struct tm *ts = localtime(&t);
    m = ts->tm_mon;

    if (m == 1 ||
        m == 3 ||
        m == 5 ||
        m == 7 ||
        m == 8 ||
        m == 10 ||
        m == 12)
            return (ts->tm_mday) / (31.0 / 2.0);
    else if (m == 2) return (ts->tm_mday) / (28.0 / 2.0);
    else return (ts->tm_mday) / (30.0 / 2.0);
}

double time_circle_hours() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_hour) / (24.0 / 2.0);
}

double time_circle_minutes() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_min) / (60.0 / 2.0);
}

double time_circle_seconds() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_sec) / (60.0 / 2.0);
}

int size_smallest(GtkWidget* win) { //Returns the size of the window (px) in the smallest orientation
    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    return width < height ? width : height;
}

static void add_ring_inside(cairo_t* cr, rings* rs, double a1, double a2) {
    rs->radius = rs->radius - (rs->stroke_width + rs->buf_width);   //Update radius to add new ring

    cairo_set_line_width(cr, rs->stroke_width);                     //Set stroke
    cairo_arc_negative(cr, rs->xc, rs->yc, rs->radius, a1, a2);     //Path out the needed arc
}


/************************************/

static void do_draw(cairo_t*, GtkWidget*);

struct {
  gboolean timer;
  gdouble alpha;
  gdouble size;
} glob;

static gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
    cr = gdk_cairo_create(gtk_widget_get_window(widget));
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
    int smallest = size_smallest(win);
    double top_center = (-0.5 * M_PI);
    rings rs;

    gtk_window_get_size(GTK_WINDOW(win), &width, &height);

    cairo_translate(cr, width/2, height/2);     //Move draw point to center

    rs.xc              = 0;
    rs.yc              = 0;
    rs.radius          = ((0.75 / 2.0) * smallest);
    rs.stroke_width    = 0.1 * rs.radius;
    rs.buf_width       = 0.02 * rs.radius;


    //Draw background
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);    //Black
    cairo_arc(cr, 0, 0, rs.radius, 0, 2 * M_PI);
    cairo_fill(cr);

    //Draw Seconds
    cairo_set_source_rgb(cr, 1.0, 0.8, 0.0);
    add_ring_inside(cr, &rs, (time_circle_seconds() * M_PI) + top_center, top_center);
    cairo_stroke(cr);

    //Draw Minutes
    cairo_set_source_rgb(cr, 1.0, 0.6, 0.0);
    add_ring_inside(cr, &rs, (time_circle_minutes() * M_PI) + top_center, top_center);
    cairo_stroke(cr);

    //Draw Hours
    cairo_set_source_rgb(cr, 1.0, 0.4, 0.0);
    add_ring_inside(cr, &rs, (time_circle_hours() * M_PI) + top_center, top_center);
    cairo_stroke(cr);

    //Draw Day of month (DOM)
    cairo_set_source_rgb(cr, 1.0, 0.2, 0.0);
    add_ring_inside(cr, &rs, (time_circle_dom() * M_PI) + top_center, top_center);
    cairo_stroke(cr);

    //Draw Month
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    add_ring_inside(cr, &rs, (time_circle_month() * M_PI) + top_center, top_center);
    cairo_stroke(cr);

}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* darea;

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

