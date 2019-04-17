# OpenCV show two cameras 同时显示两个摄像头 - Grandyang - 博客园







# [OpenCV show two cameras 同时显示两个摄像头](https://www.cnblogs.com/grandyang/p/4302501.html)







用OpenCV同时显示两个摄像头的内容的代码如下：



```
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {
    CvCapture* cam0 = cvCaptureFromCAM(CV_CAP_DSHOW + 0);
    if(!cam0)
    {
        fprintf(stderr, "Could not initialize opening of Camera 0..\n");
        system("Pause");
        return -1;
    }
    printf("cam0 initialized\n");
    double height0 = cvGetCaptureProperty(cam0, CV_CAP_PROP_FRAME_HEIGHT);
    double width0 = cvGetCaptureProperty(cam0, CV_CAP_PROP_FRAME_WIDTH);
    cvNamedWindow("Camera 0",CV_WINDOW_AUTOSIZE); //create a window called "Camera 0"

    CvCapture* cam1 = cvCaptureFromCAM(CV_CAP_DSHOW + 1);
    if(!cam1)
    {
        fprintf(stderr, "Could not initialize opening of Camera 1..\n");
        system("Pause");
        return -1;
    }
    printf("cam1 initialized\n");
    double height1 = cvGetCaptureProperty(cam1, CV_CAP_PROP_FRAME_HEIGHT);
    double width1 = cvGetCaptureProperty(cam1, CV_CAP_PROP_FRAME_WIDTH);
    cvNamedWindow("Camera 1",CV_WINDOW_AUTOSIZE); //create a window called "Camera 1"

    while (1)
    {
        IplImage *cam0Frame = cvQueryFrame(cam0);
        if (cam0Frame) {
            cvShowImage("Camera 0", cam0Frame);
        }
        IplImage *cam1Frame = cvQueryFrame(cam1);
        if (cam1Frame) {
            cvShowImage("Camera 1", cam1Frame);
        }
        if (cvWaitKey(30) == 27) //wait for 'Esc' key press for 30ms. If 'Esc' key is pressed, break loop
        {
            cout << "Esc key is pressed by user" << endl;
            break; 
        }
    }

    cvReleaseCapture(&cam0);
    cvReleaseCapture(&cam1);
    cvDestroyWindow("Camera 0");
    cvDestroyWindow("Camera 1");
    return 0;
}
```



如果只想打开一个摄像头，可以参见我的另一篇博客[OpenCV Open Camera 打开摄像头](http://www.cnblogs.com/grandyang/p/5002633.html)。












