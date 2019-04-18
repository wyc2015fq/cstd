# C++大随机数的问题 - 文章 - 伯乐在线
原文出处： [肥宝游戏，2016-11-30](http://www.cnblogs.com/fablegame/p/6116581.html#top)
C++的随机数最大值是RAND_MAX，在头文件中定义。
在windows平台下的VS是0x7fff，在MacBook的XCode是int的最大值。看来不同的平台是不同的。
但是windows的是在太他妈小了。
先上一个平时的随机函数：

```
int littleRand(int min, int max)
{
    //考虑到不同平台下RAND_MAX可能不等于0x7fff，所以不能把RAND_MAX*RAND_MAX，以免int爆掉
 
    if (min > max)
    {
        min = max;
    }
      
    int randV = rand() % (max - min + 1);
 
    int randResult = min + randV;
    return randResult;
}
```
想优化一下：想着两个相乘，最大值是1亿，而且都是随机，就应该是均等的。

```
int bigRand(int min, int max)
{
    //考虑到不同平台下RAND_MAX可能不等于0x7fff，所以不能把RAND_MAX*RAND_MAX，以免int爆掉
  
    if (min > max)
    {
        min = max;
    }
 
    int rand1 = rand() % 10000;
    int rand2 = rand() % 10000;
    int randV = (rand1 * rand2) % (max - min + 1);
 
    int randResult = min + randV;
    return randResult;
}
```
于是我运行了100万次，生成10以内的随机数，统计一下出现的次数。

```
int main()
{
    srand(time(0));
    std::map<int, int> a;
    std::map<int, int> b;
    for (int i = 0; i < 1000000; i ++)
    {
        a[bigRand(1, 10)]++;
        b[littleRand(1, 10)]++;
    }
    for (int i = 1; i <= 10; ++ i)
    {
        cout << i <<": \t"<< a[i] << " \t " << b[i] << endl;
    }
 
    return 0;
}
```
结果：
![](http://jbcdn2.b0.upaiyun.com/2017/01/f9769c5d473e70266c5eaf681a9d66cf.png)
左边的是大随机数的次数，右边的是原来的。
可以看出，右边的比较正常，基本上是平均的。
而左边的，明显隔一个就大一点。等于1的特别高。至于怎么产生的我也想不通。
不过可以想象一下，缩小范围。就随机1和2。
1和2的概率都是0.5，两个数相乘是4.我们可以得到1-4范围的随机数。
但是概率明显不是均等的。假如执行两次，1和2都出现
就可以等到1：1次，2：2次，3：0次，4：1次。
明显，相乘的肯定是有差别的.
来，再把小时候妈妈叫我的乘法口诀看一遍。

```
std::map<int, int> r;
for(int i = 0; i < 10; ++i)
{
    for (int j = 0; j < 10; ++j) {
        r[i * j %10]++;
    }
}
for (int i = 0 ; i < 10; ++i) {
    cout << i <<"\t"<< r[i] << endl;
}
```
结果：
0     27
1     4
2     12
3     4
4     12
5     9
6     12
7     4
8     12
9     4
那应该怎么搞呢，其实应该用移位的方法。你算出来的数是10进制的话，每次向左移动1为，就是乘以10，例如我上面的代码，

```
int bigRand(int min, int max)
{ 
    if (min > max)
    {
        min = max;
    }
 
    int rand1 = rand() % 10000;
    int rand2 = rand() % 10000;
    int randV = (rand1 * 10000 + rand2) % (max - min + 1);//改成这样
 
    int randResult = min + randV;
    return randResult;
}
```
10000以下和10000以上完全不相干，两次随机不相干。只有这样才能使正确的随机。
其实就是求模的时候，模式多少，第一次随机就要乘以多少。rand1乘以的是rand2的模。
百度了一下其他人的随机数。

```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
unsigned long ulrand(void) {
    return (
     (((unsigned long)rand()<<24)&0xFF000000ul)
    |(((unsigned long)rand()<<12)&0x00FFF000ul)
    |(((unsigned long)rand()    )&0x00000FFFul));
}
int i;
unsigned long ul;
void main() {
    srand(time(NULL));
    for (i=0;i<10;i++) {
        ul=ulrand();
        printf("%010lu 0x%08x\n",ul,ul);
    }
}
```
还有这个简单点的：

```
int BigRand()
{
    return RAND_MAX*rand() + rand();
}
```
其实都是通过移位的方式，移出那个随机数的最大值就行。
不过有个问题要考虑的，就是跨平台问题。不同平台的整形的长度可能不同，RAND_MAX也可能不同。总是移位，可能会出现数组越界。
所以为了安全起见还是用我写的两个10000最大值的随机数吧。
