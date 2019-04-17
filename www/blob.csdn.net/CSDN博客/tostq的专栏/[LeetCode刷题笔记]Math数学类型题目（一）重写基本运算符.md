# [LeetCode刷题笔记]Math数学类型题目（一）重写基本运算符 - tostq的专栏 - CSDN博客





2016年07月22日 21:47:34[tostq](https://me.csdn.net/tostq)阅读数：1025








**原创文章**


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)

      个人觉得Math类的题最为经典的，就是重写各类常见的运算，比如乘法、除法、加法、平方、开方之类，题目都不难，但要求做题者要有一定的数学知识，及对计算机计算结构有所了解。
这一类题的主要难点：
1、一般不能使用+、-、*、/的符号，比如不让使用+号做加法运算
2、注意数据的溢出，比如最小整数-2147483648的绝对值会溢出
3、计算速度要快
4、注意输入数（一般是整数）的符号
一般的解题思路：
1、通过>>、<<进位符号
2、减少重复循环
2、避免数据溢出INT_MAX=0x7fffffff
3、利用二分法来解决未知变量
下面给出几题的例子。

**一、加减法的重写**
**371. Sum of Two Integers**
这个题目要求计算两个整数的加法，却要求你不能使用+或-法
不用+或-来实现加法，难道还用乘法？刚看完这题我是一脸懵逼的.jpg
但是仔细一想，应该是通过二进制的与或非来求解的。

另外作为本科学EE的我来说，突然想起了，曾经还做过通过与非门构建全加器的实验，所以不难发现：
比如13（01101）+25（11001）=100110
13&25=01001表示的是进位
13^25=10100表示的是半加法
半加+进位<<1就得到了最后结果，所以我们可以通过迭代从而有如下解法




```cpp
class Solution {
public:
    int getSum(int a, int b) {
        return a & b ? getSum((a & b) << 1, a ^ b) : a | b;
    }
};
```
当进位为零的时候，就可以直接通过或来得到加法（异或也是一样的）



**二、乘除法的重写**
**29. Divide Two Integers**
这一个题目是求两个整数的除法，也就是重写除法符号/
可能我们第一眼看过去，可以通过不停的减法，就可以完成除法了。
比如5/2=... 5-2=3, 3-2=1<2, 所以5/2=2;
但是这种将减法来代替除法的方法，会消耗大量的时间
运行时会提出Time Limit Error
所以正确的方法是通过>> 这个右移命令，其表示/2
比如9=2*4+1，所以9/2=4.


```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        if(divisor==0||(dividend == INT_MIN && divisor == -1))return INT_MAX;        
        int sign=((dividend>=0&&divisor>=0)||(dividend<=0&&divisor<=0))?1:-1;
        long long dvd = labs(dividend);
        long long dvs = labs(divisor);
        int res=0;
        while(dvd>=dvs){
            long long temp=dvs;
            int r=1;
            while(dvd>=(temp<<1)){
                temp=temp<<1;
                r=r<<1;
            }
            dvd=dvd-temp;
            res=res+r;
        }
        return sign*res;
    }
};
```
       这里我们可以看到long long的数据结构，我们将输入的数据变成了长整型，为什么要这么做呢？
原因是当输入为-2147483648时，它的绝对值为2147483648，这是超过整数范围的-2147483648~2147483647的，所以需要把其绝对值变成Long long。
另外temp变为Long 还有另一层意思，temp需要乘以2，
**TIPS:**
**对于做加法、乘法或平方之类变量，一定要观察其会不会出现数据溢出**
所以这一题的难点在于
1、对数据溢出的处理
2、理解除法运算在计算机中的实现
同理，乘法可以用<<和类似的方法来计算。

**三、平方（多次方）的重写**
这类题总共有两类题：求某数的平方及判断某数是否是平方数
**1、求某数的平方**
**50. Pow(x, n)**
这道题求的是x的n次方，比较直观的方式就是利用while循环，x连续相乘n次。
这样的话总共需要循环n次，花得时间还是太长
不过我们利用x^n=x*x^(n-1)，来减少循环次数。

