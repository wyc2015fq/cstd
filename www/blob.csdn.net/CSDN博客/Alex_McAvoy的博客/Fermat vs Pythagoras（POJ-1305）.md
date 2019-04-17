# Fermat vs. Pythagoras（POJ-1305） - Alex_McAvoy的博客 - CSDN博客





2019年03月20日 20:05:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[POJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Computer generated and assisted proofs and verification occupy a small niche in the realm of Computer Science. The first proof of the four-color problem was completed with the assistance of a computer program and current efforts in verification have succeeded in verifying the translation of high-level code down to the chip level. 

This problem deals with computing quantities relating to part of Fermat's Last Theorem: that there are no integer solutions of a^n + b^n = c^n for n > 2. 

Given a positive integer N, you are to write a program that computes two quantities regarding the solution of x^2 + y^2 = z^2, where x, y, and z are constrained to be positive integers less than or equal to N. You are to compute the number of triples (x,y,z) such that x < y < z, and they are relatively prime, i.e., have no common divisor larger than 1. You are also to compute the number of values 0 < p <= N such that p is not part of any triple (not just relatively prime triples). 

# Input

The input consists of a sequence of positive integers, one per line. Each integer in the input file will be less than or equal to 1,000,000. Input is terminated by end-of-file

# Output

For each integer N in the input file print two integers separated by a space. The first integer is the number of relatively prime triples (such that each component of the triple is <=N). The second number is the number of positive integers <=N that are not part of any triple whose components are all <=N. There should be one output line for each input line.

# **Sample Input**

**1025100**

# Sample Output

**1 44 916 27**


题意：每组数据给出一个数 n，要求在 n 的范围内，毕达哥拉斯三元组的个数，以及 n 以内毕达哥拉斯三元组不涉及的数

思路：

以 25 为例，25 以内的本原的三元组有：(3,4,5)、(7,24,25)、(5,12,13)、(8,15,17)，即第一个要输出 4；所有的毕达哥拉斯三元组除上述 4 个外，还有：(6,8,10)、(9,12,15)、(12,16,20)、(15,20,25)，不包含在这些三元组中的数共有 9 个，即第二个要输出 9

依据毕达哥拉斯三元组定理，可以求出 n 以内所有三元组的个数与值，由于要求不涉及的数，可在求毕达哥拉斯三元组时进行标记，最后枚举所有数，当遇到没有标记的数，计数器+1即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int GCD(int a,int b){
    if(b==0)
        return a;
    return GCD(b,a%b);
}
bool vis[N];
int pythagoras(int n){//返回n以内本原的毕达哥拉斯个数
    int res=0;//本原三元组的个数

    int m=sqrt(n*1.0);
    for(int i=1;i<=m;i+=2){//从1开始，每次+2，保证为奇数
        for(int j=2;j<=m;j+=2){//从2开始，每次+2，保证为偶数
            int a=max(i,j);//大的为m
            int b=min(i,j);//小的为n

            if(GCD(i,j)!=1)//要求m、n互质
                continue;

            int x=a*a-b*b;
            int y=2*a*b;
            int z=a*a+b*b;
            for(int k=1;k*z<=n;k++){//标记所有毕达哥拉斯三元组的x、y、z
                vis[x*k]=true;
                vis[y*k]=true;
                vis[z*k]=true;
            }

            if( (a*a+b*b)<=n )//保证在范围内
                res++;
        }
    }
    return res;
}
int main(){
    int n;
    while(scanf("%d",&n)!=EOF){
        memset(vis,false,sizeof(vis));
        int res1=pythagoras(n);//本原的毕达哥拉斯个数
        int res2=0;
        for(int i=1;i<=n;i++)//枚举所有数
            if(!vis[i])//不是毕达哥拉斯三元组
                res2++;

        printf("%d %d\n",res1,res2);
    }
    return 0;
}
```






