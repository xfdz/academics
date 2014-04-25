#ifndef XWINDOWMANAGEMENT_H
#define	XWINDOWMANAGEMENT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

struct XWinManage
{
    Window win;
    XEvent report;
    Display *display;
    GC gc, gc_yellow, gc_red, gc_grey;
    unsigned int win_width, win_height;
};

void initXlib(int argc, char **argv, XWinManage *xManager);


#endif	/* XWINDOWMANAGEMENT_H */

