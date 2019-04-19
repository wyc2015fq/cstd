# 动态库编译时（引用静态库）could not read symbols: Bad value - 三少GG - CSDN博客
2014年08月19日 22:04:15[三少GG](https://me.csdn.net/scut1135)阅读数：6159
### 编译动态库时遇到relocation R_X86_64_32 against `a local symbol'的错误
编译动态库时遇到如下错误：
... relocation R_X86_64_32 against `a local symbol' can not be used when making a shared object; recompile with -fPIC
... could not read symbols: Bad value
解决办法编译器已经提示了：recompile with -fPIC
但是我们应该重新编译谁带上这个参数呢？经过我几番折腾，发现如下情况：
1、编译.o文件的时候，没有加参数-fPIC，这问题个在gcc version 3.4.6版本没有发生，可能那个编译器默认都给加上吧。
2、当前程序中有用到某个静态库，那个静态库编译.o文件的时候没有加上-fPIC（静态库其实就是.o文件打包）。补充一点：我发现手写Makefile时即使不加-fPIC也没有这个问题，这一点很郁闷，这个只在用automake工具编译出.a文件时才出现过。
知道原因了，解决办法自然有了，**保证你编译.o文件的时候，都加上-fPIC，这样你才能编译出动态库来。**
**************
I'm Trying to Link a static Library to a shared library , I'm Getting the Following error
/usr/bin/ld: ../../../libraries/log4cplus/**liblog4cplus.a**(**fileappender.o**): relocation R_X86_64_32S against `a local symbol' can not be used when making a shared object; recompile with -fPIC
../../../libraries/log4cplus/liblog4cplus.a: could not read symbols: Bad value
collect2: ld returned 1 exit status
Assuming you are generating a shared library, most probably what happens is that the variant of `liblog4cplus.a` you
 are using wasn't compiled with `-fPIC`.
 In linux, you can confirm this by extracting the object files from the static library and [checking
 their relocations](http://stackoverflow.com/questions/1340402/how-can-i-tell-with-something-like-objdump-if-an-object-file-has-been-built-wi):
```cpp
```
ar -x liblog4cplus.a  
readelf --**relocs ****fileappender****.o **| egrep '(GOT|PLT|JU?MP_SLOT)'
```
```
If the output is empty, then the static library is not PIC and cannot be used to generate a shared object.
Since the static library contains object code which was already compiled, providing the -fPIC flag won't help.
You need to get ahold of a version of `liblog4cplus.a` compiled
 with `-fPIC` and
 use that one instead.
