# C/C++ 下的void main() - 在思索中前行！ - CSDN博客





2014年08月10日 16:06:46[_Tham](https://me.csdn.net/txl16211)阅读数：517








      很多人甚至市面上的一些书籍，都使用了void main( )，其实这是错误的。C/C++ 中从来没有定义过void main( )。C++之 父 Bjarne Stroustrup在他的主页上的 FAQ中明确地写 着 The definition void main( ) { /* ... */ } is not and never has been C++, nor has it even been
 C. （ void main( ) 从来就不存在于 C++ 或者 C ）。下面我分别说一下 C 和 C++ 标准中对 main 函数的定义。
- C

在 C89 中，main( )是可以接受的。Brian W. Kernighan和 Dennis M. Ritchie的经典巨 著 The C programming Language 2e（《C 程序设计语言第二版》）用的就是 main( )。不过在最新的 C99标准 中，只有以下两种定义方式是正确的：

int main( void ) int main( int argc, char *argv[] )

（参考资料：ISO/IEC 9899:1999 (E) Programming languages — C 5.1.2.2.1 Program startup）

当然，我们也可以做一点小小的改动。例如：char *argv[] 可以写成 char **argv；argv 和 argc 可以改成别的变量名（如 intval 和 charval），不过一定要符合变量的命名规则。

如果不需要从命令行中获取参数，请用int main(void) ；否则请用int main( int argc, char *argv[] ) 。

main 函数的返回值类型必须是 int ，这样返回值才能传递给程序的调用者（如操作系统）。

如 果 main 函数的最后没有写 return 语句的话，C99 规定编译器要自动在生成的目标文件中（如 exe 文件）加入return 0; ， 表示程序正常退出。不过，我还是建议你最好在main函数的最后加上return 语句，虽然没有这个必要，但这是一个好的习惯。注意，vc6不会在目标 文件中加入return 0; ，大概是因为 vc6 是 98 年的产品，所以才不支持这个特性。现在明白我为什么建议你最好加上 return
 语句 了吧！不过，gcc3.2（Linux 下的 C 编译器）会在生成的目标文件中加入 return 0; 。
- C++

C++98 中定义了如下两种 main 函数的定义方式：

int main( ) int main( int argc, char *argv[] )

（参考资料：ISO/IEC 14882(1998-9-01)Programming languages — C++ 3.6Start and termination）

int main( ) 等 同于 C99 中的 int main( void ) ；int main( int argc, char *argv[] ) 的用法也 和 C99 中定义的一样。同样，main 函数的返回值类型也必须是int。如果main函数的末尾没写return语句，C++98 规定编译器要自 动在生成的目标文件中加入 return 0; 。同样，vc6 也不支持这个特性，但是 g++3.2（Linux 下的 C++
 编译器）支持。
- 关于 void main

在 C 和 C++ 中， 不接收任何参数也不返回任何信息的函数原型为“void foo(void);”。可能正是因为这个，所以很多人都误认为如果不需要程序返回值时可以把 main函数定义成void main(void) 。然而这是错误的！main 函数的返回值应该定义为 int 类型，C 和 C++ 标准中都是这 样规定的。虽然在一些编译器中，void main 可以通过编译（如 vc6），但并非所有编译器都支持 void main
 ，因为标准中从来没有定 义过 void main 。g++3.2 中如果 main 函数的返回值不是 int 类型，就根本通不过编译。而 gcc3.2 则会发出警告。所 以，如果你想你的程序拥有很好的可移植性，请一定要用 int main 。
- 返回值的作用

main 函数的返回值用于说明程序的退出状态。如果返回 0，则代表程序正常退出；返回其它数字的含义则由系统决定。通常，返回非零代表程序异常退出。下面我们在 winxp环境下做一个小实验。首先编译下面的程序：

int main( void ) { return 0; }

然 后打开附件里的“命令提示符”，在命令行里运行刚才编译好的可执行文件，然后输入“echo %ERRORLEVEL%”，回车，就可以看到程序的返回值 为 0 。假设刚才编译好的文件是 a.exe ，如果输入“a && dir”，则会列出当前目录下的文件夹和文件。但是如果改成 “return -1”，或者别的非 0 值，重新编译后输入“a && dir”，则 dir 不会执行。因 为 && 的含义是：如果 && 前面的程序正常退出，则继续执行
 && 后面的程序，否则不执行。也 就是说，利用程序的返回值，我们可以控制要不要执行下一个程序。这就是 int main 的好处。如果你有兴趣，也可以把 main 函数的返回值类型 改成非 int 类型（如 float），重新编译后执行“a && dir”，看看会出现什么情况，想想为什么会出现那样的情况。顺便 提一下，如果输入 a || dir的话，则表示如果 a 异常退出，则执行 dir 。
- 那么 int main( int argc, char *argv[], char *envp[] ) 呢？

这当然也不是标准 C/C++ 里面定义的东西！char *envp[] 是某些编译器提供的扩展功能，用于获取系统的环境变量。因为不是标准，所以并非所有编译器都支持，故而移植性差，不推荐使用。



