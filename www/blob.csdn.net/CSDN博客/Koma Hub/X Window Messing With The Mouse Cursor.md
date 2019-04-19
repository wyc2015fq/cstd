# X Window Messing With The Mouse Cursor - Koma Hub - CSDN博客
2019年01月14日 21:17:07[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：25
个人分类：[x11																[xlib](https://blog.csdn.net/Rong_Toa/article/category/8512049)](https://blog.csdn.net/Rong_Toa/article/category/8511788)
***xlib-programming-******Basic Graphics Programming With The Xlib Library***
**目录**
[Creating And Destroying A Mouse Cursor](#Creating%20And%20Destroying%20A%20Mouse%20Cursor)
[Setting A Window's Mouse Cursor](#Setting%20A%20Window' rel=)
Often we see programs that modify the shape of the mouse pointer (also called the X pointer) when in certain states. For example, a busy application would often display a sand clock over its main window, to give the user a visual hint that they should wait. Without such a visual hint, the user might think that the application got stuck. Lets see how we can change the mouse cursor for our windows.
# Creating And Destroying A Mouse Cursor
There are two methods for creating cursors. One of them is by using a set of pre-defined cursors, that are supplied by Xlib. The other is by using user-supplied bitmaps.
In the first method, we use a special font named "cursor", and the function XCreateFontCursor(). This function accepts a shape identifier, and returns a handle to the generated cursor. The list of allowed font identifiers is found in the include file <X11/cursorfont.h>.
Here are a few such cursors:
XC_arrow
The normal pointing-arrow cursor displayed by the server.
XC_pencil
A cursor shaped as a pencil.
XC_watch
A sand watch.
And creating a cursor using these symbols is very easy:
```cpp
#include <X11/cursorfont.h> /* defines XC_watch, etc. */
/* this variable will hold the handle of the newly created cursor. */
Cursor watch_cursor;
/* create a sand watch cursor. */
watch_cursor = XCreateFontCursor(display, XC_watch);
```
The other methods of creating a cursor is by using a pair of pixmaps with depth of one (that is, two color pixmaps). One pixmap defines the shape of the cursor, while the other works as a mask, specifying which pixels of the cursor will be actually drawn. The rest of the pixels will be transparent.
Creating such a cursor is done using the XCreatePixmapCursor() function. As an example, we will create a cursor using the "icon.bmp" bitmap. We will assume that it was already loaded into memory, and turned into a pixmap, and its handle is stored in the 'bitmap' variable. We will want it to be fully transparent. That is, only the parts of it that are black will be drawn, while the white parts will be transparent. To achieve this effect, we will use the icon both as the cursor pixmap and as the mask pixmap. Try to figure out why...
```cpp
/* this variable will hold the handle of the newly created cursor. */
Cursor icon_cursor;
/* first, we need to define foreground and background colors for the cursor. */
XColor cursor_fg, cursor_bg;
/* access the default color map of our screen. */
Colormap screen_colormap = DefaultColormap(display, DefaultScreen(display));
/* allocate black and while colors. */
Status rc = XAllocNamedColor(display,
                            screen_colormap,
                            "black",
                            &cursor_fg,
                            &cursor_fg);
if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - cannot allocate 'black' ??!!??\n");
    exit(1);
}
Status rc = XAllocNamedColor(display,
                            screen_colormap,
                            "white",
                            &cursor_bg,
                            &cursor_bg);
if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - cannot allocate 'white' ??!!??\n");
    exit(1);
}
/* finally, generate the cursor. make the 'hot spot' be close to the */
/* top-left corner of the cursor - location (x=5, y=4). */
icon_cursor = XCreatePixmapCursor(display, bitmap, bitmap,
                                    &cursor_fg, &cursor_bg,
                                    5, 4);
```
One thing to be explained is the 'hot spot' parameters. When we define a cursor, we need to define which pixel of the cursor is the pointer delivered to the user in the various mouse events. Usually, we will choose a location of the cursor that visually looks like a hot spot. For example, in an arrow cursor, the tip of the arrow will be defined as the hot spot.
Finally, when we are done with a cursor and no longer need it, we can release it using the XFreeCursor() function:
```cpp
XFreeCursor(display, icon_cursor);
```
# Setting A Window's Mouse Cursor
After we have created a cursor, we can tell the X server to attach this cursor to any of our windows. This is done using the XDefineCursor(), and causes the X server to change the mouse pointer to the shape of that cursor, each time the mouse pointer moves into and across that window. We can later detach this cursor from our window using the XUndefineCursor() function. This will cause the default cursor to be shown when the mouse enter that windows.
```cpp
/* attach the icon cursor to our window. */
XDefineCursor(display, win, icon_cursor);
/* detach the icon cursor from our window. */
XUndefineCursor(display, win);
```
As an example, look at our cursor.c program, and see how mouse cursors are set, changed and removed. Run the program, place the mouse pointer over the created window, and watch.
