# OpenCV基础篇之像素操作对比度调节 - xiahouzuoxin - CSDN博客





2014年07月30日 12:39:01[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4386
所属专栏：[OpenCV学习一步步](https://blog.csdn.net/column/details/zx-opencv.html)









## 程序及分析

/* * FileName : contrast.cpp * Author   : xiahouzuoxin @163.com * Version  : v1.0 * Date     : Tue 29 Jul 2014 08:09:57 PM CST * Brief    :  *  * Copyright (C) MICL,USTB */#include <cv.h>#include <highgui.h>#include <iostream>usingnamespacecv;usingnamespacestd;intmain(intargc,char*argv[]){floatalpha=;intbeta=;Matimage;Matresult;if(argc<){cout<<"Usage:./contrast [image_name]"<<endl;return-;}image=imread(argv[]);if(!image.data){cout<<"Read image error."<<endl;}cout<<"Enter alpha value[1-3]:"<<endl;cin>>alpha;cout<<"Enter beta value[0-100]:"<<endl;cin>>beta;result.create(image.size(),image.type());intn=image.rows*image.cols*image.channels();uchar*p=image.data;uchar*q=result.data;for(inti=;i<n;i++){*q++=saturate_cast<uchar>(alpha*(*p++)+beta);}namedWindow("origin",CV_WINDOW_AUTOSIZE);imshow("origin",image);namedWindow("result",CV_WINDOW_AUTOSIZE);imshow("result",result);waitKey();return;}
- 用到Mat::create成员方法用于对Mat对象的数据赋值
- 
saturate_cast在operations.hpp中定义和声明，这里的传人参数为float类型，调用的是

```
template<> inline uchar saturate_cast<uchar>(float v)
{int iv = cvRound(v); return saturate_cast<uchar>(iv);}
```

其中调用重载函数saturate_cast(int)完成，该函数的实现是：

```
template<> inline uchar saturate_cast<uchar>(int v)
{return (uchar)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0)}
```

由于?:运算符是右结合特性，saturate_cast函数的作用就是：限制数据范围为0~255，超过255的值取255，小于0的值取0

- 
像素操作：这里没有使用ptr指针（因为操作不用考虑像素的具体x、y坐标，是单像素到单像素的映射），当然使用ptr也可以。Mat的data成员是数据头指针，程序通过指针*p与*q索引图片中的元素进行操作赋值


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E5%83%8F%E7%B4%A0%E6%93%8D%E4%BD%9C%E5%AF%B9%E6%AF%94%E5%BA%A6%E8%B0%83%E8%8A%82.md#%E6%95%88%E6%9E%9C)效果

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E5%83%8F%E7%B4%A0%E6%93%8D%E4%BD%9C%E5%AF%B9%E6%AF%94%E5%BA%A6%E8%B0%83%E8%8A%82/result.png)

左边为元素图像，右边为alpha=1，Beta=50时的增强亮度后的图像。



