# 一套帮助你理解 C 语言的测试题 - 文章 - 伯乐在线
原文出处： [nowamagic](http://www.nowamagic.net/librarys/veda/detail/775)
在 [steven kobes](http://stevenkobes.com/ctest.html) 上发现一套很有趣的C语言测试题，如果你招聘C语言相关开发人员，或者正在学习C语言，很值得做一做。
如果没有做，下面内容暂时不要看，最好自己先完成一遍。
OK，假设你做的答案没有完全正确，那你可以继续看下去了，否则，后面内容对你来说就是小菜一碟，不值得看。
第一题：

C
```
#include 
static jmp_buf buf;
int main(void)
{
	volatile int b = 3;
	if (setjmp(buf) != 0)
	{
		printf("%dn", b);
		exit(0);
	}
	b = 5;
	longjmp(buf, 1);
}
```
输出结果为A）3 B）5 C）0 D）都不是
答案为B，也就是输出5。
关键点在于理解setjmp以及longjmp，（http://en.wikipedia.org/wiki/Setjmp.h ）第一次运行到setjmp，会设置jmp_buf，然后返回0。当调用longjmp时，会把longjmp里面的非0值作为setjmp的返回值返回（如果longjmp的value参数为0，setjmp恢复后返回1，也就是当恢复到setjmp存储点的时候，setjmp一定不会返回0）。
setjmp-longjmp组合的用处类似于游戏中的存盘读盘功能，经常被用于类似C++的异常恢复操作。
第二题：

C
```
struct node
{
    int a;      
    int b;      
    int c;
};
struct node s = { 3, 5, 6 };
struct node *pt = &s;
printf("%dn", *(int*)pt);
```
返回结果为3，这个算是比较简单，pt为指向结构s的指针，然后将pt转换为int指针，进行dereference，取出一个int值，那就是结构中第一个数。
我们将题目改动一下，如下代码：

C
```
struct node
{
    char a;   
    char b;  
    short c;   
    int d;
};
struct node s = { 3, 5, 6, 99 };
struct node *pt = &s;
printf("%Xn", *(int*)pt);
```
需要注意的是一般32位C编译器都认为char是8bit，short是16bit，int为32bit，所以node在内存中应该正好是对齐的，也就是abc这几个成员之间没有空隙。最终结果应该为60503，如果不是，欢迎你告诉我你具体的编译环境以及硬件配置。
第三题：

C
```
int foo(int x, int n){
	int val = 1;
	if (n > 0)
	{
		if (n % 2 == 1) val *= x;
		val *= foo(x * x, n / 2);
	}
	return val;
}
```
这道题其实最简单的办法就是在纸上做一个推演计算，一步一步跑一下，就能得到答案了，这里面没有任何复杂的C语言概念。
第四题：

C
```
int a[5] = { 1, 2, 3, 4, 5 };
int *ptr = (int*)(&a + 1);
printf("%d %dn", *(a + 1), *(ptr – 1));
```
这道题考的其实是指向数组的指针，&a是一个隐式的指向int [5]数组的指针，它和int* ptr是不一样的，如果真要定义这个指针，应该是int (*ptoa)[5]。所以ptoa每一次加一操作都相当于跨越int a[5]的内存步长（也就是5个int长度），也就是说&a + 1其实就是指向了a[5]这个位置，实际上内存里面这个位置是非法的，但是对ptr的强制转换导致了后面ptr-1的内存步长改为了1个int长度，所以ptr-1实际指向了a[4]。至于*(a+1)没什么好说的，值就是2。
第五题：

C
```
void foo(int[][3]);
int main(void)
{
	int a[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
	foo(a);
	printf("%dn", a[2][1]);
	return 0;
}
void foo(int b[][3])
{
	++b;
	b[1][1] = 9;
}
```
其实和前一题有异曲同工之妙，++b的步长实际上是3个int，也就是++b运算以后，b指向{4,5,6}这个数组的开始，而b[1]就是{7,8,9}, b[1][1]实际上就是8这个值也就是main函数中的a[2][1].
第六题：

C
```
int a, b, c, d;
a = 3;
b = 5;
c = a, b;
d = (a, b);
printf("c=%d  ", c);
printf("d=%dn", d);
```
这个其实有两个C语言知识点，一个是等号操作符优先级高于逗号操作符，另一个是逗号操作符相当于运算逗号前半部后半部的表达式，然后返回后半部表达式的值。所以c等于a（先计算等号），而d等于b（逗号表达式返回b）。
第七题：

C
```
int a[][3] = {1, 2, 3, 4, 5, 6};
int (*ptr)[3] = a;
printf("%d %d ", (*ptr)[1], (*ptr)[2]);
++ptr;
printf("%d %dn", (*ptr)[1], (*ptr)[2]);
```
依然是2维数组相关题目，ptr为指向int [3]数组的指针，首先指向a[0]，所以(*ptr)[1], (*ptr)[2]就是a[0][1], a[0][2].然后++ptr，相当于ptr指向了a[1]，这时得到的是a[1][1]，a[1][2]，所以结果就是2，3, 5, 6。
第八题：

C
```
int *f1(void)
{
	int x = 10;   return &x;
}
int *f2(void)
{
	int *ptr;   *ptr = 10;   return ptr;
}
int *f3(void)
{
	int *ptr;   ptr = malloc(sizeof *ptr);   return ptr;
}
```
这里考的是返回一个指针的问题，一般来说返回指针的函数，里面一定有malloc之类的内存申请操作，传入指针类型，则是对指针指向的内容做修改。如果想修改指针本身，那就要传入指针的指针。
第九题：

C
```
int i = 3;   int j;
j = sizeof(++i + ++i);
printf("i=%d j=%dn", i, j);
```
这道题考的内容其实就是sizeof，如果计算表达式，那么表达式是不会做计算的，也就是不管加加减减，sizeof就是针对i计算大小。在32位机器上，这个j应该为4。
我将代码扩展了一下，看看大家能不能想到结果：

C
```
short m;    int n;     double dn;
int j = sizeof ( m + n);
int k = sizeof ( n + n);
int l = sizeof ( m);
int l2 = sizeof (m * m);
int l3 = sizeof (m + dn);
int l4 = sizeof (m + m);
```
第十题：

C
```
void f1(int*, int);
void (*p[2])(int*, int);
int main(void)
{
	int a = 3;
	int b = 5;
	p[0] = f1;
	p[1] = f1;
	p[0](&a, b);
	printf("%d %d ", a, b);
	p[1](&a, b);
	printf("%d %dn", a, b);
	return 0;
}
void f1(int *p, int q)
{
	int tmp = *p;   *p = q;   q = tmp;
}
```
函数指针的数组p勉强算是一个知识点，另外一个知识点就是第八题提到的，对于int q这样的参数，是不会修改其内容的。而*p则可修改p指向的内容。
第十一题：

C
```
void e(int);
int main(void)
{
	int a = 3;
	e(a);
	putchar('n');
	return 0;
}
void e(int n)
{
	if (n > 0)
	{
		e(–n);
		printf("%d ", n);
		e(–n);
	}
}
```
这道题自己debug一下就完全明白了，主要知识点就是递归调用，另外前置后置自减操作的返回值问题。
第十二题：

C
```
typedef int (*test)(float*, float*);
test tmp;
```
也是经常出现的一类题，对复杂的指针定义做解析，实际上K&R里面（5.12）也有介绍该如何解读。不熟悉的朋友可以试着练习练习标准库中的bsearch，qsort以及signal函数。
第十三题：

C
```
char p;
char buf[10] = {1, 2, 3, 4, 5, 6, 9, 8};
p = (buf + 1)[5];
printf("%dn", p);
```
也就是p实际指向*(buf + 1 + 5)，写的更诡异一些就是p=5[buf +1];也是同样结果。
第十四题：
类似十三题，也是把数组弄得有些诡异，(p += sizeof(int))[-1];相当于*(p + sizeof(int) + (-1))。
第十五题：

C
```
int ripple(int n, …)
{
	int i, j, k;
	va_list p;
	k = 0;
	j = 1;
	va_start(p, n);
	for (; j < n; ++j)
    {
        i = va_arg(p, int);
        for (; i; i &= i – 1)
        ++k;
    }
    return k;
}
int main(void)
{
    printf("%dn", ripple(3, 5, 7));
    return 0;
}
```
这道题也是两个知识点，一个是可变参数函数定义以及如何实现，va_arg会把5，7依次取出来。另一个知识点是i &= i-1，实际上是计算了i二进制形式中1的个数，每次计算都会消减掉最低有效位上的1。比如7二进制表示为111。i &= i –1的计算结果依次为110，100, 000 （也就是0）。在hacker’s Delights这本书里介绍了很多类似技巧。
第十六题：

C
```
int counter(int i)
{
	static int count = 0;
	count = count + i;
	return count;
}
int main(void)
{
	int i, j;
	for (i = 0; i <= 5; i++)  j = counter(i);
    printf("%dn", j);
    return 0;
}
```
只要了解静态局部变量的真正内涵，这道题就是小菜啦。
