#include "cvaux.h"
#include "highgui.h"
#include "GPA.h"
#include <stdio.h>

#pragma comment (lib,"cxcore.lib")
#pragma comment (lib,"cv.lib")
#pragma comment (lib,"highgui.lib")
//#pragma comment (lib,"GPAdll.lib")

//this is a sample for foreground detection functions
int main(int argc, char** argv)
{
    IplImage*       tmp_frame = NULL;
    IplImage*       out = NULL;
    CvCapture*      cap = NULL;
    GPA* g=NULL;
    int fr;

    if( argc < 2 )
    {
        printf("please specify video file name \n");
        exit(0);
    }

    cap = cvCaptureFromFile(argv[1]);
    tmp_frame = cvQueryFrame(cap);
    if(!tmp_frame)
    {
        printf("bad video \n");
        exit(0);
    }

    cvNamedWindow("纠正前", 1);
    cvNamedWindow("纠正后", 1);

    for( fr = 1;tmp_frame; tmp_frame = cvQueryFrame(cap), fr++ )
    {
        char k;
        int h = tmp_frame->height, w = tmp_frame->width, al = tmp_frame->widthStep;
        unsigned char* a = tmp_frame->imageData;
        double t;
        if (NULL==out) {
          out = cvCreateImage(cvSize(tmp_frame->width,tmp_frame->height), IMG_DEPTH_8U, 3);
        }
        cvCopy(tmp_frame, out, 0);
        t = (double)cvGetTickCount();
        if (NULL==g) {
          g = GPA_new(h, w, a, al);
        } else {
          GPA_update(g, a, al, out->imageData, out->widthStep);
        }
        t = (double)cvGetTickCount() - t;
        cvFlip(out, 0, 0);
        //printf( "%.1f\n", t/(cvGetTickFrequency()*1000.) );
        cvShowImage("纠正前", tmp_frame);
        cvShowImage("纠正后", out);
        k = cvWaitKey(50);
        if( k == 27 ) break;
    }

    if (g) GPA_free(g);
    cvReleaseImage(&out);
    cvReleaseCapture(&cap);

    return 0;
}
