# BZOJ2301——[HAOI2011]Problem b - westbrook1998的博客 - CSDN博客





2018年11月14日 21:01:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：21标签：[莫比乌斯反演																[整除分块																[数论](https://so.csdn.net/so/search/s.do?q=数论&t=blog)
个人分类：[BZOJ刷题																[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)](https://blog.csdn.net/westbrook1998/article/category/8335089)





> 
Description

对于给出的n个询问，每次求有多少个数对(x,y)，满足a≤x≤b，c≤y≤d，且gcd(x,y) = k，gcd(x,y)函数为x和y的最大公约数。

Input

第一行一个整数n，接下来n行每行五个整数，分别表示a、b、c、d、k

Output

共n行，每行一个整数表示满足要求的数对(x,y)的个数

Sample Input

2

2 5 1 5 1

1 5 1 5 2

Sample Output

14

3

HINT

100%的数据满足：1≤n≤50000，1≤a≤b≤50000，1≤c≤d≤50000，1≤k≤50000
初学莫比乌斯反演和线性筛莫比乌斯函数

看了题解总算能看懂

首先题目要求的是区间$(a,b)$和$(b,c)$各取一个数$x,y$满足$gcd(x,y)=k$的数对个数，也就等于$gcd(x/k,y/k)=1$的个数。

然后这个区间也可以通过前缀和思想和容斥原理来求，我们现在设$calc(x,y)$表示$a\in[1,x],b\in [1,y]$区间中$gcd(a,b)==1$的个数，那所求就能表示为$calc(b,d)-calc(a-1,d)-calc(b,c-1)+calc(a-1,c-1)$。

然后问题就转化为来计算这个函数$calc(x,y)$了，所以我们定义$f(i)$为$gcd(x,y)=i$的数对数，定义$F(i)$为满足$i|gcd(x,y)$的数对数，$F(i)=\lfloor \frac{n}{i}\rfloor \lfloor \frac{m}{i}\rfloor$，比如现在$n=10,m=12$，$F(6)$就是表示这个区间内$gcd(x,y)=6,12,18,...$的数对数，可以得到是$2$，也就是$(6,6),(6,12)$，所以$F(n)=\sum \limits _{n|d}f(d)$，然后通过莫比乌斯反演得到$f(n)=\sum\limits _{n|d}\mu(\frac{d}{n})F(d)=\sum\limits_{n|d}\mu(\frac{d}{n})\lfloor \frac{a}{n}\rfloor \lfloor \frac{b}{n}\rfloor$。

现在已知$k$要求的就是$f(k)$，还有已知区间边界$a,b$，如果直接枚举$\frac{d}{n}$还不行，这里因为后面两个整除是可以分块的，所以还要求个莫比乌斯函数的前缀和，直接分块处理
代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e7+50;
//同时筛出素数和莫笔乌斯函数
int p[N],miu[N];
bool check[N];
int pre[N];
void init(){
    int t;
    miu[1]=1;
    check[1]=true;
    for(int i=2;i<=N;i++){
        if(!check[i]){
            p[++p[0]]=i;
            miu[i]=-1;
        }
        for(int j=1;j<=p[0];j++){
            t=i*p[j];
            if(t>N){
                break;
            }  
            check[t]=true;
            //有平方因子，函数值为0
            if(i%p[j]==0){
                miu[t]=0;
            }else{
                //质因子数量改变，符号改变
                miu[t]=-miu[i];
            }
        }
    }
    //前缀和
    for(int i=1;i<N;i++){
        pre[i]=pre[i-1]+miu[i];
    }
}
int t,a,b,c,d,k;
int calc(int x,int y){
    x/=k;
    y/=k;
    if(x>y){
        swap(x,y);
    }
    int ans=0;
    int t;
    //枚举d/n，即莫比乌斯函数，只要枚举到小的数即可，gcd不可能大于那个小数
    for(int i=1;i<=x;i=t+1){
        t=min(x/(x/i),y/(y/i));
        ans+=(pre[t]-pre[i-1])*(x/i)*(y/i);
    }
    return ans;
}
int main(void){
    init();
    scanf("%d",&t);
    while(t--){
        scanf("%d%d%d%d%d",&a,&b,&c,&d,&k);
        printf("%d\n",calc(b,d)-calc(a-1,d)-calc(b,c-1)+calc(a-1,c-1));
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=整除分块&t=blog)](https://so.csdn.net/so/search/s.do?q=莫比乌斯反演&t=blog)




