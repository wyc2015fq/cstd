# C++ 随机数rand()和srand() - 在思索中前行！ - CSDN博客





2014年08月10日 18:31:05[_Tham](https://me.csdn.net/txl16211)阅读数：990标签：[随机数																[rand																[C++随机数																[C++ rand																[C++ srand](https://so.csdn.net/so/search/s.do?q=C++ srand&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)





       标准函数库中函数rand()可以生成0~RAND_MAX之间的一个随机数，其中RAND_MAX 是stdlib.h中定义的一个整数，它与系统有关。 例如在我的机器上，RAND_MAX=32767。可以使用 printf("%ld",RAND_MAX);查看。

调用rand()可以生成一个随机数，但我们往往需要的是一个有上下界的随机数。

例如 要产生一个[a,b]之间的随机整数x

```cpp
x = rand()%(b-a+1)+a
```

产生区间[a,b]上的随机实数
```cpp
z = ((double)rand()/RAND_MAX)*(b-a) + a
```

### rand()简介：   

   生成伪随机数的函数包含在标准库 <cstdlib>中，所以首先需在头文件中包含进来。

注意：在C语言中包含在标准库<stdlib.h>中。
函数一：int   rand(void)； 
    从srand   (seed)中指定的seed开始，返回一个[seed,   RAND_MAX（0x7fff）)间的随机整数。 

    函数二：void   srand(unsigned   seed)； 
    参数seed是rand()的种子，用来初始化rand()的起始值。 

用法：它需要提供一个种子，这个种子会对应一个随机数，如果使用相同的种子后面的rand()函数会出现一样的随机数。如： srand(1); 直接使用1来初始化种子。不过为了防止随机数每次重复常常使用系统时间来初始化，即使用 time函数来获得系统时间，它的返回值为从 00:00:00 GMT, January 1, 1970 到现在所持续的秒数，然后将time_t型数据转化为(unsigned)型再传给srand函数，即：
 srand((unsigned) time(&t)); 还有一个经常用法，不需要定义time_t型t变量,即： srand((unsigned) time(NULL)); 直接传入一个空指针，因为你的程序中往往并不需要经过参数获得的t数据。srand((int)getpid()); 使用程序的ID(getpid())来作为初始化种子，在同一个程序中这个种子是固定的。


    可以认为rand()在每次被调用的时候，它会查看： 

    1） 如果用户在此之前调用过srand(seed)，给seed指定了一个值，那么它会自动调用 srand(seed)一次来初始化它的起始值。 

    2） 如果用户在此之前没有调用过srand(seed)，它会自动调用srand(1)一次。 


   根据上面的第一点我们可以得出： 

   1） 如果希望rand（）在每次程序运行时产生的值都不一样，必须给srand(seed)中的seed一个变值，这个变值必须在每次程序运行时都不一样（比如到目前为止流逝的时间）。 

   2） 否则，如果给seed指定的是一个定值，那么每次程序运行时rand（）产生的值都会一样，虽然这个值会是[seed,   RAND_MAX（0x7fff）)之间的一个随机取得的值。 

   3） 如果在调用rand()之前没有调用过srand(seed)，效果将和调用了srand(1)再调用rand()一样（1也是一个定值）。 

举几个例子，假设我们要取得0～6之间的随机整数（不含6本身）： 


例一，不指定seed： 


```cpp
for(int   i=0;i <10;i++){
    ran_num=rand()   %   6; 
    cout < <ran_num < < "   "; 
}
```
每次运行都将输出：5   5   4   4   5   4   0   0   4   2 



例二，指定seed为定值1： 


```cpp
srand(1); 
for(int   i=0;i <10;i++){
    ran_num=rand()   %   6; 
    cout < <ran_num < < "   "; 
}
```
每次运行都将输出：5   5   4   4   5   4   0   0   4   2 

跟例子一的结果完全一样。 


例三，指定seed为定值6： 

```cpp
srand(6); 
for(int   i=0;i <10;i++){
    ran_num=rand()   %   6; 
    cout < <ran_num < < "   "; 
}
```
每次运行都将输出：4   1   5   1   4   3   4   4   2   2 

随机值也是在[0,6）之间，随得的值跟srand(1)不同，但是每次运行的结果都相同。 


例四，指定seed为当前系统流逝了的时间（单位为秒）：time_t   time(0)： 

```cpp
#include   <ctime> 
//… 
srand((unsigned)time(0)); 
for(int   i=0;i <10;i++){
    ran_num=rand()   %   6; 
    cout < <ran_num < < "   "; 
}
```


第一次运行时输出：0   1   5   4   5   0   2   3   4   2 

第二次：3   2   3   0   3   5   5   2   2   3 

总之，每次运行结果将不一样，因为每次启动程序的时刻都不同（间隔须大于1秒？见下）。 

**关于time_t   time(0)：**

time_t被定义为长整型，它返回从1970年1月1日零时零分零秒到目前为止所经过的时间，单位为秒。比如假设输出：

cout < <time(0); 

值约为1169174701，约等于37（年）乘365（天）乘24（小时）乘3600（秒）（月日没算）。 

另外，关于ran_num   =   rand()   %   6， 

将rand()的返回值与6求模是必须的，这样才能确保目的随机数落在[0,6)之间，否则rand()的返回值本身可能是很巨大的。 

一个通用的公式是： 

要取得[a,b)之间的随机整数，使用（rand()   %   (b-a)）+   a   （结果值将含a不含b）。 

在a为0的情况下，简写为rand()   %   b。 

最后，关于伪随机浮点数： 

用rand()   /   double(RAND_MAX)可以取得0～1之间的浮点数（注意，不同于整型时候的公式，是除以，不是求模），举例： 

```cpp
double   ran_numf=0.0; 
srand((unsigned)time(0)); 
for(int   i=0;i <10;i++){
    ran_numf   =   rand()   /   (double)(RAND_MAX); 
    cout < <ran_numf < < "   "; 
}
```
运行结果为：0.716636，0.457725，…等10个0～1之间的浮点数，每次结果都不同。 


如果想取更大范围的随机浮点数，比如1～10，可以将 

rand()   /(double)(RAND_MAX)   改为   rand()   /(double)(RAND_MAX/10) 

运行结果为：7.19362，6.45775，…等10个1～10之间的浮点数，每次结果都不同。 
至于100，1000的情况，如此类推。 



**计算机产生随机序列rand()（线性同余方法）**

众所周知，计算机产生随机序列实际上是一个有很长周期的序列（通过线性同余算法），这就是伪随机数。随机数产生时需要一个seed(种子)，才能产生随机数。

在Pascal 和 Basic 中，都可以使用 randomize;来初始化种子。而在C中，需要用srand(unsigned int)函数给予一个种子。 例如srand(time(NULL));

一般来说在OI中，依靠随机数的算法较为稳定，它不受随机序列的影响（难道有人看过默认的随机序列，专门设计出一组卡随机的测试数据？？？？）。所以srand调用与否没有太大关系。况且time()函数定义在 time.h 中，某些评测系统禁止使用time.h。我一般都是srand(rand());。

```
线性同余方法（LCG）是个产生伪随机数的方法。
它是根据递归公式：
其中A,B,M是产生器设定的常数。
LCG的周期最大为M，但大部分情况都会少于M。要令LCG达到最大周期，应符合以下条件：
   1. B,M互质；
   2. M的所有质因子的积能整除A − 1；
   3. 若M是4的倍数，A − 1也是；
   4. A,B,N0都比M小；
   5. A,B是正整数。
```

使用线性同余算法产生伪随机数 转自[http://www.blog.edu.cn/user1/20989/archives/2005/989163.html](http://www.blog.edu.cn/user1/20989/archives/2005/989163.html) C语言中有个random(n)函数，可以产生0——n-1之间的伪随机数，rand()函数可以产生0——32767之间的伪随机数，一般需要配合使用srand(long)给出随机种子，或者使用randomize()函数来根据系统时间指定随机种子。

```
常用的产生伪随机数的算法是线性同余法，下面是我写的代码，相信通过代码，大家可以很容易地明白伪随机数原理：

     算法：an+1=b*an+c mod m；a0=d。d为种子。
```

```cpp
int d;/*种子*/
long My_Rand(long b,long c,long m)
{
   return  d=(b*d+c)%m;/*产生随机数，并记录，用来产生下一个伪随机数*/
}

long gcd1(long m,long n)
{/*求最大公约数，算法中的b和m一般取互素*/
   long r;
   while((r=m%n)!=0)
   {
     m=n;
     n=r;
   }
   return n;
}

long gcd(long m)
{/*计算与m互素的b*/
   long i=2;
   while(gcd1(i,m)!=1)
      i++;
   return i;
}

void main()
{
   long b,c,m;
   int count;
   int i;
   m=32767;
   b=gcd(m);
   c=5001;
   d=3;
   count=800;
   for(i=0;i<count;i++)/*产生800个伪随机数*/
   {
      if(i%10==0)
           printf("\n");
      printf("%8ld",My_Rand(b,c,m));
   }
}
```](https://so.csdn.net/so/search/s.do?q=C++ rand&t=blog)](https://so.csdn.net/so/search/s.do?q=C++随机数&t=blog)](https://so.csdn.net/so/search/s.do?q=rand&t=blog)](https://so.csdn.net/so/search/s.do?q=随机数&t=blog)




