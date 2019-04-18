# VS2010配置OpenCV - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年07月17日 11:00:57[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2550标签：[2010																[build																[include																[image																[video																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[C/C++](https://blog.csdn.net/fjssharpsword/article/category/6480441)





1.下载OpenCV并安装
[http://www.opencv.org.cn/download/](http://www.opencv.org.cn/download/)

   下载必须superpack.exe，本例下载版本为：OpenCV-2.3.0-win-superpack.exe

   下载后解压到任意目录，本例解压到D:\OpenCV2.3；

    同时安装好VS2010；

2.设置OpenCV环境变量

   计算机->（右键）属性->高级系统设置->高级（标签）->环境变量：

  1)新建环境变量opencv=D:\OpenCV2.3\build；

  2)为path新增路径：%opencv%\x86\vc10\bin；

  重启或注销计算机使配置生效；

3.打开VS2010，新建Visual C++下Win32控制台应用程序demo，后面工程步骤默认即可；


  // demo.cpp : 定义控制台应用程序的入口点。

  #include "stdafx.h"

  #include "highgui.h"



  int _tmain(int argc, _TCHAR* argv[])

  {

      const char* imagename = "D:/demo/demo.jpg"; 

      cv::Mat img = cv::imread(imagename);

      if(img.empty())return -1; //是否加载成功

      if(!img.data)return -1;

      cv::namedWindow("image", CV_WINDOW_AUTOSIZE);

      cv::imshow("image", img);

      cv::waitKey();

      return 0;

  }

4.配置工程demo的OpenCV依赖

  1)demo项目->属性->配置属性->VC++目录->包含目录：

     新增=D:\OpenCV2.3\build\include;

             =D:\OpenCV2.3\build\include\opencv;

             =D:\OpenCV2.3\build\include\opencv2;

  2)demo项目->属性->配置属性->VC++目录->库目录：

     新增=D:\OpenCV2.3\build\x86\vc10\lib;

5.配置工程demo的链接器

  1)demo项目->属性(配置=debug)->配置属性->链接器->输入->附加依赖项：

    新增=opencv_core230d.lib;opencv_highgui230d.lib;opencv_video230d.lib;opencv_ml230d.lib;opencv_legacy230d.lib;opencv_imgproc230d.lib

  2)demo项目->属性(配置=release)->配置属性->链接器->输入->附加依赖项：

    新增=opencv_core230.lib;opencv_highgui230.lib;opencv_video230.lib;opencv_ml230.lib;opencv_legacy230.lib;opencv_imgproc230.lib

  3)如需调用opencv其他DLL中的方法，在D:\OpenCV2.3\build\x86\vc10\bin目录下，将相应DLL文件引用到项目中即可；

6.编译生成demo并执行；](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)




