# OpenCV: kalman滤波的代码段 - wishchinYang的专栏 - CSDN博客
2013年07月17日 11:24:27[wishchin](https://me.csdn.net/wishchin)阅读数：1091
序言：在我的疲劳检测工程 AviTest中！显示框为320*240,使用OpenCV的kalman滤波算法，可以实现简单的锁相追踪-实现对眼球的位置锁定。
代码如下：
```cpp
CvPoint WishchinKalman( IplImage* Image,CvPoint pCenter){
    CvPoint correctMat;
    correctMat.x = 0;
    correctMat.y = 0;
    const int stateNum   = 4;  
    const int measureNum = 2;  
    const int winHeight = 240;  
    const int winWidth  = 320;  
    IplImage* img    = cvCreateImage(cvSize(winWidth,winHeight),8,3);
    //cvCvtColor(Image,img,CV_GRAY2BGR);
    img = cvCloneImage(Image);
    CvKalman* kalman = cvCreateKalman( stateNum, measureNum, 0 );//state(x,y,detaX,detaY)  
    CvMat* process_noise = cvCreateMat( stateNum, 1, CV_32FC1 );  
    CvMat* measurement = cvCreateMat( measureNum, 1, CV_32FC1 );//measurement(x,y)  
    CvRNG rng = cvRNG(-1);  
    float A[stateNum][stateNum] ={//transition matrix    状态转移矩阵为什么用这个？????wishchin
        1,0,1,0,  
        0,1,0,1,  
        0,0,1,0,  
        0,0,0,1  
    };  
    memcpy( kalman->transition_matrix->data.fl,A,sizeof(A));  
    cvSetIdentity(kalman->measurement_matrix,cvRealScalar(1) );  
    cvSetIdentity(kalman->process_noise_cov,cvRealScalar(1e-5));  
    cvSetIdentity(kalman->measurement_noise_cov,cvRealScalar(1e-1));  
    cvSetIdentity(kalman->error_cov_post,cvRealScalar(1));  
    cvRandArr(&rng,kalman->state_post,CV_RAND_UNI,cvRealScalar(0),cvRealScalar(winHeight>winWidth?winWidth:winHeight));  
    CvFont font;  
    cvInitFont(&font,CV_FONT_HERSHEY_SCRIPT_COMPLEX,1,1);  
    const CvMat* prediction=cvKalmanPredict(kalman,0);  
    measurement->data.fl[0]=(float)pCenter.x;  
    measurement->data.fl[1]=(float)pCenter.y;
    //4.update  
    const CvMat* correction=cvKalmanCorrect( kalman, measurement );       
    correctMat = cvPoint((int)correction->data.fl[0],(int)correction->data.fl[1]);
    ////关键接口 wishchin
    //draw   
    cvSet(img,cvScalar(255,255,255,0));  
    //cvCircle(img,correctMat,5,CV_RGB(0,255,0),3);//predicted point with green  
    //cvReleaseImage(&img);
    return correctMat;
    //return 0;  
}
```
