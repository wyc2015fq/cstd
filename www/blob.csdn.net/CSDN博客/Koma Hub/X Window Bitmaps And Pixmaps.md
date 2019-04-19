# X Window Bitmaps And Pixmaps - Koma Hub - CSDN博客
2019年01月14日 21:01:07[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：49
个人分类：[x11																[xlib](https://blog.csdn.net/Rong_Toa/article/category/8512049)](https://blog.csdn.net/Rong_Toa/article/category/8511788)
***xlib-programming-******Basic Graphics Programming With The Xlib Library***
**目录**
[What Is An X Bitmap? An X Pixmap?](#What%20Is%20An%20X%20Bitmap%3F%20An%20X%20Pixmap%3F)
[Loading A Bitmap From A File](#Loading%20A%20Bitmap%20From%20A%20File)
[Drawing A Bitmap In A Window](#Drawing%20A%20Bitmap%20In%20A%20Window)
[Creating A Pixmap](#Creating%20A%20Pixmap)
[Drawing A Pixmap In A Window](#Drawing%20A%20Pixmap%20In%20A%20Window)
[Freeing A Pixmap](#Freeing%20A%20Pixmap)
One thing many so-called "Multi-Media" applications need to do, is display images. In the X world, this is done using bitmaps and pixmaps. We have already seen some usage of them when setting an icon for our application. Lets study them further, and see how to draw these images inside a window, along side the simple graphics and text we have seen so far.
One thing to note before delving further, is that Xlib supplies no means of manipulating popular image formats, such as gif, jpeg or tiff. It is left up to the programmer (or to higher level graphics libraries) to translate these image formats into formats that the X server is familiar with - x bitmaps, and x pixmaps.
# What Is An X Bitmap? An X Pixmap?
An X bitmap is a two-color image stored in a format specific to the X window system. When stored in a file, the bitmap data looks like a C source file. It contains variables defining the width and height of the bitmap, an array containing the bit values of the bitmap (the size of the array = width * height), and an optional hot-spot location (will be explained later, when discussing mouse cursors).
A X pixmap is a format used to store images in the memory of an X server. This format can store both black and white images (such as x bitmaps) as well as color images. It is the only image format supported by the X protocol, and any image to be drawn on screen, should be first translated into this format.
In actuality, an X pixmap can be thought of as a window that does not appear on the screen. Many graphics operations that work on windows, will also work on pixmaps - just supply the pixmap ID instead of a window ID. In fact, if you check the manual pages, you will see that all these functions accept a 'Drawable', not a 'Window'. since both windows and pixmaps are drawables, they both can be used to "draw on" using functions such as XDrawArc(), XDrawText(), etc.
# Loading A Bitmap From A File
We have already seen how to load a bitmap from a file to memory, when we demonstrated setting an icon for an application. The method we showed earlier required the inclusion of the bitmap file in our program, using the C pre-processor '#include' directive. We will see here how we can access the file directly.
```cpp
/* this variable will contain the ID of the newly created pixmap. */
Pixmap bitmap;
/* these variables will contain the dimensions of the loaded bitmap. */
unsigned int bitmap_width, bitmap_height;
/* these variables will contain the location of the hot-spot of the */
/* loaded bitmap. */
int hotspot_x, hotspot_y;
/* this variable will contain the ID of the root window of the screen */
/* for which we want the pixmap to be created. */
Window root_win = DefaultRootWindow(display);
/* load the bitmap found in the file "icon.bmp", create a pixmap */
/* containing its data in the server, and put its ID in the 'bitmap' */
/* variable. */
int rc = XReadBitmapFile(display, root_win,
                        "icon.bmp",
                        &bitmap_width, &bitmap_height,
                        &bitmap,
                        &hotspot_x, &hotspot_y);
/* check for failure or success. */
switch (rc) {
    case BitmapOpenFailed:
        fprintf(stderr, "XReadBitmapFile - could not open file 'icon.bmp'.\n");
        break;
    case BitmapFileInvalid:
        fprintf(stderr,
            "XReadBitmapFile - file '%s' doesn't contain a valid bitmap.\n",
            "icon.bmp");
        break;
    case BitmapNoMemory:
        fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
        break;
    case BitmapSuccess:
        /* bitmap loaded successfully - do something with it... */
        break;
}
```
Note that the 'root_win' parameter has nothing to do with the given bitmap - the bitmap is not associated with this window. This window handle is used just to specify the screen that we want the pixmap to be created for. This is important, as the pixmap must support the same number of colors as the screen does, in order to make it useful.
# Drawing A Bitmap In A Window
Once we got a handle to the pixmap generated from a bitmap, we can draw it on some window, using the XCopyPlane() function. This function allows us to specify what drawable (a window, or even another pixmap) to draw the given pixmap onto, and at what location in that drawable.
```cpp
/* draw the previously loaded bitmap on the given window, at location */
/* 'x=100, y=50' in that window. we want to copy the whole bitmap, so */
/* we specify location 'x=0, y=0' of the bitmap to start the copy from, */
/* and the full size of the bitmap, to specify how much of it to copy. */
XCopyPlane(display, bitmap, win, gc,
            0, 0,
            bitmap_width, bitmap_height,
            100, 50,
            1);
```
As you can see, we could also copy a given rectangle of the pixmap, instead of the whole pixmap. Also note the last parameter to the XCopyPlane function (the '1' at the end). This parameter specifies which plane of the source image we want to copy to the target window. For bitmaps, we always copy plane number 1. This will become clearer when we discuss color depths below.
# Creating A Pixmap
Sometimes we want to create an un-initialized pixmap, so we can later draw into it. This is useful for image drawing programs (creating a new empty canvas will cause the creation of a new pixmap on which the drawing can be stored). It is also useful when reading various image formats - we load the image data into memory, cerate a pixmap on the server, and then draw the decoded image data onto that pixmap.
```cpp
/* this variable will store the handle of the newly created pixmap. */
Pixmap pixmap;
/* this variable will contain the ID of the root window of the screen */
/* for which we want the pixmap to be created. */
Window root_win = DefaultRootWindow(display);
/* this variable will contain the color depth of the pixmap to create. */
/* this 'depth' specifies the number of bits used to represent a color */
/* index in the color map. the number of colors is 2 to the power of */
/* this depth. */
int depth = DefaultDepth(display, DefaultScreen(display));
/* create a new pixmap, with a width of 30 pixels, and height of 40 pixels. */
pixmap = XCreatePixmap(display, root_win, 30, 40, depth);
/* just for fun, draw a pixel in the middle of this pixmap. */
XDrawPoint(display, pixmap, gc, 15, 20);
```
# Drawing A Pixmap In A Window
Once we got a handle to pixmap, we can draw it on some window, using the XCopyArea() function. This function allows us to specify what drawable (a window, or even another pixmap) to draw the given pixmap onto, and at what location in that drawable.
```cpp
/* draw the previously loaded bitmap on the given window, at location */
/* 'x=100, y=50' in that window. we want to copy the whole bitmap, so */
/* we specify location 'x=0, y=0' of the bitmap to start the copy from, */
/* and the full size of the bitmap, to specify how much of it to copy. */
XCopyArea(display, bitmap, win, gc,
            0, 0,
            bitmap_width, bitmap_height,
            100, 50);
```
As you can see, we could also copy a given rectangle of the pixmap, instead of the whole pixmap.
One important note should be made - it is possible to create pixmaps of different depths on the same screen. When we perform copy operations (a pixmap onto a window, etc), we should make sure that both source and target have the same depth. If they have a different depth, the operation would fail. The exception to this is if we copy a specific bit plane of the source pixmap, using the XCopyPlane() function shown earlier. In such an event, we can copy a specific plain to the target window - in actuality, setting a specific bit in the color of each pixel copied. This can be used to generate strange graphic effects in window, but is beyond the scope of out tutorial.
# Freeing A Pixmap
Finally, when we are done using a given pixmap, we should free it, in order to free resources of the X server. This is done using the XFreePixmap() function:
```cpp
/* free the pixmap with the given ID. */
XFreePixmap(display, pixmap);
```
After freeing a pixmap - we must not try accessing it again.
To summarize this section, take a look at the draw-pixmap.c program, to see a pixmap being created using a bitmap file, and then tiled up on a window on your screen.
