# Codeforces999A——Mishka and Contest - westbrook1998的博客 - CSDN博客





2018年08月10日 17:42:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Mishka started participating in a programming contest. There are n problems in the contest. Mishka’s problem-solving skill is equal to k. 

  Mishka arranges all problems from the contest into a list. Because of his weird principles, Mishka only solves problems from one of the ends of the list. Every time, he chooses which end (left or right) he will solve the next problem from. Thus, each problem Mishka solves is either the leftmost or the rightmost problem in the list. 

  Mishka cannot solve a problem with difficulty greater than k. When Mishka solves the problem, it disappears from the list, so the length of the list decreases by 1. Mishka stops when he is unable to solve any problem from any end of the list. 

  How many problems can Mishka solve? 

  Input 

  The first line of input contains two integers n and k (1≤n,k≤100) — the number of problems in the contest and Mishka’s problem-solving skill. 

  The second line of input contains n integers a1,a2,…,an (1≤ai≤100), where ai is the difficulty of the i-th problem. The problems are given in order from the leftmost to the rightmost in the list. 

  Output 

  Print one integer — the maximum number of problems Mishka can solve. 

  Examples 

  Input 

  8 4 

  4 2 3 1 5 1 6 4 

  Output 

  5 

  Input 

  5 2 

  3 1 2 1 3 

  Output 

  0 

  Input 

  5 100 

  12 34 55 43 21 

  Output 

  5 

  Note 

  In the first example, Mishka can solve problems in the following order: [4,2,3,1,5,1,6,4]→[2,3,1,5,1,6,4]→[2,3,1,5,1,6]→[3,1,5,1,6]→[1,5,1,6]→[5,1,6], so the number of solved problems will be equal to 5. 

  In the second example, Mishka can’t solve any problem because the difficulties of problems from both ends are greater than k. 

  In the third example, Mishka’s solving skill is so amazing that he can solve all the problems.
今天的题不是很顺利 

一整个下午没过题

这题我的思路就是从左到右和从右到左分别循环一次，注意从左到右的时候要记录下最后一个位置，然后从右到左就到这个位置就可以

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=105;
int a[MAXN];
int n,k;
int main(void){
    scanf("%d%d",&n,&k);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    int ans=0;
    int flag=0;
    for(int i=0;i<n;i++){
        if(a[i]<=k){
            ans++;
            flag=i;
        }
        else{
            break;
        }
    }
    for(int i=n-1;i>flag;i--){
        if(a[i]<=k){
            ans++;
        }
        else{
            break;
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





