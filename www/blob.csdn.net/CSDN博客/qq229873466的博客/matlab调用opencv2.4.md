# matlab调用opencv2.4 - qq229873466的博客 - CSDN博客

2015年12月02日 19:17:11[qq229873466](https://me.csdn.net/qq229873466)阅读数：475


新建个test.cpp文件，输入如下测试代码

```cpp
#include <iostream>
#include "mex.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    try
    {
        mexPrintf("come\n");      
        Mat img = imread("dog.jpg");      
        if(img.empty()){
         mexPrintf("empty\n");
        }else{
            Mat result;
            Canny(img,result,50,150);
            imshow("canny-dog",result);
        }
    }
    catch(Exception ex)
    {
        char err[512];
        sprintf(err, "Error:%s", ex.what());
        mexPrintf(err);
    }
    mexPrintf("ok");
}
```
在matlab新建个脚本或用命令行输入以下编译命令

```cpp
mex test.cpp -I"E:\opencv\opencv2.4\build\include" -I"E:\opencv\opencv2.4\build\include\opencv" -I"E:\opencv\opencv2.4\build\include\opencv2" -L"E:\opencv\opencv2.4\build\x64\vc10\lib" -lopencv_contrib2410 -lopencv_core2410 -lopencv_features2d2410 -lopencv_flann2410 -lopencv_gpu2410 -lopencv_highgui2410 -lopencv_imgproc2410 -lopencv_legacy2410 -lopencv_ml2410 -lopencv_nonfree2410 -lopencv_objdetect2410 -lopencv_ocl2410 -lopencv_photo2410 -lopencv_stitching2410 -lopencv_superres2410 -lopencv_ts2410 -lopencv_video2410 -lopencv_videostab2410
```

注意格式：mex 文件名 -I"头文件目录" -L"库文件目录" -l库文件名字

库文件名字是那些不带d的lib（Release）

之后运行命令，得到一个test.mexw64文件，然后运行test()输出结果

