# 2014 Multi-University Training Contest 8 - CD's Coding - CSDN博客





2014年08月15日 13:20:26[糖果天王](https://me.csdn.net/okcd00)阅读数：713标签：[MUTC																[多校																[1006																[1007																[1008](https://so.csdn.net/so/search/s.do?q=1008&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





﻿﻿

# 这次咱Team453出了仨题~



# Monster（1006）
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 2201    Accepted Submission(s): 510**


Problem Description


   Teacher Mai has a kingdom. A monster has invaded this kingdom, and Teacher Mai wants to kill it.

   Monster initially has h HP. And it will die if HP is less than 1.

   Teacher Mai and monster take turns to do their action. In one round, Teacher Mai can attack the monster so that the HP of the monster will be reduced by a. At the end of this round, the HP of monster will be increased by b.

   After k consecutive round's attack, Teacher Mai must take a rest in this round. However, he can also choose to take a rest in any round.

   Output "YES" if Teacher Mai can kill this monster, else output "NO".





Input


   There are multiple test cases, terminated by a line "0 0 0 0".

   For each test case, the first line contains four integers h,a,b,k(1<=h,a,b,k <=10^9).





Output


   For each case, output "Case #k: " first, where k is the case number counting from 1. Then output "YES" if Teacher Mai can kill this monster, else output "NO".






Sample Input


5 3 2 2
0 0 0 0





Sample Output


Case #1: NO







这道题大家都出了咱就不怎么多说啦~ 

大概意思呢是说有个怪物，他有h滴血，我们的攻击力呢是每次a点，但打完之后怪会恢复b点血，我们每次最多持续攻击k次之后就得休息一次呆呆的望着怪回一回合的血，问咱能不能打过这个怪——

所以呀，有这么几种情况要考虑哦~ （打完K局休息1局的这一个轮回称作一个周目）

1)  一招克敌，这个不能忘记哦，如果我们第一刀就能秒怪要单独写哦，免得怪第一次恢复近似无限大还怎么玩对不对~

2)  一周目内怪回血前的那一刀秒怪（在第K次攻击时攻击积累为最大，所以非第一次攻击到这一次都可以用这一个来判断完）

3)  一周目中总伤害大于零，那么终有一日怪必死无疑

Code：


```cpp
#include<cstdio>
#include<iostream>
using namespace std;
typedef long long ll;
int main()
{
    ll h,a,b,k;
    int cas=1;
    while(~scanf("%I64d%I64d%I64d%I64d",&h,&a,&b,&k))
	{
        if (h==0&&a==0&&b==0&&k==0) break;
        if (k*a>(k+1)*b||a>=h||(k-1)*(a-b)+a>=h)  printf("Case #%d: YES\n",cas);
        else printf("Case #%d: NO\n",cas);
        cas++;
    }
    return 0;
}
```







# Multiplication table（1007 By Bluewind）
**Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 370    Accepted Submission(s): 71**


Problem Description


   Teacher Mai has a multiplication table in base p.

   For example, the following is a multiplication table in base 4:

*  0  1  2  3
0 00 00 00 00
1 00 01 02 03
2 00 02 10 12
3 00 03 12 21

   But a naughty kid maps numbers 0..p-1 into another permutation and shuffle the multiplication table.

   For example Teacher Mai only can see:

1*1=11 1*3=11 1*2=11 1*0=11
3*1=11 3*3=13 3*2=12 3*0=10
2*1=11 2*3=12 2*2=31 2*0=32
0*1=11 0*3=10 0*2=32 0*0=23

   Teacher Mai wants you to recover the multiplication table. Output the permutation number 0..p-1 mapped into.

   It's guaranteed the solution is unique.





Input


   There are multiple test cases, terminated by a line "0".

   For each test case, the first line contains one integer p(2<=p<=500).

   In following p lines, each line contains 2*p integers.**The (2*j+1)-th number x and (2*j+2)-th number y in the i-th line indicates equation i*j=xy in the shuffled multiplication table.   Warning: Large IO!**





