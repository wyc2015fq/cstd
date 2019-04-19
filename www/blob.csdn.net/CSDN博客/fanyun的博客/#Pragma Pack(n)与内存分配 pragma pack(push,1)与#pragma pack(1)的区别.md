# #Pragma Pack(n)与内存分配 pragma pack(push,1)与#pragma pack(1)的区别 - fanyun的博客 - CSDN博客
2018年05月27日 21:56:36[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：252
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
        在网络协议编程中，经常会处理不同协议的数据报文。一种方法是通过指针偏移的方法来得到各种信息，但这样做不仅编程复杂，而且一旦协议有变化，程序修改起来也比较麻烦。在了解了编译器对结构空间的分配原则之后，我们完全可以利用这一特性定义自己的协议结构，通过访问结构的成员来获取各种信息。这样做，不仅简化了编程，而且即使协议发生变化，我们也只需修改协议结构的定义即可，其它程序无需修改，省时省力。
```cpp
#pragma  pack （1） //设定为1字节对齐           作用：调整结构体的边界对齐，让其以一个字节对齐；<使结构体按1字节方式对齐>
#pragma pack(push) //保存当前对齐状态
#pragma pack(4)    //设定为4字节对齐
#pragma  pack （） //按结构数据内最大的字节对齐
#pragma  pack(push,1)//把原来对齐方式压栈，并设置新的对齐方式为1个字节对齐
#pragma  pack(pop)  //恢复默认对齐方式
```
        由上可见，加入push和pop可以使对齐恢复到原来状态，而不是编译器默认，可以说后者更优，但是很多时候两者差别不大。
例子：
```cpp
struct data
{
char a;
double b;
};
```
        #pragma pack()
        若不用#pragma pack(1)和#pragma pack()括起来，则data按编译器默认方式对齐（成员中size最大的那个）。即按8字节（double）对齐，则sizeof(sample)==16.成员char a占了8个字节（其中7个是空字节）；若用#pragma pack(1)，则data按1字节方式对齐sizeof(sample)＝＝9.（无空字节），比较节省空间啦，有些场和还可使结构体更易于控制。
        举例说明：
例子1：
```cpp
#pragma pack(1)
struct node{
  char f;
  int e;
  short int a;
  char b;};
struct node n;
printf("%d\n",sizeof(n));
```
按一字节对齐，不考虑补齐对齐，结构体所占大小为8。
例子2：
```cpp
#pragma pack(2)
struct node{
  char f;
  int e;
  short int a;
  char b;};
struct node n;
printf("%d\n",sizeof(n));
```
按2字节对齐，考虑补齐对齐，结构体所占内存大小为10.
例子3：
```cpp
#pragma pack(4)
struct node{
  char f;
  int e;
  short int a;
  char b;};
struct node n;
printf("%d\n",sizeof(n));
```
按4字节对齐，考虑补齐对齐，结构体所占内存大小为12。
