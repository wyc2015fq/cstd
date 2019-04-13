
# 检测直线（利用cv::ximgproc::FastLineDetector） - 郭云飞的专栏 - CSDN博客


2017年12月08日 18:56:31[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1150


**介绍：**
在OpenCV3.0 以上版本的contrib模块中，有一个**cv::ximgproc::FastLineDetector**类。定义位置：

```python
// 需要下载contrib模块
opencv_contrib/modules/ximgproc/include/opencv2/ximgproc/fast_line_detector.hpp
```
所谓Fast，是相对于正式模块中的**cv::imgproc::LineSegmentDetector**类来说，速度更快。FastLineDetector类依据下边的论文实现：
```python
// 论文
Outdoor Place Recognition in Urban Environments using Straight Lines
// 下载地址：
http://cvlab.hanyang.ac.kr/~jwlim/files/icra14linerec.pdf
```
大致的原理是：先在输入图像上，应用canny边缘检测；然后根据在canny边缘图像上进行分析，找到直线。

**例程：**
该例程对比了上述俩直线检测算子的运行耗时情况。

```python
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/io.h>
#include <sys/times.h>
#include <iomanip>
#include <tuple>
#include <cstdlib>
using namespace std;
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
using namespace cv::ximgproc;
int main(int argc, char** argv)
{
    std::string in;
    cv::CommandLineParser parser(argc, argv, "{@input|../samples/data/corridor.jpg|input image}{help h||show help message}");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    in = parser.get<string>("@input");
    Mat image = imread(in, IMREAD_GRAYSCALE);
    if( image.empty() )
    {
        return -1;
    }
    // Create LSD detector
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector();
    vector<Vec4f> lines_lsd;
    // Create FLD detector
    // Param               Default value   Description
    // length_threshold    10            - Segments shorter than this will be discarded
    // distance_threshold  1.41421356    - A point placed from a hypothesis line
    //                                     segment farther than this will be
    //                                     regarded as an outlier
    // canny_th1           50            - First threshold for
    //                                     hysteresis procedure in Canny()
    // canny_th2           50            - Second threshold for
    //                                     hysteresis procedure in Canny()
    // canny_aperture_size 3             - Aperturesize for the sobel
    //                                     operator in Canny()
    // do_merge            false         - If true, incremental merging of segments
    //                                     will be perfomred
    int    length_threshold    = 10;
    float  distance_threshold  = 1.41421356f;
    double canny_th1           = 50.0;
    double canny_th2           = 50.0;
    int    canny_aperture_size = 3;
    bool   do_merge            = false;
    Ptr<FastLineDetector> fld = createFastLineDetector(
            length_threshold,
            distance_threshold, 
            canny_th1, 
            canny_th2, 
            canny_aperture_size,
            do_merge);
    vector<Vec4f> lines_fld;
    // Because of some CPU's power strategy, it seems that the first running of
    // an algorithm takes much longer. So here we run both of the algorithmes 10
    // times to see each algorithm's processing time with sufficiently warmed-up
    // CPU performance.
    for(int run_count = 0; run_count < 10; run_count++) {
        lines_lsd.clear();
        int64 start_lsd = getTickCount();
        lsd->detect(image, lines_lsd);
        // Detect the lines with LSD
        double freq = getTickFrequency();
        double duration_ms_lsd = double(getTickCount() - start_lsd) * 1000 / freq;
        std::cout << "Elapsed time for LSD: " 
                  << setw(10) << setiosflags(ios::right) << setiosflags(ios::fixed) << setprecision(2) 
                  << duration_ms_lsd << " ms." << std::endl;
        lines_fld.clear();
        int64 start = getTickCount();
        // Detect the lines with FLD
        fld->detect(image, lines_fld);
        double duration_ms = double(getTickCount() - start) * 1000 / freq;
        std::cout << "Ealpsed time for FLD: " 
                  << setw(10) << setiosflags(ios::right) << setiosflags(ios::fixed) << setprecision(2)
                  << duration_ms << " ms." << std::endl;
    }
    
    // Show found lines with LSD
    Mat line_image_lsd(image);
    lsd->drawSegments(line_image_lsd, lines_lsd);
    imshow("LSD result", line_image_lsd);
    // Show found lines with FLD
    Mat line_image_fld(image);
    fld->drawSegments(line_image_fld, lines_fld);
    imshow("FLD result", line_image_fld);
    waitKey();
    
    return 0;
}
```
结果：![](https://img-blog.csdn.net/20171208185338538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171208185459745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看出，俩算法的效果差不多；但FLD要更快！



