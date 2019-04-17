# Codeforces1004C——Sonya and Robots - westbrook1998的博客 - CSDN博客





2018年08月09日 19:22:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：22标签：[后缀预处理](https://so.csdn.net/so/search/s.do?q=后缀预处理&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Since Sonya is interested in robotics too, she decided to construct robots that will read and recognize numbers. 

  Sonya has drawn n numbers in a row, ai is located in the i-th position. She also has put a robot at each end of the row (to the left of the first number and to the right of the last number). Sonya will give a number to each robot (they can be either same or different) and run them. When a robot is running, it is moving toward to another robot, reading numbers in the row. When a robot is reading a number that is equal to the number that was given to that robot, it will turn off and stay in the same position. 

  Sonya does not want robots to break, so she will give such numbers that robots will stop before they meet. That is, the girl wants them to stop at different positions so that the first robot is to the left of the second one. 

  For example, if the numbers [1,5,4,1,3] are written, and Sonya gives the number 1 to the first robot and the number 4 to the second one, the first robot will stop in the 1-st position while the second one in the 3-rd position. In that case, robots will not meet each other. As a result, robots will not be broken. But if Sonya gives the number 4 to the first robot and the number 5 to the second one, they will meet since the first robot will stop in the 3-rd position while the second one is in the 2-nd position. 

  Sonya understands that it does not make sense to give a number that is not written in the row because a robot will not find this number and will meet the other robot. 

  Sonya is now interested in finding the number of different pairs that she can give to robots so that they will not meet. In other words, she wants to know the number of pairs (p, q), where she will give p to the first robot and q to the second one. Pairs (pi, qi) and (pj, qj) are different if pi≠pj or qi≠qj. 

  Unfortunately, Sonya is busy fixing robots that broke after a failed launch. That is why she is asking you to find the number of pairs that she can give to robots so that they will not meet. 

  Input 

  The first line contains a single integer n (1≤n≤105) — the number of numbers in a row. 

  The second line contains n integers a1,a2,…,an (1≤ai≤105) — the numbers in a row. 

  Output 

  Print one number — the number of possible pairs that Sonya can give to robots so that they will not meet. 

  Examples 

  Input 

  5 

  1 5 4 1 3 

  Output 

  9 

  Input 

  7 

  1 2 1 1 1 3 2 

  Output 

  7 

  Note 

  In the first example, Sonya can give pairs (1, 1), (1, 3), (1, 4), (1, 5), (4, 1), (4, 3), (5, 1), (5, 3), and (5, 4). 

  In the second example, Sonya can give pairs (1, 1), (1, 2), (1, 3), (2, 1), (2, 2), (2, 3), and (3, 2).
这题是今天训练的最后一题 

有一个数列，左右两头各有一个机器人，机器人有一个数，然后相向走，走到和所给的数相同时即停下，然后求使得两个机器人不会碰到的数对个数 

首先暴力是过不了的，超时了。 

所以肯定要预处理一下，所以先一个循环计算出dp[]，即从i到最后这一段数中不同的数有多少个（即对结果有贡献的，仔细想想，确实是这样），然后就从左枚举区间左端点了，然后也要注意重复的去掉，然后区间不重叠（机器人不相撞），然后就能过了
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=100050;
//标记从左边开始枚举的数是否前面已出现过
bool vis[MAXN];
//记录从i到结束这一段有多少个不重复数字
int dp[MAXN];
//dp不重复数字时的标记
bool flag[MAXN];
int a[MAXN];
int main(void){
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    dp[n]=1;
    flag[a[n]]=true;
    for(int i=n-1;i>0;i--){
        if(!flag[a[i]]){
            flag[a[i]]=true;
            dp[i]=dp[i+1]+1;
        }
        else{
            dp[i]=dp[i+1];
        }
    }
    long long ans=0;
    for(int i=1;i<=n-1;i++){
        if(!vis[a[i]]){
            ans+=dp[i+1];
            vis[a[i]]=true;
        }
    }
    printf("%lld\n",ans);
    return 0;
}
```






