# Codeforces1005E1——Median on Segments (Permutations Edition) - westbrook1998的博客 - CSDN博客





2018年08月09日 08:14:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[map](https://so.csdn.net/so/search/s.do?q=map&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given a permutation p1,p2,…,pn. A permutation of length n is a sequence such that each integer between 1 and n occurs exactly once in the sequence. 

  Find the number of pairs of indices (l,r) (1≤l≤r≤n) such that the value of the median of pl,pl+1,…,pr is exactly the given number m. 

  The median of a sequence is the value of the element which is in the middle of the sequence after sorting it in non-decreasing order. If the length of the sequence is even, the left of two middle elements is used. 

  For example, if a=[4,2,7,5] then its median is 4 since after sorting the sequence, it will look like [2,4,5,7] and the left of two middle elements is equal to 4. The median of [7,1,2,9,6] equals 6 since after sorting, the value 6 will be in the middle of the sequence. 

  Write a program to find the number of pairs of indices (l,r) (1≤l≤r≤n) such that the value of the median of pl,pl+1,…,pr is exactly the given number m. 

  Input 

  The first line contains integers n and m (1≤n≤2⋅105, 1≤m≤n) — the length of the given sequence and the required value of the median. 

  The second line contains a permutation p1,p2,…,pn (1≤pi≤n). Each integer between 1 and n occurs in p exactly once. 

  Output 

  Print the required number. 

  Examples 

  Input 

  5 4 

  2 4 5 3 1 

  Output 

  4 

  Input 

  5 5 

  1 2 3 4 5 

  Output 

  1 

  Input 

  15 8 

  1 15 2 14 3 13 4 8 12 5 11 6 10 7 9 

  Output 

  48 

  Note 

  In the first example, the suitable pairs of indices are: (1,3), (2,2), (2,3) and (2,4).
这题一开始有思路但是一直没过 

看了一下题解 发现一些我写的不好的地方 

首先是我用两个数组来存i前面这一段比他大和比他小的数的个数，其实只要用一个数组保存他们的差就好了 

然后，读取的时候定位中位数的位置，然后左边的要加入map，然后等会后面枚举右边，就可以直接查前面有多少个符合要求的区间，而不用for扫一遍
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;
const int MAXN=200050;
int a[MAXN];
int c[MAXN];
map<int,int> res;
int main(void){
    int n,m;
    int f;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
        if(a[i]==m){
            f=i;
        }
    }
    c[f]=0;
    res[0]++;
    for(int i=f-1;i>=1;i--){
        if(a[i]==m){
            c[i]=c[i+1];
        }
        else if(a[i]<m){
            c[i]=c[i+1]-1;
        }
        else{
            c[i]=c[i+1]+1;
        }
        res[c[i]]++;
    }
    // for(int i=0;i<=n;i++){
    //     printf("%d ",c[i]);
    // }
    // printf("\n");
    // for(int i=0;i<=n;i++){
    //     printf("%d ",gre[i]);
    // }
    // printf("\n");
    long long ans=0;
    int tmp=0;
    for(int i=f;i<=n;i++){
        if(a[i]==a[f]){
            tmp+=0;
        }
        else if(a[i]>a[f]){
            tmp++;
        }
        else if(a[i]<a[f]){
            tmp--;
        }
        //printf("%d\n",tmp);
        ans+=res[-1*tmp];
        ans+=res[1-tmp];
    }
    printf("%lld\n",ans);
    return 0;
}
```






