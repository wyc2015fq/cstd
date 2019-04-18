# C 语言宏定义 #define 的理解与资料整理 - 文章 - 伯乐在线
原文出处： [慧可](http://www.cnblogs.com/haore147/p/3646934.html)
### 1. 利用 define 来定义 数值宏常量
#define 宏定义是个演技非常高超的替身演员，但也会经常耍大牌的，所以我们用它要慎之又慎。它可以出现在代码的任何地方，从本行宏定义开始，以后的代码就就都认识这个宏了；也可以把任何东西定义成宏。因为编译器会在预编译的时候用真身替换替身，而在我们的代码里面却又用常常用替身来帮忙。
看例子：

```
#define PI 3.141592654
```
在此后的代码中你尽可以使用PI 来代替3.141592654，而且你最好就这么做。不然的话，如果我要把PI 的精度再提高一些，你是否愿意一个一个的去修改这串数呢？你能保证不漏不出错？而使用PI 的话，我们却只需要修改一次（这是十分高效的）。
这种情况还不是最要命的，我们再看一个例子：

```
#define ERROR_POWEROFF  -1
```
如果你在代码里不用ERROR_POWEROFF 这个宏而用-1，尤其在函数返回错误代码的时候（往往一个开发一个系统需要定义很多错误代码）。肯怕上帝都无法知道-1 表示的是什么意思吧。这个-1，我们一般称为“魔鬼数”，上帝遇到它也会发狂的。所以，我奉劝你代码里一定不要出现“魔鬼数”。(这里是从代码可读性的角度进行考虑！)
但是我们利用define来定义数值类型的数据，一般只是用来定义  常量 ，如果 要定义一些变量，则可以使用c语言中const这个关键字。
我们已经讨论了const 这个关键字，我们知道const 修饰的数据是有类型的，而define 宏定义的数据没有类型。为了安全，我建议你以后在定义一些宏常数的时候用const代替，编译器会给const 修饰的只读变量做类型校验，减少错误的可能。
但一定要注意const修饰的不是常量而是readonly 的变量，const 修饰的只读变量不能用来作为定义数组的维数，也不能放在case 关键字后面。
### 2.利用 define 来定义 字符串宏常量
除了定义宏常数之外，经常还用来定义字符串，尤其是**路径**：

```
A),#define ENG_PATH_1 E:\English\listen_to_this\listen_to_this_3
B),#define ENG_PATH_2 “E:\English\listen_to_this\listen_to_this_3”
```
噢，到底哪一个正确呢？如果路径太长，一行写下来比较别扭怎么办？用反斜杠接续符 ” 啊：

```
C), #define ENG_PATH_3 E:\English\listen_to_this\listen\_to_this_3
```
还没发现问题？这里用了4 个反斜杠，到底哪个是接续符？回去看看接续符反斜杠。
反斜杠作为接续符时，在本行其后面不能再有任何字符，空格都不行。所以，只有最后一个反斜杠才是接续符。至于A)和B)，那要看你怎么用了，既然define 宏只是简单的替换，那给ENG_PATH_1 加上双引号不就成了：“ENG_PATH_1”。
但是请注意：有的系统里规定路径的要用双反斜杠“\”,比如（这是正确的版本）：

```
#define ENG_PATH_4 E:\\English\\listen_to_this\\listen_to_this_3
```
### 3.用 define 宏定义 注释符号
上面对define 的使用都很简单，再看看下面的例子：

```
#define BSC //
#define BMC /*
#define EMC */
 
D),BSC my single-line comment
E),BMC my multi-line comment EMC
```
D)和E)都错误，为什么呢？因为注释先于预处理指令被处理,当这两行被展开成//…或/*…*/时,注释已处理完毕,此时再出现//…或/*…*/自然错误.（这一条需要对编译预处理有所理解，才能体会。看来我还得再写一篇这方面的文章。）
因此,试图用宏开始或结束一段注释是不行的。
### 4.用define 宏定义表达式
这些都好理解，下面来点有“技术含量”的，定义一年有多少秒：

