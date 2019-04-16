# Matlab高级教程_第三篇：Matlab转码C/C++方式(混编)_第一部分 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第三篇：Matlab转码C/C++方式(混编)_第一部分](https://www.cnblogs.com/noah0532/p/9491195.html)





0. 其实Matlab的转码混编大多数就是为了现成的算法函数不用再写了，2就是为了方便提高代码运行速度用C语言去运行。



1. MEX文件：

　　Mex文件是一种可在matlab环境中嗲用C语言（或fortran）衍生程序，mex的编译结果实际上是一个带输出函数的mexFunction的dll文件。

　　备注：如果观察Matlab的发展史可知，Matlab最早是用了fortran的线性函数库作为教学软件给学生们学习的。

　　Mex从字面上是matlab和execute两个单词的缩写。

　　.Mex文件实现了一种其他语言与matlab的接口，通过mex文件可以在matlab中像调用内嵌函数一样调用使用C语言和Fortran等语言辨析的函数，实现了代码重用，同时也能够提高matlab环境中数据处理的效率。

　　Mex文件格式：


```cpp
#include "mex.h"
```



```cpp
void
```

```cpp
mexFunction (
```

```cpp
int
```

```cpp
nlhs, mxArray *plhs[],
```

```cpp
int
```

```cpp
nrhs,
```

```cpp
const
```

```cpp
mxArray *prhs[] )
```



```cpp
{
```



```cpp
}
```


 　　编写Mex程序的编译器可以使用matlabdiamante编辑器，也可以使用自己的C++编译器你，如VS2008等。

　　上面这四个参数分别用来输出和输入数据：nlhs是输出参数个数，plhs是输出参数指针，nrhs是输入参数个数，prhs是输入参数指针。

　　注意：对输出和数据参数的操作都是通过指针方式进行的；matlab指针是从1开始，C的指针是从0开始的。C语言需要首先分配内存空间，因此在编码的时候也需要注意。



2. MEX环境配置

　　在matlab中配置需要输入

>>mex -setup

　　但是常有错误，其matlab自带的编译器也并不太好用，因此可以选用网上的编译器拷贝到matlab当中使用。当然还有一种可能是因为盗版的问题。因此在输入上面命令的时候可能会提示如下信息：

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180817002445273-824803882.png)

错误使用 mex
未找到支持的编译器或 SDK。您可以安装免费提供的 MinGW-w64 C/C++ 编译器；请参阅安装 MinGW-w64 编译器。有关更多选项，请访问
http://www.mathworks.com/support/compilers/R2016b/win64.html。 

　　这里用的是matlab2016b版本，因此我们需要MinGW-w64 C/C++这个编译器。具体配置方式见3。



3. 具体配置编译器的方法

　　上TDM-GCC官网去下载这个编译器，注意64位和32位不同。

　　网址：http://tdm-gcc.tdragon.net/download

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180817003032302-179062185.png)

**第一步**：把这个文件下载下来，安装上，这里建议安装到C盘并记住这个安装目录。

　　“Create”----“MinGW-64/TDM64(32-bit and 64-bit)”----C:\TDM-GCC-64----SourceForge Default----安装

**第二步**：在matlab命令行运行命令setenv(‘MW_MINGW64_LOC’,folder)，这里的foler就是刚才的目录，因此改为：

　　setenv('MW_MINGW64_LOC','C:\TDM-GCC-64')

**第三步**：设定环境变量

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180817021002692-1553620507.png)

**第四步**：重启matlab

**第五步**：检测一下，还是跟上吗一样输入mex -setup

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180817023907068-546031887.png)

　　这样就表示成功了，这些更新新的API不用管它。

　　实际过程要选mex -setup C++编译器。后面具体转码过程再说。这些是安装matla编译器也就是第一步。



4. 注意：在这个转码的过程中千万Matlab编码的版本要略小于Visual Studio的版本。这样防止转码不兼容的问题。这里采用的matlab R2016b

