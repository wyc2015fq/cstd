# 在C语言中，double、long、unsigned、int、char类型数据所占字节数 - 建建的博客 - CSDN博客
2017年03月22日 16:04:22[纪建](https://me.csdn.net/u013898698)阅读数：162
个人分类：[C++基础知识](https://blog.csdn.net/u013898698/article/category/6734624)

和机器字长及编译器有关系：
所以，int，long int，short int的宽度都可能随编译器而异。但有几条铁定的原则（ANSI/ISO制订的）： 
1 sizeof(short int)<=sizeof(int) 
2 sizeof(int)<=sizeof(long int) 
3 short int至少应为16位（2字节） 
4 long int至少应为32位。 
unsigned 是无符号的意思。
例如：
**16位编译器**
char ：1个字节
char*(即指针变量): 2个字节
short int : 2个字节
int：  2个字节
unsigned int : 2个字节
float:  4个字节
double:   8个字节
long:   4个字节
long long:  8个字节
unsigned long:  4个字节
**32位编译器**
char ：1个字节
char*（即指针变量）: 4个字节（32位的寻址空间是2^32, 即32个bit，也就是4个字节。同理64位编译器）
**short int : 2个字节**
int：  4个字节
unsigned int : 4个字节
float:  4个字节
double:   8个字节
**long:   4个字节**long long:  8个字节
unsigned long:  4个字节
**64位编译器**
char ：1个字节
char*(即指针变量): 8个字节
short int : 2个字节
int：  4个字节
unsigned int : 4个字节
float:  4个字节
double:   8个字节
long:   8个字节
long long:  8个字节
unsigned long:  8个字节 
