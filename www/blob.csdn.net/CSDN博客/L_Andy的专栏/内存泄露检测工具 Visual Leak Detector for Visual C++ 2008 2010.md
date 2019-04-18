# 内存泄露检测工具 Visual Leak Detector for Visual C++ 2008/2010 - L_Andy的专栏 - CSDN博客

2015年03月12日 11:33:08[卡哥](https://me.csdn.net/L_Andy)阅读数：1102
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



Visual Leak Detector是一款免费的检查内存泄露的工具。最新的2.1版本可以很好的支持VS2008/2010。

点击[这里](http://visualstudiogallery.msdn.microsoft.com/7c40a5d8-dd35-4019-a2af-cb1403f5939c)下载

安装后，在VS中设置Tool->Options 选择Projects and Solutions中的VC++ Directories，将Visual Leak Detector安装路径中的include和lib路径设置Include Files和Library Files中，如有必要可以将安装路径下bin里的相应dll拷贝至System32下。

创建一个简单的工程，其中指针b所指向的内存没有被正确地释放。

**[cpp]**[view
 plain](http://blog.csdn.net/hunter8777/article/details/6330531#)[copy](http://blog.csdn.net/hunter8777/article/details/6330531#)

- void leak()  
- {  
- int *b = newint[10];  
- for (int i = 0; i < 10; ++i)  
-     {  
-         b[i] = i;  
-     }  
- }  
- 
- 
- int main()  
- {  
-     leak();   
- return 0;  
- }  

include<vld.h>，运行就能在output窗口看到效果啦。

![](http://hi.csdn.net/attachment/201104/18/0_1303093597RgCf.gif)

输出的部分主要分为两块

Call Stack部分：

是泄露内存的调用堆栈，其中显示了泄露资源创建的位置，双击便可以定位到相应的代码部分。

Data部分：

即使泄露部分的内存内容。

注：在自己机器上VS2008上，用了下1.0和1.9b的版本，都出现了不同的问题（1.0版本不能定位泄露代码，1.9b没有正确检查出泄露）。2.1版本运行一切正常。

