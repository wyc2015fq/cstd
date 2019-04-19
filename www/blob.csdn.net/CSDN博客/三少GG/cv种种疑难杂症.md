# cv种种疑难杂症 - 三少GG - CSDN博客
2011年04月20日 13:03:00[三少GG](https://me.csdn.net/scut1135)阅读数：1276
1.
太谢谢了 、。。。。。原来我真的在用这个已在库中定义的值。。。。。太感谢了。。。。。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.
## error C2872: 'IServiceProvider' : ambiguous symbol
(2007-07-04 11:51:09)
|||
今天早上 被这个错误烦了很久 。。。在网上找解决的办法竟然全是英文的 。。。更头大
硬着头皮看了几篇 问题好像都不大一样 于是就仿佛大海捞针一般 慢慢的找吧 挨个的试
终于 把问题解决了 回过头想想还真不是个事 。。。但是 为了提醒自己 也为了可能也会犯这个错误的 其他的朋友们 我还是记录下来吧
解决办法： 调整自定义的头文件和包含的系统头文件，命名空间的前后位置。因为位置不对很可能会引起在调用时候的混淆。
不管怎么说 要长记性哦 。。。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# OpenCV Change Logs
[http://opencv.willowgarage.com/wiki/OpenCV%20Change%20Logs](http://opencv.willowgarage.com/wiki/OpenCV%20Change%20Logs)
The library has been reorganized. Instead of cxcore, cv, cvaux, highgui and ml we now have several smaller modules: 
- opencv_core - core functionality (basic structures, arithmetics and linear algebra, dft, XML and YAML I/O ...). 
- 
opencv_imgproc - image processing (filter, [GaussianBlur](wiki/GaussianBlur), erode, dilate, resize, remap, cvtColor, calcHist etc.) 
- 
opencv_highgui - GUI and image & video I/O 
- opencv_ml - statistical machine learning models (SVM, Decision Trees, Boosting etc.) 
- opencv_features2d - 2D feature detectors and descriptors (SURF, FAST etc., 
- including the new feature detectors-descriptor-matcher framework) 
- opencv_video - motion analysis and object tracking (optical flow, motion templates, background subtraction) 
- 
opencv_objdetect - object detection in images (Haar & LBP face detectors, HOG people detector etc.) 
- opencv_calib3d - camera calibration, stereo correspondence and elements of 3D data processing 
- opencv_flann - the Fast Library for Approximate Nearest Neighbors (FLANN 1.5) and the OpenCV wrappers 
- opencv_contrib - contributed code that is not mature enough
- opencv_legacy - obsolete code, preserved for backward compatibility 
- opencv_gpu - acceleration of some OpenCV functionality using CUDA (relatively unstable, yet very actively developed part of OpenCV) 
If you detected OpenCV and configured your make scripts using CMake or pkg-config tool, your code will likely build fine without any changes. Otherwise, you will need to modify linker parameters (change the library names) and update the include paths. 
It is still possible to use #include <cv.h> etc. but the recommended notation is: 
- #include "opencv2/imgproc/imgproc.hpp" 
