# Can you find it?（HDU-2141） - Alex_McAvoy的博客 - CSDN博客





2019年02月17日 17:59:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[HDU																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Give you three sequences of numbers A, B, C, then we give you a number X. Now you need to calculate if you can find the three numbers Ai, Bj, Ck, which satisfy the formula Ai+Bj+Ck = X. 

# Input

There are many cases. Every data case is described as followed: In the first line there are three integers L, N, M, in the second line there are L integers represent the sequence A, in the third line there are N integers represent the sequences B, in the forth line there are M integers represent the sequence C. In the fifth line there is an integer S represents there are S integers X to be calculated. 1<=L, N, M<=500, 1<=S<=1000. all the integers are 32-integers. 

# Output

For each case, firstly you have to print the case number as the form "Case d:", then for the S queries, you calculate if the formula can be satisfied or not. If satisfied, you print "YES", otherwise print "NO". 

# **Sample Input**

**3 3 31 2 31 2 31 2 331410**

# Sample Output

**Case 1:NOYESNO**


题意：每组数据给出为 A、B、C 的数组，再给出 x 个数，判断这 x 个数内是否有满足 Ai+Bj+Ck=x 的情况

思路：

本质是三个数组去求和，然后去判断有没有与 x 个数相等的情况，由于自己去暴力枚举一定会超时，因此可以先将前两个数组两两相加构成一个新的数组 ab，排序后再让新数组 ab 与 x-c[i] 不断的二分，直到得到 ab=x-ci  或退出循环为止

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
#define N 501
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int a[N],b[N],c[N];
int ab[N*N];
int cnt;
bool judge(int x){
    int left=0,right=cnt;
    while(left<=right){
        int mid=(left+right)/2;
        if(ab[mid]<x)
            left=mid+1;
        else if(ab[mid]>x)
            right=mid-1;
        else
            return true;
    }
    return false;
}
int main(){
    int Case=1;
    int l,n,m;
    while(scanf("%d%d%d",&l,&n,&m)!=EOF&&(l+m+n)){
        for(int i=0;i<l;i++)
            scanf("%d",&a[i]);
        for(int i=0;i<n;i++)
            scanf("%d",&b[i]);
        for(int i=0;i<m;i++)
            scanf("%d",&c[i]);

        cnt=0;
        for(int i=0;i<l;i++)
            for(int j=0;j<n;j++)
                ab[cnt++]=a[i]+b[j];

        sort(ab,ab+cnt);

        printf("Case %d:\n",Case++);

        int t;
        scanf("%d",&t);
        while(t--){
            int x;
            scanf("%d",&x);

            bool flag=false;
            for(int i=0;i<m;i++){
                int y=x-c[i];
                if(judge(y)){
                    flag=true;
                    break;
                }
            }

            if(flag)
                printf("YES\n");
            else
                printf("NO\n");
        }
    }
    return 0;
}
```






