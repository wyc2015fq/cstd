# windows任务管理器中的工作设置内存,内存专用工作集,提交大小详解 - DoubleLi - 博客园






[](http://shashanzhao.com/)

![](http://shashanzhao.com/wp-content/uploads/2013/06/neicun.png)

虽然是中文字,但是理解起来还是很困难,什么叫工作设置内存,什么叫[内存专用工作集](http://shashanzhao.com/archives/tag/%e5%86%85%e5%ad%98%e4%b8%93%e7%94%a8%e5%b7%a5%e4%bd%9c%e9%9b%86),什么叫[提交大小](http://shashanzhao.com/archives/tag/%e6%8f%90%e4%ba%a4%e5%a4%a7%e5%b0%8f),区别是什么,让人看了一头雾水.



通俗的讲工作设置内存是程序占用的物理内存(包含与其他程序共享的一部分), 内存专用工作集是程序独占的物理内存, 提交大小是程序独占的内存(包含物理内存和在页面文件中的内存).

注:页面文件就是存放不在物理内存中的内存,文件路径一般在C:\pagefile.sys,目的是为了能够让更多的进程运行,即使合起来所占用的内存已经超过物理内存. 只要将其中一部分内存转入页面文件, 物理内存就可以空出来继续运行新的进程.

它们之间的关系:

工作设置内存 = 内存专用工作集 + 与其他进程共享的物理内存.

提交大小 = 内存专用工作集 + 保存在页面文件中的独占内存.

(其中有些例外,有时候内存专用工作集可能会比提交大小大一点点,不过可以不用考虑)



工作设置内存和内存专用工作集在程序不变化情况下会变化, 系统会考虑程序活动情况,物理内存剩余量等减少或增加物理内存.

提交大小在程序不变化情况下不会变化,因为程序就是占用了那么多专用内存.

如果要测试程序占用的内存大小,建议在程序加载完毕后记录提交大小.其他的会变化建议不要记录.



还想要了解更清楚?好孩子.

下面通过代码来详细了解.

首先windows任务管理器已经不够用了,需要用Process explorer和VMMap工具.

接下来解释一些名词:
|windows任务管理器中的称谓|Process explorer中的称谓|VMMap中的称谓|
|----|----|----|
|工作设置内存|Working set|Total WS|
|内存专用工作集|WS [Private](http://shashanzhao.com/archives/tag/private)|Private WS|
|提交大小|Private Bytes|Private|
|无对应选项可显示|Virtual Size|Size|
|无对应选项可显示|无对应选项可显示|[Committed](http://shashanzhao.com/archives/tag/committed)|



名词说明:

Virtual Size: 程序总的所使用的内存(包含共享,非共享,物理,页面,为程序保留的但未分配的内存)

Committed: Virtual Size减去为程序保留的但未分配的内存

为程序保留的但未分配的内存: 就是告诉系统我要一块内存,但暂时不用,不过分配的地址得给我,系统就给他一个不用的地址,但不分配内存,等程序申请要使用时,就从页面或物理内存中分配出来放在那个地址上.



关于页面文件和物理内存如何转换稍微讲解一下,当程序要访问某个地址, 系统发现这个地址不在物理内存里,就会产生中断,然后去读取页面文件,把页面文件中与内存相关的数据拷贝到物理内存,然后标记一下这个地址已经在物理内存中了,然后继续让程序运行.



现在来看个例子:

程序main.exe定义了一个5M的全局变量,5M的常量,并申请了一个5M的内存,程序代码大小为2M.



> 
char g_str[5*1024*1024];

const char STR[5*1024*1024];

void main()

{

char *p = (char*) HeapAlloc(GetProcessHeap(), 0, 1024*1024*5);

call_lib();

}




程序lib.dll定义了一个4M的全局变量,4M的常量,并申请了一个4M的内存,程序代码大小为1M.



> 
char g_str[4*1024*1024];

const char STR[4*1024*1024];

void call_lib()

{

char *p = (char*) HeapAlloc(GetProcessHeap(), 0, 1024*1024*4);

}


Main.exe程序调用了lib.dll



先看main.exe总的占用的内存.

Virtual Size = 5M的全局变量 + 5M的常量 + 5M的内存 + 2M的程序代码

+ 4M的全局变量 + 4M的常量 + 4M的内存 + 1M的程序代码

= 30M.



然后看main.exe独占的内存:

Private Bytes = 5M的全局变量 + 5M的内存

+ 4M的常量 + 4M的内存

=18M.

由上可以看出独占指的是只能进程自己使用, 比如代码,常量等就是共享的.

分配的内存,可变的全局或静态变量就是独占的.



至于Working set和WS Private,因为其为变化量,所以无法计算其值.



Working set物理内存变化规律:

一般情况下在分配出来内存后,物理内存只分配4K左右,等访问到超过4K的时候,会慢慢加大物理内存,等到系统总的物理内存小于一定值后,又会将不在前台的进程的物理内存减少.

如果进程想休息了, 可以调用SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

来将自己的一些内存从物理内存转移到页面文件.



测试环境: Win7 32位



参考:

VMMAP显示和process explorer不一样

[http://superuser.com/questions/185318/process-explorer-not-showing-the-biggest-user-of-my-ram](http://superuser.com/questions/185318/process-explorer-not-showing-the-biggest-user-of-my-ram)



VMMap内存详解

[http://www.cnblogs.com/georgepei/archive/2012/03/07/2383548.html](http://www.cnblogs.com/georgepei/archive/2012/03/07/2383548.html)









