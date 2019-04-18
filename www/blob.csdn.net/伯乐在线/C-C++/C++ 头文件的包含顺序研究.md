# C++ 头文件的包含顺序研究 - 文章 - 伯乐在线
原文出处： [朱金灿](http://blog.csdn.net/clever101/article/details/7269058)
### 一．《Google C++ 编程风格指南》里的观点
公司在推行编码规范，领导提议基本上使用《Google C++ 编程风格指南》。其中《Google C++ 编程风格指南》对于头文件的包含顺序是这样的：
Names and Order of Includes
link ▽Use standard order for readability and to avoid hidden dependencies:C library, C++ library, other libraries’ .h, your project’s .h.
All of a project’s header files should belisted as descendants of the project’s source directory without use of UNIXdirectory shortcuts . (the current directory) or .. (the parent directory). Forexample, google-awesome-project/src/base/logging.h should be included as
#include “base/logging.h”
In dir/foo.cc or dir/foo_test.cc, whosemain purpose is to implement or test the stuff in dir2/foo2.h, order yourincludes as follows:
dir2/foo2.h (preferred location — seedetails below).
C system files.
C++ system files.
Other libraries’ .h files.
Your project’s .h files.
The preferred ordering reduces hiddendependencies. We want every header file to be compilable on its own. Theeasiest way to achieve this is to make sure that every one of them is the first.h file #included in some .cc.
dir/foo.cc and dir2/foo2.h are often in thesame directory (e.g. base/basictypes_test.cc and base/basictypes.h), but can bein different directories too.
Within each section it is nice to order theincludes alphabetically.
For example, the includes ingoogle-awesome-project/src/foo/internal/fooserver.cc might look like this:


```
#include "foo/public/fooserver.h"  // Preferred location.
#include <sys/types.h>
#include <unistd.h>
#include <hash_map>
#include <vector>
#include "base/basictypes.h"
#include"base/commandlineflags.h"
#include "foo/public/bar.h"
```
在这里我谈一下我对上面的理解（如不当，还请诸位同学指正）：
1.      为了加强可读性和避免隐含依赖，应使用下面的顺序：C标准库、C++标准库、其它库的头文件、你自己工程的头文件。不过这里最先包含的是首选的头文件，即例如a.cpp文件中应该优先包含a.h。首选的头文件是为了减少隐藏依赖，同时确保头文件和实现文件是匹配的。具体的例子是：假如你有一个cc文件（linux平台的cpp文件后缀为cc）是google-awesome-project/src/foo/internal/fooserver.cc，那么它所包含的头文件的顺序如下：


```
#include <sys/types.h>  
#include <unistd.h>  
  
#include <hash_map>  
#include <vector>  
  
#include "base/basictypes.h"  
#include "base/commandlineflags.h"  
#include "foo/public/bar.h"
```
2. 在包含头文件时应该加上头文件所在工程的文件夹名，即假如你有这样一个工程base，里面有一个logging.h，那么外部包含这个头文件应该这样写：
#include “base/logging.h”，而不是#include “logging.h”
我们看到的是这里《Google C++ 编程风格指南》倡导的原则背后隐藏的目的是：
1. 为了减少隐藏依赖，同时头文件和其实现文件匹配，应该先包含其首选项（即其对应的头文件）。
2. 除了首选项外，遵循的是从一般到特殊的原则。不过我觉得《Google C++ 编程风格指南》的顺序：C标准库、C++标准库、其它库的头文件、你自己工程的头文件中漏了最前面的一项：操作系统级别的头文件，比如上面的例子sys/types.h估计不能归入C标准库，而是Linux操作系统提供的SDK吧。因此我觉得更准确的说法应该是：OS SDK .h , C标准库、C++标准库、其它库的头文件、你自己工程的头文件。
3.之所以要将头文件所在的工程目录列出，作用应该是命名空间是一样的，就是为了区分不小心造成的文件重名。
### 二．《C++编程思想》中的不同观点
与《Google C++ 编程风格指南》不同的是，《C++编程思想》倡导一种不同的规则。《C++编程思想》P432提到：
头文件被包含的顺序是从“最特殊到最一般”。这就是，在本地目录的任何头文件首先被包含。然后是我们自己的所有“工具”头文件，随后是第三方库头文件，接着是标准C++库头文件和C库头文件。
要了解其原因：可以看JohnLakos在《Large ScaleC++ Softwre Design》(注：其中文译名为《大规模C++程序设计》)中的一段话：
保证.h文件的组成部分不被它自身解析（parse）,这可以避免潜在的使用错误。因为被自身解析缺乏明确提供的声明或定义。在.c文件的第一行包含.h 文件能确保所有对于构件的物理界面重要的内部信息块都在.h中（如果的确是缺少了某些信息块，一旦编译这个.c文件时就可以发现这个问题）。
如果包含头文件的顺序是“从最特殊到最一般”，如果我们的头文件不被它自己解析。我们将马上找到它，防止麻烦事情发生。
### 三．我的试验
到底哪一种包含顺序好呢？我使用VS 2005编一个控制台测试工程TestInc，里面有几个文件。
MyMath.h的代码如下：


```
#pragma once  
double acos(double Num);
```
MyMath.cpp的代码如下：


```
double acos(double Num)  
{  
    return 1.0;  
}
```
TestInc.cpp的代码如下：


```
#include "TestInc.h"  
#include <stdio.h>  
#include <math.h>  
  
  
int _tmain(int argc, _TCHAR* argv[])  
{  
    double a = acos(0.5);  
    return 0;  
}
```
结果出现错误：
1>c:program filesmicrosoft visualstudio 8vcincludemath.h(107) : error C2732: 链接规范与“acos”的早期规范冲突
1>       c:program filesmicrosoft visual studio 8vcincludemath.h(107) : 参见“acos”的声明
然后我把TestInc.cpp的头文件包含顺序改为：


```
#include <stdio.h>  
#include <math.h>  
#include "TestInc.h"
```
则编译通过了。在调试运行时main函数调用还是C标准库的函数acos，看来函数调用的顺序是按头文件的包含顺序来的，即我自定义的acos函数被覆盖了(如果TestInc.h里包含了内联函数，则优先调用的是内联函数)。
从这个小实验中我得出如下结论：《Google C++ 编程风格指南》和《C++编程思想》倡导的包含头文件的顺序各有优点，《Google C++ 编程风格指南》应该能大量减少隐藏的头文件依赖，而《C++编程思想》则很容易让你清楚知道你所定义的接口是否和系统库及第三方库发生冲突。
### 四．头文件包含中的预编译功能
在Visual Studio环境下开发我们发现几乎每个cpp文件都要包含stdafx.h这个文件，而且要把它放在最前面的位置，否则就会出错。这是为什么呢？
原来Visual Studio采用一种预编译的机制。要了解预编译机制，先介绍一下预编译头。所谓的预编译头就是把一个工程中的那一部分代码，预先编译好放在一个文件里（通常是以.pch为扩展名的），这个文件就称为预编译头文件这些预先编译好的代码可以是任何的C/C++代码，甚至是inline的函数，但是必须是稳定的，在工程开发的过程中不会被经常改变。如果这些代码被修改，则需要重新编译生成预编译头文件。注意生成预编译头文件是很耗时间的。同时你得注意预编译头文件通常很大，通常有6- 7M大。注意及时清理那些没有用的预编译头文件。
也许你会问：现在的编译器都有Time stamp的功能，编译器在编译整个工程的时候，它只会编译那些经过修改的文件，而不会去编译那些从上次编译过，到现在没有被修改过的文件。那么为什么还要预编译头文件呢？答案在这里，我们知道编译器是以文件为单位编译的，一个文件经过修改后，会重新编译整个文件，当然在这个文件里包含的所有头文件中的东西（.eg Macro, Preprocessor ）都要重新处理一遍。 VC的预编译头文件保存的正是这部分信息。以避免每次都要重新处理这些头文件。
根据上文介绍，预编译头文件的作用当然就是提高便宜速度了，有了它你没有必要每次都编译那些不需要经常改变的代码。编译性能当然就提高了。
要使用预编译头，我们必须指定一个头文件，这个头文件包含我们不会经常改变的代码和其他的头文件，然后我们用这个头文件来生成一个预编译头文件（.pch 文件）想必大家都知道StdAfx.h这个文件。很多人都认为这是VC提供的一个“系统级别”的，编译器带的一个头文件。其实不是的，这个文件可以是任何名字的。我们来考察一个典型的由AppWizard生成的MFC Dialog Based　程序的预编译头文件。（因为AppWizard会为我们指定好如何使用预编译头文件，默认的是StdAfx.h，这是VC起的名字）。我们会发现这个头文件里包含了以下的头文件：


```
#include <afxext.h> // MFC extensions  
#include <afxdisp.h> // MFC Automation classes  
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls  
#include <afxcmn.h>
```
这些正是使用MFC的必须包含的头文件，当然我们不太可能在我们的工程中修改这些头文件的，所以说他们是稳定的。
那么我们如何指定它来生成预编译头文件。我们知道一个头文件是不能编译的。所以我们还需要一个cpp文件来生成.pch 文件。这个文件默认的就是StdAfx.cpp。在这个文件里只有一句代码就是：#include“Stdafx.h”。原因是理所当然的，我们仅仅是要它能够编译而已―――也就是说，要的只是它的.cpp的扩展名。我们可以用/Yc编译开关来指定StdAfx.cpp来生成一个.pch文件，通过/Fp 编译开关来指定生成的pch文件的名字。打开project ->Setting->C/C++ 对话框。把Category指向Precompiled Header。在左边的树形视图里选择整个工程，Project Options(右下角的那个白的地方)可以看到 /Fp “debug/PCH.pch”，这就是指定生成的.pch文件的名字，默认的通常是　.pch。然后，在左边的树形视图里选择 StdAfx.cpp，这时原来的Project Option变成了 Source File Option（原来是工程，现在是一个文件，当然变了）。在这里我们可以看到 /Yc开关，/Yc的作用就是指定这个文件来创建一个Pch文件。/Yc后面的文件名是那个包含了稳定代码的头文件，一个工程里只能有一个文件的可以有 YC开关。VC就根据这个选项把 StdAfx.cpp编译成一个Obj文件和一个PCH文件。
这样，我们就设置好了预编译头文件。也就是说，我们可以使用预编译头功能了。以下是注意事项：
1）如果使用了/Yu，就是说使用了预编译，我们在每个.cpp文件的最开头，包含你指定产生pch文件的.h文件（默认是stdafx.h）不然就会有问题。如果你没有包含这个文件，就告诉你Unexpected file end.
2）如果你把pch文件不小心丢了，根据以上的分析，你只要让编译器生成一个pch文件就可以了。也就是说把stdafx.cpp（即指定/Yc的那个cpp文件）重新编译一遍就可以了。
那么在Linux平台下有没有这种预编译机制呢？如果有，它是怎么实现的呢？Linux平台下GCC编译器也实现了预编译机制的。这里以开源IDE CodeBlocks（CodeBlocks内置了GCC编译器）的工程为例来说明Linux平台的实现：
使用CodeBlocks建一个C++工程，然后新建一个my_pch.h，输入如下代码：

```
/*************************************************************** 
 * Name:      my_pch.h 
 * Purpose:   Header to create Pre-Compiled Header (PCH) 
 * Author:     () 
 * Created:   2010-10-26 
 * Copyright:  () 
 * License: 
 * 使用方法: 项目构建选项-->其他选项-->填入下面两行 
 -Winvalid-pch 
 -include my_pch.h 
 **************************************************************/  
  
#ifndef MY_PCH_H_INCLUDED  
#define MY_PCH_H_INCLUDED  
  
// put here all your rarely-changing header files  
  
#include <iostream>  
#include <string>  
  
#endif
```
然后在项目构建选项–>其他选项–>填入下面两行


```
-Winvalid-pch
-include my_pch.h
```
就可以启用预编译文件头。
然后 main.cpp 就可以不用 include 头文件了，直接这样就可以编译了


```
int main()  
{   
using namespace std;  
    cout << "Hello world!" << endl;  
    return 0;  
}
```
即使在上面的代码写上下面一行，其实是不起作用的：


```
#include <iostream>
```
### 参考文献：
1. 编译器处理相关
http://blog.donews.com/xzwenlan/archive/2004/12/23/211668.aspx
2. GoogleC++ Style Guide
http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#Names_and_Order_of_Includes
3.《C++编程思想》
4. VC++的预编译功能
http://www.neu.edu.cn/cxsj/pointchart/c11/VC++%282%29.html
