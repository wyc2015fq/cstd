# 在IAR环境下调试CC2510二疑惑 - xqhrs232的专栏 - CSDN博客
2015年04月30日 11:41:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1148
原文地址::[http://blog.gkong.com/jili1986729_110437.ashx](http://blog.gkong.com/jili1986729_110437.ashx)
相关文章
1、IAR Embedded Workbench 不能调试的问题----[http://wenku.baidu.com/link?url=W3b6DxljMLdSC7DvBwattDvNDd32MDzRS8rjaE_4LwxQeg9_lYg4CDVQADTa6POw4zGON7ap1MVcIJaE77ulKipVmLs-YDTyLZ7vXvxs7N3](http://wenku.baidu.com/link?url=W3b6DxljMLdSC7DvBwattDvNDd32MDzRS8rjaE_4LwxQeg9_lYg4CDVQADTa6POw4zGON7ap1MVcIJaE77ulKipVmLs-YDTyLZ7vXvxs7N3)
2、 IAR调试出现The stack plug-in failed to set a breakpoint on "main". ……的解决方法----[http://bbs.csdn.net/topics/390393118](http://bbs.csdn.net/topics/390393118)
3、IAR无法仿真调试解决办法----[http://download.csdn.net/detail/bianyuanshuren/7889359](http://download.csdn.net/detail/bianyuanshuren/7889359)
IAR是指瑞典IAR公司推出的IAR Embedded Workbench软件；IAR EW是一个能够支持多种微处理器开发的集成开发环境，针对用户特定的目标系统，用创建项目的方式来进行管理和开发，其项目管理功能强大，用户只要创建一个工作区（Workspace），就可以在其中开发一个或多个项目。
在使用IAR调试CC2510过程中遇到了两个问题：
一、总出现无法编译错误
提示:Fatal Error[Pe005]: could not open source file "string.h"
提示有严重问题：无法打开string.h文件！
用Windows搜索功能搜索到string.h文件，并且拷到项目文件夹内，还是不行。初步判断IAR的编译程序找不到string.h文件，说明路径是不对。按“ALT+F7”进入“Options
 for node”,点左边的“C/C++ Compiler”,再点右边的标签“Preprocessor”如下图：
![](http://wbymcs51.blog.bokee.net/userfilemodule/download.do?action=reference&id=2020599&bokeeName=wbymcs51)
问题就出在这里：在Include paths框内包含文件的路径设置不对：
$TOOLKIT_DIR$\INC\
该句话就是指定包含文件的路径；
其中“$TOOLKIT_DIR$ ” 的意思是：包含文件的路径在IAR安装路径的8051文件夹下，对于我的电脑IAR安装在C盘，那么具体就表示它就表示:
$TOOLKIT_DIR$相当于：
C:\Program Files\IAR Systems\Embedded Workbench 4.05 Evaluation version\8051
$TOOLKIT_DIR$\INC\相当于：
C:\Program Files\IAR Systems\Embedded Workbench 4.05 Evaluation version\8051\INC\
可是在INC目录下找不到string.h文件，这就是问题所在。在INC目录下又有两个文件夹：CLIB和DLIB，而string.h文件在CLIB文件夹内；将Include
 paths框内改为：$TOOLKIT_DIR$\INC\CLIB\；编译又出现错误提示：
Fatal Error[Pe005]: could not open source file "iocc2510.h"
将Include paths框内改为：
$TOOLKIT_DIR$\INC\
$TOOLKIT_DIR$\INC\CLIB\；
编译正常！
另外，$PROJ_DIR$的意思是包含文件指向工程目录的上一级目录中，例如：我们的工程文件为：D: \DESIGN\WIRELESS\CC2510; 那么$PROJ_DIR$就表示：D:
 \DESIGN\WIRELESS\；了解这些，我们就可灵活配置。
二、指令总是判断错误
看以下程序：
#include <ioCC2510.h>
#define uint unsigned int
#define uchar unsigned char
#define led1   P1_0                  //控制红LED
#define led2  P1_1                  //控制绿LED
**uint TempFlag;**              //用来标志是否要闪烁
/***************************
//主函数
***************************/
void main()
{
      // uint TempFlag;                       //AA句
      while(1) 
      {
             TempFlag = 5;                   //A句
              if(5 == TempFlag)              //B句
             {
                   led1 = !led1;                 //C句
                    TempFlag = 0;
                    Delay(600);
               }
               else
               {
                    Led2 = !led2;                //D句
Delay(600);
  }
        }
  }
看以上程序，尽管有A句-- TempFlag = 5，但是B句--if(5 == TempFlag) 判断总是不成立，C句总不执行；将A句--TempFlag
 = 5改了好多数字都没有改观。若将变量TempFlag定义在AA句处，改为局部变量就正常了；但是其他地方也要用变量TempFlag，所以这样改不行；查看汇编程序，发现if(5
 == TempFlag)语句对应的汇编指令用MOVX，也就是变量TempFlag被定义在了外部RAM；可线路板上并未扩展外部RAM；那么TempFlag的数据很可能是0XFF，将A句改为：TempFlag
 = 0xff，C句就可执行了。可能是IAR设置不合适：按“ALT+F7”进入“Options for node”,点左边的“General
 Options”,再点右边的标签“Target”,如下图：
![](http://wbymcs51.blog.bokee.net/userfilemodule/download.do?action=reference&id=2020600&bokeeName=wbymcs51)
可见：Data model栏设置不对，改为：small就正常了。[wbymcs51.blog.bokee.net](http://wbymcs51.blog.bokee.net/)
      通过以上两例，我们可以看出，IAR的环境设置比较重要。在使用IAR或KEIL C时，不能仅仅按有些简单的说明步骤去做，一定要清楚每个设置是在做什么，为什么要这样做，才会心中有数。我们觉得徐爱钧老师的两本书讲的比较详细：《单片机高级语言C51 Windows 环境编程与应用》和《IAR
 EWARM嵌入式系统编程与实践》，讲到了编译、汇编和连接的相关命令。其实，以上界面的每个设置都对应增减一条命令代码或在某个命令中增减一个参数项，通过界面设置和命令代码对比，就会知其然，更知其所以然。
[阅读(3181)](http://blog.gkong.com/jili1986729_110437.ashx)[评论(0)](http://blog.gkong.com/jili1986729_110437.ashx#comment)
