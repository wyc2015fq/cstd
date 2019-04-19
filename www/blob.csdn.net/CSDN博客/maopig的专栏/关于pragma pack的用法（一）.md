# 关于pragma pack的用法（一） - maopig的专栏 - CSDN博客
2011年09月13日 09:39:00[maopig](https://me.csdn.net/maopig)阅读数：618
一个很重要的参数
#pragma pack(n)
数据边界对齐方式:
以如下结构为例: struct {
                    char a;
                    WORD b;
                    DWORD c;
                    char d;
                   }
在Windows默认结构大小: sizeof(struct) = 4+4+4+4=16;
与#pragma pack(4)一样
若设为 #pragma pack(1), 则结构大小: sizeof(struct) = 1+2+4+1=8;
若设为 #pragma pack(2), 则结构大小: sizeof(struct) = 2+2+4+2=10;
在#pragma pack(1)时:空间是节省了,但访问速度降低了;
结构体对齐的具体含义（#pragma pack）
朋友帖了如下一段代码：
#pragma pack(4)
class TestB
{
public:
int aa;
char a;
short b;
char c;
};
int nSize = sizeof(TestB);
这里nSize结果为12，在预料之中。
现在去掉第一个成员变量为如下代码：
#pragma pack(4)
class TestC
{
public:
char a;
short b;
char c;
};
int nSize = sizeof(TestC);
按照正常的填充方式nSize的结果应该是8，为什么结果显示nSize为6呢？
事实上，很多人对#pragma pack的理解是错误的。
#pragma pack规定的对齐长度，实际使用的规则是：
结构，联合，或者类的数据成员，第一个放在偏移为0的地方，以后每个数据成员的对齐，按照#pragma pack指定的数值和这个数据成员自身长度中，比较小的那个进行。
也就是说，当#pragma pack的值等于或超过所有数据成员长度的时候，这个值的大小将不产生任何效果。
而结构整体的对齐，则按照结构体中最大的数据成员 和 #pragma pack指定值 之间，较小的那个进行。
具体解释
#pragma pack(4)
class TestB
{
public:
int aa; //第一个成员，放在[0,3]偏移的位置，
char a; //第二个成员，自身长为1，#pragma pack(4),取小值，也就是1，所以这个成员按一字节对齐，放在偏移[4]的位置。
short b; //第三个成员，自身长2，#pragma pack(4)，取2，按2字节对齐，所以放在偏移[6,7]的位置。
char c; //第四个，自身长为1，放在[8]的位置。
};
这个类实际占据的内存空间是9字节
类之间的对齐，是按照类内部最大的成员的长度，和#pragma pack规定的值之中较小的一个对齐的。
所以这个例子中，类之间对齐的长度是min(sizeof(int),4)，也就是4。
9按照4字节圆整的结果是12，所以sizeof(TestB)是12。
如果
#pragma pack(2)
     class TestB
{
public:
int aa; //第一个成员，放在[0,3]偏移的位置，
char a; //第二个成员，自身长为1，#pragma pack(4),取小值，也就是1，所以这个成员按一字节对齐，放在偏移[4]的位置。
short b; //第三个成员，自身长2，#pragma pack(4)，取2，按2字节对齐，所以放在偏移[6,7]的位置。
char c; //第四个，自身长为1，放在[8]的位置。
};
//可以看出，上面的位置完全没有变化，只是类之间改为按2字节对齐，9按2圆整的结果是10。
//所以 sizeof(TestB)是10。
最后看原贴：
现在去掉第一个成员变量为如下代码：
#pragma pack(4)
class TestC
{
public:
char a;//第一个成员，放在[0]偏移的位置，
short b;//第二个成员，自身长2，#pragma pack(4)，取2，按2字节对齐，所以放在偏移[2,3]的位置。
char c;//第三个，自身长为1，放在[4]的位置。
};
//整个类的大小是5字节，按照min(sizeof(short),4)字节对齐，也就是2字节对齐，结果是8
//所以sizeof(TestC)是8。 
