# C/C++与Matlab混合编程初探 - 心纯净，行致远 - CSDN博客





2018年03月23日 13:52:36[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：640








==================================================================

**% 欢迎转载，尊重原创，所以转载请注明出处。**

[**% http://blog.csdn.net/bendanban/article/details/37830495**](http://blog.csdn.net/bendanban/article/details/37830495)

=======================分割线以下为正文================================


**     有一个通俗的比喻， 如果程序设计语言是车，那么C 语言就是全能手， C十十语言是加强版的C 语言， MATLAB 是科学家用来完成特殊任务的工具。作为使用MATLAB 的科学家和工程师， 通过混合程序设计，就可以借用CIC十十语言这两个全能手增强MATLAB 的功能；作为使用C/C十十语言开发的开发者，也可以通过混合程序设计来使用MATLAB强大的科学计算与数据可视化功能。**


Matlab 拥有丰富的功能，编程简单。不过，有些情况下，Matlab程序的执行速度比较慢。C/C++编译执行的程序速度比较快，编程难度上比Matlab要高一些。因此存在一种方案，就是使用Matlab实现我们的实验程序，用C/C++来实现Matlab程序中比较耗时的部分，从Matlab程序中调用C/C++的程序以实现加速。

**本文主要讲解如何在Matlab中调用C/C++的程序，以及如何编写可供Matlab程序调用的C/C++程序。**

     本文主要分以下几部分：

     Hello Matlab，用一个简单的例子来说明如何在Matlab中调用C/C++的程序，以及可供Matlab调用的C/C++程序应该注意哪些基本事项。

     Matlab调用C/C++程序传递参数。讨论在C/C++中是如何使用Matlab传来的参数的。

     Matlab与C/C++混合编程的方法论。给出在一般情况下使用Matlab与C/C++混合编程的方法步骤，让大家有一个较为清晰的应用此技术的思路。

     关于数据存储的说明。说明数据在Matlab中的存储方式。

**注意：**本文认为读者会使用Matlab，掌握C/C++语言，并且有一台计算机。计算机上安装有Windows的操作系统，操作系统上装有Matlab以及Visual Studio（比如VS2008，VS2010等）。或者计算机上安装有Linux的操作系统，系统上装有Matlab，GCC。




## hello Matlab

     我们一步步完成一个叫“Hello Matlab”的程序。

     第一步：在你的计算机D盘下，创建一个目录命名为HiMat。在D:\HiMat目录下创建一个文本文件，命名为“abhimat.cpp”。将Code 1中的代码拷贝到“abhimat.cpp”文件中，**保存**。(**注意**，这里建立目录以及命名等行为不是规定的，只是为了讲解方便)。



**[cpp]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- #include "mex.h"
- void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])  
- {  
-   mexPrintf("hello matlab in C/CPP.\n");  
- }  


Code 1, abhimat.cpp

     “abhimat.cpp”就是将要在Matlab中被调用的C/C++代码。

     第二步：在Matlab中编译“abhimat.cpp”。启动Matlab，进入D:\HiMat目录，在Matlab命令窗口中执行code 2中的命令，根据提示完成C/C++编译环境的配置。**注意**：如果你有多个编译器，建议选择最新的。



**[plain]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- mex -setup  


Code 2, 设置Matlab的C/C++编译环境


     完成配置后，在Matlab中执行Code 3的命令来编译abhimat.cpp。




**[plain]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- mex abhimat.cpp  


Code 3, 编译abhimat.cpp

    第三步：执行编译后的C/C++程序。在Matlab命令窗口输入“abhimat”或者”abhimat()”，都可以调用编译后的程序，推荐使用后者。

     细心的读者已经注意到了：
- 在Matlab中调用的C/C++函数名就是编译后以mex*（这里，*表示任意多个字符，例如mexw64）为后缀名的文件名。
- Matlab执行abhimat()命令后，实际执行的是mexFunction函数中的程序。



## Matlab调用C/C++程序传递参数




    此节我们讨论下，在供Matlab调用的C/C++程序中，我们是如何知道Matlab调用的参数类型、个数的。

    给出Matlab中调用C/C++程序的一个实例，如code 4所示。



**[plain]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- c = [1 2;3 4;5 6];  
- d = [1 1;1 1;1 1];  
- [a, b] = abfunc(c, d);  