Output


   For each case, output one line.

   First output "Case #k:", where k is the case number counting from 1. The following are p integers, indicating the permutation number 0..p-1 mapped into.





Sample Input


4
2 3 1 1 3 2 1 0
1 1 1 1 1 1 1 1
3 2 1 1 3 1 1 2
**1 0 1 1 1 2 1 3**
0





Sample Output


Case #1: 1 3 2 0




岩哥的代码，先贴着，等我看完再写结题报告

Code：


```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<vector>
using namespace std;

int intab[509][509*2];
int incnt[509], cnt[509];
int p, tp, id, nums;
int stod[509],dtos[509];

bool find(){
    for(int i=0; i<p; ++i){
        if(dtos[i]==-1){
            return true;
        }
    } 
    return false;
}

int main()
{
//    freopen("in.txt","r",stdin);
    int Cases=1;    
    while(scanf("%d",&p)!=EOF){
        if(p==0)
            break;
        
        memset(cnt,0,sizeof(cnt));
        for(int i=0; i<p; ++i){
            for(int j=0; j<p; ++j){
                cnt[(i*j)/p]++;
                cnt[(i*j)%p]++;
            }
        } 
        memset(incnt,0,sizeof(incnt));
        for(int i=0; i<p; ++i){
            for(int j=1; j<=p*2; ++j){
                scanf("%d",&intab[i][j]);
                incnt[intab[i][j]]++;
            }
        }
        memset(stod,-1,sizeof(stod));
        memset(dtos,-1,sizeof(dtos));
        for(int i=0; i<p; ++i){
            nums=0;
            for(int j=0; j<p; ++j){
                if(incnt[i]==cnt[j])
                    nums++,id=j;
            }
            if(nums==1){
                dtos[i]=id;
                stod[id]=i;
            }
        }
        while(find()){
            for(int i=0; i<p; ++i){
                if(dtos[i]!=-1)
                    continue;
                for(int j=1; j<2*p; j+=2){
                    if(dtos[intab[i][j]]!=-1 && dtos[intab[i][j+1]]!=-1){
                        nums=dtos[intab[i][j]]*p+dtos[intab[i][j+1]];
                        if((j-1)/2==i && dtos[i]==-1){
                            dtos[i]=(int)sqrt((double)nums);
                            stod[dtos[i]]=i;
                        } 
                        else if(dtos[(j-1)/2]>0){
                            dtos[i]=nums/dtos[(j-1)/2];
                            stod[dtos[i]]=i;
                        }
                    }
                }
            }
        }
        printf("Case #%d: %d",Cases,stod[0]);
        Cases++;
        for(int i=1; i<p; ++i){
            printf(" %d",stod[i]);
        } 
        printf("\n");
    }
    return 0;
}
```








# Number Transformation（1008）
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 1395    Accepted Submission(s): 379**


Problem Description


   Teacher Mai has an integer x.

   He does the following operations k times. In the i-th operation, x becomes the least integer no less than x, which is the multiple of i.

   He wants to know what is the number x now.





Input


   There are multiple test cases, terminated by a line "0 0".

   For each test case, the only one line contains two integers x,k(1<=x<=10^10, 1<=k<=10^10).






Output


   For each test case, output one line "Case #k: x", where k is the case number counting from 1.





Sample Input


2520 10
2520 20
0 0





Sample Output


Case #1: 2520
Case #2: 2600







这道题是说，有两个数x,k，REP(i,1,k)把x变成不小于x的最小的i的倍数

关于这个操作啊，千万不能傻乎乎的每次都遍历哦，仔细想想看，这是不是这样一个过程呢——


```cpp
plus=kk-x%kk;
x+=plus;
```





接着，我们先把模拟代码写出来