　　visual studio的版本是2017版本。具体的这个版本适用在matlab当中有，可以参照相关资料。也就是2中红色警告字体当中的那个网址，里面都有对于编码版本和SDK包等的相关配置信息。



5. 关于官网的转码例子（视频）

官网视频地址：https://ww2.mathworks.cn/videos/matlab-to-c-made-easy-81870.html?elqsid=1534613526595&potential_use=Home

中文视频地址：https://ww2.mathworks.cn/videos/generating-cc-code-from-matlab-using-matlab-coder-82536.html?elqsid=1534701215344&potential_use=Home



《Matlab to C Made Easy》

Generating realable and portable C code from your Matlab alogrithms

**Agenda**

*Motivation

-Why translate Matlab to C？

-Challenges of manual translation

*Using matlab Coder

-Three-step workflow for generation code

*Use cases

-integrate algorithms using source code/libraries

-Accelerate through Mex

-Prototype by generating EXE

5.1 记住要在MATALB本地文件夹下进行转码

5.2 例子如下：

* 观察编译环境是否在MATLAB下，且添加子文件夹到目录

* 编辑器中生成函数：

function c=myMult(a,b)

% Mutiple two inputs

c = a*b;

* 另外在生成一个测试函数的m文件（文件名myTestBench）

myMult(10,20) //注意这里不要加分号，要显示结果

* 在app或者命令行输入Coder，打开命令。

** Select---》输入刚才的函数名“myMult”

E:\MATLAB\R2016b\Work\SimpleDemo\myMult.prj //会看到生成的工程项目

下一步

** Define Input Types---》 输入刚才的测试名“myTestBench”

会看到函数当中的输入函数名，在这里定义函数的长短类型，这里选择双精度，如果变量长度不定，可以选择inf

下一步

** Check for RunTime Issues---》进行生成过程，完成后（在这里会遇到函数需要预定义变量长度，预先分配内存空间等问题需要重新解决）

下一步

** Generate Code---》这里进行生成代码选项

生成代码



**6. 关于生成代码过程中的一些问题总结**

** 问题1**：%#codegen的说明

把一些提示代码警告去掉



**问题2**：实现约束

function a = foo(b,c)

a = b*c

有可能是一个元素乘以一个元素

有可能是点乘

有可能是一个矩阵乘法

数据结构还有可能是逻辑性、整数型、复数型等

*** 多形性**：同一个matlab对应不同C语句的实现

*** 内存分配**：内存的预分配、动态内存还是静态内存

*** 矩阵行列处理**：

*** 定点数据类型**：



**关于实现约束的例子**：Newtom-Rhapson算法的例子（**更高的例子的实现**）

* 预分配

* 寻找更有效的构架

* 编译选项

函数内容：



**问题3**：各部分功能翻译解释：

3.1 **Select 界面（添加函数内容）**

，![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820112419674-240276231.png)

 Numeric Conversion （数值转换，一般选择默认）

：默认

：Convert to fixed point（转换为不动点数据类型，小数点固定）

：Convert to single precision（转换为单精度数据类型）



The MATLAB Coder workflow generates standalone C and C++ code from MATLAB code，to begin，select your entry-point functin(s).（选择入口点函数）

Generate code for function：输入生成函数名

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820113032087-766569631.png)

Project location：项目名地址（后缀为.prj）

The specified project already exits.you can reopen the existing project,overwrite it,or enter  a differnt name.（指定的项目已经退出。您可以重新打开心有的项目）



**3.2 Define界面**（Define Input Types，定义输入类型）

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820113538903-495640024.png)

To conver MATLAB to C,you must define the type of each input for every entry point function.

（MATALB转换为C，你必须定义所有入口点函数的每一个输入变量类型）

To automatically define input types,call xxx or enter a script that calls xxx in the MATLAB prompt below:

可以定义单个的输入变量类型、还可以添加全局变量类型，如果选择Autodefine Input Types（自动定义输入类型），也可以自动选择，但是也需要手动观察是否有错误。

