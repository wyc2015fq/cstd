
#include <stdio.h>
#include "std/rand_c.h"
#include "std/img_c.h"
#include "std/gui_c.h"
#include "std/drawing_c.h"

static void help()
{
    printf("\nThis program demonstrates OpenCV drawing and text output functions.\n"
    "Usage:\n   ./drawing\n");
}
static COLOR randomColor(rng_t* rng)
{
  int icolor = rng_int32(rng);
  return _RGB(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}
static COLOR randomColor(rng_t* rng, int alpha)
{
  int icolor = rng_int32(rng);
  return _RGBA(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255, alpha);
}

int test_drawing()
{
    help();
    char wndname[] = "Drawing Demo";
    const int NUMBER = 100;
    const int DELAY = 5;
    int lineType = LINE_AA; // change it to LINE_8 to see non-antialiased graphics
    int i, width = 1000, height = 700;
    int x1 = -width/2, x2 = width*3/2, y1 = -height/2, y2 = height*3/2;
    rng_t* rng = mt_static();

    img_t image[1] = { 0 };
    imsetsize(image, height, width, 3, 1);
    imfill(image, 0);
    imshow2(wndname, image);
    waitkey(DELAY);

    for (i = 0; i < NUMBER * 2; i++)
    {
        Point pt1, pt2;
        pt1.x = rng_uniform(rng,x1, x2);
        pt1.y = rng_uniform(rng,y1, y2);
        pt2.x = rng_uniform(rng,x1, x2);
        pt2.y = rng_uniform(rng,y1, y2);

        int arrowed = rng_uniform(rng,0, 6);
        COLOR color = randomColor(rng, 255);
        int thickness = rng_uniform(rng, 1, 10);
        if( arrowed < 3 )
            line( image, pt1, pt2, color, thickness, lineType );
        else
            arrowedLine(image, pt1, pt2, color, thickness, lineType);

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }

#if 0
    for (i = 0; i < NUMBER * 2; i++)
    {
        Point pt1, pt2;
        pt1.x = rng_uniform(rng,x1, x2);
        pt1.y = rng_uniform(rng,y1, y2);
        pt2.x = rng_uniform(rng,x1, x2);
        pt2.y = rng_uniform(rng,y1, y2);
        int thickness = rng_uniform(rng,-3, 10);
        int marker = rng_uniform(rng,0, 10);
        int marker_size = rng_uniform(rng,30, 80);

        if (marker > 5)
            rectangle(image, pt1, pt2, randomColor(rng), MAX(thickness, -1), lineType);
        else
            drawMarker(image, pt1, randomColor(rng), marker, marker_size );

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }
#endif
    for (i = 0; i < NUMBER; i++)
    {
        Point center;
        center.x = rng_uniform(rng,x1, x2);
        center.y = rng_uniform(rng,y1, y2);
        Size axes;
        axes.width = rng_uniform(rng,0, 200);
        axes.height = rng_uniform(rng,0, 200);
        double angle = rng_uniform(rng,0, 180);

        ellipse( image, center, axes, angle, angle - 100, angle + 200,
                 randomColor(rng), rng_uniform(rng,-1,9), lineType );

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i< NUMBER; i++)
    {
        Point pt[2][3];
        pt[0][0].x = rng_uniform(rng,x1, x2);
        pt[0][0].y = rng_uniform(rng,y1, y2);
        pt[0][1].x = rng_uniform(rng,x1, x2);
        pt[0][1].y = rng_uniform(rng,y1, y2);
        pt[0][2].x = rng_uniform(rng,x1, x2);
        pt[0][2].y = rng_uniform(rng,y1, y2);
        pt[1][0].x = rng_uniform(rng,x1, x2);
        pt[1][0].y = rng_uniform(rng,y1, y2);
        pt[1][1].x = rng_uniform(rng,x1, x2);
        pt[1][1].y = rng_uniform(rng,y1, y2);
        pt[1][2].x = rng_uniform(rng,x1, x2);
        pt[1][2].y = rng_uniform(rng,y1, y2);
        const Point* ppt[2] = {pt[0], pt[1]};
        int npt[] = {3, 3};

        polylines(image, ppt, npt, 2, true, randomColor(rng), rng_uniform(rng,1,10), lineType);

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i< NUMBER; i++)
    {
        Point pt[2][3];
        pt[0][0].x = rng_uniform(rng,x1, x2);
        pt[0][0].y = rng_uniform(rng,y1, y2);
        pt[0][1].x = rng_uniform(rng,x1, x2);
        pt[0][1].y = rng_uniform(rng,y1, y2);
        pt[0][2].x = rng_uniform(rng,x1, x2);
        pt[0][2].y = rng_uniform(rng,y1, y2);
        pt[1][0].x = rng_uniform(rng,x1, x2);
        pt[1][0].y = rng_uniform(rng,y1, y2);
        pt[1][1].x = rng_uniform(rng,x1, x2);
        pt[1][1].y = rng_uniform(rng,y1, y2);
        pt[1][2].x = rng_uniform(rng,x1, x2);
        pt[1][2].y = rng_uniform(rng,y1, y2);
        const Point* ppt[2] = {pt[0], pt[1]};
        int npt[] = {3, 3};

        fillPoly(image, ppt, npt, 2, randomColor(rng), lineType);

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i < NUMBER; i++)
    {
        Point center;
        center.x = rng_uniform(rng,x1, x2);
        center.y = rng_uniform(rng,y1, y2);

        circle(image, center, rng_uniform(rng,0, 300), randomColor(rng),
               rng_uniform(rng,-1, 9), lineType);

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }
#if 0
    for (i = 1; i < NUMBER; i++)
    {
        Point org;
        org.x = rng_uniform(rng,x1, x2);
        org.y = rng_uniform(rng,y1, y2);

        putText(image, "Testing text rendering", org, rng_uniform(rng,0,8),
                rng_uniform(rng,0,100)*0.05+0.1, randomColor(rng), rng_uniform(rng,1, 10), lineType);

        imshow2(wndname, image);
        if(waitkey(DELAY) >= 0)
            return 0;
    }

    Size textsize = getTextSize("OpenCV forever!", FONT_HERSHEY_COMPLEX, 3, 5, 0);
    Point org((width - textsize.width)/2, (height - textsize.height)/2);

    Mat image2;
    for( i = 0; i < 255; i += 2 )
    {
        image2 = image - Scalar::all(i);
        putText(image2, "OpenCV forever!", org, FONT_HERSHEY_COMPLEX, 3,
                Scalar(i, i, 255), 5, lineType);

        imshow2(wndname, image2);
        if(waitkey(DELAY) >= 0)
            return 0;
    }
#endif
    waitkey(-1);
    return 0;
}