```cpp
class Solution {
public:
    double myPow(double x, int n) {
        double res=1;
        x = n>=0?x:1/x;
        unsigned int nl = abs(n);
        while(nl){
            if (nl&1) res*=x;
            nl>>=1;
            x*=x;
        }
        return res;
    }
};
```
      这里我们并不需要考虑是否会出现整型溢出的情况，因为n不会太大。
不过这里定义unsigned int的原因是因为我们通过n1&1来判断当前位是否为0，而当n1为负数时，其首位为1，这样会影响我们判断。
另外还有一种方法是通过递归方式，算法思路是相同，不过更加直观。

```cpp
double myPow(double x, int n) {
    x= n>=0?x:(1./x);
    unsigned int temp= n>=0?n:(-n);
    if(temp==0)
        return 1;
    
    double t=myPow(x,temp>>1);
    if(n&1){
        return x*t*t;
    }else{
        return t*t;
    }
}
```
**2、判断是否是某数的平方**
**231. Power of Two**
该题判断某数是否是2的平方
通过while来不停地除以2是比较直观的方法，比如这样。

```cpp
class Solution {
public:
    bool isPowerOfTwo(int n) {
        while(n/2>0){
            if(n%2!=0)return false;
            n=n/2;
        }
        return n==1?true:false;
    }
};
```
       不过更简单的方法是观察数的固有特性
       比如2^4=1000，2^5=10000，显然通过二进制，我们就可以方便地发现平方数的特性
       所以就有了如下解法

```cpp
bool isPowerOfTwo(int n) {
    return (n>0)&&(!(n&(n-1)));
}
```
同理关于判断某数是否是3次方数
**353. Power of Three**
3次方数，可以通过判断其能否被最大3次方数整除来确定。

```cpp
bool isPowerOfThree(int n)
    {
        int maxPowerOfThree = (int)pow(3, (int)(log(INT_MAX)/log(3)));
        return n>0 && maxPowerOfThree%n==0;
    }
```
      再来一例子：
**342. Power of Four**
判断某数是否是4次方数，还要求你不能使用递归和循环
4次方，表示为4^1=100，4^2=10000……4^n=(2^n)^2

```cpp
class Solution {
public:
    bool isPowerOfFour(int num) {
        int a=sqrt(num);
        return num>0 && a*a==num && !(a&(a-1));
    }
};
```

**四、开方的重写**
       同平方问题一样，开方问题也有类似两类问题：
1、计算某数的开方
2、判断某数是否是能开方
不过开方相比于平方，不能可以变成数的连续相乘，所以就没有那么直观了
开方问题需要找到x，满足x*x==n。
同许多求解未知参数的问题一样，开方也可以通过二分法来解决。
**69. Sqrt(x)**

```cpp
// Binary search 
int mySqrt(int x) { // here x>0
    int low=0,high=x,mid=0;
    if(x<2)return x;
    while(low<high){
        mid=(low+high)/2;
        if(x/mid>=mid)low=mid+1;
        else high=mid;
    }
    return high-1;
}
```
另外，我们需要注意到Low+high也很可能导致数据溢出
所以比较推荐的写法是：

```cpp
mid=low+(high-low)>>1
```
这里还将除2用>>1来代替，加快计算速度
**2、判断某数是否是能开方367. Valid Perfect Square**

```cpp
bool isPerfectSquare(int num) {
    if(num<2)return true;
    num=(unsigned int)num;
    unsigned int low=1,mid;
    unsigned int high=(num>>1)+1;


    while(low<high){
        mid=(low+high)>>1;


        long int sq=mid*mid;
        if(sq==num)return true;
        if(sq<num)
            low=mid+1;
        else
            high=mid-1;


    }
    return low*low==num?true:false;
}
```
     这里将sq变为Long int，是为了防止mid*mid溢出












