# hdu1016——Prime Ring Problem - westbrook1998的博客 - CSDN博客





2018年08月12日 19:46:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：25标签：[dfs																[搜索](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
A ring is compose of n circles as shown in diagram. Put natural number 1, 2, …, n into each circle separately, and the sum of numbers in two adjacent circles should be a prime.  

  Note: the number of first circle should always be 1.  
![这里写图片描述](https://odzkskevi.qnssl.com/427b1dd2282412709d583cc17eda983e?v=1533545405)

  Input 

  n (0 < n < 20).  

  Output 

  The output format is shown as sample below. Each row represents a series of circle numbers in the ring beginning from 1 clockwisely and anticlockwisely. The order of numbers must satisfy the above requirements. Print solutions in lexicographical order.  

  You are to write a program that completes above process.  

  Print a blank line after each case.  

  Sample Input 

  6 

  8 

  Sample Output 

  Case 1: 

  1 4 3 2 5 6 

  1 6 5 2 3 4 

  Case 2: 

  1 2 3 8 5 6 7 4 

  1 2 5 8 3 4 7 6 

  1 4 7 6 5 8 3 2 

  1 6 7 4 3 8 5 2
这题也是很经典的搜索问题，要求一个序列1-n 使得环相邻两个数和为素数 

数据很小，素数先打表，然后dfs记录在搜索的是哪个数以及是第几个数，然后递归到最底层当n==num时直接输出就行，一开始想这个保存路径想了很久一直没找到方法，原来直接输出就好了，然后还要注意是环，最后一个数和1也是相邻的，这里也要处理一下，我是用一个数组记录

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
int n;
bool prime[40];
bool vis[25];
int res[25];
int a[25];
void init(){
    prime[2]=prime[3]=prime[5]=prime[7]=prime[11]=prime[13]=prime[17]=prime[19]=prime[23]=prime[29]=prime[31]=prime[37]=true;
    res[0]=1;
    for(int i=1;i<=n;i++){
        a[i]=i;
    }
    a[n+1]=1;
}
//搜到第几个数
void dfs(int t,int num){
    //要考虑最后一个数和1
    if(num==n && prime[t+1]){
        for(int i=0;i<n-1;i++){
            printf("%d ",res[i]);
        }
        printf("%d\n",res[n-1]);
    }
    vis[t]=true;
    for(int i=1;i<=n+1;i++){
        if(vis[a[i]]){
            continue;
        }
        else{
            if(prime[a[i]+t]){
                res[num++]=a[i];
                dfs(a[i],num);
                vis[a[i]]=false;
                num--;
            }
        }
    }
}
int main(void){
    int c=1;
    while(~scanf("%d",&n)){
        init();
        printf("Case %d:\n",c++);
        dfs(1,1);
        printf("\n");
    }
    return 0;
}
```






