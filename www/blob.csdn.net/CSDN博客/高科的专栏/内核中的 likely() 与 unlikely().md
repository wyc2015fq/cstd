
# 内核中的 likely() 与 unlikely() - 高科的专栏 - CSDN博客

2012年08月23日 16:06:13[高科](https://me.csdn.net/pbymw8iwm)阅读数：1981


在 2.6 内核中，随处可以见到 likely() 和 unlikely() 的身影，那么为什么要用它们？它们之间有什么区别？
首先要明确：
if(likely(value)) 等价于 if(value)
if(unlikely(value)) 也等价于 if(value)
也就是说 likely() 和 unlikely() 从阅读和理解代码的角度来看，是一样的！！！
这两个宏在内核中定义如下：
\#define likely(x)       __builtin_expect((x),1)
\#define unlikely(x)     __builtin_expect((x),0)
__builtin_expect() 是 GCC (version >= 2.96）提供给程序员使用的，目的是将“分支转移”的信息提供给编译器，这样编译器可以对代码进行优化，以减少指令跳转带来的性能下降。
__builtin_expect((x),1) 表示 x 的值为真的可能性更大；
__builtin_expect((x),0)表示 x 的值为假的可能性更大。
也就是说，使用 likely() ，执行 if 后面的语句的机会更大，使用unlikely()，执行else 后面的语句的机会更大。
例如下面这段代码，作者就认为 prev 不等于 next 的可能性更大，
if (likely(prev != next)) {
next->timestamp = now;
...
} else {
...;
}
通过这种方式，编译器在编译过程中，会将可能性更大的代码紧跟着起面的代码，从而减少指令跳转带来的性能上的下降。
下面以两个例子来加深这种理解：
第一个例子： example1.c
int testfun(int x)
{
if(__builtin_expect(x, 0)) {
^^^--- We instruct the compiler, "else" block is more probable
x = 5;
x = x * x;
} else {
x = 6;
}
return x;
}
在这个例子中，我们认为 x 为0的可能性更大
编译以后，通过 objdump 来观察汇编指令，在我的 2.4 内核机器上，结果如下：
\# gcc -O2 -c example1.c
\# objdump -d example1.o
Disassembly of section .text:
00000000 <testfun>:
0:   55                      push   %ebp
1:   89 e5                   mov    %esp,%ebp
3:   8b 45 08                mov    0x8(%ebp),%eax
6:   85 c0                   test   %eax,%eax
8:   75 07                   jne    11 <testfun+0x11>
a:   b8 06 00 00 00          mov    $0x6,%eax
f:   c9                      leave
10:   c3                      ret
11:   b8 19 00 00 00          mov    $0x19,%eax
16:   eb f7                   jmp    f <testfun+0xf>
可以看到，编译器使用的是 jne （不相等跳转）指令，并且 else block 中的代码紧跟在后面。
8:   75 07                   jne    11 <testfun+0x11>
a:   b8 06 00 00 00          mov    $0x6,%eax
第二个例子：example2.c
int testfun(int x)
{
if(__builtin_expect(x, 1)) {
^^^ --- We instruct the compiler, "if" block is more probable
x = 5;
x = x * x;
} else {
x = 6;
}
return x;
}
在这个例子中，我们认为 x 不为 0 的可能性更大
编译以后，通过 objdump 来观察汇编指令，在我的 2.4 内核机器上，结果如下：
\# gcc -O2 -c example2.c
\# objdump -d example2.o
Disassembly of section .text:
00000000 <testfun>:
0:   55                      push   %ebp
1:   89 e5                   mov    %esp,%ebp
3:   8b 45 08                mov    0x8(%ebp),%eax
6:   85 c0                   test   %eax,%eax
8:   74 07                   je     11 <testfun+0x11>
a:   b8 19 00 00 00          mov    $0x19,%eax
f:   c9                      leave
10:   c3                      ret
11:   b8 06 00 00 00          mov    $0x6,%eax
16:   eb f7                   jmp    f <testfun+0xf>
这次编译器使用的是 je （相等跳转）指令，并且 if block 中的代码紧跟在后面。
8:   74 07                   je     11 <testfun+0x11>
a:   b8 19 00 00 00          mov    $0x19,%eax


