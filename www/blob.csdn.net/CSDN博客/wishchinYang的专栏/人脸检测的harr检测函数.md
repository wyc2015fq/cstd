# 人脸检测的harr检测函数 - wishchinYang的专栏 - CSDN博客
2013年07月17日 12:06:15[wishchin](https://me.csdn.net/wishchin)阅读数：821
个人分类：[人脸识别																[图像特征																[OpenCV																[目标追踪](https://blog.csdn.net/wishchin/article/category/1508415)](https://blog.csdn.net/wishchin/article/category/5778933)](https://blog.csdn.net/wishchin/article/category/1860001)](https://blog.csdn.net/wishchin/article/category/1508467)
眼球追踪需要对人脸进行识别，然后再对人眼进行识别，判断人眼张合度，进而判断疲劳...
解析：人脸检测的harr检测函数使用方法
代码理解：
       利用训练集，检测出脸部，画出框
```cpp
void CAviTestDlg::HaarFaceDetect( IplImage* TheImage, CvBox2D* faceBox)
{
    if( !cascade ) {  return ;  }
            storage = cvCreateMemStorage(0);
            int scale = 1;   int i;
            IplImage* temp = cvCreateImage( cvSize(TheImage->width/scale,TheImage->height/scale), 8, 3 );//多余！
            CvPoint pt1, pt2;
           if( cascade ){
                CvSeq* faces = cvHaarDetectObjects( TheImage, cascade, storage,
                    1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                    cvSize(40, 40) );
                for( i = 0; i < (faces ? faces->total : 0); i++ ){
                    CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
                    pt1.x = r->x*scale;
                    pt2.x = (r->x+r->width)*scale;
                    pt1.y = r->y*scale;
                    pt2.y = (r->y+r->height)*scale;
                    //cvRectangle( TheImage, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );// 画矩形，P1，P2,矩形对角线上的两个顶点。。。
                }
                if ( faces->total == 0 ) { MessageBox("人脸检测丢失，请重新加载","人脸检测丢失丢失",0);  return;}//
          
                else {
            faceBox->center.x = (float)(pt1.x+pt2.x)/2.0;
            faceBox->center.y = (float)(pt1.y+pt2.y)/2;
            faceBox->size.width = (float)(pt2.x - pt1.x);
            faceBox->size.height = (float)(pt2.y - pt1.y);
                }
            }
            cvReleaseImage( &temp );
}
```
