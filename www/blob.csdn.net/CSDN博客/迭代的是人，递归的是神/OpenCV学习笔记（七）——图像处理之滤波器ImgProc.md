# OpenCV学习笔记（七）——图像处理之滤波器ImgProc - 迭代的是人，递归的是神 - CSDN博客





2011年10月26日 14:31:19[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：20849
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









先介绍几个最基本的核滤波器相关的类

2D图像滤波器基础类BaseFilter：dst(x,y) = F(src(x,y), src(x+1,y)... src(x+wdith-1,y), src(y+1,x)... src(x+width-1, y+height-1) ); 相关的调用函数为getLinearFilter、getMorphologyFilter

单行核滤波器基础类BaseRowFilter：dst(x,y) = F(src(x,y), src(x+1,y),...src(x+width-1,y));相关的调用函数为getLinearRowFilter、getMorphologyRowFilter

单列核滤波器基础类BaseColumnFilter：dst(x,y) = F(src(x,y), src(x,y+1),...src(x,y+width-1));相关的调用函数为getColumnSumFilter、getLinearColumnFilter、getMorphologyColumnFilter

类FilterEngine：该类可以应用在对图像的任意滤波操作当中，在OpenCV滤波器函数中扮演着很重要的角色，相关的函数有createBoxFitler、createDerivFitlter、createGaussianFilter、createLinearFilter、createMorphologyFilter、createSeparableLinearFilter

基于这些类有一些基本的滤波器bilateralFilter、blur、boxFilter

还有一些形态学操作如：dilate、erode、morphologyEx

还有基于核和图像卷积的滤波器filter2D

还有一些典型的滤波器如GaussianBlur、medianBlur、Laplacian、pyrMeanShiftFiltering、sepFilter2D

还有Sobel、Scharr运算符

其他一些函数有borderInterpolate、buildPyramid、copyMakeBorder、createBoxFilter、createDirivFilter、createGaussianFliter、createLinearFilter、createMorphologyFilter、createSeparableLinearFilter、getDerivKernels、getGaussianKernel、getKernelType、getStructuringElement、pyrDown、pyrUp

还老版本的滤波器cvSmooth

这里介绍一下我使用Laplacian滤波的心得，这个函数的第三个参数为输出的图像的深度，注意经过拉普拉斯算子处理后得到的值是有正有负的，所以输出图像的深度最好为输入图像深度的2倍，才能有效防止数据溢出，如必须要使用8位的数据，可以再使用函数convertScaleAbs处理。而且要注意使用的拉普拉斯算子掩膜的中心系数为负。