```
#define SEC_A_YEAR 60*60*24*365
```
这个定义没错吧？很遗憾，很有可能错了，至少不可靠。你有没有考虑在16 位系统下把这样一个数赋给整型变量的时候可能会发生溢出？一年有多少秒也不可能是负数吧。
改一下：

```
#define SEC_A_YEAR （60*60*24*365）UL
```
又出现一个问题，这里的括号到底需不需要呢？继续看一个例子，定义一个宏函数，求x 的平方：

```
#define SQR (x) x * x
```
对不对？试试：假设x 的值为10，SQR (x)被替换后变成10*10。没有问题。
再试试：假设x 的值是个表达式10+1，SQR (x)被替换后变成10+1*10+1。问题来了，这并不是我想要得到的。怎么办？括号括起来不就完了？

```
#define SQR (x) （（x）*（x））
```
最外层的括号最好也别省了，看例子，求两个数的和：

```
#define SUM (x) （x）+（x）
```
如果x 的值是个表达式5*3,而代码又写成这样：SUM (x)* SUM (x)。替换后变成：（5*3）+（5*3）*（5*3）+（5*3）。又错了！所以最外层的括号最好也别省了。我说过define 是个演技高超的替身演员，但也经常耍大牌。要搞定它其实很简单，别吝啬括号就行了。
注意这一点：宏函数被调用时是以实参代换形参。而不是“值传送”。
### 5.宏定义中的空格
另外还有一个问题需要引起注意，看下面例子：

```
#define SUM （x） （x）+（x）
```
这还是定义的宏函数SUM（x）吗？显然不是。编译器认为这是定义了一个宏：SUM，其代表的是（x） （x）+（x）。
为什么会这样呢？其关键问题还是在于SUM 后面的这个空格。所以在定义宏的时候一定要注意什么时候该用空格，什么时候不该用空格。这个空格仅仅在定义的时候有效，在使用这个宏函数的时候，空格会被编译器忽略掉。也就是说，上一节定义好的宏函数SUM（x）在使用的时候在SUM 和（x）之间留有空格是没问题的。比如：SUM（3）和SUM （3）的意思是一样的。
### 6.#undef
#undef 是用来撤销宏定义的，用法如下：

```
#define PI 3.141592654
…
// code
#undef PI
```
//下面的代码就不能用PI 了，它已经被撤销了宏定义。
写好C语言，漂亮的宏定义很重要，使用宏定义可以防止出错，提高可移植性，可读性，方便性 等等。下面列举一些成熟软件中常用得宏定义：
1，防止一个头文件被重复包含

```
#ifndef COMDEF_H
#define COMDEF_H
//头文件内容
#endif
```
2，重新定义一些类型，防止由于各种平台和编译器的不同，而产生的类型字节数差异，方便移植。这里已经不是#define的范畴了。

```
typedef unsigned char boolean; /* Boolean value type. */
typedef unsigned long int uint32; /* Unsigned 32 bit value */
typedef unsigned short uint16; /* Unsigned 16 bit value */
typedef unsigned char uint8; /* Unsigned 8 bit value */
typedef signed long int int32; /* Signed 32 bit value */
typedef signed short int16; /* Signed 16 bit value */
typedef signed char int8; /* Signed 8 bit value */
//下面的不建议使用
typedef unsigned char byte; /* Unsigned 8 bit value type. */
typedef unsigned short word; /* Unsinged 16 bit value type. */
typedef unsigned long dword; /* Unsigned 32 bit value type. */
typedef unsigned char uint1; /* Unsigned 8 bit value type. */
typedef unsigned short uint2; /* Unsigned 16 bit value type. */
typedef unsigned long uint4; /* Unsigned 32 bit value type. */
typedef signed char int1; /* Signed 8 bit value type. */
typedef signed short int2; /* Signed 16 bit value type. */
typedef long int int4; /* Signed 32 bit value type. */
typedef signed long sint31; /* Signed 32 bit value */
typedef signed short sint15; /* Signed 16 bit value */
typedef signed char sint7; /* Signed 8 bit value */
```
3，得到指定地址上的一个字节或字

