#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    std::string in;
    CommandLineParser parser(argc, argv, "{@input|../data/building.jpg|input image}{help h||show help message}");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    in = parser.get<string>("@input");

    img_t image = imread(in, IMREAD_GRAYSCALE);

    if( image.empty() )
    { return -1; }

#if 0
    Canny(image, image, 50, 200, 3); // Apply canny edge
#endif

    // Create and LSD detector with standard or no refinement.
#if 1
    Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
#else
    Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif

    double start = double(cGetTickCount());
    vector<Vec4f> lines_std;

    // Detect the lines
    ls->detect(image, lines_std);

    double duration_ms = (double(cGetTickCount()) - start) * 1000 / getTickFrequency();
    std::cout << "It took " << duration_ms << " ms." << std::endl;

    // Show found lines
    img_t drawnLines(image);
    ls->drawSegments(drawnLines, lines_std);
    imshow("Standard refinement", drawnLines);

    waitKey();
    return 0;
}
