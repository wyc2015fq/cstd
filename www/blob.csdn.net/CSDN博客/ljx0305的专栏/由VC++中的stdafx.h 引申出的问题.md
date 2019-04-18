# 由VC++中的stdafx.h 引申出的问题 - ljx0305的专栏 - CSDN博客
2009年11月04日 17:29:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1270标签：[vc++																[header																[file																[library																[compiler																[output](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
**stdafx.h 是一个特别的头文件，这个头文件里面的内容包括了大多数.cpp文件所引用的标准的和不轻易改变的头文件的定义。可以说，stdafx.h 是一个很多头文件的集合。举个例子来说，如果用户要写一个Win32 API的程序，而且这个程序包括了10个.cpp文件，由于这10个.cpp文件都包括了windows.h, string, vector等等文件，所以用户便可以把这些重复定义的头文件都放在stdafx.h ****中。当用户在编译的时候，VC++编译器会首先编译在stdafx.cpp 的内容（一般这个 stdafx.cpp 里面只包括 #include "stdafx.h" 这一句话），并且把这些都保存在一个 precompiled header file（即.pch文件）里面以供其它9个.cpp 文件来使用。其优点是显而易见的，即省去了每个 .cpp 引用的头文件的编译，而只是编译一次，这个尤其对于大型的项目很重要。比如，我有一个中型的项目，大概有100个左右的 .cpp 文件。 如果程序员取消了这个 Precompiled 的功能，那么第一次编译就要花半个小时左右的时间来编译；但是如果采用了这个 Precompiled 的功能，那么也就花费几分钟左右的时间。很多人不喜欢引用Precompiled 的功能，认为 precompiled headers 把所有在 .cpp****文件中定义的头文件都引用了，不论有用没有都包括进来了，但是对于大型项目，这还是很重要的，而且是高效的。据我所知，VC++是惟一个支持precompiled header 功能的IDE。如果用户想取消这个功能,则 ****Project->Settings->C/C++->Category:Precompiled Headers dialog-> Not use Precompiled headers.接下来说说具体如何操作,新建一个 Win32 Dynamic-Link Library 的Project, 命名为TestDLL, 然后选择 a DLL that exports some symbols 。这样就按照MS默认的定义自动生成并添加了 TestDLL.cpp , TestDLL.h , stdAfx.cpp, stdAfx.h 四个文件。首先分别Compile这四个文件，先Compile TestDLL来试一下，结果发现出错：**--------------------Configuration: TestDLL - Win32 Debug--------------------
Compiling...
TestDLL.cpp
**d:/my_project/c++/dllpractise04/testdll/testdll.cpp(4) : fatal error C1083: Cannot open    precompiled header file: 'Debug/TestDLL.pch': No such file or directory**
Error executing cl.exe.
TestDLL.obj - 1 error(s), 0 warning(s)
**原因在于第一个编译TestDLL.cpp的时候，由于系统默认的选择了****precompiled headers 的功能，所以就假定系统已经生成了 .pch 文件，即**** precompiled headers file。 但是由于我们这个程序是第一次编译，在文件夹中根本就没有这个文件，所以自然Compiler是找不到这个文件的。我们只能做的是先编译 stdAfx.cpp ，或者 Build All.**Deleting intermediate files and output files for project 'TestDLL - Win32 Debug'.
--------------------Configuration: TestDLL - Win32 Debug--------------------
Compiling...
StdAfx.cpp         看，系统首先complie的是StdAfx.cpp哦。注意这个顺序
Compiling...
TestDLL.cpp      其次才是我们自己定义的cpp.
Linking...
   Creating library Debug/TestDLL.lib and object Debug/TestDLL.exp
TestDLL.dll - 0 error(s), 0 warning(s)**好了，这个Project便成功了。可是我们还是不能停步，接着研究一下。在TestDLL.cpp中，把前两行的 #include "stdafx.h"   #include "TestDLL.h" 换一个位置变成，**** #include "TestDLL.h"   ****#include "stdafx.h"。 重新编译TestDLL.dll 或者 Build All，都发生了如下的错误：**Deleting intermediate files and output files for project 'TestDLL - Win32 Debug'.
--------------------Configuration: TestDLL - Win32 Debug--------------------
Compiling...
StdAfx.cpp
Compiling...
TestDLL.cpp
D:/My_Project/C++/DLLPRACTISE04/TestDLL/TestDLL.cpp(26) : error C2144: syntax error : missing ';' before type 'int'
D:/My_Project/C++/DLLPRACTISE04/TestDLL/TestDLL.cpp(26) : error C2501: 'TESTDLL_API' : missing storage-class or type specifiers
D:/My_Project/C++/DLLPRACTISE04/TestDLL/TestDLL.cpp(26) : fatal error C1004: unexpected end of file found
Error executing cl.exe.
TestDLL.dll - 3 error(s), 0 warning(s)**发现这么多的错误，但是唯独肯定的是StdAfx.cpp编译通过了，问题就出在我们刚才调换了的顺序。****系统默认下,在声明包含stdafx.h的语句之前的语句都将被完全忽略。****所以在 Compile 的时候就把第一句话 #include "TestDLL.h" 忽略了, 而在 TestDLL.h 中定义了关于TESTDLL_API 的意义**. **解决的办法要么把顺序调换回来，或者在Project->Settings->C/C++->Category:Precompiled Headers dialog-> Not use Precompiled headers.****重新编译就可以通过了.**
**小常识：stdafx.h 中的AFX代表**** Application Framework eXtensions. AFX是最早MFC小组的缩写。**
**引用于:http://hi.baidu.com/kxw102/blog/item/89d24d095c4a5137e824881c.html**
