# codeforce——432A - Choosing Teams - westbrook1998的博客 - CSDN博客





2018年04月25日 21:41:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：70标签：[codeforces																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[codeforce](https://blog.csdn.net/westbrook1998/article/category/7608645)








看不懂题意是硬伤……

> 
The Saratov State University Olympiad Programmers Training Center (SSU OPTC) has n students. For each student you know the number of times he/she has participated in the ACM ICPC world programming championship. According to the ACM ICPC rules, each person can participate in the world championship at most 5 times. 

  The head of the SSU OPTC is recently gathering teams to participate in the world championship. Each team must consist of exactly three people, at that, any person cannot be a member of two or more teams. What maximum number of teams can the head make if he wants each team to participate in the world championship with the same members at least k times? 

  Input 

  The first line contains two integers, n and k (1 ≤ n ≤ 2000; 1 ≤ k ≤ 5). The next line contains n integers: y1, y2, …, yn (0 ≤ yi ≤ 5), where yi shows the number of times the i-th person participated in the ACM ICPC world championship. 

  Output 

  Print a single number — the answer to the problem. 

  Examples 

  inputCopy 

  5 2 

  0 4 5 1 0 

  outputCopy 

  1 

  inputCopy 

  6 4 

  0 1 2 3 4 5 

  outputCopy 

  0 

  inputCopy 

  6 5 

  0 0 0 0 0 0 

  outputCopy 

  2
百度了才了解了题意…就是在一列数中选择参加比赛次数加上k仍然小于5次的选手，每三人一队 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn=101;
int main(void){
    int n,k;
    while(scanf("%d%d",&n,&k)!=EOF){
        int c=0;
        for(int i=0;i<n;i++){
            int a;
            scanf("%d",&a);
            if(a+k<=5){
                c++;
            }
        }
        printf("%d\n",c/3);
    }
}
```