这里的这个界面（这里采用R2016b）可能和其他版本不一样，在早期版本，可以直接定义，在2016版本还要生成一个关于要生成函数的测试脚本（script）来进行测试，

通过这个位置进行变量类型的定义（之前可以不用生成这个测试脚本调试，可以直接调试入口点函数每一个输入变量的类型）

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820114207855-1811828217.png)

关于输入变量类型进行说明（**注意：C语言的变量需要预定义，因此不光要定义入口的函数尺寸，过程中的函数尺寸，还有出口的函数尺寸**）

 double/single（双精度/单精度数据类型）

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820114410170-2127925600.png)

可以选择1*1的标量

可以选择1*n列的向量

可以选择m行*1列的向量

可以选择m*n的矩阵

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820114604848-1757614599.png)

第一：确定长度仅此为1

第二：确定长度最长为1（可以为零，一侧长度可以规定最长延伸到那个位置）

第三：无约束无线长度

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820114933810-1068916365.png)

定义所有未定义作为xx双精度

add global添加全局变量（没有需要不选一般）



**3.3 Check For Run-Time Issues（检查运行时，检查函数当中是否存在编码的错误）**

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820115151891-244154391.png)

Enter code or select a script that exercises XXX

键入代码或选择一个脚本测试XXX（待生成函数）

Generating trial code（生成试代码）

Building Mex（建立Mex文件，主要代码的转码要在MATLAB目录下生成，一般建立一个工作文件夹）

Running test file with Mex（用Mex去测试文件）

（如果编码过程中，注入变量长度未定义等等问题会在这个地方通过不了）

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820115652010-1515648538.png)

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180820115731636-852620640.png)



**3.4 CHECK FOR ISUES**

**3.5 SETTINGS（设定项）**

 Paths（路径）：

　　Project folder（项目路径）

　　　　A subfolder of the project folder（项目目录下的一个子目录）

　　MATLAB current working folder（MATLAB目前工作路径）

　　Specified folder（指定文件夹）



Speed（速度）：

　　Enable responsiveness to CTRL+C and graphics refreshing（启用对CTRL+C和图形刷新的响应性：**默认可选**）

　　Keep extrinsic calls（保存外部呼叫：**默认可选**）

　　Ensure memory integrity（保证内存完整性：**默认可选**）

　　Saturate on integer overflow（这样当模块的值输入与枚举值的基础值不匹配时，Simulink 将使用枚举类型的默认值：**默认可选**）



Memory（内存）：

　　Global data synchronization mode（全局数据同步模式）：

　　　　At MEX-function entry，exit and extrinsic call：（MEX 函数输入，退出和外部呼叫：**默认可选**）

　　Variable Sizing Support（变量持仓支持）：

　　　　Dynamic memory allocation（动态内存分配法）：

　　　　　　For arrays with max size at or above threshold（对于最大或大于阈值的阵列）

　　　　　　　　Dynamic memory allocation threshold（动态内存大小阈值）

　　　　　　For all variable-sized arrays（对于所有可变大小的数组：**默认可选**）

　　Stack usage max（堆栈使用最大值）：



Code，Apperance（代码，外观）：

　　Generated file partitioning method（生成文件划分方法）：

　　　　Generate one file for each MATLAB file（为每个Matlab文件生成一个文件：**默认可选**）： 

　　　　Generate all functions into a single file（将所有函数生成到单一文件中）：

　　Comments（备注）：

　　　　Include comments（后面都可以不选）



Debugging（检测）：

　　Enable C compiler debug mode（启用C编译器模式调用）：

　　Always create a code generation report（始终创建代码生成报告：**默认可选**）：

　　Automatically launch a report if one is generated（如果生成报表，则自动启动报表）：



Custom Code（自定义代码）：这些可以不用选

All Settings（所有设置）



**3.6 Generate Code（生成代码）**：

![](https://images2018.cnblogs.com/blog/1328368/201808/1328368-20180821013732686-362060224.png)

Toolchain（工具链）














