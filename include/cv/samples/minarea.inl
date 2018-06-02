#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "This program demonstrates finding the minimum enclosing box, triangle or circle of a set\n"
         << "of points using functions: minAreaRect() minEnclosingTriangle() minEnclosingCircle().\n"
         << "Random points are generated and then enclosed.\n\n"
         << "Press ESC, 'q' or 'Q' to exit and any other key to regenerate the set of points.\n\n"
         << "Call:\n"
         << "./minarea\n"
         << "Using OpenCV v" << CC_VERSION << "\n" << endl;
}

int main( int /*argc*/, char** /*argv*/ )
{
    help();

    img_t img(500, 500, CC_8UC3);
    RNG& rng = theRNG();

    for(;;)
    {
        int i, count = rng.uniform(1, 101);
        vector<Point> points;

        // Generate a random set of points
        for( i = 0; i < count; i++ )
        {
            Point pt;
            pt.x = rng.uniform(img->cols/4, img->cols*3/4);
            pt.y = rng.uniform(img->rows/4, img->rows*3/4);

            points.push_back(pt);
        }

        // Find the minimum area enclosing bounding box
        RotatedRect box = minAreaRect(img_t(points));

        // Find the minimum area enclosing triangle
        vector<CPoint2f> triangle;

        minEnclosingTriangle(points, triangle);

        // Find the minimum area enclosing circle
        CPoint2f center, vtx[4];
        float radius = 0;
        minEnclosingCircle(img_t(points), center, radius);
        box.points(vtx);

        img = Scalar::all(0);

        // Draw the points
        for( i = 0; i < count; i++ )
            circle( img, points[i], 3, Scalar(0, 0, 255), FILLED, LINE_AA );

        // Draw the bounding box
        for( i = 0; i < 4; i++ )
            line(img, vtx[i], vtx[(i+1)%4], Scalar(0, 255, 0), 1, LINE_AA);

        // Draw the triangle
        for( i = 0; i < 3; i++ )
            line(img, triangle[i], triangle[(i+1)%3], Scalar(255, 255, 0), 1, LINE_AA);

        // Draw the circle
        circle(img, center, cRound(radius), Scalar(0, 255, 255), 1, LINE_AA);

        imshow( "Rectangle, triangle & circle", img );

        char key = (char)waitKey();
        if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
            break;
    }

    return 0;
}
