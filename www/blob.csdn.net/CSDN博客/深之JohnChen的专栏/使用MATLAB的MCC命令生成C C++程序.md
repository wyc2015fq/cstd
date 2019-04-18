# 使用MATLAB的MCC命令生成C/C++程序 - 深之JohnChen的专栏 - CSDN博客

2017年06月02日 12:07:32[byxdaz](https://me.csdn.net/byxdaz)阅读数：922


转载：http://blog.csdn.net/hippig/article/details/8519421

**首先，配置MATLAB**

用如下命令：

**mex –setup**

**mbuild -setup**

***例一：将m文件转化成库文件使用***

1、建立一个名为ceshidll.m的M函数文件，该函数的功能是输入两组数完成两组数据的插值拟合,并将结果用图形表示：

ceshidll.m文件内容如下：

function ceshidll(x,y)

a=min(x):0.1:max(x);

b = interp1(x,y,a,''spline'');%一维插值函数

plot(x,y,''*'',a,b);

2、在MATLAB Command中用如下命令编译函数ceshidll.m：

>>  **mcc**-t -W libhg:dlltest-T link:lib -h libmmfile.mlib ceshidll.m

参数说明：

-t 将m文件编译为C\C++代码

-W libhg:dlltest 生成使用C函数图形库的文件，生成的文件名为dlltest

-T link:lib 生成库文件

-h 辅助选项，可以将任何被调用的辅助函数都包含到编译的文件中

libmmfile.mlib连接任何需要的共享函数库

ceshidll.m 被编译的文件名

编译完成后在MATLAB当前目录下会生成以下文件： ceshidll.c、ceshidll.h 、dlltest.c 、dlltest.exports、dlltest.h、dlltest.mlib、dlltest.exp、dlltest.lib、dlltest.dll。其中**dlltest.h 、dlltest.lib和dlltest.dll**文件是我们需要的。

使用方法：

 #include "matlab.h"

#include "dlltest.h"

#pragma comment(lib,"dlltest")

关键代码：

UpdateData(TRUE);　        //刷新输入数据

double X[100],Y[100];

CString AA,BB,a;

int i=1;

mxArray *A=NULL;　         //初始化矩阵

mxArray *B=NULL;

　　AA=m_edit1;　          //字符串赋值

　　BB=m_edit2;

.....//将字符转化为数字
mlfEnterNewContext(0, 0);　//自动管理内存
dlltestInitialize();
mlfCeshidll(A,B);　        //调用dll文件中函数
mxDestroyArray(A);　       //释放矩阵内存
mxDestroyArray(B);

mlfRestorePreviousContext(0, 0);

***例二：将m文件 转换成对应的C\C++文件***

1、在MATLAB中编写如下函数： 
```
function [x]=gjfcz(A,b)

%A=[-1.5 1 2; 4 2 3 ; -3 2 8]

%b=[3;5;6]

x=A\b
```
保存名为gjfcz.m，该函数的功能为求解线形方程组，可参考第四章的内容。

2、在MATLAB的命令窗口输入以下命令：`**mcc –m gjfcz.m**`

该命令用来生成对应的C文件和可执行程序。在MATLAB工作目录下(一般是MATLAB\work)将会生成如下文件：gjfcz.exe，gjfcz.c，gjfcz.h，gjfcz_main.c，其中gjfcz.c，gjfcz.h是我们需要的文件。

3、新建名为JXXFC基于对话框的工程，面板上添加一个按扭。

4、拷贝gjfcz.c，gjfcz.h两文件到工程目录下，并将文件引入工程(Project->Add to Project->Files)。

5、为按扭添加如下响应代码：

```
void CJXXFCDlg::OnButton1()

{

　　static　　double Adata[]={-1.5,4,-3,1,2,2,2,3,8};

　　static　　double bdata[]={3,5,6};

　　double　 Xdata[100];

　　mxArray *A = NULL;//赋初值

　　mxArray *b = NULL;

　　mxArray *x = NULL;

　　/* 使用自动内存管理*/

　  mlfEnterNewContext(0, 0);

　 //创建矩阵

　　mlfAssign(&A, mlfDoubleMatrix(3, 3, Adata, NULL));

　　mlfAssign(&b, mlfDoubleMatrix(3, 1, bdata, NULL));

　　InitializeModule_gjfcz();

　　x=mlfGjfcz(A,b);//调用gjfcz.c中的函数求解

　　TerminateModule_gjfcz();

　 memcpy(Xdata,mxGetPr(x),3*sizeof(double)); 　// mxGetPr(x)用来得到x的地址，从而获得matlab输出

　 CString R;

　 R.Format("%f\n%f\n%f",Xdata[0],Xdata[1],Xdata[2]);

　 MessageBox(R);

　 /* 释放矩阵所占的内存*/

　　mxDestroyArray(A);

　　mxDestroyArray(b);

　　mxDestroyArray(x);

　　/* 禁用自动内存管理*/

　　mlfRestorePreviousContext(0, 0);

}
```

***例三：利用图形库画图***

写一个简单的m函数：

```
function y=huatu_test()

x=-10:0.1:10;

y=sin(x);

plot(x,y,''*'')
```

文件保存为huatu_test.m。

**mcc -t -W libhg:dlltest -T link:lib -h libmmfile.mlib huatu_test.m**

#include "dlltest.h"

打开dlltest.h文件，里面有有关函数的定义，找到其中三个函数：

```
extern mxArray * mlfHuatu_test(void);

extern void dlltestInitialize(void);

extern void dlltestTerminate(void);
```

从函数意思不难知道它们的作用，dlltestInitialize用来初始化dll库，dlltestTerminate用来结束调用dll,mlfHuatu_test为主程序执行函数。将三个函数拷贝到button响应代码中，进行修改：

```
void CCeshiDlg::OnButton1()

{

dlltestInitialize();

mlfHuatu_test();

dlltestTerminate();

}
```

利用mcc命令，通过不同的参数设置可以生成不同的文件，例如：

```
mcc -B sgl myfun 将myfun.m文件生成对应的c文件和使用c图形库的可执行程序

mcc -B sglcpp myfun 将myfun.m文件生成相应的c++文件和使用c++图形库的可执行程序
```
mcc的参数实际上有很多，例如：

`mcc -t -W main -L C -T link:exe -h libmmfile.mlib myfun`

该命令是将myfun.m生成可执行c程序

为了简化选项设置，编译器提供了宏选项，实际上上述命令利用一个参数就可以了：

`mcc -m myfun`

该命令和上述命令是等价的,也是用来生成可执行c程序。关于mcc命令详细参数设置可以参考MATLAB帮助文档。

大家在使用VC调用MATLAB中遇到什么问题，可以发电子邮件到c_dinco@sina.com,把遇到的问题说清楚，正在写书，同时有什么好的建议，也欢迎发邮件来。

关于程序运行的说明：

1、根据实际情况修改VC中头文件和库文件的路径;

2、如果自己编写的程序图形不能显示菜单栏和工具栏,拷贝文件夹bin到当前目录下

## mcc、mbuild和mex命令详解：http://blog.sina.com.cn/s/blog_5928f2dd0100gf27.html

