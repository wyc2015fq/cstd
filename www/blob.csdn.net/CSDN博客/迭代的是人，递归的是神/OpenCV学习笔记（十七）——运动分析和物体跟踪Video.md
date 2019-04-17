# OpenCV学习笔记（十七）——运动分析和物体跟踪Video - 迭代的是人，递归的是神 - CSDN博客





2011年11月08日 17:33:41[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：19950
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









这个库中相关的函数有calcOpticalFlowPyrLK、calcOpticalFlowFarneback、estimateRigidTransform、updateMotionHistory、calcMotionGradient、calcGlobalOrientation、segmentMotion、CamShift、meanShift；还有卡尔曼滤波器类KalmanFilter，类内成员函数有构造函数、init、predict、correct。背景单元类BackgroundSubtractor，包括运算符重载，getBackgroundImage，其派生类BackgroundSubtractorMOG和BackgroundSubtractorMOG2，以及只支持Python版本的函数CalcOpticalFlowBM、CalcOpticalFlowHS，c版本的函数cvCalcOpticalFlowLK

这些函数每一个背后几乎都是一篇论文，很多都是经典的方法，我才疏识浅，只能边学边总结，先介绍一些我用过的函数，待日后陆续补充

calcOpticalPlowPyrLK是利用Lucas-Kanade方法计算图像的光流场



