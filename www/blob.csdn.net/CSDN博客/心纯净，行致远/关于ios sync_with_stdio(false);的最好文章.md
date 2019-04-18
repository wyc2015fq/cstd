# 关于ios::sync_with_stdio(false);的最好文章 - 心纯净，行致远 - CSDN博客





2018年11月13日 10:21:15[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：69








在竞赛中，遇到大数据时，往往读文件成了程序运行速度的瓶颈，需要更快的读取方式。相信几乎所有的C++学习者都在cin机器缓慢的速度上栽过跟头，于是从此以后发誓不用cin读数据。还有人说Pascal的read语句的速度是C/C++中scanf比不上的，C++选手只能干着急。难道C++真的低Pascal一等吗？答案是不言而喻的。一个进阶的方法是把数据一下子读进来，然后再转化字符串，这种方法传说中很不错，但具体如何从没试过，因此今天就索性把能想到的所有的读数据的方式都测试了一边，结果是惊人的。

竞赛中读数据的情况最多的莫过于读一大堆整数了，于是我写了一个程序，生成一千万个随机数到data.txt中，一共55MB。然后我写了个程序主干计算运行时间，代码如下：

最简单的方法就算写一个循环scanf了，代码如下：

```cpp
#include <ctime>
    int main()
    {
        int start = clock();
        //DO SOMETHING
        printf("%.3lf\n",double(clock()-start)/CLOCKS_PER_SEC);
    }
```

最简单的方法就算写一个循环scanf了，代码如下：

```cpp
<code class="language-cpp">const int MAXN = 10000000;  
       
    int numbers[MAXN];  
       
    void scanf_read()  
    {  
        freopen("data.txt","r",stdin);  
        for (int i=0;i<MAXN;i++)  
            scanf("%d",&numbers[i]);  
    }</code>
```



可是效率如何呢？在我的电脑Linux平台上测试结果为2.01秒。接下来是cin，代码如下


```cpp
const int MAXN = 10000000;
     
    int numbers[MAXN];
     
    void cin_read()
    {
        freopen("data.txt","r",stdin);
        for (int i=0;i<MAXN;i++)
            std::cin >> numbers[i];
    }
```



?[出乎我的意料，cin仅仅用了6.38秒，比我想象的要快。cin慢是有原因的，其实默认的时候，cin与stdin总是保持同步的，也就是说这两种方法可以混用，而不必担心文件指针混乱，同时cout和stdout也一样，两者混用不会输出顺序错乱。正因为这个兼容性的特性，导致cin有许多额外的开销，如何禁用这个特性呢？只需一个语句std::ios::sync_with_stdio(false);，这样就可以取消cin于stdin的同步了。程序如下：

```cpp
const int MAXN = 10000000;
     
    int numbers[MAXN];
     
    void cin_read_nosync()
    {
        freopen("data.txt","r",stdin);
        std::ios::sync_with_stdio(false);
        for (int i=0;i<MAXN;i++)
            std::cin >> numbers[i];
    }
```

取消同步后效率究竟如何？经测试运行时间锐减到了2.05秒，与scanf效率相差无几了！有了这个以后可以放心使用cin和cout了。

接下来让我们测试一下读入整个文件再处理的方法，首先要写一个字符串转化为数组的函数，代码如下 

```cpp
const int MAXS = 60*1024*1024;
    char buf[MAXS];
     
    void analyse(char *buf,int len = MAXS)
    {
        int i;
        numbers[i=0]=0;
        for (char *p=buf;*p && p-buf<len;p++)
            if (*p == ' ')
                numbers[++i]=0;
            else
                numbers[i] = numbers[i] * 10 + *p - '0';
    }
```

把整个文件读入一个字符串最常用的方法是用fread，代码如下：

```cpp
const int MAXN = 10000000;
    const int MAXS = 60*1024*1024;
     
    int numbers[MAXN];
    char buf[MAXS];
     
    void fread_analyse()
    {
        freopen("data.txt","rb",stdin);
        int len = fread(buf,1,MAXS,stdin);
        buf[len] = '\0';
        analyse(buf,len);
    }
```

上述代码有着惊人的效率，经测试读取这10000000个数只用了0.29秒，效率提高了几乎10倍！掌握着种方法简直无敌了，不过，我记得fread是封装过的read，如果直接使用read，是不是更快呢？代码如下：

```cpp
const int MAXN = 10000000;
    const int MAXS = 60*1024*1024;
     
    int numbers[MAXN];
    char buf[MAXS];
     
    void read_analyse()
    {
        int fd = open("data.txt",O_RDONLY);
        int len = read(fd,buf,MAXS);
        buf[len] = '\0';
        analyse(buf,len);
    }
```

转载：[ios::sync_with_stdio(false)](https://blog.csdn.net/yujuan_mao/article/details/8119529)



