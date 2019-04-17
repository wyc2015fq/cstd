# hdu6435——Problem J. CSGO - westbrook1998的博客 - CSDN博客





2018年09月22日 23:20:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[状态压缩																[位运算](https://so.csdn.net/so/search/s.do?q=位运算&t=blog)](https://so.csdn.net/so/search/s.do?q=状态压缩&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
You are playing CSGO.

There are n Main Weapons and m Secondary Weapons in CSGO. You can only choose one Main Weapon and one Secondary Weapon. For each weapon, it has a composite score S.

The higher the composite score of the weapon is, the better for you.

Also each weapon has K performance evaluations x[1], x[2], …, x[K].(range, firing rate, recoil, weight…)

So you shold consider the cooperation of your weapons, you want two weapons that have big difference in each performance, for example, AWP + CZ75 is a good choose, and so do AK47 + Desert Eagle.

All in all, you will evaluate your weapons by this formula.(MW for Main Weapon and SW for Secondary Weapon)
![在这里插入图片描述](https://odzkskevi.qnssl.com/e7551dc9a785ee5c0a133897967273a9?v=1537624906)

Now you have to choose your best Main Weapon & Secondary Weapon and output the maximum evaluation.

Input

Multiple query.

On the first line, there is a positive integer T, which describe the number of data. Next there are T groups of data.

for each group, the first line have three positive integers n, m, K.

then, the next n line will describe n Main Weapons, K+1 integers each line S, x[1], x[2], …, x[K]

then, the next m line will describe m Secondary Weapons, K+1 integers each line S, x[1], x[2], …, x[K]

There is a blank line before each groups of data.

T<=100, n<=100000, m<=100000, K<=5, 0<=S<=1e9, |x[i]|<=1e9, sum of (n+m)<=300000

Output

Your output should include T lines, for each line, output the maximum evaluation for the corresponding datum.

Sample Input

2

2 2 1

0 233

0 666

0 123

0 456

2 2 1

100 0 1000 100 1000 100

100 0

Sample Output

543

2000
开始尝试做一下多校的题…

这题看了题解才懂的，要选两个武器一主一副，使得满足那个式子的最大值，因为有绝对值的原因，所以去掉绝对值之后就存在由于两个值大小关系而导致的顺序不同，比如有时候是主武器的属性减副武器，有时候是副武器减主武器，因为k也就是副属性的个数最多只要5,用一个二进制数来表示一个状态，表示每个属性的系数是正是负，然后先计算出主武器所有状态下的最大的一个值（不管是选哪个武器），同时算出副武器的最大值，然后最后枚举主武器状态，而副武器状态就是相反的，找出最大值，这里面用到状态压缩和位运算的一些技巧

代码：

```
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long ll;
const int N=100050;
const ll INF=0x3f3f3f3f3f3f3f3f;
ll mw[40],sw[40];
ll tmp[8];
int t;
int n,m,k;
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%d%d%d",&n,&m,&k);
        //题目会出现负数，因此要初始化为-INF
        for(int i=0;i<40;i++){
            mw[i]=-INF;
            sw[i]=-INF;
        }
        //每次计算一个主武器的值（包括主属性和副属性（或加或减，通过二进制位进行状态压缩，找出最大值））
        for(int i=0;i<n;i++){
            //主武器的主属性
            ll val;
            scanf("%lld",&val);
            for(int j=0;j<k;j++){
                scanf("%lld",&tmp[j]);
            }
            //读取
            for(int j=0;j<(1<<k);j++){
                ll cnt=val;
                //状态压缩，有2^k种状态
                for(int t=0;t<k;t++){
                    //如果该位是1,则加上副属性，否则减
                    if((j>>t)&1){
                        cnt+=tmp[t];
                    }
                    else{
                        cnt-=tmp[t];
                    }
                }
                //更新主武器（所有）在j状态（即某些副属性加，某些减）下的最大值
                mw[j]=max(mw[j],cnt);
            }
        }
        for(int i=0;i<m;i++){
            ll val;
            scanf("%lld",&val);
            for(int j=0;j<k;j++){
                scanf("%lld",&tmp[j]);
            }
            for(int j=0;j<(1<<k);j++){
                ll cnt=val;
                for(int t=0;t<k;t++){
                    if((j>>t)&1){
                        cnt+=tmp[t];
                    }
                    else{
                        cnt-=tmp[t];
                    }
                }
                sw[j]=max(sw[j],cnt);
            }
        }
        //考虑负数值
        ll _max=-INF;
        //枚举主武器和副武器状态相反的状态
        for(int i=0;i<(1<<k);i++){
            _max=max(_max,mw[i]+sw[(1<<k)-i-1]);
        }
        printf("%lld\n",_max);
    }
    return 0;
}
```






