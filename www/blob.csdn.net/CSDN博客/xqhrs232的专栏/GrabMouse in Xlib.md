# GrabMouse in Xlib - xqhrs232的专栏 - CSDN博客
2017年11月07日 11:36:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：158
原文地址::[http://blog.csdn.net/lemon6orange/article/details/51119607](http://blog.csdn.net/lemon6orange/article/details/51119607)
需考虑鼠标拖动窗口、屏幕缩放等事件。
在Xlib程序中，所有的事情都是被事件驱动的。
流程：
1.使用事件遮罩给事件型别注册 XSelectInput(display,win,ButtonReleaseMask );
2.接收事件－撰写事件循环
while(1) { 
XNextEvent(display,&an_event); 
     switch (an_event.type){ 
       caseExpose: 
         /* handle this event type...*/ 
        . 
        . 
        break; 
       default: /* unknown event type - ignoreit. */ 
         break; 
     } 
} 
参：
http://ffmpeg.org/doxygen/trunk/x11grab_8c.html#a48e04508b36fdecda802d671a25a0e47
http://stackoverflow.com/questions/14561267/how-to-read-mouse-click-event-from-x-server
//cursor.c
/* ref: http://www.ishiboo.com/~danny/Projects/xwarppointer/ */
#include <stdio.h>
#include <string.h>
//头文件
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//全局变量
Display *display;
Window root;
//初始化
void init()
{
    if ((display = XOpenDisplay(NULL)) == NULL) {
        fprintf(stderr, "Cannot open local X-display.\n");
        return;
    }
    root = DefaultRootWindow(display);
}
//得到坐标
void GetCursorPos(int *x,int *y)
{
    int tmp;unsigned int tmp2;
    Window fromroot, tmpwin;
    XQueryPointer(display, root, &fromroot, &tmpwin, x, y, &tmp, &tmp, &tmp2);
}
//设置坐标
void SetCursorPos(int x,int y)
{
    int tmp;
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);
}
//模拟点击
/* http://www.linuxquestions.org/questions/programming-9/simulating-a-mouse-click-594576/ */
void mouseClick(int button)
{
    Display *display = XOpenDisplay(NULL);
    XEvent event;
    if(display == NULL)
    {
        printf("Errore nell'apertura del Display !!!\n");
        return;
    }
    memset(&event, 0x00, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    event.xbutton.subwindow = event.xbutton.window;
    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) printf("Errore nell'invio dell'evento !!!\n");
    XFlush(display);
    usleep(100000);
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) printf("Errore nell'invio dell'evento !!!\n");
    XFlush(display);
    XCloseDisplay(display);
}
int main()
{
    init();
    int x=-1,y=-1;
    GetCursorPos(&x,&y);
    printf("%d %d\n",x,y);
    SetCursorPos(0,0);
    XCloseDisplay(display);
    mouseClick(Button1);
    return 0;
}
//grabmouse.cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
int main(){
    Display* display;
    int screen_num;
    Screen *screen;
    Window root_win;
    XEvent report;
    XButtonEvent *xb = (XButtonEvent *)&report;
    int i;
    Cursor cursor;
    display = XOpenDisplay(0);
    if (display == NULL){
        perror("Cannot connect to X server");
        exit (-1);
    }
    screen_num = DefaultScreen(display);
    screen = XScreenOfDisplay(display, screen_num);
    root_win = RootWindow(display, XScreenNumberOfScreen(screen));
    //cursor = XCreateFontCursor(display, XC_crosshair);
      cursor = XCreateFontCursor(display, XC_left_ptr);
    i = XGrabPointer(display, root_win, False,
                ButtonReleaseMask | ButtonPressMask|Button1MotionMask, GrabModeSync,
                GrabModeAsync, root_win, cursor, CurrentTime);
    if(i != GrabSuccess){
        perror("Can't grab the mouse");
        exit(-1);
    }
    //XSelectInput(display,DefaultRootWindow(display), ExposureMask | ButtonPressMask);
    for(i = 0; i < 10; i++){
        XAllowEvents(display, SyncPointer, CurrentTime);
        XWindowEvent(display, root_win, ButtonPressMask | ButtonReleaseMask, &report);
        //XNextEvent(display,&report);
        switch(report.type){
            case ButtonPress:
                printf("Press @ (%d, %d)\n", xb->x_root, xb->y_root);
            //printf("Press @ (%d, %d)\n",report.xbutton.x, report.xbutton.y);
            break;
            case ButtonRelease:
            printf("Release @ (%d, %d)\n", xb->x_root, xb->y_root);
                //printf("Release @ (%d, %d)\n", report.xbutton.x, report.xbutton.y);
            break;
        }
    }
    XFlush(display);
    XUngrabServer(display);
    XCloseDisplay( display );
    return 0;
}

