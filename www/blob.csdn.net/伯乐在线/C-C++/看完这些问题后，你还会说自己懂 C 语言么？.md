# 看完这些问题后，你还会说自己懂 C 语言么？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [xxmen](http://www.jobbole.com/members/xxmen) 翻译，[Alick](http://www.jobbole.com/members/a1ickgu0) 校稿。未经许可，禁止转载！
英文出处：[Dmitri Gribenko](http://kukuruku.co/hub/programming/i-do-not-know-c)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这篇文章的目的是让每个程序员（特别是 C 程序员）说：我真的不懂 C。我想要让大家看到 C 语言的那些阴暗角落比我们想象中更近，甚至那些平常的代码中就包含着未定义的行为。
这篇文章设置了一系列的问题和答案。所有的例子都是从源代码中单独分离出来的。
1.

C
```
int i;
int i = 10;
```
Q：这段代码正确吗？是否会因为变量被定义了两次而导致错误的出现？注意这是源于同一个源码文件，而不是函数体或代码段的一部分。
A：是的，这段代码是正确的。第一行是临时的定义直到编译器处理了第二行的定义之后才成为正式的“定义”。
2.

C
```
extern void bar(void);
void foo(int *x)
{
  int y = *x;  /* (1) */
  if(!x)       /* (2) */
  {
    return;    /* (3) */
  }
  bar();
  return;
}
```
Q: 这样写的结果是即使 x 是空指针 bar() 函数都会被调用，并且程序不会崩溃。这是否是优化器的错误，或者全部是正确的？
A: 全部都是正确的。如果 x 是空指针，未定义的行为出现在第 (1) 行， 没有人欠程序员什么，所以程序并不会在第 (1) 行崩溃， 也不会试图在第 (2) 行返回假如已经成功运行第 (1) 行。让我们来探讨编译器遵循的规则，它都按如下的方式进行。在对第 (1) 行的分析之后，编译器认为 x 不会是一个空指针，于是第 (2) 行和 第 (3) 行就被认定为是没用的代码。变量 y 被当做没用的变量去除。从内存中读取的操作也会被去除，因为 *x 并不符合易变类型（volatile）。
这就是无用的变量如何导致空指针检查失效的例子。
3.有这样一个函数：

C
```
#define ZP_COUNT 10
void func_original(int *xp, int *yp, int *zp)
{
  int i;
  for(i = 0; i < ZP_COUNT; i++)
  {
    *zp++ = *xp + *yp;
  }
}
```
有人想要按如下方式来优化它：

C
```
void func_optimized(int *xp, int *yp, int *zp)
{
  int tmp = *xp + *yp;
  int i;
  for(i = 0; i < ZP_COUNT; i++)
  {
    *zp++ = tmp;
  }
}
```
Q：调用原始的函数和调用优化后的函数，对于变量 zp 是否有可能获得不同的结果?
A：这是可能的，当 yp == zp 时结果就不同。
4.

C
```
double f(double x)
{
  assert(x != 0.);
  return 1. / x;
}
```
Q: 这个函数是否可能返回最大下界（inf） ？假设浮点数运算是按照IEEE 754 标准（大部分机器遵循）执行的， 并且断言语句是可用的（NDEBUG 并没有被定义）。
A：是的，这是可以的。通过传入一个非规范化的 x 的值，比如 1e-309.
5.

C
```
int my_strlen(const char *x)
{
  int res = 0;
  while(*x)
  {
    res++;
    x++;
  }
  return res;
}
```
Q: 上面提供的函数应该返回以空终止字符结尾的字符串长度，找出其中存在的一个 bug 。
A: 使用 int 类型来存储对象的大小是错误的，因为无法保证 int 类型能够存下任何对象的大小，应该使用 size_t。
6.

C
```
#include <stdio.h>
#include <string.h>
int main()
{
  const char *str = "hello";
  size_t length = strlen(str);
  size_t i;
  for(i = length - 1; i >= 0; i--)
  {
    putchar(str[i]);
  }
  putchar('n');
  return 0;
}
```
Q: 这个循环是死循环。这是为什么？
A: size_t 是无符号类型。 如果 i 是无符号类型， 那么 i >= 0 永远都是正确的。
7.

C
```
#include <stdio.h>
void f(int *i, long *l)
{
  printf("1. v=%ldn", *l); /* (1) */
  *i = 11;                  /* (2) */
  printf("2. v=%ldn", *l); /* (3) */
}
int main()
{
  long a = 10;
  f((int *) &a, &a);
  printf("3. v=%ldn", a);
  return 0;
}
```
这个程序分别用两个不同的编译器编译并且在一台小字节序的机器上运行。获得了如下两种不同的结果：

C
```
1. v=10    2. v=11    3. v=11
1. v=10    2. v=10    3. v=11
```
Q：你如何解释第二种结果？
A：所给程序存在未定义的行为。程序违反了编译器的强重叠规则（strict aliasing）。虽然 int 在第 (2) 行被改变了，但是编译器可以假设任何的 long 都没有改变。我们不能间接引用那些和其他不兼容类型指针相重名的指针。这就是编译器之所以可以传递和在第一行的执行过程中被读取的相同的 long (第(3)行)的原因。
8.

C
```
#include <stdio.h>
int main()
{
  int array[] = { 0, 1, 2 };
  printf("%d %d %dn", 10, (5, array[1, 2]), 10);
}
```
Q: 这个代码是否是正确的？如果不存在未定义行为，那么它会输出什么？
A: 是的， 这里使用了逗号运算符。首先，逗号左边的参数被计算后丢弃，然后，右边的参数经过计算后被当做整个运算符的值使用，所以输出是 10 2 10。
注意在函数调用中的逗号符号（比如 f(a(), b())）并不是逗号运算符，因此也就不会保证运算的顺序，a() 和 b() 会以随机的顺序计算。
9.

C
```
unsigned int add(unsigned int a, unsigned int b)
{
  return a + b;
}
```
Q: 函数 add(UINT_MAX, 1) 的结果是什么？
A：对于无符号数的溢出结果是有定义的，结果是 2^(CHAR_BIT * sizeof(unsigned int)) ，所以函数 add 的结果是 0 。
10.

C
```
int add(int a, int b)
{
  return a + b;
}
```
Q：函数 add(INT_MAX, 1) 的结果是什么？
A：有符号整数的溢出结果是未定义的行为。
11.

C
```
int neg(int a)
{
  return -a;
}
```
Q：这里是否可能出现未定义的行为？如果是的话，是在输入什么参数时发生的？
A：neg(INT_MIN)。如果 ECM 用补码表示负整数， 那么 INT_MIN 的绝对值比 INT_MAX 的绝对值大一。在这种情况下，-INT_MIN 造成了有符号整数的溢出，这是一种未定义的行为。
12.

C
```
int div(int a, int b)
{
  assert(b != 0);
  return a / b;
}
```
Q：这里是否可能出现未定义的行为？如果是的话，是在什么参数上发生的？
A：如果 ECM 用补码表示负数， 那么 div(INT_MIN, -1) 导致了与上一个例子相同的问题。
