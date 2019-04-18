# VS下控制台运行保持（不要一闪而过） - fighting！！！ - CSDN博客
2017年10月30日 10:00:24[dujiahei](https://me.csdn.net/dujiahei)阅读数：248
原文地址：
VS下控制台运行保持（不要一闪而过） - weixinhum - CSDN博客  http://blog.csdn.net/weixinhum/article/details/44233579
以前上课的时候是用VC++6.0来学习编程的，编完打印出来的东西就直接显示在控制台上。而在Visual Studio下会出现控制台一闪而过的情况。这个问题其实是很好解决的，方法有多种，下面列举两种常用的。
第一种
包含头文件
**[cpp]**[view
 plain](http://blog.csdn.net/weixinhum/article/details/44233579#)[copy](http://blog.csdn.net/weixinhum/article/details/44233579#)
- #include <stdlib.h>  
然后在程序的末尾加入
**[cpp]**[view
 plain](http://blog.csdn.net/weixinhum/article/details/44233579#)[copy](http://blog.csdn.net/weixinhum/article/details/44233579#)
- system("pause");  
它就停住了，其利用的是system这个函数来从程序里调用“pause”命令，而pause命令则是在控制台上输出一行类似于“请按任意键继续...”的字，等待用户按一个键，然后返回。这样做控制台就保持等待输入的状态而不会一闪而过了。其实只要愿意，可以以此类推用system函数来做很多事，比如用
**[cpp]**[view
 plain](http://blog.csdn.net/weixinhum/article/details/44233579#)[copy](http://blog.csdn.net/weixinhum/article/details/44233579#)
- system("再生薰衣草.bmp");  
这句话来代替上面的那一句或写在其前面，则程序运行完成之后会自动打开程序所在路径下的"再生薰衣草.bmp"这张图片，如果这张图片正是图像处理的结果，则会方便的多。
第二种
按Ctrl+F5进行调试，通常我们按Visual
 Studio平台上的运行按钮（F5为其快捷键）对于控制台程序而言是直接运行程序的EXE文件，而并没有进入调试状态。而按Ctrl+F5则是在CMD.EXE中运行EXE文件，在这种运行状态下为了用户能看到效果，程序便自行添加了system.pause效果停顿在那，也就相当于上面第一种方法介绍的效果了。
