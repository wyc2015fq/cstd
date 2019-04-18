# 解决Windows 程序界面闪烁问题的一些经验 - gauss的专栏 - CSDN博客
2013年01月04日 00:21:12[gauss](https://me.csdn.net/mathlmx)阅读数：237
119人阅读[评论](http://blog.csdn.net/pizi0475/article/details/7951866#comments)(0)收藏[举报](http://blog.csdn.net/pizi0475/article/details/7951866#report)
#pragma data_seg介绍
         用#pragma data_seg建立一个新的数据段并定义共享数据，其具体格式为： 
　　#pragma data_seg （"shareddata") 
　　HWND sharedwnd=NULL;//共享数据 
　　#pragma data_seg()   
-----------------------------------------------------------------------------------------------------
1，#pragma data_seg()一般用于DLL中。也就是说，在DLL中定义一个共享的，有名字的数据段。最关键的是：这个数据段中的全局变量可以被多个进程共享。否则多个进程之间无法共享DLL中的全局变量。
2，共享数据必须初始化，否则微软编译器会把没有初始化的数据放到.BSS段中，从而导致多个进程之间的共享行为失败。
3，你所谓的结果正确是一种错觉。如果你在一个DLL中这么写：
#pragma data_seg("MyData")
int g_Value; // Note that the global is not initialized.
#pragma data_seg()
DLL提供两个接口函数：
int GetValue()
{
      return g_Value;
}
void SetValue(int n)
{
      g_Value = n;
}
然后启动两个进程A和B，A和B都调用了这个DLL，假如A调用了SetValue(5); B接着调用int m = GetValue(); 那么m的值不一定是5，而是一个未定义的值。因为DLL中的全局数据对于每一个调用它的进程而言，是私有的，不能共享的。假如你对g_Value进行了初始化，那么g_Value就一定会被放进MyData段中。换句话说，如果A调用了SetValue(5); B接着调用int m = GetValue(); 那么m的值就一定是5！这就实现了跨进程之间的数据通信！
----------------------------------------------------------------------------------------------------
     有的时候我们可能想让一个应用程序只启动一次，就像单件模式(singleton)一样，实现的方法可能有多种，这里说说用#pragma data_seg来实现的方法，很是简洁便利。
应用程序的入口文件前面加上
#pragma data_seg("flag_data")
int app_count = 0;
#pragma data_seg()
#pragma comment(linker,"/SECTION:flag_data,RWS")
然后程序启动的地方加上
if(app_count>0)     // 如果计数大于0，则退出应用程序。
{
   //MessageBox(NULL, "已经启动一个应用程序", "Warning", MB_OK);   
   //printf("no%d application", app_count);
   return FALSE;
}
app_count++;
Windows 在一个Win32程序的地址空间周围筑了一道墙。通常，一个程序的地址空间中的数据是私有的，对别的程序而言是不可见的。但是执行STRPROG的多个执行实体表示了STRLIB在程序的所有执行实体之间共享数据是毫无问题的。当您在一个STRPROG窗口中增加或者删除一个字符串时，这种改变将立即反映在其它的窗口中。
在全部例程之间，STRLIB共享两个变量：一个字符数组和一个整数（记录已储存的有效字符串的个数）。STRLIB将这两个变量储存在共享的一个特殊内存区段中：
#pragma       data_seg ("shared")          int                  iTotal = 0 ;          WCHAR                szStrings [MAX_STRINGS][MAX_LENGTH + 1] = { '\0' } ;          #pragma       data_seg ()        第一个#pragma叙述建立数据段，这里命名为shared。您可以将这段命名为任何一个您喜欢的名字。在这里的#pragma叙述之后的所有初始化了的变量都放在shared数据段中。第二个#pragma叙述标示段的结束。对变量进行专门的初始化是很重要的，否则编译器将把它们放在普通的未初始化数据段中而不是放在shared中。
连结器必须知道有一个「shared」共享数据段。在「Project Settings」对话框选择「Link」页面卷标。选中「STRLIB」时在「Project Options」字段（在Release和Debug设定中均可），包含下面的连结叙述：
/SECTION:shared,RWS        字母RWS表示段具有读、写和共享属性。或者，您也可以直接用DLL原始码指定连结选项，就像我们在STRLIB.C那样：
#pragma comment(linker,"/SECTION:shared,RWS")        共享的内存段允许iTotal变量和szStrings字符串数组在STRLIB的所有例程之间共享。因为MAX_STRINGS等于256，而 MAX_LENGTH等于63，所以，共享内存段的长度为32,772字节－iTotal变量需要4字节，256个指针中的每一个都需要128字节
            
