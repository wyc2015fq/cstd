# opencv StereoBM各个参数的意义 - KinboSong的博客 - CSDN博客
2017年06月08日 15:01:42[songjinbo3](https://me.csdn.net/KinboSong)阅读数：3315
另一篇很好的参考博客：[https://cloud.tencent.com/developer/article/1065819](https://cloud.tencent.com/developer/article/1065819)
**from [http://blog.csdn.net/chenyusiyuan/article/details/5967291](http://blog.csdn.net/chenyusiyuan/article/details/5967291)**
**5． 如何设置BM、SGBM和GC算法的状态参数？**
**（1）StereoBMState**
// 预处理滤波参数
- **preFilterType：预处理滤波器的类型**，主要是用于降低亮度失真（photometric distortions）、消除噪声和增强纹理等, 有两种可选类型：CV_STEREO_BM_NORMALIZED_RESPONSE（归一化响应） 或者 **CV_STEREO_BM_XSOBEL（水平方向Sobel算子，默认类型）**, 该参数为 int 型；
- **preFilterSize：预处理滤波器窗口大小**，容许范围是[5,255]，一般**应该在 5x5..21x21 之间**，参数必须为**奇数**值, int 型
- **preFilterCap：预处理滤波器的截断**值，预处理的输出值仅保留[-preFilterCap, preFilterCap]范围内的值，**参数范围：1 - 31（文档中是31，但代码中是 63）**, int
// SAD 参数
- **SADWindowSize：SAD窗口大小**，容许范围是[5,255]，一般**应该在 5x5 至 21x21 之间**，参数必须是**奇数**，int 型
- **minDisparity：最小视差**，**默认值为 0**, 可以是负值，int 型
- **numberOfDisparities：视差窗口**，即最大视差值与最小视差值之差, 窗口大小**必须是 16 的整数倍**，int 型
// 后处理参数
- **textureThreshold：低纹理区域的判断阈值**。如果当前SAD窗口内所有邻居像素点的x导数绝对值之和小于指定阈值，则该窗口对应的像素点的视差值为 0（That is, if the sum of absolute values of x-derivatives computed over SADWindowSize by SADWindowSize pixel neighborhood is smaller than the parameter, no disparity
 is computed at the pixel），该参数不能为负值，int 型
- **uniquenessRatio：视差唯一性百分比**， 视差窗口范围内最低代价是次低代价的(1 + uniquenessRatio/100)倍时，最低代价对应的视差值才是该像素点的视差，否则该像素点的视差为 0 （the minimum margin in percents between the best (minimum) cost function value and the second best value to accept the computed disparity,
 that is, accept the computed disparity d^ only if SAD(d) >= SAD(d^) x (1 + uniquenessRatio/100.) for any d != d*+/-1 within the search range ），该参数不能为负值，**一般5-15左右的值比较合适，**int 型
- **speckleWindowSize：检查视差连通区域变化度的窗口大小**, 值为 0 时取消 speckle 检查，int 型
- **speckleRange：视差变化阈值**，当窗口内视差变化大于阈值时，该窗口内的视差清零，int 型
// OpenCV2.1 新增的状态参数
- **roi1, roi2：左右视图的有效像素区域**，一般**由双目校正阶段的 cvStereoRectify 函数传递**，也可以自行设定。一旦在状态参数中设定了 roi1 和 roi2，OpenCV 会通过cvGetValidDisparityROI 函数计算出视差图的有效区域，在有效区域外的视差值将被清零。
- **disp12MaxDiff：左视差图（直接计算得出）和右视差图（通过cvValidateDisparity计算得出）之间的最大容许差异**。超过该阈值的视差值将被清零。该参数**默认为 -1**，即不执行左右视差检查。int 型。注意在程序调试阶段最好保持该值为 -1，以便查看不同视差窗口生成的视差效果。具体请参见《[使用OpenGL动态显示双目视觉三维重构效果示例](http://www.opencv.org.cn/forum/viewtopic.php?f=1&t=10757)》一文中的讨论。
在上述参数中，对视差生成效果影响较大的**主要参数是 SADWindowSize、numberOfDisparities 和 uniquenessRatio 三个，一般只需对这三个参数进行调整，其余参数按默认设置即可**。
在OpenCV2.1中，BM算法有C和C++ 两种实现模块。
**（2）StereoSGBMState**
SGBM算法的状态参数大部分与BM算法的一致，下面只解释不同的部分：
- **SADWindowSize：SAD窗口大小**，容许范围是[1,11]，**一般应该在 3x3 至 11x11 之间**，参数必须是奇数，int 型
- **P1, P2：控制视差变化平滑性的参数**。P1、P2的值越大，视差越平滑。P1是相邻像素点视差增/减 1 时的惩罚系数；P2是相邻像素点视差变化值大于1时的惩罚系数。**P2必须大于P1**。OpenCV2.1提供的例程 **stereo_match.cpp 给出了 P1 和 P2 比较合适的数值**。
- **fullDP：布尔值**，当设置为 TRUE 时，运行双通道动态编程算法（full-scale 2-pass dynamic programming algorithm），会占用O(W*H*numDisparities)个字节，对于高分辨率图像将占用较大的内存空间。**一般设置为 FALSE**。
**注意OpenCV2.1的SGBM算法是用C++ 语言编写的，没有C实现模块**。与H. Hirschmuller提出的原算法相比，主要有如下变化：
- 算法默认运行单通道DP算法，只用了5个方向，而fullDP使能时则使用8个方向（可能需要占用大量内存）。
- 算法在计算匹配代价函数时，采用块匹配方法而非像素匹配（不过SADWindowSize=1时就等于像素匹配了）。
- 匹配代价的计算采用BT算法（”[Depth Discontinuities by Pixel-to-Pixel Stereo](http://www.ces.clemson.edu/~stb/publications/p2p_iccv1998.pdf)” by S. Birchfield and C. Tomasi），并没有实现基于互熵信息的匹配代价计算。
- 增加了一些BM算法中的预处理和后处理程序。
**（3）StereoGCState**
GC算法的状态参数只有两个：**numberOfDisparities 和 maxIters** ，并且**只能通过 cvCreateStereoGCState 在创建算法状态结构体时一次性确定**，不能在循环中更新状态信息。GC算法并不是一种实时算法，但可以得到物体轮廓清晰准确的视差图，适用于静态环境物体的深度重构。