//xcursor.c
/**
 *gcc -lX11 -lXext xcursorBusy.c -o xcursorBusy
 *xcursorBusy
 *xcursorBusy resort 2>logfile
 */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>
static const unsigned char xlib_spinning_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x7c, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00,
    0xfc, 0x3b, 0x00, 0x00, 0x7c, 0x38, 0x00, 0x00, 0x6c, 0x54, 0x00, 0x00,
    0xc4, 0xdc, 0x00, 0x00, 0xc0, 0x44, 0x00, 0x00, 0x80, 0x39, 0x00, 0x00,
    0x80, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const unsigned char xlib_spinning_mask_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x1e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
    0xfe, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0xfe, 0x3b, 0x00, 0x00,
    0xfe, 0x7f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00,
    0xee, 0xff, 0x01, 0x00, 0xe4, 0xff, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00,
    0xc0, 0x7f, 0x00, 0x00, 0x80, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
Cursor
cursorCreateSpinning (Display * dpy)
{
    Pixmap cursor;
    Pixmap mask;
    XColor fg, bg;
    Cursor xcursor;
    fg.pixel = 0;
    fg.red = 0;
    fg.green = 0;
    fg.blue = 0;
    fg.flags = 0xf;
    bg.pixel = 0xffffffff;
    bg.red = 0xffff;
    bg.green = 0xffff;
    bg.blue = 0xffff;
    bg.flags = 0xf;
    cursor = XCreatePixmapFromBitmapData (dpy, DefaultRootWindow(dpy),
                                          (char *) xlib_spinning_bits,
                                          32, 32, 0xffffffff, 0x0, 1);
    mask   = XCreatePixmapFromBitmapData (dpy, DefaultRootWindow(dpy),
                                          (char *) xlib_spinning_mask_bits,
                                          32, 32, 0xffffffff, 0x0, 1);
    xcursor = XCreatePixmapCursor (dpy, cursor, mask, &fg, &bg, 2, 2);
    XFreePixmap (dpy, mask);
    XFreePixmap (dpy, cursor);
    return xcursor;
}
int
main(int argc, char **argv)
{
    Display *d;
    Window w;
    Cursor busy_ptr;
    char *dn = getenv ("DISPLAY");
    fprintf (stderr, "got display %s\n", dn);
    d = XOpenDisplay( dn );
    if (d == NULL) {
        fprintf (stderr, "%s display open error!", dn);
    } else {
        fprintf (stderr, "display open normal\n");
    }
    w = RootWindow (d, DefaultScreen (d));
    fprintf (stderr, "%s get root window %d\n", dn, DefaultScreen (d));
#if 1
        // busy cursor/normal cursor
    if (argc > 1) {
        // resort default cursor by WM
        // busy_ptr = 0; or
        // XUndefineCursor (d, w); goto cursor_out;
        busy_ptr = XCreateFontCursor (d, XC_left_ptr); // or X_Cursor
        fprintf (stderr, "resort default cursor\n");
    } else {
        busy_ptr = cursorCreateSpinning (d);
        fprintf (stderr, "create busy cursor\n");
    }
    XDefineCursor(d, w, busy_ptr);
    XFreeCursor(d, busy_ptr);
#else
        // transparent cursor
    Pixmap bm_no;
    Colormap cmap;
    Cursor no_ptr;
    XColor black, dummy;
    static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
    cmap = DefaultColormap(d, DefaultScreen(d));
    XAllocNamedColor(d, cmap, "black", &black, &dummy);
    bm_no = XCreateBitmapFromData(d, w, bm_no_data, 8, 8);
    no_ptr = XCreatePixmapCursor(d, bm_no, bm_no, &black, &black, 0, 0);
    XDefineCursor(d, w, no_ptr);
    XFreeCursor(d, no_ptr);
    if (bm_no != None)
        XFreePixmap(d, bm_no);
    XFreeColors(d, cmap, &black.pixel, 1, 0);
#endif
    XMapWindow( d, w );
    XSync(d, False);
    //XFlush( d );
    fprintf (stderr, "let cursor loop\n");
#if 0
    while( 1 ){
        XEvent xe;
        XNextEvent(d, &xe);
        switch (xe.type) {
            case KeyPress:
            case ButtonPress:
                fprintf (stderr, "cursor exit\n");
                goto cursor_out;
            default:
                fprintf (stderr, "catch event %i\n", xe.type);
        }
    }
#endif
cursor_out:
    fprintf (stderr, "release resource\n");
    XDestroyWindow(d, w);
    XCloseDisplay(d);
    return 0;
}

