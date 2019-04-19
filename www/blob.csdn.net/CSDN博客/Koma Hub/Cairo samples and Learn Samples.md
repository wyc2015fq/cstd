# Cairo samples and Learn Samples - Koma Hub - CSDN博客
2019年03月02日 18:35:42[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：225
个人分类：[Cairo																[GTK](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/8614705)
**Table of Contents**
[Cairo samples](#Cairo%20samples)
[arc](#arc)
[arc negative](#arc%20negative)
[clip](#clip)
[clip image](#clip%20image)
[curve rectangle](#curve%20rectangle)
[curve to](#curve%20to)
[dash](#dash)
[fill and stroke2](#fill%20and%20stroke2)
[fill style](#fill%20style)
[gradient](#gradient)
[image](#image)
[imagepattern](#imagepattern)
[multi segment caps](#multi%20segment%20caps)
[rounded rectangle](#rounded%20rectangle)
[set line cap](#set%20line%20cap)
[set line join](#set%20line%20join)
[text](#text)
[text align center](#text%20align%20center)
[text extents](#text%20extents)
[My demo](#My%C2%A0demo%20%3CGitHub%3E)
# Cairo samples
[https://www.cairographics.org/samples/](https://www.cairographics.org/samples/)
This page contains samples of cairo's rendered output and the code snippets used to create them. The snippets are meant to be short, and easy to understand.
The original snippets were created by Øyvind Kolås for a paper submitted to [GUADEC 2004](http://2004.guadec.org/). All of his original snippet code is considered to be part of the public domain.
## [arc](https://www.cairographics.org/samples/arc/)
```cpp
double xc = 128.0;
double yc = 128.0;
double radius = 100.0;
double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */
cairo_set_line_width (cr, 10.0);
cairo_arc (cr, xc, yc, radius, angle1, angle2);
cairo_stroke (cr);
/* draw helping lines */
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_set_line_width (cr, 6.0);
cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
cairo_fill (cr);
cairo_arc (cr, xc, yc, radius, angle1, angle1);
cairo_line_to (cr, xc, yc);
cairo_arc (cr, xc, yc, radius, angle2, angle2);
cairo_line_to (cr, xc, yc);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302180719874.png)
## [arc negative](https://www.cairographics.org/samples/arc_negative/)
```cpp
double xc = 128.0;
double yc = 128.0;
double radius = 100.0;
double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */
cairo_set_line_width (cr, 10.0);
cairo_arc_negative (cr, xc, yc, radius, angle1, angle2);
cairo_stroke (cr);
/* draw helping lines */
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_set_line_width (cr, 6.0);
cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
cairo_fill (cr);
cairo_arc (cr, xc, yc, radius, angle1, angle1);
cairo_line_to (cr, xc, yc);
cairo_arc (cr, xc, yc, radius, angle2, angle2);
cairo_line_to (cr, xc, yc);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302180809773.png)
## [clip](https://www.cairographics.org/samples/clip/)
```cpp
cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
cairo_clip (cr);
cairo_new_path (cr);  /* current path is not
                         consumed by cairo_clip() */
cairo_rectangle (cr, 0, 0, 256, 256);
cairo_fill (cr);
cairo_set_source_rgb (cr, 0, 1, 0);
cairo_move_to (cr, 0, 0);
cairo_line_to (cr, 256, 256);
cairo_move_to (cr, 256, 0);
cairo_line_to (cr, 0, 256);
cairo_set_line_width (cr, 10.0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302180843612.png)
## [clip image](https://www.cairographics.org/samples/clip_image/)
```cpp
int              w, h;
cairo_surface_t *image;
cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2*M_PI);
cairo_clip (cr);
cairo_new_path (cr); /* path not consumed by clip()*/
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
cairo_scale (cr, 256.0/w, 256.0/h);
cairo_set_source_surface (cr, image, 0, 0);
cairo_paint (cr);
cairo_surface_destroy (image);
```
![](https://img-blog.csdnimg.cn/2019030218090665.png)
## [curve rectangle](https://www.cairographics.org/samples/curve_rectangle/)
Unless you know what you are doing, you should probably use [rounded rectangle](https://www.cairographics.org/samples/rounded_rectangle/) instead.
```cpp
/* a custom shape that could be wrapped in a function */
double x0      = 25.6,   /* parameters like cairo_rectangle */
       y0      = 25.6,
       rect_width  = 204.8,
       rect_height = 204.8,
       radius = 102.4;   /* and an approximate curvature radius */
double x1,y1;
x1=x0+rect_width;
y1=y0+rect_height;
if (!rect_width || !rect_height)
    return;
if (rect_width/2<radius) {
    if (rect_height/2<radius) {
        cairo_move_to  (cr, x0, (y0 + y1)/2);
        cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
        cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
        cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
        cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
    } else {
        cairo_move_to  (cr, x0, y0 + radius);
        cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
        cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
        cairo_line_to (cr, x1 , y1 - radius);
        cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
        cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
    }
} else {
    if (rect_height/2<radius) {
        cairo_move_to  (cr, x0, (y0 + y1)/2);
        cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
        cairo_line_to (cr, x1 - radius, y0);
        cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
        cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
        cairo_line_to (cr, x0 + radius, y1);
        cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
    } else {
        cairo_move_to  (cr, x0, y0 + radius);
        cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
        cairo_line_to (cr, x1 - radius, y0);
        cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
        cairo_line_to (cr, x1 , y1 - radius);
        cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
        cairo_line_to (cr, x0 + radius, y1);
        cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
    }
}
cairo_close_path (cr);
cairo_set_source_rgb (cr, 0.5, 0.5, 1);
cairo_fill_preserve (cr);
cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
cairo_set_line_width (cr, 10.0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181017863.png)
## [curve to](https://www.cairographics.org/samples/curve_to/)
```cpp
double x=25.6,  y=128.0;
double x1=102.4, y1=230.4,
       x2=153.6, y2=25.6,
       x3=230.4, y3=128.0;
cairo_move_to (cr, x, y);
cairo_curve_to (cr, x1, y1, x2, y2, x3, y3);
cairo_set_line_width (cr, 10.0);
cairo_stroke (cr);
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_set_line_width (cr, 6.0);
cairo_move_to (cr,x,y);   cairo_line_to (cr,x1,y1);
cairo_move_to (cr,x2,y2); cairo_line_to (cr,x3,y3);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181055320.png)
## [dash](https://www.cairographics.org/samples/dash/)
```cpp
double dashes[] = {50.0,  /* ink */
                   10.0,  /* skip */
                   10.0,  /* ink */
                   10.0   /* skip*/
                  };
int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
double offset = -50.0;
cairo_set_dash (cr, dashes, ndash, offset);
cairo_set_line_width (cr, 10.0);
cairo_move_to (cr, 128.0, 25.6);
cairo_line_to (cr, 230.4, 230.4);
cairo_rel_line_to (cr, -102.4, 0.0);
cairo_curve_to (cr, 51.2, 230.4, 51.2, 128.0, 128.0, 128.0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181200985.png)
## [fill and stroke2](https://www.cairographics.org/samples/fill_and_stroke2/)
```cpp
cairo_move_to (cr, 128.0, 25.6);
cairo_line_to (cr, 230.4, 230.4);
cairo_rel_line_to (cr, -102.4, 0.0);
cairo_curve_to (cr, 51.2, 230.4, 51.2, 128.0, 128.0, 128.0);
cairo_close_path (cr);
cairo_move_to (cr, 64.0, 25.6);
cairo_rel_line_to (cr, 51.2, 51.2);
cairo_rel_line_to (cr, -51.2, 51.2);
cairo_rel_line_to (cr, -51.2, -51.2);
cairo_close_path (cr);
cairo_set_line_width (cr, 10.0);
cairo_set_source_rgb (cr, 0, 0, 1);
cairo_fill_preserve (cr);
cairo_set_source_rgb (cr, 0, 0, 0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181246337.png)
## [fill style](https://www.cairographics.org/samples/fill_style/)
```cpp
cairo_set_line_width (cr, 6);
cairo_rectangle (cr, 12, 12, 232, 70);
cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);
cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
cairo_set_source_rgb (cr, 0, 0.7, 0); cairo_fill_preserve (cr);
cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);
cairo_translate (cr, 0, 128);
cairo_rectangle (cr, 12, 12, 232, 70);
cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);
cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
cairo_set_source_rgb (cr, 0, 0, 0.9); cairo_fill_preserve (cr);
cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181326270.png)
## [gradient](https://www.cairographics.org/samples/gradient/)
```cpp
cairo_pattern_t *pat;
pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
cairo_rectangle (cr, 0, 0, 256, 256);
cairo_set_source (cr, pat);
cairo_fill (cr);
cairo_pattern_destroy (pat);
pat = cairo_pattern_create_radial (115.2, 102.4, 25.6,
                                   102.4,  102.4, 128.0);
cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
cairo_set_source (cr, pat);
cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
cairo_fill (cr);
cairo_pattern_destroy (pat);
```
![](https://img-blog.csdnimg.cn/20190302181400295.png)
## [image](https://www.cairographics.org/samples/image/)
```cpp
int              w, h;
cairo_surface_t *image;
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
cairo_translate (cr, 128.0, 128.0);
cairo_rotate (cr, 45* M_PI/180);
cairo_scale  (cr, 256.0/w, 256.0/h);
cairo_translate (cr, -0.5*w, -0.5*h);
cairo_set_source_surface (cr, image, 0, 0);
cairo_paint (cr);
cairo_surface_destroy (image);
```
![](https://img-blog.csdnimg.cn/20190302181435959.png)
## [imagepattern](https://www.cairographics.org/samples/imagepattern/)
```cpp
int              w, h;
cairo_surface_t *image;
cairo_pattern_t *pattern;
cairo_matrix_t   matrix;
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
pattern = cairo_pattern_create_for_surface (image);
cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
cairo_translate (cr, 128.0, 128.0);
cairo_rotate (cr, M_PI / 4);
cairo_scale (cr, 1 / sqrt (2), 1 / sqrt (2));
cairo_translate (cr, -128.0, -128.0);
cairo_matrix_init_scale (&matrix, w/256.0 * 5.0, h/256.0 * 5.0);
cairo_pattern_set_matrix (pattern, &matrix);
cairo_set_source (cr, pattern);
cairo_rectangle (cr, 0, 0, 256.0, 256.0);
cairo_fill (cr);
cairo_pattern_destroy (pattern);
cairo_surface_destroy (image);
```
![](https://img-blog.csdnimg.cn/2019030218150728.png)
## [multi segment caps](https://www.cairographics.org/samples/multi_segment_caps/)
```cpp
cairo_move_to (cr, 50.0, 75.0);
cairo_line_to (cr, 200.0, 75.0);
cairo_move_to (cr, 50.0, 125.0);
cairo_line_to (cr, 200.0, 125.0);
cairo_move_to (cr, 50.0, 175.0);
cairo_line_to (cr, 200.0, 175.0);
cairo_set_line_width (cr, 30.0);
cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181601286.png)
## [rounded rectangle](https://www.cairographics.org/samples/rounded_rectangle/)
```cpp
/* a custom shape that could be wrapped in a function */
double x         = 25.6,        /* parameters like cairo_rectangle */
       y         = 25.6,
       width         = 204.8,
       height        = 204.8,
       aspect        = 1.0,     /* aspect ratio */
       corner_radius = height / 10.0;   /* and corner curvature radius */
double radius = corner_radius / aspect;
double degrees = M_PI / 180.0;
cairo_new_sub_path (cr);
cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
cairo_close_path (cr);
cairo_set_source_rgb (cr, 0.5, 0.5, 1);
cairo_fill_preserve (cr);
cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
cairo_set_line_width (cr, 10.0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181637194.png)
## [set line cap](https://www.cairographics.org/samples/set_line_cap/)
```cpp
cairo_set_line_width (cr, 30.0);
cairo_set_line_cap  (cr, CAIRO_LINE_CAP_BUTT); /* default */
cairo_move_to (cr, 64.0, 50.0); cairo_line_to (cr, 64.0, 200.0);
cairo_stroke (cr);
cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);
cairo_move_to (cr, 128.0, 50.0); cairo_line_to (cr, 128.0, 200.0);
cairo_stroke (cr);
cairo_set_line_cap  (cr, CAIRO_LINE_CAP_SQUARE);
cairo_move_to (cr, 192.0, 50.0); cairo_line_to (cr, 192.0, 200.0);
cairo_stroke (cr);
/* draw helping lines */
cairo_set_source_rgb (cr, 1, 0.2, 0.2);
cairo_set_line_width (cr, 2.56);
cairo_move_to (cr, 64.0, 50.0); cairo_line_to (cr, 64.0, 200.0);
cairo_move_to (cr, 128.0, 50.0);  cairo_line_to (cr, 128.0, 200.0);
cairo_move_to (cr, 192.0, 50.0); cairo_line_to (cr, 192.0, 200.0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181711905.png)
## [set line join](https://www.cairographics.org/samples/set_line_join/)
```cpp
cairo_set_line_width (cr, 40.96);
cairo_move_to (cr, 76.8, 84.48);
cairo_rel_line_to (cr, 51.2, -51.2);
cairo_rel_line_to (cr, 51.2, 51.2);
cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER); /* default */
cairo_stroke (cr);
cairo_move_to (cr, 76.8, 161.28);
cairo_rel_line_to (cr, 51.2, -51.2);
cairo_rel_line_to (cr, 51.2, 51.2);
cairo_set_line_join (cr, CAIRO_LINE_JOIN_BEVEL);
cairo_stroke (cr);
cairo_move_to (cr, 76.8, 238.08);
cairo_rel_line_to (cr, 51.2, -51.2);
cairo_rel_line_to (cr, 51.2, 51.2);
cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181753866.png)
## [text](https://www.cairographics.org/samples/text/)
```cpp
cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
cairo_set_font_size (cr, 90.0);
cairo_move_to (cr, 10.0, 135.0);
cairo_show_text (cr, "Hello");
cairo_move_to (cr, 70.0, 165.0);
cairo_text_path (cr, "void");
cairo_set_source_rgb (cr, 0.5, 0.5, 1);
cairo_fill_preserve (cr);
cairo_set_source_rgb (cr, 0, 0, 0);
cairo_set_line_width (cr, 2.56);
cairo_stroke (cr);
/* draw helping lines */
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_arc (cr, 10.0, 135.0, 5.12, 0, 2*M_PI);
cairo_close_path (cr);
cairo_arc (cr, 70.0, 165.0, 5.12, 0, 2*M_PI);
cairo_fill (cr);
```
![](https://img-blog.csdnimg.cn/20190302181843843.png)
## [text align center](https://www.cairographics.org/samples/text_align_center/)
```cpp
cairo_text_extents_t extents;
const char *utf8 = "cairo";
double x,y;
cairo_select_font_face (cr, "Sans",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size (cr, 52.0);
cairo_text_extents (cr, utf8, &extents);
x = 128.0-(extents.width/2 + extents.x_bearing);
y = 128.0-(extents.height/2 + extents.y_bearing);
cairo_move_to (cr, x, y);
cairo_show_text (cr, utf8);
/* draw helping lines */
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_set_line_width (cr, 6.0);
cairo_arc (cr, x, y, 10.0, 0, 2*M_PI);
cairo_fill (cr);
cairo_move_to (cr, 128.0, 0);
cairo_rel_line_to (cr, 0, 256);
cairo_move_to (cr, 0, 128.0);
cairo_rel_line_to (cr, 256, 0);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181914636.png)
## [text extents](https://www.cairographics.org/samples/text_extents/)
```cpp
cairo_text_extents_t extents;
const char *utf8 = "cairo";
double x,y;
cairo_select_font_face (cr, "Sans",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_NORMAL);
cairo_set_font_size (cr, 100.0);
cairo_text_extents (cr, utf8, &extents);
x=25.0;
y=150.0;
cairo_move_to (cr, x,y);
cairo_show_text (cr, utf8);
/* draw helping lines */
cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
cairo_set_line_width (cr, 6.0);
cairo_arc (cr, x, y, 10.0, 0, 2*M_PI);
cairo_fill (cr);
cairo_move_to (cr, x,y);
cairo_rel_line_to (cr, 0, -extents.height);
cairo_rel_line_to (cr, extents.width, 0);
cairo_rel_line_to (cr, extents.x_bearing, -extents.y_bearing);
cairo_stroke (cr);
```
![](https://img-blog.csdnimg.cn/20190302181943263.png)
# My demo <GitHub>

