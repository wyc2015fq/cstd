# C语言指针整理 - 一个跳popping的quant的博客 - CSDN博客





2018年10月31日 15:34:30[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：629标签：[C语言																[指针](https://so.csdn.net/so/search/s.do?q=指针&t=blog)](https://so.csdn.net/so/search/s.do?q=C语言&t=blog)
个人分类：[data structures and algorithms](https://blog.csdn.net/FrankieHello/article/category/8237051)








### 1、内存地址

内存是按照字节编址的，所以它的寻址单位是**字节**（Byte），而一个字节又有是由8位（bit）的0或1组成。

如果一台计算机的内存是4GB，也就是说它的内存总字节数有![2^{32}](https://private.codecogs.com/gif.latex?2%5E%7B32%7D)个，所以要想表示这台计算机的内存地址，就要用到32位（bit）。类似于这样的32位来表示一个内存地址：0110 1101 1100 0010 1101 1110 0101 1101，由于写起来不方便，所以我们将上述地址用其**十六进制**表示：0x6DC2DE5D。

所以这样的话4GB内存的寻址空间就从0x00000000到0xFFFFFFFF，总共有![2^{32}](https://private.codecogs.com/gif.latex?2%5E%7B32%7D)个。

![](https://img-blog.csdnimg.cn/20181031132048787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

### 2、指针变量

明白内存地址后，指针实际上就是内存地址，**指针变量则是存储指针的变量，也即是存储了内存地址**。下面对指针变量从下面几个方面进行分析。

**指针变量的类型**

定义一个int* p为例，这个指针变量的类型就是int *。

**指针变量所指向的数据类型**

p变量所指向的类型是int数据类型，编译器通过指针所指向的这片内存区域作来决定这片内存区域存放什么数据类型，因为int类型是4个字节的大小，所以从p指针变量指向的内存地址到后四字节这片内存区域就存放了一个int的数据类型。

借这个例子就可以很好理解c语言中malloc函数了。malloc函数作用是**分配指定大小的连续内存空间**，并返回这个内存空间的首地址，而这个地址指向的数据类型不明确，所以通常还需要对它进行强制转换，这样就得到了一个指向int类型数据的内存首地址。

```cpp
void test(){
    int* p = (int*)malloc(sizeof(int));
    printf("%d", p);
}
```

** 指针变量的大小**

通过上面的分析，指针变量的值就是它指向的内存地址，是一个数值类型，无论它指向什么数据类型，这个数值类型都是占据了4个字节，也就是说指针变量本身就占据了4个字节。

```cpp
int main()
{
    int* p;
    printf("%d",sizeof(p));    // 输出4，p变量的大小是4字节
    return 0;
}
```

### 3、地址运算符

*运算符称作指针运算符或者间接引用运算符，它的作用就是取一个指针变量所指向的内存区域所存储的数据值。

&运算符称作取地址符，它的作用就是取一个变量所在的内存地址。

```cpp
void test(){
    int a = 10;
    printf("%x", &a);  //60fefc
    int b = *(&a);
    printf("%d", b);   //10
}
```

地址运算符应用时，通常结合指针变量。

```cpp
void test(){
    int a = 10;
    int* p = &a;
    printf("%d", *p);   //10
}
```

Ref:

[C语言（彻底理解指针和地址，彻底！）](https://blog.csdn.net/vincent040/article/details/51072675)

[C语言指针详解(经典,非常详细)](https://blog.csdn.net/constantin_/article/details/79575638)




