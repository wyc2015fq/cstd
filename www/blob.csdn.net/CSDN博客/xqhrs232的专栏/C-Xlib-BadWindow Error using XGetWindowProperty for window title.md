# C - Xlib - BadWindow Error using XGetWindowProperty for window title - xqhrs232的专栏 - CSDN博客
2017年11月05日 23:02:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：225
原文地址::[https://stackoverflow.com/questions/25349163/c-xlib-badwindow-error-using-xgetwindowproperty-for-window-title](https://stackoverflow.com/questions/25349163/c-xlib-badwindow-error-using-xgetwindowproperty-for-window-title)
相关文章
1、[How
 to identify top-level X11 windows using xlib?](https://stackoverflow.com/questions/1201179/how-to-identify-top-level-x11-windows-using-xlib)----[https://stackoverflow.com/questions/1201179/how-to-identify-top-level-x11-windows-using-xlib](https://stackoverflow.com/questions/1201179/how-to-identify-top-level-x11-windows-using-xlib)
2、[Get
 UTF-8 input with X11 Display](https://stackoverflow.com/questions/18246848/get-utf-8-input-with-x11-display)----[https://stackoverflow.com/questions/18246848/get-utf-8-input-with-x11-display](https://stackoverflow.com/questions/18246848/get-utf-8-input-with-x11-display)

I want to get a list of all open windows' titles using Xlib in C. I am running Ubuntu 12.04. I am using the following code to accomplish this:
```
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
Window *list(Display *disp, unsigned long *len)
{
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False), type;
    int form;
    unsigned long remain;
    unsigned char *list;
    XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False, XA_WINDOW,
                            &type, &form, &len, &remain, &list);
    return (Window *)list;
}
char *name(Display *disp, Window window)
{
    Atom prop = XInternAtom(disp, "WM_NAME", False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;
    XGetWindowProperty(disp, window, prop, 0, 1024, False, AnyPropertyType,
                            &type, &form, &len, &remain, &list);
    return (char*)list;
}
int main(int argc, char *argv[])
{
    Display *disp;
    Window *wlist;
    unsigned long len;
    char *wname;
    disp = XOpenDisplay(NULL);
    wlist = (Window*)list(disp, &len);
    int i;
    for(i = 0; i < (int)len; i++){
            if(wlist[i] != 0){
                    wname = name(disp, wlist[i]);
                    printf("%d: %s\n", i, wname);
                    free(wname);
            }
    }
return 0;
}
```
Now the problem that I'm having is that this goes through most windows and then gives me a BadWindow error:
```
0: DNDCollectionWindow
1: launcher 
2: Desktop
3: panel
4: Dash
5: Hud
6: Switcher
7: Update Manager
8: Terminal
9: Ask a Question - Stack Overflow - Mozilla Firefox
X Error of failed request:  BadWindow (invalid Window parameter)
  Major opcode of failed request:  20 (X_GetProperty)
  Resource id in failed request:  0x41
  Serial number of failed request:  22
  Current serial number in output stream:  22
```
So I'm wondering if anyone knows what is causing this/how to fix it?
As far as I can tell the list function is returning some windows that I can't retrieve the name of, but I'm not sure.
Thanks in advance!
