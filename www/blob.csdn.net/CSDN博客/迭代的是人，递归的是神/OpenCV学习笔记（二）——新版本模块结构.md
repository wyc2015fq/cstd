# OpenCV学习笔记（二）——新版本模块结构 - 迭代的是人，递归的是神 - CSDN博客





2011年10月25日 20:00:13[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：19781
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









还是老话题，2.2版本对OpenCV可是进行了大刀阔斧的改革，用c++重新了大部分结构，而不是1.X版本中的c结构。这些模块包括：

core——定义了基本数据结构，包括最重要的Mat和一些其他的模块

imgproc——该模块包括了线性和非线性的图像滤波，图像的几何变换，颜色空间转换，直方图处理等等

video——该模块包括运动估计，背景分离，对象跟踪

calib3d——基本的多视角几何算法，单个立体摄像头标定，物体姿态估计，立体相似性算法，3D信息的重建

features2d——显著特征检测，描述，特征匹配

objdetect——物体检测和预定义好的分类器实例（比如人脸，眼睛，面部，人，车辆等等）

highgui——视频捕捉、图像和视频的编码解码、图形交互界面的接口

gpu——利用GPU对OpenCV模块进行加速算法

ml——机器学习模块（SVM，决策树，Boosting等等）

flann——Fast Library for Approximate Nearest Neighbors（FLANN）算法库

legacy——一些已经废弃的代码库，保留下来作为向下兼容

还有一些其他的模块，比如FLANN算法库、Google测试包、Python bingdings等等。





