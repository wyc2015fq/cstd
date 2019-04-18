# MSVC vs. MinGW 之dll玩转攻略手记 - 深之JohnChen的专栏 - CSDN博客

2014年08月16日 19:17:14[byxdaz](https://me.csdn.net/byxdaz)阅读数：7168


MinGW:

c -> o gcc -c a.c

c -> exe gcc a.c libs.o -o a.exe (从主程序a.c,附加libs，生成a.exe)

o -> exe gcc a.o b.o ... -o main.exe

c -> dll,def,a gcc a.c -shared -o a.dll -Wl,--output-def,a.def,--out-implib,liba.a

a -> dll a2dll liba.a 

dll -> a: dlltool --dllname a.dll --def a.def --output-lib liba.a (需要def文件)

a -> def: dumpbin /exports lib.a > lib.def (在windows上调用，def需要修改)

dll -> def : pexports a.dll -o > a.def (这里的-o是指给函数标序号)

lib -> def : reimp -d a.lib

lib -> a: (for __cdecl functions in most case) reimp a.lib; (for __stdcall functions)

MSVC:

c -> lib cl /LD a.c (注意已经定义了export列表)

c -> dll cl /LD a.c

c -> obj cl /c a.c

c -> exe cl a.c /out:a.exe

dll ->lib lib /machine:ix86 /def:a.def /out:a.lib (需要def文件)

obj ->lib lib a.obj b.obj... /out:mylib.lib

dll ->def DUMPBIN a.dll /EXPORTS /OUT:a.def (生成的def需要做修正)

lib ->def reimp -d a.lib (这个要在MSYS+MinGW下用)

关于这些工具的适用范围可以很容易的理解和记忆。

dll和exe都是PE文件，所以可以使用pexports.

lib和a是静态库文件，都是归档类型，不是PE格式。所以不能使用pexports.

dll可以使用dlltool.

lib可以使用lib, 和reimp(lib->a工具)

所有的bin文件，包括dll,exe,lib,a都可以使用dumpbin.

参考:

[http://hi.baidu.com/kaien_space/blog/item/5e77fafa2ba9ff16a8d3110a.html](http://hi.baidu.com/kaien_space/blog/item/5e77fafa2ba9ff16a8d3110a.html)

Mingw官网文档: [http://www.mingw.org/wiki/MSVC_and_MinGW_DLLs](http://www.mingw.org/wiki/MSVC_and_MinGW_DLLs)
[http://oldwiki.mingw.org/index.php/CreateImportLibraries](http://oldwiki.mingw.org/index.php/CreateImportLibraries)
[http://www.mingw.org/wiki/FAQ](http://www.mingw.org/wiki/FAQ)
[http://hi.baidu.com/opaquefog/blog/item/9b21b6deb324e25dccbf1ab7.html](http://hi.baidu.com/opaquefog/blog/item/9b21b6deb324e25dccbf1ab7.html)
[http://qzone.qq.com/blog/8330936-1238659272](http://qzone.qq.com/blog/8330936-1238659272)

[http://hi.baidu.com/jzinfo/blog/item/b0aa1d308de99f9da8018e00.html](http://hi.baidu.com/jzinfo/blog/item/b0aa1d308de99f9da8018e00.html)

本篇测试用代码:

1. main.cpp

#include <iostream>

#include <stdlib.h>

#include "mylib.h"

using namespace std;

int main()

{

char str[]="Hello world!";

printhello(str);

return 0;

}

2. mylib.cpp

#include <iostream>

#include <stdlib.h>

#include "mylib.h"

using namespace std;

void EXPORT printhello(char *str)

{

cout << str << endl;

}

3. mylib.h

#define EXPORT __declspec(dllexport)

extern "C"

{

void EXPORT printhello(char *str);
}

**关于DLL的定义和使用:**1. 需要外部调用的函数，定义的时候要在函数声明前加入

__declspec(dllexport)

方便起见，可以定义成宏

#define EXPORT __declspec(dllexport)

然后在定义函数声明的时候使用，例如:

void EXPORT printhello(char *str);

只有注明EXPORT的函数才能出现在dll的输出表中，外部函数才能调用。

关于函数调用约定__cdecl 还是 __stdcall。我们可以在输出函数名前添加，也可以用cl编译的时候指明

/Gd 使用 __cdecl 调用约定 (C declaration,是C和C++默认格式)，手动堆栈平衡(支持可变参数)

/Gz 使用 __stdcall 调用约定 (是pascal, fortran等的调用约定)， 自动堆栈平衡

此外还有其他的调用约定，如_fastcall，把前两个参数通过寄存器传递，调用速度快。

要使用dll,我们可以动态调用它，也可以转换成lib库静态调用。

动态调用就是说，先用LoadLibrary加载到内存中。再用GetProcAddress得到函数地址就可以使用了。比较麻烦。

静态调用就是说，先为需要调用的函数生成def文件，然后制作静态库lib文件。再用这个lib调用dll里的函数。

**__cdecl 和****__stdcall**

VC上编译C和C++程序时，默认使用__cdecl函数调用约定。如果想生成__stdcall的函数，我们可以使用/GZ编译.例如:

cl /Gz /LD mylib.cpp

这样生成的dll和lib就是使用的__stdcall约定

通过下面的命令

dumpbin /exports mylib.dll 或mylib.lib 我们可以看到

ordinal hint RVAname

10 0000107E
[_printhello@4](mailto:_printhello@4)

下面是__cdecl的函数名书写规格

ordinal hint RVAname

10 0000107E printhello

可见__stdcall里多了前缀和后缀。



**DEF文件格式:**

LIBRARY DLLNAME.DLL

EXPORTS

fonctionname1 @1

fonctionname2 @2

................

(注意__cdecl和__stdcall调用约定的DEF文件书写区别。)

**从DLL制作def文件:**

MinGW上的实现方法:

pexports mydll.dll -o > mydll.def

MSVC上的实现方法:

1. 制作一份dll的导出函数表,使用VC的dumpbin命令

dumpbin mydll.dll /exports > mylib.def

2. 打开def文件修改之

i） 添加 LIBRARY mydll.dll
EXPORTS

ii) 在EXPORTS后面加入改好的需要导出到lib中函数名列表

(注意: 因为函数调用约定的不同，所以导出的函数名会有前缀或后缀，这些都尽量不要修改。否则可能无法正常调用！具体操作，后面会举例说明)
(非常值得注意的是: 这个EXPORTS的函数名列表，有些情况下，你可能根本无法知道这些函数名的书写规则。不要总认为dumpbin中得到的名字就可以通用了。也不要认为pexports得到的def文件就不用修改了。事实上，如果我们声明函数的时候没有用extern "C"{}. 那么你在VC中调用一个MinGW的dll时你就会发现，问题变得很棘手。def的函数名书写规则和你要调用dll的函数约定有关，和dll里的前后缀无关。切记！)
另外，在使用dll里的函数的时候，需要一份.h文件，对调用的函数进行声明。这里的函数名里没有那些附加的前后缀的，这点要注意。

**在VC中生成dll和lib(调用dll库)**

cl /LD mylib.cpp(得到mylib.dll和mylib.lib)

**在MinGW中生成dll, def和a(调用dll库)**

g++ mylib.cpp -shared -o mylib_linux.dll -Wl,--output-def,mydll.def,--out-implib,mylib.a





**在VC中生成静态库lib(不使用dll)**

(注意: 静态库(lib和a)实际上就是一种归档文件)

cl -c mylib.cpp编译cpp得到obj

lib mylib.obj /out:mylib.lib (生成归档文件mylib.lib)

**在MinGW中生成静态库a(不使用dll)**

g++ -c mylib.cpp 得到o文件

ar r mylib.a mylib.o 生成归档文件mylib.a



**VC中调用VC的dll (lib方式)**

分为有lib文件和无lib文件两种情况。第二种情况需要先生成lib文件再调用。

我们感兴趣的是第二种情况。具体操作实战如下:

我们只有一个mylib.dll, 需要调用一个输出函数printhello

1. 制作def

dumpbin mylib.dll /exports >mylib.def

得到一份完整的输出函数列表。

在开头添加 LIBRARY mylib.dll

再把下面的输出函数信息
ordinal hint RVA name

10 0000107E
printhello
修改成
EXPORTS
printhello@1

其他的信息都删除。

也可以在MinGW上直接调用 pexports mylib.dll > mylib.def (这就是MinGW的方便之处了)

2. 生成lib

需要mylib.dll和mylib.def

lib /machine:ix86 /def:mylib.def

这样就会生成mylib.lib和mylib.exp两个文件。(mylib.exp可以删除)

3. 通过lib调用dll

在程序main.cpp中加入#include "mylib.h"

这样就可以调用这个函数了

cl main.cpp mylib.lib编译生成main.exe文件。
(注意:这个程序的运行需要dll的参与！编译后lib文件可以删除，但mylib.dll不能删除，切记！）

**MinGW调用MinGW的dll (直接连接和 a连接)**

MinGW的dll可以像静态库.a那样直接连接使用。

g++ main.cpp mylib.dll

也可以使用

g++ main.cpp -lmylib

直接调用dll编译生成main.exe文件。非常的简单方面。

当然，从研究的角度来说，我们讨论一下如何通过库文件a来实现连接。其方法和VC的情况雷同。

考虑无a文件的情况。

1. 制作def
直接调用 pexports mylib.dll > mylib.def

2. 生成a

需要mylib.dll和mylib.def

dlltool --dllname mylib.dll --def mylib.def --output-lib libmylib.a

这样就会生成库文件libmylib.a文件。

3. 通过a调用dll

在程序main.cpp中加入#include "mydll.h"

这样就可以调用dll里的函数了

g++ main.cpp libmylib.a -o main.exe 编译生成main.exe文件。


**VC中调用MinGW的dll**

现在如何在VC中调用MinGW生成的mylib_linux.dll呢？(注意:VC无法使用MinGW的a文件，也无法像MinGW那样直接调用dll)

我们可以使用def文件生成VC可用的lib，通过lib调用

如果你没有def文件，那就用前面说过的方法（dumpbin(手动)，或pexports(自动)) 生成一个。

如果你已经有了def文件。内容如下:

LIBRARY mylib_linux.dll

EXPORTS

printhello @1

下面的命令可以根据def生成lib

lib /machine:ix86 /def:mydll.def生成mydll.lib。

通过lib调用dll

cl main.cpp mydll.lib 生成main.exe调用dll

**MinGW中调用VC的dll**

如过dll是__cdecl约定，那么可以像静态库那样直接使用。如果

如果使用的是__stdcall调用约定。这时，我们无法像__cdecl那样直接使用了。我们有两种思路，一种是生成VC上的lib，然后直接调用。另一种是制作def和a文件，通过他们调用dll。

(注意，在这种情况下，不能用reimp从lib得到a。即使这个lib可以直接使用。生成的a也不能用。想得到可用的a文件需要按以下步骤操作)

1. 制作def
调用 pexports mylib.dll | sed "s/_//" > mylib.def （sed部分去除函数名前的_）

未完待续。。。


2. 生成a

需要mylib.dll和mylib.def

dlltool -U -D mylib.dll -d mylib.def -l libmylib.a （注意，这个-U绝对不能少）

这样就会生成库文件libmylib.a文件。

3. 通过a调用dll

在程序main.cpp中加入#include "mydll.h"

这样就可以调用dll里的函数了

g++ main.cpp libmylib.a -o main.exe 编译生成main.exe文件。

注意，调用__stdcall的函数也必须声明为__stdcall如下:

extern "C"

{

void __stdcall printhello(char *str);

}

在VC中则不需要修改代码，cl编译的时候使用/Gz即可。

写到这里，基本上已经讨论的差不多了。至于静态库.lib和.a之间的转换。据说这是同一种类型的归档文件，不同的只是归档里包含的内容。lib里包含的是.obj文件，a里包含的是.o文件。然而，这两种文件的格式据说也是相同的，然而我们发现lib和a无法通用！(请注意，这里的静态库lib不是调用dll时的那种静态库lib。那种lib只是起到索引和连接dll的功能，而这里所说的静态库是脱离dll工作的库，函数过程都包含在库里了)。以前我曾经写过一篇文章，讨论过cygwin上的库和mingw通用的方法。其实现在我们可以更清晰的明白，他们之所以通用是因为两者都是用gcc编译的。同一种编译器出来的结果当然可以兼容。那篇文章的价值在于。cygwin虽然工具包很多，但是要独立运行还需要dll支持。而mingw则可以生成不依赖于dll独立运行的程序。

那么lib和a不能通用的原因到底在哪里呢？起初觉得有可能在编译器上。因为vc的编译器cl和gcc的编译器编译出来的obj和o文件虽然格式相同却不能通用。实验表明，拿o文件给cl用，或拿obj给gcc用都通不过。然而，我对这点还不表示怀疑！因为我觉得最可能的原因并不在这里。行不通的原因很可能是两个编译器调用了不同的库函数。cl调用了MSVC提供的库函数，而gcc调用了他自己的库函数。所以我们回发现，通用静态库失败时显示的都是库函数没定义之类的错误。其实是函数名的符号不能识别。如果知道所需的库函数的具体文件，并加入到项目的编译中，相信很可能就会解决问题。不论如何，目前静态库的通用方法还有待进一步的探讨。

本文转自：[http://hi.baidu.com/kaien_space/blog/item/ae765e0aa3de501695ca6bb9.html](http://hi.baidu.com/kaien_space/blog/item/ae765e0aa3de501695ca6bb9.html)

