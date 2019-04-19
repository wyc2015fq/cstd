# gdb调试Clang2.6 (clang-cc) - 电影旅行敲代码 - CSDN博客
2016年07月02日 12:58:00[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2917
# 引言
为了更好的阅读Clang和LLVM源码，有了调试Clang的想法。最早的时候使用VisualStudio编译Clang，但是VS只能编译Clang3.2以后的版本（官方没有说，我是试出来的），Clang最新版本的代码已很复杂了，有很多冗余的信息，不太好理清思路。
Visual Studio调试Clang图示： 
![这里写图片描述](https://img-blog.csdn.net/20160702122235991)
在调试的时候，最好使用 **‘-cc1’ **选项来进行调试，这样就不会走 driver -> compilation 这条线（这条线会fork一个新进程出来），会省掉很多冗余无趣的工作。
[StackOverflow](http://stackoverflow.com/questions/28105766/how-to-debug-llvm-source)上也有关于这个问题的讨论：
> 
An issue you’re going to have in trying to debug clang is that the first invocation of clang spawns another clang process. **What you should do is use clang -v to get the -cc1 command line** and use that as your run arguments in gdb.
***注：[Clang2.6](http://llvm.org/releases/2.6/docs/ReleaseNotes.html)只支持C/Object-C编译，并完全不支持C++的编译。***
## gdb 调试Clang2.6
## 安装Clang
安装Clang的教程有很多，我这里就不详述了。
```
$ svn co http://llvm.org/svn/llvm-project/llvm/branches/release_26/ llvm
$ cd llvm/tools/
$ svn co http://llvm.org/svn/llvm-project/cfe/branches/release_26/ clang
$ cd ..
$ mkdir build
$ cd build/
$ ../configure --enable-debug-symbols --disable-optimized
```
只是Clang2.6出现的年代，GCC大概只是4.4/4.5的版本，我本机上的GCC的版本是4.8.4，在编译的时候会出现类型错误。
` error: ‘ptrdiff_t’ does not name a type`
这个错误和头文件 **#include <cstddef>**相关，虽然添加该头文件可以解决该问题，但是毕竟只是权宜之计。
[Porting to GCC 4.6](https://gcc.gnu.org/gcc-4.6/porting_to.html)有关于这个问题的说明：
> 
Header dependency changes
Many of the standard C++ library include files have been edited to no longer include <cstddef> to get namespace std -scoped versions of size_t and ptrdiff_t.
As such, C++ programs that used the macros NULL or offsetof without including <cstddef> will no longer compile. The diagnostic produced is similar to:
error: ‘ptrdiff_t’ does not name a type 
  error: ‘size_t’ has not been declared 
  error: ‘NULL’ was not declared in this scope 
  error: there are no arguments to ‘offsetof’ that depend on a template 
  parameter, so a declaration of ‘offsetof’ must be available 
**Fixing this issue is easy: just include <cstddef>.**
既然改源码不可行，就只能将GCC版本降低到4.6以下了：
```
$ sudo apt-get install gcc-4.4 g++-4.4 gcc-4.4-multilib g++-4.4-multilib
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.4 100
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 100
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
$ sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.4 100
$ sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.8 50
```
另外，使用编译好的**Clang**版本进行编译有可能会出现以下错误：
`fatal error 'stddef.h' file not found`
这个问题现在还没有明确的解决办法，这个问题和gcc的库相关，现在唯一的解决办法就是不要使用/usr/local/bin/clang，而要使用本地编译版本中的~/llvm/build/clang，当然这样只是权宜之计。
### gdb 调试Clang2.6
由于Clang2.6不支持 **‘-cc1’**选项，所以只能使用 **driver -> compilation** 这一条线，来进行调试，但是clang2.6中的clang可执行文件只是关于driver的，断点只能打到和driver相关的源文件中，我怎么都无法将断点打到编译源码中（例如：Parser或者Sema）。
这个问题花了我一晚上，以为是gdb用法出了问题，直到第二天早晨想起来在编译Clang的时候隐约记得生成了一个可执行文件 **clang-cc**，而且这个文件在生成的时候花费的时间较长，所以就找了下，发现果然有这么一个可执行文件。
```
ubuntu@VM-194-242-ubuntu:~/llvm/build/Debug/bin$ locate clang-cc
/home/ubuntu/llvm/build/Debug/bin/clang-cc
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc/Debug
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc/Makefile
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc/Debug/.dir
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc/Debug/clang-cc.d
/home/ubuntu/llvm/build/tools/clang/tools/clang-cc/Debug/clang-cc.o
/home/ubuntu/llvm/tools/clang/tools/clang-cc
/home/ubuntu/llvm/tools/clang/tools/clang-cc/CMakeLists.txt
/home/ubuntu/llvm/tools/clang/tools/clang-cc/Makefile
/home/ubuntu/llvm/tools/clang/tools/clang-cc/clang-cc.cpp
/usr/local/libexec/clang-cc
```
然后进入到该目录下，编译了一个文件发现可行，才意识到 **clang-cc** 才是Clang2.6真正的可执行文件，help了一下，熟悉的命令出现了。
![这里写图片描述](https://img-blog.csdn.net/20160702124644537)
后面只要按照gdb的命令来进行调试就行了，例如：
`$ gdb --args ./clang-cc -emit-llvm ~/file/test.c -S -o ~/file/testfile.ll`
断点也可以打上，例如：
```
(gdb) b CodeGenModule.cpp : 597
Breakpoint 2 at 0x81536ac: file /home/ubuntu/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp, line 597.
(gdb) c
Continuing.
Breakpoint 2, clang::CodeGen::CodeGenModule::EmitGlobalDefinition (this=0x960f9b0, GD=...)
    at /home/ubuntu/llvm/tools/clang/lib/CodeGen/CodeGenModule.cpp:598
598   const ValueDecl *D = GD.getDecl();
(gdb) l
593   // Otherwise emit the definition.
594   EmitGlobalDefinition(GD);
595 }
596 
597 void CodeGenModule::EmitGlobalDefinition(GlobalDecl GD) {
598   const ValueDecl *D = GD.getDecl();
599   
600   if (const CXXConstructorDecl *CD = dyn_cast<CXXConstructorDecl>(D))
601     EmitCXXConstructor(CD, GD.getCtorType());
602   else if (const CXXDestructorDecl *DD = dyn_cast<CXXDestructorDecl>(D))
(gdb)
```
**另：如果想要调试clang static analyzer的话，可以在visual studio中，将调试命令改为:**
`clang --analyze -Xanalyzer -analyzer-checker=core.DivideZero test.c`
