# Just For Fun：在windows下模拟一个windows病毒软件（windows.h） - Koma Hub - CSDN博客
2018年04月24日 15:23:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：211
在windows下编程可以通过include  windows.h来调用windows控件，使用起来也是非常的方便。简单介绍几个函数。
***1.Sleep()进行倒计时***
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int main(int argc,char **argv)
{
    int i;
    printf("Ready! ");
    for (i = 10;i >= 0;i--)
    {
        printf("%5d\b\b\b\b\b", i);
        Sleep(100);
        /* following sentence same as "Sleep(100);" */
        //system("sleep 0.1");
    }
    printf("\nGO!\n");
        
    return 1;
}
```
运行：
```
D:\test>a.exe
Ready!     0
GO!
```
程序中注释的system也可以调用windows下的命令。
***2.MessageBoxA弹出窗口***
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int main(int argc,char **argv)
{
    /* 3 = 0~3 */
    MessageBoxA(0,"Panda burn virus on your computer!","Warning",3);
    return 1;
}
```
运行：
```
D:\test>gcc tmp.c
D:\test>a.exe
```
显示：
![](https://img-blog.csdn.net/20180424145227914)
其中函数的最后一个参数可以设为0~3，每一个数值对应的窗口不一样。
***3.ShellExecuteA运行一个软件***
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int main(int argc,char **argv)
{
    /* 0.隐藏,1.正常,3.最大化,6.最小化 */
    ShellExecuteA(0, "open","notepad", 0, 0, 1);
    system(" start 360se www.baidu.com");
    return 1;
}
```
运行：
```
D:\test>gcc tmp.c
D:\test>a.exe
```
结果就打开了记事本和百度主页：
![](https://img-blog.csdn.net/20180424145540897)
***4.生成一个无限循环的恶搞软件***
代码如下：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
int main(int argc,char **argv)
{
    int i=0;
    /* 3 = 0~3 */
    do{
        MessageBoxA(0,"Panda burn virus on your computer!","Warning",i++%3);
    }while(1);
    return 1;
}
```
编译：
```
D:\test>gcc tmp.c
D:\test>a.exe
```
我在[如何实现利用C语言代码封装成可以安装的windows软件？](https://blog.csdn.net/rong_toa/article/details/79761981)中介绍了怎么生成一个可以安装的软件，最后得到安装的软件。
首先导入可执行文件：
![](https://img-blog.csdn.net/20180424150928936)
按照步骤执行下一步（期间需要设定软件图标，安装信息等），直到finish：
![](https://img-blog.csdn.net/20180424151101741)
下图是从输入到软件安装包的输出：
![](https://img-blog.csdn.net/20180424151230716)
软件封装算是完成了。
**下面我们安装试试（挺好玩的）：**
第一步：双击Virus.exe图标
![](https://img-blog.csdn.net/201804241518510)
没错，我当然安装，点击下一步：
![](https://img-blog.csdn.net/20180424151909479)
为了不“中毒”我不安装在C盘，安装但在D盘，点击下一步：
![](https://img-blog.csdn.net/20180424151945366)
下一步：
![](https://img-blog.csdn.net/20180424152008860)
很开心，“病毒”安装上了。双击这个软件，就会无间断的跳出警告窗口，关掉一个，又会弹出一个，烦不胜烦：
![](https://img-blog.csdn.net/20180424152151716)
Just For Fun！
