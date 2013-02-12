#include "clock.h"


double time_circle_month() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_mon + 1) / 12.0;
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
            return (ts->tm_mday) / 31.0;
    else if (m == 2) return (ts->tm_mday) / 28.0;
    else return (ts->tm_mday) / 30.0;
}

double time_circle_hours() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_hour) / 24.0;
}

double time_circle_minutes() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_min) / 60.0;
}

double time_circle_seconds() {
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    return (ts->tm_sec) / 60.0;
}

void draw_clock(cairo_t* cr, rings* rs) {
    double segments[5];

    segments[0] = time_circle_seconds() * 2 * G_PI;
    segments[1] = time_circle_minutes() * 2 * G_PI;
    segments[2] = time_circle_hours() * 2 * G_PI;
    segments[3] = time_circle_dom() * 2 * G_PI;
    segments[4] = time_circle_month() * 2 * G_PI;

    draw_rings(cr, rs, segments, 5);
}
