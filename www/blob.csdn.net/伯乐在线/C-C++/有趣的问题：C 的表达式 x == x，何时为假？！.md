# 有趣的问题：C 的表达式 x == x，何时为假？！ - 文章 - 伯乐在线
原文出处： [GFree_Wind](http://blog.chinaunix.net/uid-23629988-id-3126229.html)
今天的问题很有趣，C的表达式 x == x，何时为假呢？即下面的代码：


```
if (x == x) {
printf("Equaln");
}
else {
printf("Not equaln");
}
```
什么时候输出为”Not equal”呢？看上去很有趣吧。x与自己比较，怎么会不等呢？请大家想一想，容我慢慢道来。
这是我今天发现的一个问题，很有意思。最终的解决方案就是判断x == x何时为false。不卖关子了，请看下面的代码：


```
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
    float x = 0xffffffff;
    if (x == x) {
        printf("Equal\n");
    }
    else {
        printf("Not equal\n");
    }
    if (x >= 0) {
        printf("x(%f) >= 0\n", x);
    }
    else if (x < 0) {
        printf("x(%f) < 0\n", x);
    }
    int a = 0xffffffff;
    memcpy(&x, &a, sizeof(x));
    if (x == x) {
        printf("Equal\n");
    }
    else {
        printf("Not equal\n");
    }
    if (x >= 0) {
        printf("x(%f) >= 0\n", x);
    }
    else if (x < 0) {
        printf("x(%f) < 0\n", x);
    }
    else {
        printf("Surprise x(%f)!!!\n", x);
    }
    return 0;
}
```
编译gcc -g -Wall test.c，看执行结果：


```
[fgao@fgao-vm-fc13 test]$ ./a.out
Equal
x(4294967296.000000) >= 0
Not equal
Surprise x(-nan)!!!
```
最后两行的输出是不是有点surprise啊。
下面先简单解释一下：
1. 当float x = 0xffffffff：这时将整数赋给一个浮点数，由于float和int的size都是4，而浮点数的存储格式与整数不同，其需要将某些位作为小数位，所以float的范围要小于int的范围。因此这里牵涉到了整数转换浮点的规定——这个大家可以查一下资料。因为这个转换其实很少用到，我也就不查了。但是总之，这个转换是合法的。但是最终的值很可能不是你想要的结果——尤其是当浮点的范围小于整数的范围时。
2. 即使整数转换成浮点，数值再不是期望值，但它也一定是一个合法的浮点数值。所以第一个x == x，一定为true，且x不是大于0，就是小于0。这时x存的并不是0xffffffff。
3. 当使用memcpy将0xff填充到x的地址时，这时x存的保证为0xffffffff。结果很遗憾，这个不是一个合法的float的值。因此奇怪的现象发生了，x并不等于x。原因则是由cpu的浮点指令相关，有兴趣的同学可以搜索一下。
4. 作为一个非法的float值，当它与其它任何数值比较时，都会返回false。这也就造成了，后面惊奇的结果，x既不大于等于0，也不小于0。
总结一下：一般来说，浮点类型很少被使用，也不应该在程序中鼓励使用。不仅其效率比整数低，且由于浮点类型特殊的存储格式，很容易造成一些意想不到的错误。如果真的无法避免时，一定要小心小心再小心。特别要注意今天的主题，这种非法的浮点值，会导致任何比较判断都失败。而判断这种浮点值的方法也很简单，如果x != x，那么该浮点即为非法浮点值。
在大家的评论中，有的朋友说是不同类型的比较。固然不同类型的比较会容易引起问题，但是这里并不是不同类型比较造成的。将文中的0换成浮点类型0.0，结果不变。重点是在于非法的浮点值。
另外，还有朋友认为这是由于示例中使用memcpy来填充浮点导致的问题。其实这里我只是选用这种方法而已。还有其它方法可以导致产生这种浮点的exception，即NaN。也许其它方法更自然。这里我选用memcpy只是为了对比，将0xffffffff直接赋给浮点这种情况。
看了为了更清楚的说明NaN，需要另外一篇文章
