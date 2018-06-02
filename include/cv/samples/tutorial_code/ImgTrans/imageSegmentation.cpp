/**
 * @function Watershed_and_Distance_Transform.cpp
 * @brief Sample code showing how to segment overlapping objects using Laplacian filtering, in addition to Watershed and Distance Transformation
 * @author OpenCV Team
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int, char** argv)
{
//! [load_image]
    // Load the image
    CvMat src = imread(argv[1]);

    // Check if everything was fine
    if (!src.data)
        return -1;

    // Show source image
    imshow("Source Image", src);
//! [load_image]

//! [black_bg]
    // Change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform
    for( int x = 0; x < src.rows; x++ ) {
      for( int y = 0; y < src.cols; y++ ) {
          if ( src.at<Vec3b>(x, y) == Vec3b(255,255,255) ) {
            src.at<Vec3b>(x, y)[0] = 0;
            src.at<Vec3b>(x, y)[1] = 0;
            src.at<Vec3b>(x, y)[2] = 0;
          }
        }
    }

    // Show output image
    imshow("Black Background Image", src);
//! [black_bg]

//! [sharp]
    // Create a kernel that we will use for accuting/sharpening our image
    CvMat kernel = (Mat_<float>(3,3) <<
            1,  1, 1,
            1, -8, 1,
            1,  1, 1); // an approximation of second derivative, a quite strong kernel

    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CC_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    CvMat imgLaplacian;
    CvMat sharp = src; // copy source image to another temporary one
    filter2D(sharp, imgLaplacian, CC_32F, kernel);
    src.convertTo(sharp, CC_32F);
    CvMat imgResult = sharp - imgLaplacian;

    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CC_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CC_8UC3);

    // imshow( "Laplace Filtered Image", imgLaplacian );
    imshow( "New Sharped Image", imgResult );
//! [sharp]

    src = imgResult; // copy back

//! [bin]
    // Create binary image from source image
    CvMat bw;
    cvtColor(src, bw, CC_BGR2GRAY);
    threshold(bw, bw, 40, 255, CC_THRESH_BINARY | CC_THRESH_OTSU);
    imshow("Binary Image", bw);
//! [bin]

//! [dist]
    // Perform the distance transform algorithm
    CvMat dist;
    distanceTransform(bw, dist, CC_DIST_L2, 3);

    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1., NORM_MINMAX);
    imshow("Distance Transform Image", dist);
//! [dist]

//! [peaks]
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, .4, 1., CC_THRESH_BINARY);

    // Dilate a bit the dist image
    CvMat kernel1 = CvMat::ones(3, 3, CC_8UC1);
    dilate(dist, dist, kernel1);
    imshow("Peaks", dist);
//! [peaks]

//! [seeds]
    // Create the CC_8U version of the distance image
    // It is needed for findContours()
    CvMat dist_8u;
    dist.convertTo(dist_8u, CC_8U);

    // Find total markers
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CC_RETR_EXTERNAL, CC_CHAIN_APPROX_SIMPLE);

    // Create the marker image for the watershed algorithm
    CvMat markers = CvMat::zeros(dist.size(), CC_32SC1);

    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
        drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

    // Draw the background marker
    circle(markers, Point(5,5), 3, CC_RGB(255,255,255), -1);
    imshow("Markers", markers*10000);
//! [seeds]

//! [watershed]
    // Perform the watershed algorithm
    watershed(src, markers);

    CvMat mark = CvMat::zeros(markers.size(), CC_8UC1);
    markers.convertTo(mark, CC_8UC1);
    bitwise_not(mark, mark);
//    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
                                  // image looks like at that point

    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    CvMat dst = CvMat::zeros(markers.size(), CC_8UC3);

    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                dst.at<Vec3b>(i,j) = colors[index-1];
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }

    // Visualize the final image
    imshow("Final Result", dst);
//! [watershed]

    waitKey(0);
    return 0;
}