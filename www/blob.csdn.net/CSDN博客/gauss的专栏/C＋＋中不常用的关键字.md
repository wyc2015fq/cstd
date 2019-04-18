# C＋＋中不常用的关键字 - gauss的专栏 - CSDN博客
2013年02月05日 22:28:34[gauss](https://me.csdn.net/mathlmx)阅读数：166
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

mutable关键字
    关键字mutable是C＋＋中一个不常用的关键字,他只能用于类的非静态和非常量数据成员我们知道一个对象的状态由该对象的非静态数据成员决定,所以随着数据成员的改变,对像的状态也会随之发生变化!
如果一个类的成员函数被声明为const类型,表示该函数不会改变对象的状态,也就是该函数不会修改类的非静态数据成员.但是有些时候需要在该类函数中对类的数据成员进行赋值.这个时候就需要用到mutable关键字了
例如:
class Demo
{
public:
    Demo(){}
    ~Demo(){}
public:
    bool getFlag() const
    {
        m_nAccess＋＋;
        return m_bFlag;
    }
private:
    int m_nAccess;
    bool m_bFlag;
};
int main()
{
    return 0;
}
编译上面的代码会出现 error C2166: l-value specifies const object的错误说明在const类型的函数中改变了类的非静态数据成员.这个时候需要使用mutable来修饰一下要在const成员函数中改变的非静态数据成员
m_nAccess,代码如下:
class Demo
{
public:
    Demo(){}
    ~Demo(){}
public:
    bool getFlag() const
    {
        m_nAccess＋＋;
        return m_bFlag;
    }
private:
    mutable int m_nAccess;
    bool m_bFlag;
};
int main()
{
    return 0;
}
这样再重新编译的时候就不会出现错误了!
volatile关键字
volatile是c/c＋＋中一个鲜为人知的关键字,该关键字告诉编译器不要持有变量的临时拷贝,它可以适用于基础类型
如：int,char,long......也适用于C的结构和C＋＋的类。当对结构或者类对象使用volatile修饰的时候，结构或者类的所有 成员都会被视为volatile.使用volatile并不会否定对CRITICAL_SECTION,Mutex,Event等同步对象的需要
例如：
int i;
i = i ＋ 3;
无论如何，总是会有一小段时间，i会被放在一个寄存器中，因为算术运算只能在寄存器中进行。一般来说，volatitle关键字适用于行与行之间，而不是放在行内。
我们先来实现一个简单的函数，来观察一下由编译器产生出来的汇编代码中的不足之处，并观察volatile关键字如何修正这个不足之处。在这个函数体内存在一个busy loop(所谓busy loop也叫做busy waits,是一种高度浪费CPU时间的循环方法)
void getKey(char* pch)
{
while (*pch == 0)
;
}
当你在VC开发环境中将最优化选项都关闭之后，编译这个程序，将获得以下结果(汇编代码)
;       while (*pch == 0)
$L27
; Load the address stored in pch
mov eax, DWORD PTR _pch$[ebp]
; Load the character into the EAX register
movsx eax, BYTE PTR [eax]
; Compare the value to zero
test eax, eax
; If not zero, exit loop
jne $L28
;
jmp $L27
$L28
;}
这段没有优化的代码不断的载入适当的地址，载入地址中的内容，测试结果。效率相当的低，但是结果非常准确现在我们再来看看将编译器的所有最优化选项开关都打开以后，重新编译程序，生成的汇编代码，和上面的代码
比较一下有什么不同
;{ 
; Load the address stored in pch
mov eax, DWORD PTR _pch$[esp-4]
; Load the character into the AL register
movsx al, BYTE PTR [eax]
; while (*pch == 0)
; Compare the value in the AL register to zero
test al, al
; If still zero, try again
je SHORT $L84
;
;}
从代码的长度就可以看出来，比没有优化的情况要短的多。需要注意的是编译器把MOV指令放到了循环之外。这在单线程中是一个非常好的优化，但是，在 多线程应用程序中，如果另一个线程改变了变量的值，则循环永远不会结束。被测试的值永远被放在寄存器中，所以该段代码在多线程的情况下，存在一个巨大的 BUG。解决方法是重新
写一次getKey函数，并把参数pch声明为volatile,代码如下：
void getKey(volatile char* pch)
{
while (*pch == 0)
;
}
这次的修改对于非最优化的版本没有任何影响，下面请看最优化后的结果：
;{
; Load the address stored in pch
mov eax, DWORD PTR _pch$[esp-4]
;       while (*pch == 0)
$L84:
; Directly compare the value to zero
cmp BYTE PTR [eax], 0
; If still zero, try again
je SHORT $L84
;
;}
这次的修改结果比较完美，地址不会改变，所以地址声明被移动到循环之外。地址内容是volatile,所以每次循环之中它不断的被重新检查。把一个 const volatile变量作为参数传递给函数是合法的。如此的声明意味着函数不能改变变量的值，但是变量的值却可以被另一个线程在任何时间改变掉。
explicit关键字
我们在编写应用程序的时候explicit关键字基本上是很少使用,它的作用是"禁止单参数构造函数"被用于自动型别转换,其中比较典型的例子就是容器类型,在这种类型的构造函数中你可以将初始长度作为参数传递给构造函数.
例如:
你可以声明这样一个构造函数
class Array
{
public:
explicit Array(int size);
......
};
在这里explicit关键字起着至关重要的作用,如果没有这个关键字的话,这个构造函数有能力将int转换成Array.一旦这种情况发生,你可以给Array支派一个整数值而不会引起任何的问题,比如:
Array arr;
...
arr = 40;
此时,C＋＋的自动型别转换会把40转换成拥有40个元素的Array,并且指派给arr变量,这个结果根本就不是我们想要的结果.如果我们将构造 函数声明为explicit,上面的赋值操作就会导致编译器报错,使我们可以及时发现错误.需要注意的是:explicit同样也能阻止"以赋值语法进行 带有转型操作的初始化";
例如:
Array arr(40);//正确
Array arr = 40;//错误
看一下以下两种操作:
X x;
Y y(x);//显式类型转换
另一种
X x;
Y y = x;//隐式类型转换
这两种操作存在一个小小的差别,第一种方式式通过显式类型转换,根据型别x产生了型别Y的新对象;第二种方式通过隐式转换产生了一个型别Y的新对 象.explicit关键字的应用主要就是上面所说的构造函数定义种,参考该关键字的应用可以看看STL源代码,其中大量使用了该关键字
__based关键字
该关键字主要用来解决一些和共享内存有关的问题,它允许指针被定义为从某一点开始算的32位偏移值,而不是内存种的绝对位置
举个例子:
typedef struct tagDEMOSTRUCT {
int a;
char sz[10];
} DEMOSTRUCT, * PDEMOSTRUCT;
HANDLE hFileMapping = CreateFileMapping(...);
LPVOID lpShare = (LPDWORD)MapViewOfFile(...);
DEMOSTRUCT __based(lpShare)* lpDemo;
    上面的例子声明了一个指针lpDemo,内部储存的是从lpShare开始的偏移值,也就是lpHead是以lpShare为基准的偏移值.
上面的例子种的DEMOSTRUCT只是随便定义的一个结构,用来代表任意的结构.
虽然__based指针使用起来非常容易,但是,你必须在效率上付出一定的代价.每当你用__based指针处理数据,CPU都必须为它加上基地址,才能指向真正的位置.
转自：http://thatax.blog.163.com/blog/static/20892680200881010542205/
