# Codeforces998B——Cutting - westbrook1998的博客 - CSDN博客





2018年08月28日 19:08:27[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
There are a lot of things which could be cut — trees, paper, “the rope”. In this problem you are going to cut a sequence of integers. 

  There is a sequence of integers, which contains the equal number of even and odd numbers. Given a limited budget, you need to make maximum possible number of cuts such that each resulting segment will have the same number of odd and even integers. 

  Cuts separate a sequence to continuous (contiguous) segments. You may think about each cut as a break between two adjacent elements in a sequence. So after cutting each element belongs to exactly one segment. Say, [4,1,2,3,4,5,4,4,5,5] → two cuts → [4,1|2,3,4,5|4,4,5,5]. On each segment the number of even elements should be equal to the number of odd elements. 

  The cost of the cut between x and y numbers is |x−y| bitcoins. Find the maximum possible number of cuts that can be made while spending no more than B bitcoins. 

  Inut 

  First line of the input contains an integer n (2≤n≤100) and an integer B (1≤B≤100) — the number of elements in the sequence and the number of bitcoins you have. 

  Second line contains n integers: a1, a2, …, an (1≤ai≤100) — elements of the sequence, which contains the equal number of even and odd numbers 

  Output 

  Print the maximum possible number of cuts which can be made while spending no more than B bitcoins. 

  Examples 

  Input 

  6 4 

  1 2 5 10 15 20 

  Output 

  1 

  Input 

  4 10 

  1 3 2 4 

  Output 

  0 

  Input 

  6 100 

  1 2 3 4 5 6 

  Output 

  2 

  Note 

  In the first sample the optimal answer is to split sequence between 2 and 5. Price of this cut is equal to 3 bitcoins. 

  In the second sample it is not possible to make even one cut even with unlimited number of bitcoins. 

  In the third sample the sequence should be cut between 2 and 3, and between 4 and 5. The total price of the cuts is 1+1=2 bitcoins.
给一个数列和一个花费的上限  然后切后要保证字数列满足奇数和偶数个数相同 

我的思路是先遍历一遍找出所有可以切的点同时记录花费（注意绝对值），然后排序，然后就贪心从小的取 凑到上限为止

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
int a[105];
int x[105];
int main(void){
    int n,b;
    scanf("%d%d",&n,&b);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    int k=0;
    int ans=0;
    //奇数1 偶数-1
    int p=0;
    for(int i=0;i<n-1;i++){
        if(a[i]%2){
            p+=1;
        }
        else{
            p-=1;
        }
        if(p==0){
            x[k++]=abs(a[i+1]-a[i]);
        }
    }
    sort(x,x+k);
    // for(int i=0;i<k;i++){
    //     printf("%d ",x[i]);
    // }
    // printf("\n");
    int i=0;
    for(;i<k;i++){
        ans+=x[i];
        if(ans>b){
            break;
        }
    }
    printf("%d\n",i);
    return 0;
}
```





