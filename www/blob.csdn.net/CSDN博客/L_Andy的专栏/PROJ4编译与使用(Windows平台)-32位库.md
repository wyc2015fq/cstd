# PROJ4编译与使用(Windows平台)-32位库 - L_Andy的专栏 - CSDN博客

2017年10月05日 18:53:21[卡哥](https://me.csdn.net/L_Andy)阅读数：424


最近一直忙于做项目，每天也在码代码，所以就没有每天一发编程联系，目前项目大的框架已写好，在这里总结一下项目中的一点东西。

做的是遥感影像的项目，要用的坐标的转换，我不是遥感专业的，所以对于一些概念的理解不那么准确。因为大框架用的是GDAL库，查找资料后，确定了其中的坐标转换，即地理坐标与投影坐标的转换，要用的proj.4库。

Proj.4库是开源GIS最著名的地图投影库，著名的开源图像库gdal中的投影转换函数也是动态调用该库函数的，目前的最新版本是proj-4.9.2。proj4库的下载地址是：[https://github.com/OSGeo/proj.4](https://github.com/OSGeo/proj.4)

下载解压后，编译方式跟GDAL库的基本一样，命令行的选取请查看之前文章中的特别说明部分。

1、库目录的设置

在proj4目录下，编辑nmake.opt文件，修改INSTDIR的值即可。 如果不修改库的生成路径。编译完后，会默认值在C盘的根目录下，创建PROJ文件夹，里面有四个文件夹，分别是bin，lib，include以及share四个文件夹，其中include和lib是用来做二次开发使用，bin存放的是dll和exe文件，share里面存储的是PROJ4所定义的一些投影文件等，在发布程序的时候，share文件夹需要一同进行发布，否则在做投影转换的时候可能因为找不到其中的文件而导致转换失败。 

2、根据本机情况编译 

首先配置vs到用户环境变量 
![这里写图片描述](https://img-blog.csdn.net/20171005185016195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在开始种输入cmd 
![这里写图片描述](https://img-blog.csdn.net/20171005185120856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171005184943655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后按压缩包中的readme，执行命令： 

执行脚本in\VCVARS32.BAT(这个很关键，否则会编译失败，找不到相应函数解析) 

          nmake /f makefile.vc 

          nmake /f makefile.vc install-all

2、32位RELEASE版proj4库的编译

打开VS的32位命令行工具，然后将其工作目录切换到proj4的目录，然后输入  

nmake /f makefile.vc  

nmake /f makefile.vc install-all

3、32位DEBUG版proj4库的编译

打开VS的x86命令行工具，然后将其工作目录切换到proj4的目录，然后输入  

nmake /f makefile.vc  

nmake /f makefile.vc install-all DEBUG=1

4、64位proj4库的编译

打开VS的64位命令行工具，然后将其工作目录切换到proj4的目录，然后输入  

nmake /f makefile.vc  

nmake /f makefile.vc install-all

5、在VS中调用

将生成的文件中include和lib库分别包含到工程文件中，添加proj.lib和proj_i.lib，并且把bin下的dll文件拷到工程目录下面。

6、测试栗子

```cpp
#include <proj_api.h>
#include <iostream>

int main(int argc, char **argv)
{
    projPJ pj_merc, pj_latlong;
    double x, y;

    if (!(pj_merc = pj_init_plus("+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs")))
    exit(1);
    if (!(pj_latlong = pj_init_plus("+proj=longlat +datum=WGS84 +no_defs")))
    exit(1);

    x = -9.866554;
    y = 7.454779;
    x *= DEG_TO_RAD;
    y *= DEG_TO_RAD;
    pj_transform(pj_latlong, pj_merc, 1, 1, &x, &y, NULL);

    std::cout.precision(12);
    std::cout << "(" << x << " , " << y << ")" << std::endl;

    return 0;
}
```

