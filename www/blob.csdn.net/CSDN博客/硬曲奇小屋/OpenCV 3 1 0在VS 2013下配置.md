# OpenCV 3.1.0在VS 2013下配置 - 硬曲奇小屋 - CSDN博客





2017年10月09日 21:21:52[HardCookies](https://me.csdn.net/james_616)阅读数：211标签：[opencv																[visual studio](https://so.csdn.net/so/search/s.do?q=visual studio&t=blog)](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[c/c++																[杂项](https://blog.csdn.net/james_616/article/category/7127787)](https://blog.csdn.net/james_616/article/category/7108382)







要开始在Visual Stdio 2013上用OpenCV开发，为了能够直接支持VS 2013我们这里使用OpenCV 3.1.0。建议将VS 2013安装在C盘默认路径，否则会出现错误。 

 1. 新建项目 

在Visual C++下，选择Win32 控制台程序，项目名称为opencv_vc12（Visual Stdio 2013对应版本为vc12）。 
![这里写图片描述](https://img-blog.csdn.net/20171009211541842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 2. 拷贝相关文件 

在项目文件夹内新建文件夹opencv310，将OpenCV安装目录下build中的3个文件夹bin, include, x64拷贝至opencv310中。 

 3. 配置平台 

右键项目名称，打开属性，选择配置管理器。在活动解决方案平台下拉，点击新建，选择新平台x64。 
![这里写图片描述](https://img-blog.csdn.net/20171009211638174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 4. 添加包含目录、库目录。 

在属性—VC++ 目录中，对包含目录选择编辑，添加$(SolutionDir)\opencv310\include，点击确定。对库目录选择编辑，添加$(SolutionDir)\opencv310\x64\vc12\lib，C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\lib，点击确定。 
![这里写图片描述](https://img-blog.csdn.net/20171009211903343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

之后库目录继承的值会自动改为$(WindowsSdk_71A_LibraryPath_x64)，包含目录继承的值自动改为$(WindowsSdk_71A_IncludePath)，如下图所示。这是**关键**的步骤，否则会出现*error LNK1104: 无法打开文件“kernel32.lib”*
![这里写图片描述](https://img-blog.csdn.net/20171009211938080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20171009211954373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 5. 其他配置 

在属性—链接器—输入中，添加附加依赖项opencv310d.lib。子系统选择控制台，目标计算机选择MachineX64。在属性—配置属性—常规—平台工具集，改为Windows XP(v120_xp)。否则会出现无法运行“rc.exe”的错误。 

 6. 添加测试代码 

创建源文件main.cpp，添加如下代码
```
#include<opencv2/opencv.hpp>

using namespace cv;

int main(){
Mat picture = imread("test.jpg");
imshow("test", picture);
waitKey(0);
}
```

运行代码，显示结果如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20171009212044876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






