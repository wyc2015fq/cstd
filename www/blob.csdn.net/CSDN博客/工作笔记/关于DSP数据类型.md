# 关于DSP数据类型 - 工作笔记 - CSDN博客





2012年05月25日 13:22:26[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10923








这两天开始真正DSP编程，遇到个很头痛的问题，也是很基础的问题。归根到底，还是自己对数据类型没有弄清楚。

今天整理下：

# 存储顺序

### 大尾端和小尾端

### 1.定义

　　大尾端：高位在低地址。 



　　小尾端：尾位在低地址。 

### 2.字节层次。

　　如整数0x12345678在内存中应该如下存放： 



　　地低： base | base+1 | base+2 | base+3 | 



　　------------------------------------------------ | 



　　大端 | 12 ｜ 34 ｜ 56 ｜ 78 | 



　　----------------------------------------------- | 



　　小端 | 78 ｜ 56 ｜ 34 ｜ 12 ｜ 





　　ps：一般PC是小端存储的。 DSP中，要根据芯片得到。

### 3.字节内位序 。

　　在字节内依然是： 



　　大端：高位放在低地址。 



　　小端：低位放在低地址。 



　　即大端与小端位序完全是镜像的。下面的程序可证明。 



　　int _tmain(int argc, _TCHAR* argv[]) 



　　{ 



　　struct BitData { 



　　unsigned Bit0_3:4; 



　　unsigned Bit4_7:4; 



　　unsigned Bit8_11:4; 



　　unsigned Bit12_15:4; 



　　}; 



　　union SData { 



　　unsigned short Data; 



　　struct BitData BData; 



　　} ssData; 



　　ssData.Data=0x1234;//此时BData与Data完全共享同一空间。 



　　printf("%x\t%x\t%x\t%x\n",ssData.BData.Bit0_3,ssData.BData.Bit4_7,ssData.BData.Bit8_11,ssData.BData.Bit12_15);




　　return 0; 



　　} 



　　在运行程序前请先推理一下会输出什么，然后运行一下看看结果。 



　　上述程序在普通PC上（小尾端）结果应该是： 



　　4 3 2 1

# 内置数据类型

    char                            8 -2^7 ~ 2^7-1 %c %c、%d、%u


    signed -- char                    8 -2^7 ~ 2^7-1 %c %c、%d、%u


    unsigned -- char                8 0 ~ 2^8-1 %c %c、%d、%u


    [signed] short [int]            16 -2^15 ~ 2^15-1 %hd


    unsigned short [int]           16 0 ~ 2^16-1 %hu、%ho、%hx


    [signed] -- int                    32 -2^31 ~ 2^31-1 %d


    unsigned -- [int]                 32 0 ~ 2^32-1 %u、%o、%x


    [signed] long [int]              32 -2^31 ~ 2^31-1 %ld


    unsigned long [int]             32 0 ~ 2^32-1 %lu、%lo、%lx


    [signed] long long [int]       64 -2^63 ~ 2^63-1 %I64d


    unsigned long long [int]      64 0 ~ 2^64-1 %I64u、%I64o、%I64x


    -- -- float                            32 +/- 3.40282e+038 %f、%e、%g


    -- -- double                        64 +/- 1.79769e+308 %lf、%le、%lg %f、%e、%g


    -- long double                    96 +/- 1.79769e+308 %Lf、%Le、%Lg

几点说明：


1. 注意! 表中的每一行，代表一种基本类型。“[]”代表可省略。


    例如：char、signed char、unsigned char是三种互不相同的类型；


    int、short、long也是三种互不相同的类型。


    可以使用C++的函数重载特性进行验证，如:


    void Func(char ch) {}


    void Func(signed char ch) {}


    void Func(unsigned char ch) {}


    是三个不同的函数。


2. char/signed char/unsigned char型数据长度为1字节；


    char为有符号型，但与signed char是不同的类型。


    注意! 并不是所有编译器都这样处理，char型数据长度不一定为1字节，char也不一定为有符号型。

    某些编译器中，char 默认是有符号的（signed）。对于这类型的编译器来说，char 的表示范围通常是 -128 到 127 。而另外一些编译器中，char 默认是无符号的（unsigned）。对于这类型的编译器来说，char 的表示范围通常是 0 到 255 。一般来说，编译器的使用说明会注明它默认把 char 当作有符号的还是无符号的。

         因此char!=signed char!=unsigned char 完全看编译器的定义，使用此类型时最好带前缀



3. 将char/signed char转换为int时，会对最高符号位1进行扩展，从而造成运算问题。


    所以,如果要处理的数据中存在字节值大于127的情况，使用unsigned char较为妥当。


    程序中若涉及位运算，也应该使用unsigned型变量。


4. char/signed char/unsigned char输出时，使用格式符%c（按字符方式）；


    或使用%d、%u、%x/%X、%o，按整数方式输出；


    输入时，应使用%c，若使用整数方式，Dev-C++会给出警告，不建议这样使用。




5. int的长度，是16位还是32位，与编译器字长有关。


    16位编译器（如TC使用的编译器）下，int为16位；32位编译器（如VC使用的编译器cl.exe）下，int为32


    位。


6. 整型数据可以使用%d（有符号10进制）、%o（无符号8进制）或%x/%X（无符号16进制）方式输入输出。


    而格式符%u，表示unsigned，即无符号10进制方式。


7. 整型前缀h表示short，l表示long。


    输入输出short/unsigned short时，不建议直接使用int的格式符%d/%u等，要加前缀h。


    这个习惯性错误，来源于TC。TC下，int的长度和默认符号属性，都与short一致，


    于是就把这两种类型当成是相同的，都用int方式进行输入输出。


8. 关于long long类型的输入输出：


    "%lld"和"%llu"是linux下gcc/g++用于long long int类型(64 bits)输入输出的格式符。


    而"%I64d"和"%I64u"则是Microsoft VC++库里用于输入输出__int64类型的格式说明。


    Dev-C++使用的编译器是Mingw32，Mingw32是x86-win32 gcc子项目之一，编译器核心还是linux下的gcc。


    进行函数参数类型检查的是在编译阶段，gcc编译器对格式字符串进行检查，显然它不认得"%I64d"，


    所以将给出警告“unknown conversion type character `I' in format”。对于"%lld"和"%llu"，gcc理


    所当然地接受了。


    Mingw32在编译期间使用gcc的规则检查语法，在连接和运行时使用的却是Microsoft库。


    这个库里的printf和scanf函数当然不认识linux gcc下"%lld"和"%llu"，但对"%I64d"和"%I64u"，它则是


    乐意接受，并能正常工作的。


9. 浮点型数据输入时可使用%f、%e/%E或%g/%G，scanf会根据输入数据形式，自动处理。


    输出时可使用%f（普通方式）、%e/%E（指数方式）或%g/%G（自动选择）。


10. 浮点参数压栈的规则：float(4 字节)类型扩展成double(8 字节)入栈。


    所以在输入时，需要区分float(%f)与double(%lf)，而在输出时，用%f即可。


    printf函数将按照double型的规则对压入堆栈的float(已扩展成double)和double型数据进行输出。


    如果在输出时指定%lf格式符，gcc/mingw32编译器将给出一个警告。


11. Dev-C++(gcc/mingw32)可以选择float的长度，是否与double一致。


12. 前缀L表示long（double）。


    虽然long double比double长4个字节，但是表示的数值范围却是一样的。


    long double类型的长度、精度及表示范围与所使用的编译器、操作系统等有关。 



====================================================================

（一）32位平台： 

分为有符号型与无符号型。 

有符号型： 

short 在内存中占两个字节，范围为-2^15~（2^15-1) 

int 在内存中占四个字节，范围为-2^31~（2^31-1） 

long在内存中占四个字节，范围为-2^31~2^31-1 

无符号型：最高位不表示符号位 

unsigned short 在内存中占两个字节，范围为0~2^16-1 

unsigned int 在内存中占四个字节，范围为0~2^32-1 

unsigned long在内存中占四个字节，范围为0~2^32-1 

实型变量: 

分单精度 float 和双精度 double 两种形式： 

float:占四个字节，提供7~8位有效数字。 

double: 占八个字节，提供15~16位有效数字。 


（二）16位平台: 

1)整型(基本型)：类型说明符为int，在内存中占2个字节。 

2)短整型：类型说明符为short int或short。所占字节和取值范围均与整型(基本型)相同。 

3)长整型：类型说明符为long int或long，在内存中占4个字节。 

无符号型：类型说明符为unsigned。 

无符号型又可与上述三种类型匹配而构成： 

各种无符号类型量所占的内存空间字节数与相应的有符号类型量相同。但由于省去了符号位，故不能表示负数。 

实型变量: 

分为单精度（float型）、双精度（double型）和长双精度（long double型）三类。

单精度型占4个字节（32位）内存空间，其数值范围为3.4E-38～3.4E+38，只能提供七位有效数字。 

双精度型占8 个字节（64位）内存空间，其数值范围为1.7E-308～1.7E+308，可提供16位有效数字。 

长双精度型16 个字节（128位）内存空间，可提供18-19位有效数字。

============================================================================

C语言中5种基本数据类型的存储空间长度的排列顺序:

char = signed char = unsigned char < short int = unsigned short int <= int = unsigned int <= long int = unsigned long int <= long long int = unsigned long long int

float <= double <= long double

用short修饰int时,short int表示短整型,占2字节的数据位.用long修饰int时,long int表示长整型,占4字节的数据位,在vc++6.0中int的数据类型也是占用了4字节的数据位, 那么应用int型与long型有什么区别呢?下面就让我们来解答这个问题.我们知道现在的C++开发工具非常的多,int型在不同的系统中有可能占用的字节数会不一样,而short与long修饰的int型的字节数是固定的,任何支持标准C++编译系统中都是如此.所以如果需要编写可移植性好的程序,应该将整型数据声明为short与long修饰的int型。







