# Matlab与C/C++混合编程接口及应用 - wishchinYang的专栏 - CSDN博客
2014年03月17日 16:16:57[wishchin](https://me.csdn.net/wishchin)阅读数：786
原始链接：[http://www.cnblogs.com/lidabo/archive/2012/08/24/2654148.html](http://www.cnblogs.com/lidabo/archive/2012/08/24/2654148.html)
原始链接：[http://blog.csdn.net/jtop0/article/details/7657227](http://blog.csdn.net/jtop0/article/details/7657227)
**Matlab****与C/C++混合编程接口及应用**
　　摘要：Matlab具有很强的数值计算和分析等能力,而C/C++是目前最为流行的高级程序设计语言,两者互补结合的混合编程在科学研究和工程实践中具有非常重要的意义。从Matlab调用C/C++代码及C/C++调用m文件两方面,深入地研究了它们之间混合编程的原理和实现机制,并且给出了具体条件下的混合编程方法和步骤。实验表明,给出的Matlab与C/C++混合编程[接口](http://www.dzsc.com/product/searchfile/5540.html)及应用方法是有效、实用的。
**1引言**
Matlab是当前应用最为广泛的数学软件，具有强大的数值计算、数据分析处理、系统分析、图形显示甚至符号运算等功能[1]。利用这一完整的数学平台，用户可以快速实现十分复杂的功能，极大地提高工程分析计算的效率[2][3]。但与其他高级程序[3]相比，Matlab程序是一种解释执行程序，不用编译等预处理，程序运行速度较慢[4]。
C/C++语言是目前最为流行的高级程序设计语言之一[5]。它可对操作系统和应用程序以及硬件进行直接操作，用C/C++语言明显优于其它解释型高级语言，一些大型应用软件如
 Matlab 就是用C语言开发的。
　　在工程实践中，用户经常遇到Matlab与C/C++混合编程的问题。本文基于Matlab
 6.5和VC6.0开发环境，在Windows平台下就它们之间的混合编程问题进行深入研究并举例说明。
**2  Matlab调用C/C++**
Matlab调用C/C++的方式主要有两种：利用MEX技术和调用C/C++动态连接库。
　　在Matlab与C/C++混合编程之前，必须先对Matlab的编译应用程序mex和编译器mbuild进行正确的设置[1]：
　　对Matlab编译应用程序mex的设置：Mex –setup.
　　对Matlab编译器mbuild的设置：Mbuild –setup.
**2.1调用C/C++的MEX文件**
MEX是Matlab Executable的缩写，它是一种“可在Matlab中调用的C(或Fortran)语言衍生程序”[6]。MEX文件的使用极为方便，其调用方式与Matlab的内建函数完全相同，只需在Matlab命令提示符下键入MEX文件名即可。
一个C/C++的MEX源程序通常包括4个组成部分，其中前3个是必须包含的内容，第4个则根据所实现的功能灵活选用:(1)#include
 “mex.h”;(2)MEX文件的入口函数mexFunction, MEX文件导出名必须为mexFunction函数;(3)mx[Array](http://www.dzsc.com/data/iccompany/detail40.html);(4)API函数
　　通过简单的例子说明C/C++的MEX源程序编写和调用过程:
```cpp
#include "mex.h"
void  timeSTwo(double y[], double x[])
{ 
     y[0] = 2.0*x[0]; 
}
void  mexFunction(int  nlhs,  mxArray * plhs[], int nrhs,  const mxArray *prhs[])
{
    double *x,*y; 
    int  mrows, ncols;
　　if( nrhs!=1) 
           mexErrMsgTxt("One input required.");
　　else if( nlhs>1) 
           mexErrMsgTxt("Too manyoutput arguments");
　　mrows = mxGetM( prhs[0] ); 
    ncols = mxGetN(prhs[0]);
　　if( !mxIsDouble(prhs[0]) || mxIsComplex( prhs[0] ) || !( mrows ==1 && ncols==1 ) )
         mexErrMsgTxt( "Input must be a noncomplex scalar double." );
　　     plhs[0] = mxCreateDoubleMatrix (  mrows,  ncols,  mxREAL );
　　     x = mxGetPr( prhs[0] );    
         y = mxGetPr( plhs[0] );  
         timestwo(y,x); 
 }
```
**　可在matlab中编译，也可以直接在C++环境中编译：　**
1）.（**在****matlab中**）用指令mex
  timestwo.c编译此文件，然后在MATLAB命令行下调用生成的MEX文件即可。2).（**在****VC2008中**）和一般c++一样编译后，就会产生dll，这样可以直接在Matlab中用了，或者copy且更改后缀名.mexw32即可。*（因为Matlab R2010b**以后版本可能不支持调用dll为后缀的mex文件了**）*
**2.2调用C/C++动态连接库（即：一般普通的C程序dll没有用mex的接口函数）**
Matlab提供对动态连接库DLL文件的接口[7]。利用该接口，可在Matlab中调用动态连接库导出的函数。Matlab对DLL的接口支持各种语言编写的DLL文件。在调用DLL文件之前，需要准备函数定义的头文件。对于C/C++语言开发的DLL文件，可使用源程序中相应的头文件；而对于其他语言开发的DLL，则要手工准备等效的C语言函数定义头文件。
在Matlab中利用动态连接库接口技术通常需要完成以下4个步骤：
(1)打开动态连接库文件；(2)为调用函数准备数据；(3)调用动态连接库文件中导出的函数；(4)关闭动态连接库文件。
　　为了实现以上步骤，用到的Matlab函数有：loadlibrary, loadlibrary, calllib, libfunctions, lipointer, libstruct, libisloaded。下面举例说明Matlab调用C/C++动态连接库的方法和步骤：
a.在VC环境下，新建工程->win32动态连接库->工程名[Test1](http://www.dzsc.com/stock-ic/TEST1.html)->empty工程->完成；
b.新建->C++源文件->添加a.cpp,内容为:**#include "a.h"**
**_declspec(dllexport)**int  add(int a,  int b)  { return a+b; }
c.新建->C/C++头文件->添加a.h，内容为：
 _declspec(dllexport)  int  add(int a,intb); 然后编译生成Test1.dll动态连接库文件，将Test1.dll和a.h拷到Matlab工作目录下。
d.在Matlab命令行下，调用Test.dll：>>loadlibrary(‘Test1’,’a.h’);>>x=7;
>>y=8;>>calllib(‘Test1’,‘add’,x,y);Ans=15  >>unloadlibrary(‘Test1’).
调用DLL动态连接库的方法，为Matlab重用工程实践中积累的大量实用C/C++代码提供了一种简洁方便的方法。与调用MEX文件相比，该方法更加简便实用。但是这个接口之支持C，不支持C++库和函数的重载，这种情况下，推荐用MEX-file，若实在要用这种方法（**调用C/C++动态连接库**），则对于C++要做一些更改，详见[http://www.mathworks.de/help/techdoc/matlab_external/f43202.html#bq__4nu-1](http://www.mathworks.de/help/techdoc/matlab_external/f43202.html#bq__4nu-1)，
**3  C/C++调用Matlab**
　　在工程实践中，C/C++调用Matlab的方法主要有调用Matlab计算引擎、包含m文件转换的C/C++文件，以及调用m文件生成的DLL文件。
**3.1利用Matlab计算引擎**
Matlab的引擎库为用户提供了一些接口函数，利用这些接口函数，用户在自己的程序中以计算引擎方式调用Matlab文件。该方法采用客户机/[服务器](http://www.dzsc.com/product/searchfile/5368.html)的方式，利用Matlab引擎将Matlab和C/C++联系起来。在实际应用中，C/C++程序为客户机，Matlab作为本地[服务器](http://wiki.dzsc.com/info/1807.html)。
C/C++程序向Matlab计算引擎传递命令和数据信息，并从Matlab计算引擎接收数据信息[2]。
Matlab提供了以下几个C语言计算引擎访问函数供用户使用[8]：**engOpen，engClose， engGetVariable，engPutVariable，engEvalString，engOutputBuffer，engOpenSingleUse， engGetVisible，engSetVisible**。
　　下面以C语言编写的、调用Matlab引擎计算方程x3 ?2x+5=0根的源程序example2.c为例，说明C/C++调用Matlab计算引擎编程的原理和步骤：
```cpp
#include <windows.h>  
#include <stdlib.h>
#include <stdio.h>  
#include "engine.h"
int  PASCAL  WinMain (HANDLE  hInstance,  
                        HANDLE  hPrevInstance,
                          LPSTR lpszCmdLine,  int nCmdShow )
{
     Engine  *ep;  
<pre name="code" class="cpp">     mxArray  *P=NULL,*r=NULL;<pre name="code" class="cpp">     char buffer[301];  double poly[4] = { 1,0,-2,5 };
<pre name="code" class="cpp">     if ( !(ep =engOpen(NULL) ) ){ 
<pre name="code" class="cpp">        fprintf( stderr,"\nCan't start MATLAB engine\n" );  return EXIT_FAILURE;
      }
     P = mxCreateDoubleMatrix( 1, 4, mxREAL);  mxSetClassName( P, "p" );
     memcpy( ( char * )  mxGetPr( P ), (char *) poly,  4*sizeof(double) );
     engPutVariable( ep, P );  engOutputBuffer( ep, buffer, 300 );
     engEvalString( ep, "disp(['多项式',poly2str(p,'x'),'的根']),r=roots(p)" );
     MESSageBox(NULL,buffer,"example2展示MATLAB引擎的应用",MB_OK);
     engClose( ep );  mxDestroyArray( P );  
     return EXIT_SUCCESS;
}
```
在Matlab下运行example2.exe: mex  -f
  example2.c。运行结果如图1所示：
![](https://img-my.csdn.net/uploads/201206/12/1339502458_8461.PNG)
利用计算引擎调用Matlab的特点是：节省大量的系统资源，应用程序整体性能较好，但不能脱离Matlab的环境运行，且运行速度较慢，但在一些特别的应用[9]（例如需要进行三维图形显示）时可考虑使用。
**3.2利用mcc编译器生成的cpp和hpp文件**
Matlab自带的C++Complier--**mcc**，能将m文件转换为C/C++代码。因此，它为C/C++程序调用m文件提供了另一种便捷的方法。下面举例说明相应步骤：
a.新建example3.m： function y=exmaple3(n)
  y=0;  for i=1:n  y=y+i;  end
　　保存后在命令窗口中输入：mcc  -t  -L  Cpp  -h  example3.
　　则在工作目录下生成example3.cpp和example3.hpp两个文件。
b.在VC中新建一个基于对话框的MFC应用程序[Test2](http://www.dzsc.com/stock-ic/TEST2.html)，添加一个[按钮](http://www.dzsc.com/product/searchfile/18041.html)，并添加按钮响应函数，函数内容见f步。将上面生成的两个文件拷贝到VC工程的Test2目录下。
c.在VC中选择：工程->设置，选择属性表[Link](http://www.dzsc.com/stock-ic/LINK.html)选项，下拉菜单中选择Input，在对象
 / 库模块中加入l[IBM](http://www.dzsc.com/data/iccompany/detail74.html)mfile.lib
 libmatlb.lib libmx.lib libmat.lib libmatpm.lib **sgl.lib libmwsglm.lib libmwservices.lib**，（后三个为使用Matlab图形库时，需加入）注意用空格分开；而在忽略库中加入
 msvcrt.lib；
d.选择属性表C/C++选项，下拉菜单选General，在预处理程序定义中保留原来有的内容，并添加MSVC,IBMPC,MSWIND，并用逗号隔开。选择下拉菜单的Precompiled
 Headers 选项，在“自动使用预补偿页眉”中添加stdafx.h，然后确定。
e.选择:工具->选项，属性页选择“目录”，在include
 files加入： C:\MATLAB6p5p1\extern\include， C:\MATLAB6p5p1\extern\include\cpp；然后在
 Library files里面加入： C:\MATLAB6p5p1\bin\win32， C:\MATLAB6p5p1\extern\ lib\win32\microsoft\msvc60；注意根据用户的Matlab安装位置，修改相应目录。
f.在响应函数中添加头文件：#include "matlab.hpp"#include "example3.hpp"函数响应代码为：
int i;  mwArray n;n=10;  n=example3(n);  i=n.ExtractScalar(1);
CString str;  str.Format( "example3的返回值是:%d", I );  AfxMessageBox( str );
g.编译，连接，执行，结果如图2所示。
![](https://img-my.csdn.net/uploads/201206/12/1339502393_5521.PNG)
**3.3利用mcc编译器生成的DLL文件**
Matlab的C++ Complier不仅能够将Matlab的m文件转换为C/C++的源代码，还能产生完全脱离Matlab运行环境的独立可执行DLL程序。从而可以在C/C++程序中，通过调用DLL实现对
 Matlab代码的调用。下面通过一个简单的例子说明C/C++调用m文件生成的DLL：
a.建立m文件example4.m：
 function result = example4(para)
x=[1 para 3];  y=[1 3 1];  [plot](http://www.dzsc.com/stock-ic/PLOT.html)(
 x,y );  result=para*2;  end.然后在命令窗口中输入：
mcc  -t  -W  libhg: example4  -T  [link](http://www.dzsc.com/stock-ic/LINK.html):
 lib  -h libmmfile.mlib  libmwsglm.mlib  example4则在工作目录下会生成example4 .dll、example4 .lib和example4
 .h三个文件。
b.在VC中新建一个基于对话框的应用程序[Test3](http://www.dzsc.com/stock-ic/test3.html)，然后添加一个按钮及按钮响应函数，函数内容见d步，再将生成的3个文件拷贝到Test2工程目录下。
c.VC编译环境的设置如同3.2节c、d步；
d.在按钮函数文件添加如下的头文件：#include "example4 .h"，函数响应代码为：
mxArray*para=mxCreateDoubleScalar(2);  mxArray*
 result;  example4Initialize();
result =mlfExample4(para);  CString str;
str.Format( "%f",mxGetScalar(result) );  AfxMessageBox(str);
e.编译，连接，执行，结果如图3所示。
![](https://img-my.csdn.net/uploads/201206/12/1339502523_8969.PNG)
利用mcc编译器生成的DLL动态连接库文件，只需在C/C++编译环境中将其包含进来，调用导出函数即可实现原m文件的功能，极大地方便了用户的代码设计。
**4.结束语**
　　本文从Matlab调用C/C++代码和C/C+调用m文件两方面，详细地研究了Matlab与C/C++混合编程技术。对于Matlab调用C/C++代码，给出了常用的MEX技术和调用C/C++动态连接库的方法，并对它们进行比较。针对用户在实际中经常遇到的C/C++调用Matlab问题，通过研究给出了常用的三种方法及其特点：利用Matlab计算引擎的方法，混合编程后的可执行程序脱离不了Matlab的运行环境，运行速度很慢；利用mcc编译器将m文件转化为C/C++文件的方法，虽然能独立于Matlab运行环境，可在C/C++环境中包含生成的文件非常繁琐；但是m文件生成的DLL为用户提供了一种简洁方便的C/C++调用Matlab代码的方法。除
 Matlab自带的mcc外，Matcom也能将M文件编译为C/C++文件和DLL文件[2][8]，但混合编程原理一样，在此省略。
[**MATLAB与C/C++混合编程之MATLAB调用C程序**](http://blog.csdn.net/sbtdkj1017/article/details/1901647)
2007-11-25 15:28 12073人阅读[评论](http://blog.csdn.net/sbtdkj1017/article/details/1901647#comments)(13)[收藏](http://blog.csdn.net/jtop0/article/details/7657227)[举报](http://blog.csdn.net/sbtdkj1017/article/details/1901647#report)
通过把耗时长的函数用c语言实现，并编译成mex函数可以加快执行速度。Matlab本身是不带c语言的编译器的，所以要求你的机器上已经安装有VC,BC或Watcom C中的一种。如果你在安装Matlab时已经设置过编译器，那么现在你应该就可以使用mex命令来编译c语言的程序了。如果当时没有选，就在Matlab里键入mex -setup，下面只要根据提示一步步设置就可以了。需要注意的是，较低版本的在设置编译器路径时，只能使用路径名称的8字符形式。比如我用的VC装在路径C:/PROGRAM FILES/DEVSTUDIO下，那在设置路径时就要写成：“C:/PROGRA~1”这样设置完之后，mex就可以执行了。为了测试你的路径设置正确与否，把下面的程序存为hello.c。
/*hello.c*/
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{ mexPrintf("hello,world!/n"); 
} 
假设你把hello.c放在了C:/TEST/下，在Matlab里用CD C:/TEST/ 将当前目录改为C:/ TEST/（注意，仅将C:/TEST/加入搜索路径是没有用的）。现在敲：
mex hello.c 
如果一切顺利，编译应该在出现编译器提示信息后正常退出。如果你已将C:/TEST/加
入了搜索路径，现在键入hello，程序会在屏幕上打出一行：
hello,world! 
看看C/TEST/目录下，你会发现多了一个文件：HELLO.DLL。这样，第一个mex函数就算完成了。分析hello.c，可以看到程序的结构是十分简单的，整个程序由一个接口子过程 mexFunction构成。
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
前面提到过，Matlab的mex函数有一定的接口规范，就是指这
nlhs：输出参数数目 
plhs：指向输出参数的指针 
nrhs：输入参数数目 
例如，使用
[a,b]=test(c,d,e)
调用mex函数test时，传给test的这四个参数分别是
      2，plhs，3，prhs
其中： 
prhs[0]=c 
prhs[1]=d 
prhs[2]=e 
当函数返回时，将会把你放在plhs[0]，plhs[1]里的地址赋给a和b，达到返回数据的目的。  
细心的你也许已经注意到，prhs[i]和plhs[i]都是指向类型mxArray类型数据的指针。 这个类型是在mex.h中定义的，事实上，在Matlab里大多数数据都是以这种类型存在。当然还有其他的数据类型，可以参考Apiguide.pdf里的介绍。 
为了让大家能更直观地了解参数传递的过程，我们把hello.c改写一下，使它能根据输 
入参数的变化给出不同的屏幕输出：
```cpp
//hello.c 2.0 
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{
int i; 
i=mxGetScalar(prhs[0]); 
if(i==1) 
  mexPrintf("hello,world!/n"); 
else 
  mexPrintf("大家好！/n"); 
}
```
将这个程序编译通过后，执行hello(1),屏幕上会打出： 
          hello,world! 
而hello(0)将会得到： 
大家好！ 
现在，程序hello已经可以根据输入参数来给出相应的屏幕输出。在这个程序里，除了用到了屏幕输出函数mexPrintf（用法跟c里的printf函数几乎完全一样）外，还用到了一个函数：mxGetScalar，调用方式如下： 
   i=mxGetScalar(prhs[0]); 
"Scalar"就是标量的意思。在Matlab里数据都是以数组的形式存在的，mxGetScalar的作用就是把通过prhs[0]传递进来的mxArray类型的指针指向的数据（标量）赋给C程序里的变量。这个变量本来应该是double类型的，通过强制类型转换赋给了整形变量i。既然有标量，显然还应该有矢量，否则矩阵就没法传了。看下面的程序： 
```cpp
//hello.c 2.1 
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[], 
int nrhs, const mxArray *prhs[]) 
{ 
int *i; 
i=mxGetPr(prhs[0]); 
if(i[0]==1) 
  mexPrintf("hello,world!/n"); 
else 
  mexPrintf("大家好！/n"); 
}
```
这样，就通过mxGetPr函数从指向mxArray类型数据的prhs[0]获得了指向double类型的指针。
但是，还有个问题，如果输入的不是单个的数据，而是向量或矩阵，那该怎么处理呢 ？通过mxGetPr只能得到指向这个矩阵的指针，如果我们不知道这个矩阵的确切大小，就 
没法对它进行计算。 
为了解决这个问题，Matlab提供了两个函数mxGetM和mxGetN来获得传进来参数的行数 和列数。下面例程的功能很简单，就是获得输入的矩阵，把它在屏幕上显示出来： 
```cpp
//show.c 1.0 
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{ 
double *data; 
int M,N; 
int i,j; 
data=mxGetPr(prhs[0]); //获得指向矩阵的指针 
M=mxGetM(prhs[0]); //获得矩阵的行数 
N=mxGetN(prhs[0]); //获得矩阵的列数 
for(i=0;i<M;i++) 
{   for(j=0;j<N;j++) 
     mexPrintf("%4.3f  ",data[j*M+i]); 
     mexPrintf("/n"); 
  }
}<span style="color:#333333;"><span style="font-family:宋体;"> </span></span>
```
编译完成后，用下面的命令测试一下： 
```cpp
a=1:10; 
  b=[a;a+1]; 
  show(a) 
  show(b)
```
需要注意的是，在Matlab里，矩阵第一行是从1开始的，而在C语言中，第一行的序数为零，**Matlab里的矩阵元素b(i,j)**在传递到C中的一维数组大data后对应于**data[j*M+i]** 。 
输入数据是在函数调用之前已经在Matlab里申请了内存的，由于mex函数与Matlab共用同一个地址空间，因而在prhs[]里传递指针就可以达到参数传递的目的。但是，输出参数却需要在mex函数内申请到内存空间，才能将指针放在plhs[]中传递出去。由于返回指针类型必须是mxArray，所以Matlab专门提供了一个函数：mxCreateDoubleMatrix来实现内存的申请，函数原型如下： 
   mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity ComplexFlag) 
   m：待申请矩阵的行数 
   n：待申请矩阵的列数 
为矩阵申请内存后，得到的是mxArray类型的指针，就可以放在plhs[]里传递回去了。但是对这个新矩阵的处理，却要在函数内完成，这时就需要用到前面介绍的mxGetPr。使用 mxGetPr获得指向这个矩阵中数据区的指针（double类型）后，就可以对这个矩阵进行各种操作和运算了。下面的程序是在上面的show.c的基础上稍作改变得到的，功能是将输  
```cpp
//reverse.c 1.0 
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[], 
    int nrhs, const mxArray *prhs[]) 
{ 
double *inData; 
double *outData; 
int M,N; 
int i,j; 
inData=mxGetPr(prhs[0]); 
M=mxGetM(prhs[0]); 
N=mxGetN(prhs[0]); 
plhs[0]=mxCreateDoubleMatrix(M,N,mxREAL); 
outData=mxGetPr(plhs[0]); 
for(i=0;i<M;i++) 
  for(j=0;j<N;j++) 
   outData[j*M+i]=inData[(N-1-j)*M+i]; 
}
```
当然，Matlab里使用到的并不是只有double类型这一种矩阵，还有字符串类型、稀疏矩阵、结构类型矩阵等等，并提供了相应的处理函数。本文用到编制mex程序中最经常遇到的一些函数，其余的详细情况清参考Apiref.pdf。 
通过前面两部分的介绍，大家对参数的输入和输出方法应该有了基本的了解。具备了这些知识，就能够满足一般的编程需要了。但这些程序还有些小的缺陷，以前面介绍的re由于前面的例程中没有对输入、输出参数的数目及类型进行检查，导致程序的容错性很差，以下程序则容错性较好
```cpp
#include "mex.h" 
void mexFunction(int nlhs, mxArray *plhs[],  int nrhs, const mxArray *prhs[]) 
{ 
    double *inData; 
    double *outData; 
    int M,N; 
    //异常处理 
    //异常处理 
    if(nrhs!=1) 
        mexErrMsgTxt("USAGE: b=reverse(a)/n"); 
   if(!mxIsDouble(prhs[0])) 
        mexErrMsgTxt("the Input Matrix must be double!/n"); 
    inData=mxGetPr(prhs[0]); 
    M=mxGetM(prhs[0]); 
    N=mxGetN(prhs[0]); 
    plhs[0]=mxCreateDoubleMatrix(M,N,mxREAL); 
    outData=mxGetPr(plhs[0]); 
    for(i=0;i<M;i++) 
        for(j=0;j<N;j++) 
            outData[j*M+i]=inData[(N-1-j)*M+i]; 
  }
```
在上面的异常处理中，使用了两个新的函数：mexErrMsgTxt和mxIsDouble。MexErrMsgTxt在给出出错提示的同时退出当前程序的运行。MxIsDouble则用于判断mxArray中的数据是否double类型。当然Matlab还提供了许多用于判断其他数据类型的函数，这里不加详述。 
需要说明的是，Matlab提供的API中，函数前缀有mex-和mx-两种。带mx-前缀的大多是对mxArray数据进行操作的函数，如mxIsDouble,mxCreateDoubleMatrix等等。而带mex前缀的则大多是与Matlab环境进行交互的函数，如mexPrintf，mxErrMsgTxt等等。了解了这一点，对在Apiref.pdf中查找所需的函数很有帮助。
至此为止，使用C编写mex函数的基本过程已经介绍完了。
