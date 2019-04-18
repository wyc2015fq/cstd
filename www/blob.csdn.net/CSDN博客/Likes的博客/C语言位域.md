# C语言位域 - Likes的博客 - CSDN博客
2018年10月25日 19:02:31[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：24标签：[C语言																[C  																[位域																[教程																[快速](https://so.csdn.net/so/search/s.do?q=快速&t=blog)](https://so.csdn.net/so/search/s.do?q=教程&t=blog)](https://so.csdn.net/so/search/s.do?q=位域&t=blog)](https://so.csdn.net/so/search/s.do?q=C  &t=blog)](https://so.csdn.net/so/search/s.do?q=C语言&t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
[http://blog.sina.com.cn/s/blog_3d8529400100istl.html](http://blog.sina.com.cn/s/blog_3d8529400100istl.html)
有些信息在存储时，并不需要占用一个完整的字节， 而只需占几个或一个二进制位。例如在存放一个开关量时，只有0和1 两种状态，用一位二进位即可。为了节省存储空间，并使处理简便，Ｃ语言又提供了一种数据结构，称为“位域”或“位段”。所谓“位域”是把一个字节中的二进位划分为几个不同的区域，并说明每个区域的位数。每个域有一个域名，允许在程序中按域名进行操作。 这样就可以把几个不同的对象用一个字节的二进制位域来表示。
# 一、位域的定义和位域变量的说明
位域定义与结构定义相仿，其形式为：
struct 位域结构名
{ 位域列表 };
其中位域列表的形式为： 类型说明符 位域名：位域长度
例如：
```cpp
struct bs
{
    int a:8;
    int b:2;
    int c:6;
};
```
位域变量的说明与结构变量说明的方式相同。 可采用先定义后说明，同时定义说明或者直接说明这三种方式。例如：
```cpp
struct bs
{
    int a:8;
    int b:2;
    int c:6;
}data;
```
说明data为bs变量，共占两个字节。其中位域a占8位，位域b占2位，位域c占6位。对于位域的定义尚有以下几点说明：
1. 一个位域必须存储在同一个字节中，不能跨两个字节。如一个字节所剩空间不够存放另一位域时，应从下一单元起存放该位域。也可以有意使某位域从下一单元开始。
例如：
```cpp
struct bs
{
    unsigned a:4;
    unsigned :0;
    unsigned b:4;
    unsigned c:4;
}
```
在这个位域定义中，a占第一字节的4位，后4位填0表示不使用，b从第二字节开始，占用
4位，c占用4位。
2. 由于位域不允许跨两个字节，因此位域的长度不能大于一个字节的长度，也就是说不能超过8位二进位。
3. 位域可以无位域名，这时它只用来作填充或调整位置。无名的位域是不能使用的。例如：
```cpp
struct k
{
    int a:1;
    int :2;
    int b:3;
    int c:2;
};
```
从以上分析可以看出，位域在本质上就是一种结构类型， 不过其成员是按二进位分配的。
# 二、位域的使用
位域的使用和结构成员的使用相同，其一般形式为：
位域变量名·位域名
位域允许用各种格式输出。
```cpp
main(){
    struct bs
        {
             unsigned a:1;
             unsigned b:3;
             unsigned c:4;
        } bit,*pbit;
    bit.a=1;
    bit.b=7;
    bit.c=15;
    printf("%d,%d,%d\n",bit.a,bit.b,bit.c);
    pbit=&bit;
    pbit->a=0;
    pbit->b&=3;
    pbit->c|=1;
    printf("%d,%d,%d\n",pbit->a,pbit->b,pbit->c);
}
```
上例程序中定义了位域结构bs，三个位域为a,b,c。说明了bs类型的变量bit和指向bs类型的指针变量pbit。这表示位域也是可以使用指针的。程序的9、10、11三行分别给三个位域赋值。( 应注意赋值不能超过该位域的允许范围)程序第12行以整型量格式输出三个域的内容。第13行把位域变量bit的地址送给指针变量pbit。第14行用指针方式给位域a重新赋值，赋为0。第15行使用了复合的位运算符"&="，该行相当于：pbit->b=pbit->b&3位域b中原有值为7，与3作按位与运算的结果为3(111&011=011,十进制值为3)。同样，程序第16行中使用了复合位运算"|=".之所以要有透析基础知识这么个分栏,就是告诉大家重 在细节的道理,粗略的东西谁都懂,修炼内功为高手的必经之路.
前面的内容存在缺陷，具体还要参考如下文章：
C99规定int、unsigned int和bool可以作为位域类型，但编译器几乎都对此作了扩展，允许其它类型类型的存在。
使用位域的主要目的是压缩存储，其大致规则为：
1) 如果相邻位域字段的类型相同，且其位宽之和小于类型的sizeof大小，则后面的字
段将紧邻前一个字段存储，直到不能容纳为止；
2) 如果相邻位域字段的类型相同，但其位宽之和大于类型的sizeof大小，则后面的字
段将从新的存储单元开始，其偏移量为其类型大小的整数倍；
3) 如果相邻的位域字段的类型不同，则各编译器的具体实现有差异，VC6采取不压缩方
式，Dev-C++采取压缩方式；
4) 如果位域字段之间穿插着非位域字段，则不进行压缩；
5) 整个结构体的总大小为最宽基本类型成员大小的整数倍。
```cpp
typedef struct  AA
{
       unsigned char b1:5;
       unsigned char b2:5;
       unsigned char b3:5;
       unsigned char b4:5;
       unsigned char b5:5;
}AA;
```
sizeof(AA) = 5;　但实际上只用了25位，即4个字节，
（1）
```cpp
typedef struct  AA
{
       unsigned int b1:5;
       unsigned int b2:5;
       unsigned int b3:5;
       unsigned int b4:5;
       unsigned int b5:5;
}AA;
```
（2）
```cpp
typedef struct  AA
{
       unsigned int b1:5;
       unsigned int b2:5;
       unsigned int b3:5;
       unsigned int b4:5;
       unsigned int b5:5;
       unsigned int b6:5;
       unsigned int b7:5;
}AA;
```
（1）是5个成员,按第一条规则,共占25位,按第五条规则,即sizeof(AA)=4
现把成员加到7个，参考（2）,按第一条规则,共占35位,按第五条规则,即sizeof(AA)=8,
再看一个例子：
```cpp
struct test1
{
char a:1;
char :2;
long b:3;
char c:2;
};
int len = sizeof(test1);
```
对于上述例子，len的值应该是12.解释如下：　
首先以最长的类型位宽做为偏移量，最长的是long型，占4位，所以不同类型之间应该是4个字节的偏移，即test1应该是4字节的整数倍。　
char a：1；　//用一个字节去存储
char ：2；　 //空域。因为与前面的a的类型相同，而两个位域的位宽相加仍然少于8位，所以依然用1个字节表示
long b：3； //long类型的位宽是4个字节，与前面的char类型不同，所以b与a之间偏移4个字节，它们之间自动补充3个字节　
char c：2； //因为c与b又不同型，以test1中的最长的long类型的位宽进行偏移，所以虽然char只用1个字节就够了
//但依然要占4个字节。
总共是12字节。
///////////////////////
struct s1
　　{
　　int i: 8;
　　int j: 4;
　　int a: 3;
　　double b;
　　};
　　struct s2
　　{
　　int i: 8;
　　int j: 4;
　　double b;
　　int a:3;
　　};
　　printf("sizeof(s1)= %d\n", sizeof(s1));
　　printf("sizeof(s2)= %d\n", sizeof(s2));
　　result: 16, 24
　　第一个struct s1
　　{
　　int i: 8;
　　int j: 4;
　　int a: 3;
　　double b;
　　};
　　理论上是这样的，首先是i在相对0的位置，占8位一个字节，然后，j就在相对一个字节的位置，由于一个位置的字节数是4位的倍数，因此不用对齐，就放 在那里了，然后是a，要在3位的倍数关系的位置上，因此要移一位，在15位的位置上放下，目前总共是18位，折算过来是2字节2位的样子，由于 double是8 字节的，因此要在相对0要是8个字节的位置上放下，因此从18位开始到8个字节之间的位置被忽略，直接放在8字节的位置了，因此，总共是16字节。
