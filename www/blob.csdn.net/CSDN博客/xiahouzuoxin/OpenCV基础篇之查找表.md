# OpenCV基础篇之查找表 - xiahouzuoxin - CSDN博客





2014年07月31日 18:38:29[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6517
所属专栏：[OpenCV学习一步步](https://blog.csdn.net/column/details/zx-opencv.html)









## 程序及分析

/* * FileName : lookup_table.cpp * Author   : xiahouzuoxin @163.com * Version  : v1.0 * Date     : Sun 01 Jun 2014 04:35:37 PM CST * Brief    :  *  * Copyright (C) MICL,USTB */#include <cv.h>#include <highgui.h>usingnamespacestd;usingnamespacecv;#define QUAN_VAL1          (10)#define QUAN_VAL2          (100)voidCreateLookupTable(Mat&table,ucharquan_val){table.create(,,CV_8UC1);uchar*p=table.data;for(inti=;i<;++i){p[i]=quan_val*(i/quan_val);}}intmain(intargc,char*argv[]){Matimg;Matout1;Matout2;if(argc<){cout<<"Please input image file name."<<endl;return-;}img=imread(argv[],);Mattable;CreateLookupTable(table,QUAN_VAL1);LUT(img,table,out1);CreateLookupTable(table,QUAN_VAL2);LUT(img,table,out2);// Call OpenCV functionnamedWindow("Lena",CV_WINDOW_NORMAL);imshow("Lena",img);namedWindow("QUAN_VAL=10",CV_WINDOW_NORMAL);imshow("QUAN_VAL=10",out1);namedWindow("QUAN_VAL=100",CV_WINDOW_NORMAL);imshow("QUAN_VAL=100",out2);waitKey();return;}
- 
为什么要使用查找表？

一般的灰度图像有256个灰度级，而有时我们并不需要这么精确的灰度级（严重影响运算时间），比如黑白图像。这意味着我们以一个新的输入值划分当前的颜色空间，比如灰度到黑白色，将0~127灰度值直接赋值0，128~255赋值1，最终得到较少的黑白两色。查找表就扮演着这种降低灰度级而提高运算速度的角色。量化前和量化后的灰度值可以用下面的表达式表示：

![](https://camo.githubusercontent.com/71169beedd16dce3a70892bff08a609dea5caa1a/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c253230495f253742253543746578742537426e65772537442537443d25354366726163253742495f253742253543746578742537426f6c64253744253744253744253742512537442a51)

其中Q表示量化级别，如取10，表示：灰度值1-10用灰度值1表示，灰度值11-20用灰度值11表示，以此类推！

- 
使用上面公式和查找表有毛关系，当然没有，但上面公式代价就是要对图片遍历的每个灰度值都计算一次。因此，本着程序设计中的“以空间换时间”的基本算法策略，引入查找表，查找表就是将0~255个灰度值量化后的结果提前计算好存储在一个表中，上面程序中的CreateLookupTable就是完成该功能的函数。从程序中可以看出，查找表的数据结构定义为Mat类型。

- 
有了查找表后，要对图像中的像素灰度值进行替换，OpenVC提供了使用查找表直接计算出量化灰度级后图像的函数——LUT，其函数原型为：

```
//! transforms array of numbers using a lookup table: dst(i)=lut(src(i))
CV_EXPORTS_W void LUT(InputArray src, InputArray lut, OutputArray dst,
                      int interpolation=0);
```

注意哦，我们程序中定义的查找表类型为Mat，这里LUT的输入输出参数类型怎么是InputArray和OutputArray。在OpenCV中，InputArray/OutputArray这个接口类可以是Mat、Mat_、Mat_、vector、vector>、vector。

也就意味着当你看refman或者源代码时，如果看见函数的参数类型是InputArray/OutputArray型时，把上诉几种类型作为参数都是可以的。

另外，请注意，InputArray/OutputArray只能作为函数的形参参数使用，不要试图声明一个InputArray类型的变量，OutputArray也只是InputArray的派生。还有，还有就是OutputArrayOfArrays、InputOutputArray、InputOutputArrayOfArrays都是OutputArray的别名，碰到别慌张!


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E6%9F%A5%E6%89%BE%E8%A1%A8.md#%E6%95%88%E6%9E%9C)效果

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E6%9F%A5%E6%89%BE%E8%A1%A8/result.png)

江南好，千钟美酒，一曲满庭芳

左边是原始图片；中间是量化值Q=10时的结果，信息丢失很少；右边是量化值Q=100的图片，很多信息丢失了，但也别有一番风味。



