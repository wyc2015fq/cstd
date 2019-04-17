# 烧写Flash后的DSP程序运行不正常的情况分析 - xiahouzuoxin - CSDN博客





2014年04月12日 13:58:57[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：9465









这段时间一直在调试DSP6713的Flash烧写，现在对Flash的烧写也算心中了如。


那天，非常Happy的发现将闪烁LED烧写到Flash成功了，然后，就以为一切都OK了……


那天，成功烧写了一个300KB以上的程序，又认为，这次应该OK了……


那天，写了个Timer中断程序，烧写到Flash，却死机了……


那天，在RAM上运行很流畅的一个算法（算法中调用了CCS的atan函数），在烧写到Flash后算法却死机了……


那天，我开始思考：是什么情况导致RAM中跑得很Happy的程序烧写到Flash就运行得如此的不堪——众多的囧相。


“且行且珍惜”，珍惜这些次发现Bug的机会，因此，我要总结：在RAM中能正常运行，而烧写到Flash后无法正常运行的一些情况讨论。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%83%A7%E5%86%99Flash%E5%90%8E%E7%9A%84DSP%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E4%B8%8D%E6%AD%A3%E5%B8%B8%E7%9A%84%E6%83%85%E5%86%B5%E5%88%86%E6%9E%90.md#%E8%AF%B7%E6%A3%80%E6%9F%A5%E4%B8%AD%E6%96%AD%E5%90%91%E9%87%8F%E8%A1%A8)请检查中断向量表


中断向量表包含了所有中断的入口，在烧写Flash的时候，有两种方式可以保证中断能正常工作。具体可参见[TMS320C6713烧写Flash的通用方法](https://github.com/xiahouzuoxin/notes/blob/master/essays/TMS320C6713%E7%83%A7%E5%86%99Flash%E7%9A%84%E9%80%9A%E7%94%A8%E6%96%B9%E6%B3%95.md)的第5小节。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%83%A7%E5%86%99Flash%E5%90%8E%E7%9A%84DSP%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E4%B8%8D%E6%AD%A3%E5%B8%B8%E7%9A%84%E6%83%85%E5%86%B5%E5%88%86%E6%9E%90.md#%E8%AF%B7%E6%A3%80%E6%9F%A5%E7%A8%8B%E5%BA%8F%E4%B8%AD%E9%9A%90%E8%94%BD%E7%9A%84%E5%86%85%E5%AD%98%E9%94%99%E8%AF%AF)请检查程序中隐蔽的内存错误


很多情况下，当出现数组越界时，在RAM中的程序都能正常运行，但在烧写Flash后运行就会出现死机或程序跑飞的现象。


比如定义一个数组，

```
int x[5];
```


你使用x[5]=10这样的语句在RAM程序中是某些时候能正确运行的，在PC上应该也可以。但将这种程序烧写到Flash之后运行，DSP果断和你说拜拜！


因此，请谨慎地检查程序代码中的数组越界和指针操作。在DSP程序中，坚决不使用C库函数中的malloc函数。如果需要动态分配内存的操作，可以自己写一个，或使用uCOS II或DSP/BIOS等嵌入式操作系统。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%83%A7%E5%86%99Flash%E5%90%8E%E7%9A%84DSP%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E4%B8%8D%E6%AD%A3%E5%B8%B8%E7%9A%84%E6%83%85%E5%86%B5%E5%88%86%E6%9E%90.md#%E8%AF%B7%E5%B0%BD%E9%87%8F%E9%81%BF%E5%85%8D%E4%BD%BF%E7%94%A8mathh%E4%B8%AD%E7%9A%84%E4%B8%89%E8%A7%92%E5%8F%8Alog%E7%AD%89%E5%87%BD%E6%95%B0)请尽量避免使用math.h中的三角及log等函数


也不知道是什么原因，也可能是我对atan函数的使用方法不正确造成的吧。在我的一个最初的程序中，我是直接这样计算atan(x)的，

```
float x,y;
...
y = atan(x); // x范围为[0, 1.7]
```


在RAM中以及在PC中都多次测试过没有任何问题。


烧写Flash之后，也不是死机，但程序运行到atan这个函数的时候会卡上很长一段时间，再接着往下运行。


难道是math.h中的atan运算效率太低？但为什么RAM中就能运行呢？这个还不清楚。


于是想了个招，在要使用三角函数和log等函数的地方都使用查表法替代库函数，在精度要求高而存储空间又有限的场合，可使用查表+插值的方式。


下面是改进方法计算atan，

```
/* table of determine ATAN(x) */
const float atan_tb[] = {  // 精度(0.020)
0.00, 1.15, 2.29, 3.43, 4.57, 5.71 , 6.84, 7.97, 9.09, 10.20,
11.31, 12.41, 13.50, 14.57, 15.64, 16.70 , 17.74, 18.78, 19.80, 20.81,
21.80, 22.78, 23.75, 24.70, 25.64, 26.57 , 27.47, 28.37, 29.25, 30.11,
30.96, 31.80, 32.62, 33.42, 34.22, 34.99 , 35.75, 36.50, 37.23, 37.95,
38.66, 39.35, 40.03, 40.70, 41.35, 41.99 , 42.61, 43.23, 43.83, 44.42,
45.00, 45.57, 46.12, 46.67, 47.20, 47.73 , 48.24, 48.74, 49.24, 49.72,
50.19, 50.66, 51.12, 51.56, 52.00, 52.43 , 52.85, 53.27, 53.67, 54.07,
54.46, 54.85, 55.22, 55.59, 55.95, 56.31 , 56.66, 57.00, 57.34, 57.67,
57.99, 58.31, 58.63, 58.93, 59.24, 59.53 , 59.83 
};

y = atan_tb[((uint16_t)(x*100)) >> 1];
```


建立atan的表可以借助Matlab。在需要插值的场合，比如，上面atan_tb的精度为0.02，而我们希望在少数的一些场合下使atan在0.01的精度，如果以0.01建表将会使表的数据存储量扩大1倍，这是我们可以在0.02精度表的基础上再使用插值的方式。


比如，要计算atan(0.03)，我们可以从表中查到atan(0.02)和atan(0.04)，如果仅使用线性插值的话，则

```
atan(0.03) = (atan(0.02) + atan(0.04)) / 2
```

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%83%A7%E5%86%99Flash%E5%90%8E%E7%9A%84DSP%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E4%B8%8D%E6%AD%A3%E5%B8%B8%E7%9A%84%E6%83%85%E5%86%B5%E5%88%86%E6%9E%90.md#%E8%AF%B7%E6%A3%80%E6%9F%A5%E7%A8%8B%E5%BA%8F%E7%9A%84%E9%80%BB%E8%BE%91)请检查程序的逻辑


曾傻傻的写过一个类似下面的程序，

```
uint8_t dir;  // 低3位进行了编码，下面的switch进行解码

int dist_switch(int a, int b, int c)
{
    int max_dist;
    int min_dist;
    int result = 0;

    switch (dir) {
    case 0x00: break;
    case 0x01: max=a;min=b;break;
    case 0x02: max=a;min=c;break;
    case 0x03: max=b;min=a;break;
    case 0x04: max=b;min=c;break;
    case 0x05: max=c;min=a;break;
    case 0x06: max=c;min=b;break;
    case 0x07: break;
    default: break;
    }

    result = max * 100 / (min + max);

    return result;
}
```


咋一看，没有语法问题，switch的break语句也加上了。


问题出就出在：dir低三位进行了编码，最大编码个数应该是8。而因为实际中只用到6种情况，switch中对其它的两种编码都使用break，问题就出来了，如果我的dir=0x00会怎么样？switch语句当然没问题，问题在下一条语句：

```
result = max * 100 / (min + max);
```


dir=0x00没有对max和min进行任何的赋值，而且其它地方也没有。因此max和min作为局部变量将会是一个随机的值，这在RAM中是能够运行通过的，但烧写到Flash之后，这种局部变量的不确定性直接回导致Flash宕机。


因此，对于switch以及if...else...的逻辑问题，不能只关注它们所在范围，请仔细检查其上下文。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%83%A7%E5%86%99Flash%E5%90%8E%E7%9A%84DSP%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E4%B8%8D%E6%AD%A3%E5%B8%B8%E7%9A%84%E6%83%85%E5%86%B5%E5%88%86%E6%9E%90.md#%E8%AF%B7%E7%89%B9%E5%88%AB%E5%85%B3%E7%85%A7%E4%B8%80%E4%B8%8B%E7%A8%8B%E5%BA%8F%E4%B8%AD%E7%9A%84%E9%99%A4%E6%B3%95%E8%BF%90%E7%AE%97)请特别关照一下程序中的除法运算


x=a/b中若b可能为0，这样的程序烧写到Flash会直接导致DSP死机的。如果可以的话，尽量将除法运算转换为移位运算。


比如，要计算y=x/0.02，一个号的转换方式就是：

```
y=(uint32_t)(x*100)/2=((uint32_t)(x*100) >> 1);
```


还可以更好一点，将*100也使用移位替代，

```
uint32_t tmp_x = (uint32_t)x;
y = ((tmp_x<<6) + (tmp_x<<5) + (tmp_x<<2)) >> 1;
```


这样你就再也看不到除法运算了。



