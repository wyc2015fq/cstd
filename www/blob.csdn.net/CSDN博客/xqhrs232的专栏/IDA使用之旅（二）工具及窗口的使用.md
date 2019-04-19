# IDA使用之旅（二）工具及窗口的使用 - xqhrs232的专栏 - CSDN博客
2016年05月04日 10:33:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：638
个人分类：[反汇编/反编译/IDA](https://blog.csdn.net/xqhrs232/article/category/6212970)
原文地址::[http://blog.csdn.net/chenyujing1234/article/details/7766639](http://blog.csdn.net/chenyujing1234/article/details/7766639)
相关文章
1、[ida反汇编工具](http://www.cnblogs.com/newlist/archive/2012/07/05/2578237.html)----[http://www.cnblogs.com/newlist/archive/2012/07/05/2578237.html](http://www.cnblogs.com/newlist/archive/2012/07/05/2578237.html)
2、IDA实例教程----[http://wenku.baidu.com/link?url=MsgL1_d84M7WMfgMUXc808oElXJcqkjIywBgJrY_OuvT7tmxu6hUGi2tF1xW4Yv_tpduxVimOo3iUvtJ1-hHIT-qoHZT9RPMJL7y8US-oJC](http://wenku.baidu.com/link?url=MsgL1_d84M7WMfgMUXc808oElXJcqkjIywBgJrY_OuvT7tmxu6hUGi2tF1xW4Yv_tpduxVimOo3iUvtJ1-hHIT-qoHZT9RPMJL7y8US-oJC)
3、**1.2　反汇编静态分析工具IDA----[http://book.2cto.com/201307/27477.html](http://book.2cto.com/201307/27477.html)**
4、**1.3　反汇编引擎的工作原理----[http://book.2cto.com/201307/27478.html](http://book.2cto.com/201307/27478.html)**
转载请标明是引用于 [http://blog.csdn.net/chenyujing1234](http://blog.csdn.net/chenyujing1234)
欢迎大家拍砖!
#### 本系列内容是我根据“知其所以然论坛”博主录制的学习视频，做的笔记。
### 一、主要窗口
##### 1、显示函数调用图表
显示函数调用图表：
![](https://img-my.csdn.net/uploads/201207/20/1342766376_1394.jpg)
##### 2、显示当前函数的流程图
![](https://img-my.csdn.net/uploads/201207/20/1342766376_1394.jpg)
##### 3、窗口管理。
可以打开自己无意关闭的窗口，可以增加查看的标签选项：
![](https://img-my.csdn.net/uploads/201207/20/1342766898_3590.jpg)
或者重设窗口： 窗口->重设窗口
或者对好的窗口布局进行保存：窗口->保存
##### 4、查看
选项->常规
![](https://img-my.csdn.net/uploads/201207/20/1342767099_8326.jpg)
##### 5、数据窗口
包括String和Names
![](https://img-my.csdn.net/uploads/201207/20/1342767328_8019.jpg)
##### 6、交叉引用
在出现XREF 的地方就是有交叉引用（如下图）；
而在XREF后面的向上箭头，双击它可以跳到它跳转的地方（如下图）；
![](https://img-my.csdn.net/uploads/201207/21/1342838116_8132.jpg)
#####  7、IDA不支持撤销功能。
8、
Ctrl+滚轮实现浏览的放大和缩小。
#####  9、看懂图形概况图
如下图所示，绿色箭头表jz short loc_1305B指令成立时，跳到下面；
红色的方向箭头表示不成立时的执行方向；（红色其实是没有被执行的。）
蓝色是指指向下一个立即执行块。
（注意跳转后的地址也是loc_1305B，说明上一块的结束地址就是下一块的开始地址）。
![](https://img-my.csdn.net/uploads/201207/21/1342837183_9810.jpg)
##### 10、看懂代码中的地址
![](https://img-my.csdn.net/uploads/201207/21/1342837658_1265.jpg)
![](https://img-my.csdn.net/uploads/201207/21/1342837696_6242.jpg)
##### 11、添加注释
Enter+冒号 可以添加注释：
![](https://img-my.csdn.net/uploads/201207/21/1342837845_2397.jpg)
![](https://img-my.csdn.net/uploads/201207/21/1342837919_2470.jpg)
##### 12、名字窗口中表示的类别
F：可能是导入函数，也可能是自己写的函数；
I：导入函数；(I 类型的也可能是自己写的)，eg:
![](https://img-blog.csdn.net/20150410151828547)
A：字符串；
L：库函数；
D：全局的命令代码；
![](https://img-my.csdn.net/uploads/201207/21/1342838365_9813.jpg)
##### 13、函数中出现的标识含义
Sub_XXXXXX  
子程序
loc_xxxxxx
地址
byte_xxxxxx
8位数据
word_xxxxxx
16位数据
dword_xxxxxx
unk_xxxxxx
未知的
### 二、次要窗口
1、
 IDA View-A窗口与 Hex View-A窗口，一个是主要窗口一个是次要窗口，
它们其实是同步的，即选中其中一个窗口另一窗口位置也会发生变化。
![](https://img-my.csdn.net/uploads/201207/23/1343012540_6779.jpg)
设置上述功能是在此Hex View-A中右击：
![](https://img-my.csdn.net/uploads/201207/23/1343012678_4564.jpg)
##### 2、导出窗口
里面列出来的函数就是此文件的入口点。它是导出给用户使用的接口，通常是用户的共享库。
双击其中函数，会跳到反编译窗口的中位置。
可以对这里的函数设置断点：
![](https://img-my.csdn.net/uploads/201207/23/1343012938_1216.jpg)
##### 3、导入函数
从系统中导入的函数。如下图表DbgPrint从ntoskrnl导出的。
![](https://img-my.csdn.net/uploads/201207/23/1343013023_6729.jpg)
如果双击函数，可以看到它调用到了反编译窗口中的idata部分：
![](https://img-my.csdn.net/uploads/201207/23/1343013228_5260.jpg)
由于IDA是静态的工具，而ntoskrnl是静态库，所以能相入；如果遇到从DLL导入的名字，可能会出现乱码。
 -------------------------------
注意：Exports窗口只能列出此EXE或DLL通过.lib链接到的库的接口：
![](https://img-blog.csdn.net/20150410152240469)
##### 4、函数窗口
窗口中的函数的属性中的R，表采用EBP寄存器。我们以DbgPrint中的R属性为例，双击它跑到反汇编的地方是有 ebp的。
![](https://img-my.csdn.net/uploads/201207/23/1343013542_5755.jpg)
##### 5、结构体窗口
双击可以查看到结构的成员。
![](https://img-my.csdn.net/uploads/201207/23/1343013995_3956.jpg)
6、枚举窗口
类似于结构体类型。
### 三、其它窗口
##### 1、Segmentation
![](https://img-my.csdn.net/uploads/201207/27/1343399079_8963.jpg)
![](https://img-my.csdn.net/uploads/201207/27/1343399207_6952.jpg)
##### 2、标签
因为我们的驱动是DDK编译的，没识别也是正常的。
IDA通过签名库来支持代码块，签名是用于识别编译器生成的代码的启动顺序，以确定结构能给二进制编译器；
也可以将代码规划，由编译器的插件插入已知库，在IDA识别你认识的库时，会更多地将精力放在IDA无法识别的库。
##### 3、类型库
![](https://img-my.csdn.net/uploads/201207/27/1343399584_6008.jpg)
IDA可以从头文件中了解到结构的大小和布局，所有的信息都收集在til文件![](https://img-my.csdn.net/uploads/201207/27/1343399653_7335.jpg)中
如果要让IDA加入其他库和头文件，那么在Type Libraries中按"Insert"键来加入。
##### 4、函数调用窗口
打开函数调用窗口时，IDA会显示光标所在的函数的邻近，且生成一结图。
![](https://img-my.csdn.net/uploads/201207/27/1343400416_3985.jpg)
红色为被调用的函数。
##### 5、问题窗口
这个窗口也少用到。
显示在二进制遇到的一些困难，虽然反汇编最简单的二进制文件也是困难的。
![](https://img-my.csdn.net/uploads/201207/27/1343400545_3927.jpg)
如上图所示表示：dd 6Dh dup(0) 这条指令，IDA它不知道，需要你来分析，它发生的地址是INIT:0001404C。
