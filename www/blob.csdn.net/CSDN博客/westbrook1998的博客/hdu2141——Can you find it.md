# hdu2141——Can you find it? - westbrook1998的博客 - CSDN博客





2018年05月26日 16:27:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：49标签：[二分查找](https://so.csdn.net/so/search/s.do?q=二分查找&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Give you three sequences of numbers A, B, C, then we give you a number X. Now you need to calculate if you can find the three numbers Ai, Bj, Ck, which satisfy the formula Ai+Bj+Ck = X.  

  Input 

  There are many cases. Every data case is described as followed: In the first line there are three integers L, N, M, in the second line there are L integers represent the sequence A, in the third line there are N integers represent the sequences B, in the forth line there are M integers represent the sequence C. In the fifth line there is an integer S represents there are S integers X to be calculated. 1<=L, N, M<=500, 1<=S<=1000. all the integers are 32-integers.  

  Output 

  For each case, firstly you have to print the case number as the form “Case d:”, then for the S queries, you calculate if the formula can be satisfied or not. If satisfied, you print “YES”, otherwise print “NO”.  

  Sample Input 

  3 3 3 

  1 2 3 

  1 2 3 

  1 2 3 

  3 

  1 

  4 

  10
在三个数列中各选一个数，判断是否相加等于给定的数，先将a b两个数列加起来，再枚举g-c[i]二分判断是否在ab[]中，这里几个坑点，首先ab数组要开MAXN*MAXN，然后二分判断也要注意一些细节

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=505;
int a[MAXN],b[MAXN],c[MAXN],ab[MAXN*MAXN];
int k=0;
int l,n,m;
bool check(int x){
    int l=0,r=k;
    int m;
    while(l<=r){
        m=(l+r)/2;
        if(x==ab[m]){
            return true;
        }
        else if(x>ab[m]){
            l=m+1;
        }
        else{
            r=m-1;
        }
    }
    return false;
}
int main(void){
    int cas=1;
    while(~scanf("%d%d%d",&l,&n,&m)){
        for(int i=0;i<l;i++){
            scanf("%d",&a[i]);
        }
        for(int i=0;i<n;i++){
            scanf("%d",&b[i]);
        }
        for(int i=0;i<m;i++){
            scanf("%d",&c[i]);
        }
        k=0;
        for(int i=0;i<l;i++){
            for(int j=0;j<n;j++){
                ab[k++]=a[i]+b[j];
            }
        }
        sort(ab,ab+k);
        int q;
        int g;
        scanf("%d",&q);
        printf("Case %d:\n",cas++);
        while(q--){
            scanf("%d",&g);
            int flag=0;
            for(int i=0;i<m;i++){
                if(check(g-c[i])){
                    flag=1;
                    break;
                }
            }
            if(flag){
                printf("YES\n");
            }
            else{
                printf("NO\n");
            }
        }
    }
    return 0;
}
```