```cpp
while(kk!=k)
		{
			kk++;
			if(x%kk==0) continue;
			plus=kk-x%kk;
			cout<<kk<<":"<<plus<<endl;
			//if (plus==last)flag++;
			//else flag=0;
			//if(flag==10)
			//{
			//	x+=plus*(k-kk+1);
			//	break;
			//}
			x+=plus;
			last=plus;
		}
```





然后随手写一点数看看，是不是到了最后都一样了哇？变着变着就等差数列了呢~ 我这里输出的是每次增加的值，所以我看到的是最后的输出的数字都一样了

那么，我们只要判断啥时候一样了就可以直接乘了对不对？

然后就是这个flag，大家都能看懂，连续flag个数相同我们就开始乘，那么flag是多少为好呢？ 我wa的两次是一次flag==3，一次flag==5，第三次AC是flag==10

为啥小了不行呢？，因为咱仔细观察观察前面的plus的规律哦，是不是最终的公差是0，前面一点是2，在前面是4，然后6、8、10这样下去的呀？我们顺过来看的话，如果当前的plus小于公差，那么下一个数呢就是plus+下一个公差-当前公差——这样就有很大可能造成在公差小的时候出现abs（前一个plus公差-后一个plus公差）非0时也出错哦，所以flag大点好——


```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
typedef long long ll;

int main()
{
	ll x,k,cnt=0;
	ll last=-1,plus,flag=0;
	while(scanf("%I64d%I64d",&x,&k)!=EOF)
	{
		ll kk=1;
		cnt++;
		if(x==0 && k==0)return 0;
		if(x==1||x==2){printf("Case #%I64d: %I64d\n",cnt,k);continue;}
		while(kk!=k)
		{
			kk++;
			if(x%kk==0) continue;
			plus=kk-x%kk;
			//cout<<kk<<":"<<plus<<endl;
			if (plus==last)flag++;
			else flag=0;
			if(flag==10)
			{
				x+=plus*(k-kk+1);
				break;
			}
			x+=plus;
			last=plus;
		}
		printf("Case #%I64d: %I64d\n",cnt,x);
	}
	return 0;
}
```





然后说到这里啊，我就在想了……这个试验flag的方法会不会被人说咱是拼数据水过去的呢？那么，我们可以不用flag来试验，咱可以直接判定如果当前的abs为0即可对不对，

嘛，先贴一个学长的1008Code：


```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <time.h>
using namespace std;
const int maxint = -1u>>1;
long long n,k;
long long Cal(long long a,long long i)
{
    if (a%i==0) return a;
    return (a/i+1)*i;
}

long long pre[10];
int main() {
#ifndef ONLINE_JUDGE
   freopen("f:/in.txt","r",stdin);
#endif
   int ca=0;
   while (scanf("%I64d%I64d",&n,&k)!=EOF && (n!=0 || k!=0))
   {
       ca++;
       long long ans=n;
       long long pre4=-1;;
       long long t=50000;
       t=min(t,k);
       memset(pre,-1,sizeof(pre));
       for(long long i=1;i<=k;i++){
                int f=0;
                for(int j=0;j<9;j++){if (j>0 && (pre[j]!=pre[j+1] || pre[j+1]<0)) f=1; pre[j]=pre[j+1];}
                pre[9]=ans;
                ans=Cal(ans,i);
                pre[9]=ans-pre[9];
                if (f==0 && pre[8]==pre[9]){
                    ans=ans+pre[9]*(k-i);
                    break;
                }
       }
       printf("Case #%d: %I64d\n",ca,ans);
   }
   return 0;
}
```





那么就先到这里吧~ 难得在MUTC上出题出到这么开心，就写一篇聊以记录一下～　Ｂｙｅ～](https://so.csdn.net/so/search/s.do?q=1007&t=blog)](https://so.csdn.net/so/search/s.do?q=1006&t=blog)](https://so.csdn.net/so/search/s.do?q=多校&t=blog)](https://so.csdn.net/so/search/s.do?q=MUTC&t=blog)




