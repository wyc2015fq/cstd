# dev c++ 数据类型的范围 - ljx0305的专栏 - CSDN博客
2009年08月07日 14:27:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3115
环境: Dev-C++ 4.9.6.0 (gcc/mingw32), 使用-Wall编译选项
基本类型包括字节型（char）、整型（int）和浮点型（float/double）。
定义基本类型变量时，可以使用符号属性signed、unsigned（对于char、int），和长度属性short、long（对
于int、double）对变量的取值区间和精度进行说明。
下面列举了Dev-C++下基本类型所占位数和取值范围：
符号属性     长度属性     基本型     所占位数     取值范围       输入符举例      输出符举例
--            --          char         8         -2^7 ~ 2^7-1        %c          %c、%d、%u
signed        --          char         8         -2^7 ~ 2^7-1        %c          %c、%d、%u
unsigned      --          char         8         0 ~ 2^8-1           %c          %c、%d、%u
[signed]      short       [int]        16        -2^15 ~ 2^15-1              %hd
unsigned      short       [int]        16        0 ~ 2^16-1             %hu、%ho、%hx
[signed]      --           int         32        -2^31 ~ 2^31-1              %d
unsigned      --          [int]        32        0 ~ 2^32-1              %u、%o、%x
[signed]      long        [int]        32        -2^31 ~ 2^31-1              %ld
unsigned      long        [int]        32        0 ~ 2^32-1             %lu、%lo、%lx
[signed]      long long   [int]        64        -2^63 ~ 2^63-1             %I64d
unsigned      long long   [int]        64        0 ~ 2^64-1          %I64u、%I64o、%I64x
--            --          float        32       +/- 3.40282e+038         %f、%e、%g
--            --          double       64       +/- 1.79769e+308 %lf、%le、%lg   %f、%e、%g
--            long        double       96       +/- 1.79769e+308        %Lf、%Le、%Lg
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
这些是我根据网上一些资料和自己的看法小结的。
其中肯定有不少错误,请大家指正。
引用:http://hi.baidu.com/faruba/blog/item/79eaa7a26162fdaecbefd09c.html
