# matlab out of memory 的问题 - 家家的专栏 - CSDN博客





2012年10月15日 19:27:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：873标签：[matlab																[variables																[unix																[windows																[存储																[microsoft](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)





# **MATLAB "Out of memory"问题**



来自[http://blog.sina.com.cn/s/blog_4b892b790100m4hd.html](http://blog.sina.com.cn/s/blog_4b892b790100m4hd.html)

 首先，我要声明，matlab自带的Help才是最权威的Matlab学习资料，如果有时间好好学习一下或是可以高效的使用的话，一定受益匪浅！比如说像Out of Memory这个问题，最开始我都是用Help memory，几乎得不到任何信息；然后就是去网上搜索此类问题的解决方法，一般有这几种：

    除了升级内存和升级64位系统外，下面几个方法也是解决之道。

> 
# 增加虚拟内存  ％％试了，不行

# 采用PACK （在命令行输入 pack 整理内存空间） ％％ pack命令不能用在程序中
# 采用3GB 开关启动系统(修改 c盘根目录 boot.ini 启动选项加上 /3G 例如：multi(0)disk(0)rdisk(0)partition(1)\WINDOWS="Microsoft Windows XP Professional" /noexecute=optin /fastdetect /3G

                       ％％ 试了， 不行

# 优化程序，减少变量 （使用稀疏矩阵 sparse ） save 保存变量 load 变量，需要时再读出来

# 如果必有必要，不要启动java虚拟机，采用matlab -nojvm启动（在快捷方式属性里的 "..../matlab.exe"）改为（"...../matlab.exe" - nojvm）

#关闭Matlab Server 

# 使用 单精度 single 短整数替代 双精度。 **％％这个管用！！！！！！![MATLAB <wbr>'Out <wbr>of <wbr>memory'问题(zz)](http://www.sinaimg.cn/uc/myshow/blog/misc/gif/E___6743EN00SIGG.gif)**


    现在看来这些解决方法对于我的问题来说根本就是“治标不治本”，不能解决实际问题.重申――Matlab的Help才是Matlab的最权威的指导材料！！！

    其实我的所谓原创就是对Help out of memory的一些归纳总结而已，有兴趣的话大家可以自己去看！！！
**问题一：Matlab是如何存储矩阵的**    Matlab中矩阵是以Block，也就是块的形式存储的。也就是说，当Matlab在为即将存储的矩阵划分块时，如果没有相应大小的连续内存，即使实际内存没有被完全使用，他还是会报告“Out of Memory”。

**问题二：如何高效使用Memory**    由于在使用的过程中，由于存储单元的不断的被分配和清除，内存会被分割成不连续的区域，这是很容易造成“Out of Memory”。

**1．为矩阵变量预制内存而不是动态分配**    动态分配的过程中，由于开始Matlab所用的Block随着矩阵的增大而连续的为此矩阵分配内存，但是由于Block的不连续性，很有可能最开始分配的Block不能满足存储的需要，Matlab只好移动此Block以找到更大的Block来存储，这样在移动的过程中不但占用了大量的时间，而且很有可能它找不到更大的块，导致Out of Memory。而当你为矩阵变量预制内存时，Matlab会在计算开始前一次性找到最合适的Block，此时就不用为变量连续的分配内存。

    比较下面两个程序：

> 
for k = 2:1000

x(k) = x(k-1) + 5;

end

x = zeros(1, 1000);

for k = 2:1000

x(k) = x(k-1) + 5;

end


    显然，第二个更好！！！最好的方法是，在程序一开始就位所有大的矩阵变量预制存存储单元！！！

    尽量早的分配大的矩阵变量

    Matlab使用heap method[管理内存](http://wwv.renren.com/xn.do?ss=10791&rt=1)。当在Matlab heap中没有足够的内存使用时，它会向系统请求内存。但是只要内存碎片可以存下当前的变量，Matlab会重新使用内存。

    比如：

> 
a = rand(1e6,1);

b = rand(1e6,1);


  使用大约15.4 MB RAM

> 
c = rand(2.1e6,1);使用近似16.4 MB RAM:

a = rand(1e6,1);

b = rand(1e6,1);

clear

c = rand(2.1e6,1);


    使用32.4 MB RAM。因为Matlab不能使用a、b被clear的空间，因为它们均小于2.1 MB，而同时它们也很可能是不连续的。

    最好的方法：

> 
c = rand(2.1e6,1);

clear

a = rand(1e6,1);

b = rand(1e6,1);


    使用16.4 MB RAM


**2．尽量避免产生大的瞬时变量，当它们不用的时候应该及时clear。3．尽量的重复使用变量**（跟不用的clear掉一个意思）
**4．将矩阵转化成稀疏形式**    如果矩阵中有大量的0，最好存储成稀疏形式。稀疏形式的矩阵使用内存更少，执行时间更短。

例如：1000×1000的矩阵X，它2/3的元素为0，使用两种存储方法的比较：

> 
Name          X                   Y

Size      1000x1000           1000x1000

Bytes       8000000             4004000

Class    double array      double array (sparse)


**5．使用pack命令**

    当内存被分为很多碎片以后，其实本身可能有很大的空间，只是没有作构的连续空间即大的Block而已。如果此时Out of Memory，此时使用pack命令可以很好的解决此问题。
**6．如果可行的话，将一个大的矩阵划分为几个小的**矩阵，这样每一次使用的内存减少。
**7．增大内存**



**问题三： Increase the size of the swap file.**

    wap space的设置与使用的操作系统有关，具体的如下：
**1. UNIX**    Information about swap space can be procured by typing pstat -s at the UNIX command prompt. For detailed information on changing swap space, ask your system administrator.
**2. Linux**    Swap space can be changed by using the mkswap and swapon commands. For more information on the above commands, type man followed by the command name at the Linux prompt.
**3. Windows NT**    Follow the steps shown here: Right-click the My Computer icon, and select Properties. Select the Performance tab and click the Change button to change the amount of virtual memory.
**4.Windows 2000**    右键“我的电脑”－>属性－>高级－>性能－>设置，从而改变其虚拟内存。
**5. Windows XP**    右键“我的电脑”－>属性－>高级－>性能－>设置，从而改变其虚拟内存。   
％％ 试了， 不行


**问题四：尽量少时用系统资源（对于Windows）**    Windows中字体、窗口等都是要占用系统资源的，所以在Matlab运行时尽量不要打开不用的窗口。


**问题五：Reloading Variables on UNIX Systems**    由于我不使用UNIX系统，这里不介绍，直接从Help中粘过来

    On UNIX systems, MATLAB does not return memory to the operating system even after variables have been cleared. This is due to the manner in which UNIX manages memory. UNIX does not accept memory back from a program until the program has terminated. So,
 the amount of memory used in a MATLAB session is not returned to the operating system until you exit MATLAB.

To free up the memory used in your MATLAB session, save your workspace variables, exit MATLAB, and then load your variables back in.](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




