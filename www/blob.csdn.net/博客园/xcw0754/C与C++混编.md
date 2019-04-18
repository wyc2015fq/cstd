# C与C++混编 - xcw0754 - 博客园
# [C与C++混编](https://www.cnblogs.com/xcw0754/p/9960208.html)
了解一下C与C++如何合作，gcc和g++编译出来的东西有什么区别。
### 工具使用
objdump是个好工具，可以用于查看`.o`文件的内容，也可以查看可执行文件的内容。
查看符号表
`objdump -t foo.o`
查看正文段
`objdump -S foo.o`
查看所有session
`objdump -D foo.o`
### 正文
先来看下面这个文件foo.c
```
#include <stdio.h>
#include "foo.h"
void foo()
{
    printf("foo\n");
}
```
以`gcc -c foo.c`编译结果如下
```
0000000000000000 <_Z3foov>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   bf 00 00 00 00          mov    $0x0,%edi
   9:   e8 00 00 00 00          callq  e <_Z3foov+0xe>
   e:   90                      nop
   f:   5d                      pop    %rbp
  10:   c3                      retq
```
以`g++ -c foo.c`编译结果如下
```
0000000000000000 <foo>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   bf 00 00 00 00          mov    $0x0,%edi
   9:   e8 00 00 00 00          callq  e <foo+0xe>
   e:   90                      nop
   f:   5d                      pop    %rbp
  10:   c3                      retq
```
这个文件足够简单，可以看到区别就只是函数名而已，gcc并没有改变函数名，而g++在前后加了一些串。其实g++将参数信息插在函数名的尾部了，如上的`_Z3foov`中的`v`就代表了void。
- 如果是有1个参数int，那函数名是`_Z3fooi`。
- 如果是有1个参数double，那函数名是`_Z3food`。
- 如果有两个参数int和double，那函数名应该是`_Z3fooid`。
如果参数是个自定义的类呢，比如:
```
int foo(My my)
{
    return 0;
}
```
被编译成
```
0000000000000047 <_Z3foo2My>:
  47:   55                      push   %rbp
  48:   48 89 e5                mov    %rsp,%rbp
  4b:   89 7d f0                mov    %edi,-0x10(%rbp)
  4e:   b8 00 00 00 00          mov    $0x0,%eax
  53:   5d                      pop    %rbp
  54:   c3                      retq
```
可以看到，直接以类名拼接在末尾。
如果是个std的类呢？比如string
```
void foo(std::string my)
{
    printf("foo%s\n", my.c_str());
}
```
被编译成
```
000000000000001a <_Z3fooNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE>:
  1a:   55                      push   %rbp
  1b:   48 89 e5                mov    %rsp,%rbp
  1e:   48 83 ec 10             sub    $0x10,%rsp
  22:   48 89 7d f8             mov    %rdi,-0x8(%rbp)
  26:   48 8b 45 f8             mov    -0x8(%rbp),%rax
  2a:   48 89 c7                mov    %rax,%rdi
  2d:   e8 00 00 00 00          callq  32 <_Z3fooNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE+0x18>
  32:   48 89 c6                mov    %rax,%rsi
  35:   bf 00 00 00 00          mov    $0x0,%edi
  3a:   b8 00 00 00 00          mov    $0x0,%eax
  3f:   e8 00 00 00 00          callq  44 <_Z3fooNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE+0x2a>
  44:   90                      nop
  45:   c9                      leaveq 
  46:   c3                      retq
```
很长很长，因为类名确实很长，这个你用lstrace跑个程序就知道了，很多函数名都很长得看不懂。
### 　Ｃ++调用Ｃ
在Ｃ++源文件中是不能直接调用Ｃ源文件中的函数的，链接的时候就会报`对‘foo()’未定义的引用`，因为Ｃ++源文件编译时没问题，链接时就找不到符号了．举个例子，现在有文件main.cpp、foo.h、foo.c。
main.cpp内容如下：
```
#include "foo.h"
int main()
{
    foo();
    return 0;
}
```
foo.h内容如下：
```
#ifndef __FOO__
#define __FOO__
void foo();
#endif
```
foo.c内容如下：
```
#include <stdio.h>
void foo()
{
    printf("foo\n");
}
```
现在以如下命令编译他们
```
g++ -c main.cpp
gcc -c foo.c
g++ -o test foo.o main.o  # 这一步会报错
```
报错内容：
```
main.c:(.text+0x10)：对‘foo()’未定义的引用
collect2: error: ld returned 1 exit status
```
这是因为在链接两个`.o`文件时，找不到`foo`这个函数才报的错。`foo`确实是在`foo.o`里边的，只不过`main.o`中其实需要的是函数`_Z3foov`才对。
正确的做法是修改`foo.h`文件如下
```
#ifndef __FOO__
#define __FOO__
extern "C" {
void foo();
}
#endif
```
这样编译出来的`foo.o`没有任何区别，但是`main.o`就有区别了，里面的符号`_Z3foov`全被替换成`foo`了（用objdump -t查看），这样链接起来就没问题。
看到这里，`extern "C"`的用法也就清晰了，即告诉g++编译器，大括号内的符号都以Ｃ的符号命名方式去调用。值得注意的是，通常`foo.h`不是一直被cpp文件所include的，有时一个程序会有C和CPP文件同时需要include它，一般需要在使用`extern "C"`的时候用宏`__cplusplus`来判断此时的编译器是不是C++的，就像下面这样：
```
#ifndef __FOO__
#define __FOO__
#ifdef __cplusplus
extern "C" {
#endif
void foo();
#ifdef __cplusplus
}
#endif
#endif
```