```
#define MEM_B( x ) ( *( (byte *) (x) ) )
#define MEM_W( x ) ( *( (word *) (x) ) )
```
4，求最大值和最小值

```
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
```
5，得到一个field在结构体(struct)中的偏移量

```
#define FPOS( type, field ) \
/*lint -e545 */ ( (dword) &(( type *) 0)-> field ) /*lint +e545 */
```
6,得到一个结构体中field所占用的字节数

```
#define FSIZ( type, field ) sizeof( ((type *) 0)->field )
```
7，按照LSB格式把两个字节转化为一个Word

```
#define FLIPW( ray ) ( (((word) (ray)[0]) * 256) + (ray)[1] )
```
8，按照LSB格式把一个Word转化为两个字节

```
#define FLOPW( ray, val ) \
 
(ray)[0] = ((val) / 256); \
 
(ray)[1] = ((val) & 0xFF)
```
9，得到一个变量的地址（word宽度）

```
#define B_PTR( var ) ( (byte *) (void *) &(var) )
 
#define W_PTR( var ) ( (word *) (void *) &(var) )
```
10，得到一个字的高位和低位字节

```
#define WORD_LO(xxx) ((byte) ((word)(xxx) & 255))
 
#define WORD_HI(xxx) ((byte) ((word)(xxx) >> 8))
```
11，返回一个比X大的最接近的8的倍数

```
#define RND8( x ) ((((x) + 7) / 8 ) * 8 )
```
12，将一个字母转换为大写

```
#define UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
```
13，判断字符是不是10进值的数字

```
#define DECCHK( c ) ((c) >= '0' && (c) <= '9')
```
14，判断字符是不是16进值的数字

```
#define HEXCHK( c ) ( ((c) >= '0' && (c) <= '9') ||\
 
((c) >= 'A' && (c) <= 'F') ||\
 
((c) >= 'a' && (c) <= 'f') )
```
15，防止溢出的一个方法

```
#define INC_SAT( val ) (val = ((val)+1 > (val)) ? (val)+1 : (val))
```
16，返回数组元素的个数

```
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( (a[0]) ) )
```
17，返回一个无符号数n尾的值MOD_BY_POWER_OF_TWO(X,n)=X%(2^n)

```
#define MOD_BY_POWER_OF_TWO( val, mod_by ) \
( (dword)(val) & (dword)((mod_by)-1) )
```
18，对于IO空间映射在存储空间的结构，输入输出处理

```
#define inp(port) (*((volatile byte *) (port)))
 
#define inpw(port) (*((volatile word *) (port)))
 
#define inpdw(port) (*((volatile dword *)(port)))
 
#define outp(port, val) (*((volatile byte *) (port)) = ((byte) (val)))
 
#define outpw(port, val) (*((volatile word *) (port)) = ((word) (val)))
 
#define outpdw(port, val) (*((volatile dword *) (port)) = ((dword) (val)))
```
19,使用一些宏跟踪调试
A N S I标准说明了五个预定义的宏名。它们是：

```
_ LINE _
_ FILE _
_ DATE _
_ TIME _
_ STDC _
```
可以定义宏，例如:
当定义了_DEBUG，输出数据信息和所在文件所在行

```
#ifdef _DEBUG
 
#define DEBUGMSG(msg,date) printf(msg);printf(“%d%d%d”,date,_LINE_,_FILE_)
 
#else
 
#define DEBUGMSG(msg,date)
 
#endif
```
20，宏定义防止使用是错误
**用小括号包含。 **
例如：

```
#define ADD(a,b) （a+b）
```
用**do{}while(0)**语句包含多语句防止错误
例如：

```
#define DO(a,b) a+b;\
 
a++;
```
应用时：

```
if(….)
 
DO(a,b); //产生错误
 
else
```
解决方法: 代码就只会执行一次。和直接加花括号有什么区别呢。哦对，不能随便在程序中，任意加｛｝，组成代码块的。

```
#define DO(a,b) do{a+b;\
 
a++;}while(0)
```
