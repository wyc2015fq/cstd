
# dlib人脸检测python 与 C++ 结果对比 - 小花生的博客 - CSDN博客


2018年12月16日 22:19:52[Peanut_范](https://me.csdn.net/u013841196)阅读数：113



###### dlib人脸检测python 与 C++ 结果对比
## python
**参考代码：**
`import cv2 
import dlib
from skimage import io
detector = dlib.get_frontal_face_detector()
win = dlib.image_window()
if __name__ == '__main__':
    f = r"XZQ.jpg"
    img = io.imread(f)
    dets = detector(img,0) #使用detector进行人脸检测
    for i, d in enumerate(dets):
        x = d.left()
        y = d.top()
        w = d.width()
        h = d.height()     
        cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0))
        print("({},{},{},{})".format( x, y, w, h))
    win.set_image(img)
    io.imsave('save_XZQ_dets0.jpg',img)
    #等待点击
    dlib.hit_enter_to_continue()`**测试结果：**
dets = detector(img,1)
python:(262,118,187,187)
dets = detector(img,0)
python:(273,114,181,180)
**当dets = detector(img,1) 时，结果：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216221337507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**当dets = detector(img,0)时，结果：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216221410694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
## C++：
**参考代码：**
`#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace dlib;
using namespace std;
cv::Rect Detect(cv::Mat im)
{
    cv::Rect R;
    frontal_face_detector detector = get_frontal_face_detector();
    array2d<bgr_pixel> img;  
    assign_image(img, cv_image<uchar>(im));
    std::vector<rectangle> dets = detector(img);//检测人脸
    //查找最大脸
    if (dets.size() != 0)
    {
        int Max = 0;
        int area = 0;
        for (unsigned long t = 0; t < dets.size(); ++t)
        {           
            if (area < dets[t].width()*dets[t].height())
            {
                area = dets[t].width()*dets[t].height();
                Max = t;
            }
        }
        R.x = dets[Max].left();
        R.y = dets[Max].top();
        R.width = dets[Max].width();
        R.height = dets[Max].height();
        cout<<"("<<R.x<<","<<R.y<<","<<R.width<<","<<R.height<<")"<<endl;
    }
    return R;
}
int main()
{
    
    string path = "XZQ.jpg";
    try
    {       
        cv::Mat src, dec;
        src = cv::imread(path);
        src.copyTo(dec);
        cv::cvtColor(dec, dec, CV_BGR2GRAY);
        cv::Rect box;
        box = Detect(dec);
        cv::rectangle(src, box, cv::Scalar(0, 0, 255), 1, 1, 0);        
        //cv::imshow("frame", src);
        cv::imwrite("saveXZQ.jpg", src);
        //cv::waitKey(0); 
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
}`**测试结果：**
std::vector dets = detector(img);
C++:（273,114,181,180）
**综上：**
仅测试一张图片，python：dets = detector(img,0)和C++：std::vector dets = detector(img) 比较接近，感兴趣的可以测试，看一下结果。
**参考：**
[https://blog.csdn.net/u011045727/article/details/55505598](https://blog.csdn.net/u011045727/article/details/55505598)
---

###### 注：博众家之所长，集群英之荟萃。

