# C语言中 ++ 和 * 的优先级 - 心纯净，行致远 - CSDN博客





2018年06月23日 18:24:30[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：4340








本来我想当然的以为在C语言中指针预算符的优先级肯定比++高，后来在写程序时发现并没有这么当然。
在C语言中，自增运算符++和指针运算符*的优先级关系为
左++ > * > 右++

比如 ++ *i 相当于  ++ (*i) ，指针i对应的变量+1。

*i ++ 相当于 * (i++)  ，指针i，也就是地址值 +1（其实是加1个变量所占的地址空间）


![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <stdio.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)void main() {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int n = 2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int*i = &n;

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)*i ++;//应该为 (*i) ++;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    printf("%d ",*i);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}


比如以上程序，结果输出的并不是我们想象的2+1=3，而是垃圾数据，*i++后，指针i所指向的地址已经发生变化，而以前那个地址的数据则并没有变化。

下面的例题可以帮助理解：



# 全局变量、局部变量、静态变量的生存期

以下代码的输出是：

```cpp

```
int x, y, z, w;
void p(int *y, int x)
{
    static int w;
    *y++; x++; w = x+*--y;
    printf("%d#%d#%d#%d#",x,*y,z,w);
}
int main(void)
{
    int x, y, z, w;
    x=y=z=w=1;
    do{
        static int x;
        p(&x, y);
        printf("%d#%d#%d#%d#",x,y,z,w);
    } while(0);
    return 0;
}

答案：2#0#0#2#0#1#1#1#

解析：主要考察局部变量和全局变量的生存期，以及静态本地变量。注释后的代码如下：

```cpp
int x, y, z, w;    //这里是全局变量，定义在任何函数的外面，若不初始化赋值，则均为0；。注意，主函数里面的变量仍为局部变量
void p(int *y, int x)    //p函数接收整型指针变量和整型变量的输入，返回值为空
{
    static int w;    //定义静态变量w，若不初始化赋值，则w==0；
    *y++; x++; w = x+*--y;
    printf("%d#%d#%d#%d#",x,*y,z,w);
}
int main(void)
{
    int x, y, z, w;
    x=y=z=w=1;
    do{
        static int x;
        p(&x, y);
        printf("%d#%d#%d#%d#",x,y,z,w);
    } while(0);
    return 0;
}
```
从主函数进行分析，主函数内部定义了四个int型变量，若不进行初始化，则全为0。主函数内部定义完之后就进行了初始化，均初始化为1，所以在do-while内部，可以看到由于只定义了静态局部变量x，而且没有初始化赋值，则静态局部变量x为0，y,z,w均仍为1。所以
```cpp
printf("%d#%d#%d#%d#",x,y,z,w);
```

的输出是0#1#1#1#。

再来分析p这个函数：

```cpp
void p(int *y, int x)    //p函数接收整型指针变量和整型变量的输入，返回值为空
{
    static int w;    //定义静态变量w，若不初始化赋值，则w==0；
    *y++; x++; w = x+*--y;
    printf("%d#%d#%d#%d#",x,*y,z,w);
}
```

首先p函数无返回值，接受两个输入：int型指针变量、int型变量。p函数内部同样定义了一个静态局部变量w，但是w后面有赋值的语句

```cpp
w = x+*--y;
```



*y++和x++是两个关键，首先*和++，--运算符处于同一优先级，结合方向是自右向左。因此*y++可以看做是*（y++），但是由于y++是先执行y，跳出去与*结合，再让y++。所以*y++实际上等效于先执行*y操作，再执行y++。由于y是指针，因此y++是指针所指内存地址的向后移动，移动的大小是一个sizeof（int）。x++同理先执行x（由于没有任何操作，x不变），再让x+1，这里实际上由于x没有任何操作，x++相当于只执行了x+1，由于局部变量传入p函数的x为1，这里x就等于2了。

由于y是指针变量，因此*y表示取出指针所指内存地址的值。由于传进去的

```cpp
static int x;
        p(&x, y);
```

x是静态变量，则x=0，因此*y=0。这里要注意p的原型是void p(int *y, int x)  ，而使用p函数时，传进去的是

```cpp
p(&x, y);
```

顺序不要搞反了。w=x+*--y，这里等价于w=x+*（--y），由于在上面的*y++，y已经执行了y+1，这里（--y）先执行--，再执行y，即先执行y-1，再把y-1的结果传出去。注意这里的-1指的是减去一个int型变量的内存大小。因此y还是原来的内存位置。所以*y还是取出原来指针指向内存地址的值，即还是原来的静态局部变量x，值为0。因此w=x+*--y中x=2,（*--y）等于0，所以w=2。由于p函数里的z只能那个接受全局变量，因此z=0，所以p函数执行之后打印：2#0#0#2。



