# OpenCV基础篇之图片叠加 - xiahouzuoxin - CSDN博客





2014年07月30日 12:36:04[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：5298
所属专栏：[OpenCV学习一步步](https://blog.csdn.net/column/details/zx-opencv.html)









## 程序及分析

/* * FileName : blend.cpp * Author   : xiahouzuoxin @163.com * Version  : v1.0 * Date     : Mon 28 Jul 2014 08:47:59 PM CST * Brief    :  *  * Copyright (C) MICL,USTB */#include <cv.h>#include <highgui.h>#include <stdio.h>usingnamespacestd;usingnamespacecv;intmain(intargc,char*argv[]){doublealpha=0.5;doublebeta;doubleinput;Matsrc1,src2,dst;// User input alphacout<<"Simple Linear Blender"<<endl;cout<<"---------------------"<<endl;cout<<"Enter alpha [0-1]: ";cin>>input;if(alpha>=&&alpha<=){alpha=input;}// Read imagessrc1=imread("../test_imgs/KeNan.jpg");src2=imread("../test_imgs/Background.jpg");if(!src1.data){cout<<"Error loading src1\n"<<endl;return-;}if(!src2.data){cout<<"Error loading src2\n"<<endl;return-;}// Create WIndowsnamedWindow("Linear Blend",);beta=(1.0-alpha);addWeighted(src1,alpha,src2,beta,0.0,dst);imshow("Linear Blend",dst);waitKey();return;}

这里的图像叠加指线性叠加，设图像A的像素为fA(x)，图像B的像素为fB(x)，叠加系数为α，则线性叠加操作为：

![](https://camo.githubusercontent.com/15b0b03843a9f037d6398b2d31d6fd6035b206c4/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c61726765253230672878293d253543616c706861253230665f412878292b28312d253543616c70686129665f42287829)

α为0-1之间的值，从上面也可以看出，叠加必需保证两幅输入图像的尺寸大小相同。程序中的

```
addWeighted(src1, alpha, src2, beta, 0.0, dst);
```

完成叠加操作。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E5%9B%BE%E7%89%87%E5%8F%A0%E5%8A%A0.md#%E6%95%88%E6%9E%9C)效果

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E5%9B%BE%E7%89%87%E5%8F%A0%E5%8A%A0/result.png)



