# OpenCV基础篇之使用CMake管理工程 - xiahouzuoxin - CSDN博客





2014年07月30日 12:35:13[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：5419
所属专栏：[OpenCV学习一步步](https://blog.csdn.net/column/details/zx-opencv.html)









## 程序及分析

/* * FileName : read.cpp * Author   : xiahouzuoxin @163.com * Version  : v1.0 * Date     : Tue 13 May 2014 07:34:25 PM CST * Brief    :  *  * Copyright (C) MICL,USTB */#include <cv.h>#include <highgui.h>usingnamespacecv;usingnamespacestd;intmain(intargc,char**argv){Matimg;img=imread(argv[],);if(argc!=||!img.data){cout<<"No image data.\n"<<endl;return-;}namedWindow("Display Image",CV_WINDOW_AUTOSIZE);imshow("Display Image",img);waitKey();return;}
- 
这就是[OpenCV基础篇之读取显示图片](https://github.com/xiahouzuoxin/notes/blob/master/essays/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E8%AF%BB%E5%8F%96%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87.md)中使用的代码

- 
如果没有安装CMake，则使用下面命令安装

```
yum install cmake
```

- 
CMake是一款工程管理软件，比Makefile平台迁移性更好，通过CMake能自动生成Makefile。用于本程序的CMake文件内容如下：

```
project(read)
find_package(OpenCV REQUIRED)
add_executable(read read)
target_link_libraries(read ${OpenCV_LIBS})
```

将上述内容保存到文件名为CMakeLists.txt（必须是这个文件名）的文件中，依次执行

```
cmake ./
make
```

cmake命令直接生成Makefile，通过Makefile生成可执行文件（这也就是为什么cmake平台迁移性好，不同的平台能生成对应的Makefile，很容易迁移到Windows）。


## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E4%BD%BF%E7%94%A8CMake%E7%AE%A1%E7%90%86%E5%B7%A5%E7%A8%8B.md#%E6%95%88%E6%9E%9C)效果

![result](https://github.com/xiahouzuoxin/notes/raw/master/images/OpenCV%E5%9F%BA%E7%A1%80%E7%AF%87%E4%B9%8B%E4%BD%BF%E7%94%A8CMake%E7%AE%A1%E7%90%86%E5%B7%A5%E7%A8%8B/result.png)