Code 4, Matlab调用C/C++程序实例


    下面的工作是如何在当前目录下一个命名为abfunc.cpp的文件中实现mexFunction函数。在这个函数中如何获得Matlab命令中的c、d两个变量的值，如何返回a、b两个变量呢。

    注意mexFunction函数中的四个参数，一一作出说明：

    nlhs：mexFunction的第一个参数，它指示Matlab的调用命令中等号左侧有几个变量。例如，code 4中的调用，nlhs的值为2，因为它的等号左侧有两个变量，他们是a和b。

    plhs: mexFunction的第二个参数，它指示Matlab的调用命令中等号左侧变量的指针。例如，code 4中的调用，plhs[0]表示的是a，plhs[1]表示的是b。

    nrhs：mexFunction的第三个参数，它指示Matlab的调用命令中等号右侧的变量个数。例如，code 4中的调用，nrhs的值为2，因为它的等号右侧有两个变量，他们是c和d。

    prhs：mexFunction的第四个参数，它指示Matlab调用命令中等号右侧的变量指针。例如，code 4中的调用，prhs[0]表示的是c，prhs[1]表示的是d。

    mxArrary是一个不可见的数据类型，是Matlab定义的，大家只需要知道mxArrary的指针与Matlab中的变量一一对应就可以了。

**下面实现abfunc.cpp，功能是a=c+d; b = c-d;具体代码如code 5所示。**



**[cpp]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- #include "mex.h"
- void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])  
- {  
- double *p_c, *p_d;  
- double *p_a, *p_b;  
- 
- int c_rows, c_cols;  
- int d_rows, d_cols;  
- 
- int numEl;  
- int n;  
- 
-   mxAssert(nlhs==2 && nrhs==2, "Error: number of variables");  
- 
-   c_rows = mxGetM(prhs[0]);// get rows of c
-   c_cols = mxGetN(prhs[0]);// get cols of c
-   d_rows = mxGetM(prhs[1]);// get rows of d
-   d_cols = mxGetN(prhs[1]);// get cols of d
- 
-   mxAssert(c_rows==d_rows && c_cols==d_cols, "Error: cols and rows");  
- 
- // create output buffer
-   plhs[0] = mxCreateDoubleMatrix(c_rows, c_cols, mxREAL);  
-   plhs[1] = mxCreateDoubleMatrix(c_rows, c_cols, mxREAL);  
- 
- // get buffer pointers
-   p_a = (double*)mxGetData(plhs[0]);  
-   p_b = (double*)mxGetData(plhs[1]);  
-   p_c = (double*)mxGetData(prhs[0]);  
-   p_d = (double*)mxGetData(prhs[1]);  
- 
- // compute a = c + d; b = c - d;
-   numEl = c_rows*c_cols;  
- for (n = 0; n < numEl; n++)  
-   {  
-     p_a[n] = p_c[n] + p_d[n];  
-     p_b[n] = p_c[n] - p_d[n];  
-   }  
- }  


Code 5,  abfunc.cpp的实现


    说明一下code 5中用到的函数。这些函数大都以mx开头。mxAssert是断言，类似于C\C++中的assert。mxGetM获得Matlab传来的变量的行数，mxGetN获得Matlab传来的变量的列数。mxCreateDoubleMatrix创建一个2维的Matlab变量，形参分别用于指定变量的行数、列数、元素类型（mxREAL表示实数，mxCOMPLEX表示复数）。mxGetData用于获得内存中数据块的首地址。

    编译并测试Code 5中的代码，参见Code 6。



**[plain]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- mex abfunc.cpp  
- c = [1 2;3 4;5 6];  
- d = [1 1;1 1;1 1];  
- [a, b] = abfunc(c, d);  


Code 6, Code5的测试代码

    Code 6的输出结果如下：





**[plain]**[view plain](https://blog.csdn.net/bendanban/article/details/37830495#)[copy](https://blog.csdn.net/bendanban/article/details/37830495#)

- a =  
-      2     3  
-      4     5  
-      6     7  
- b =  
-      0     1  
-      2     3  
-      4     5  







## 关于数据存储的说明

   Matlab中的数据是按列存储的。例如，a=[1,2;3,4;5,6]，a的数据在内存中的存储顺序是：1、3、5、2、4、6。在C\C++中使用Matlab传来的变量时，**一定要注意数据的存储顺序**。




## Matlab与C/C++混合编程的方法论

鉴于@编程小手 的建议，添加一个关于使用此方法的方法论流程图。希望它能让各位明白在什么情况下可是使用这种混合编程的技术、如何一步步实现。参见图1的流程图：

![](https://img-blog.csdn.net/20140719170941410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmVuZGFuYmFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图 1、Matlab与C/C++混编方法论流程图            


