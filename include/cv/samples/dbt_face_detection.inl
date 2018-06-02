#if defined(__linux__) || defined(LINUX) || defined(__APPLE__) || defined(ANDROID) || (defined(_MSC_VER) && _MSC_VER>=1800)

#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (img_t, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
    public:
        CascadeDetectorAdapter(Ptr<CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
        {
            CC_Assert(detector);
        }

        void detect(const img_t &Image, std::vector<Rect> &objects)
        {
            Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
        }

        virtual ~CascadeDetectorAdapter()
        {}

    private:
        CascadeDetectorAdapter();
        Ptr<CascadeClassifier> Detector;
 };

int main(int , char** )
{
    namedWindow(WindowName);

    VideoCapture VideoStream(0);

    if (!VideoStream.isOpened())
    {
        printf("Error: Cannot open video stream from camera\n");
        return 1;
    }

    std::string cascadeFrontalfilename = "../../data/lbpcascades/lbpcascade_frontalface.xml";
    Ptr<CascadeClassifier> cascade = makePtr<CascadeClassifier>(cascadeFrontalfilename);
    Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    cascade = makePtr<CascadeClassifier>(cascadeFrontalfilename);
    Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    img_t ReferenceFrame;
    img_t GrayFrame;
    vector<Rect> Faces;

    do
    {
        VideoStream >> ReferenceFrame;
        cvtColor(ReferenceFrame, GrayFrame, COLOR_RGB2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

        for (size_t i = 0; i < Faces.size(); i++)
        {
            rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
        }

        imshow(WindowName, ReferenceFrame);
    } while (waitKey(30) < 0);

    Detector.stop();

    return 0;
}

#else

#include <stdio.h>
int main()
{
    printf("This sample works for UNIX or ANDROID or Visual Studio 2013+ only\n");
    return 0;
}

#endif
