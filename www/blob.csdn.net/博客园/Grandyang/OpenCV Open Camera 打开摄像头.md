# OpenCV Open Camera 打开摄像头 - Grandyang - 博客园







# [OpenCV Open Camera 打开摄像头](https://www.cnblogs.com/grandyang/p/5002633.html)







这是一个用OpenCV2.4.10打开摄像头的一个例子，参见代码如下：

```
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    CvCapture* cam0 = cvCaptureFromCAM(0);
    if(!cam0)
    {
        fprintf(stderr, "Could not initialize opening of Camera 0..\n");
        system("Pause");
        return -1;
    }
    printf("cam0 initialized\n");
    cvNamedWindow("Camera 0",CV_WINDOW_AUTOSIZE); //create a window called "Camera 0"

    while (1)
    {
        IplImage *cam0Frame = cvQueryFrame(cam0);
        if (cam0Frame) {
            cvShowImage("Camera 0", cam0Frame);
        }
        if (cvWaitKey(30) == 27) //wait for 'Esc' key press for 30ms. If 'Esc' key is pressed, break loop
        {
            cout << "Esc key is pressed by user" << endl;
            break;
        }
    }

    cvReleaseCapture(&cam0);
    cvDestroyWindow("Camera 0");

    return 0;
}
```



如果想同时打开两个摄像头，可以参见我之前的博客[OpenCV show two cameras 同时显示两个摄像头](http://www.cnblogs.com/grandyang/p/4302501.html)。














