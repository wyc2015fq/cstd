# Codeforces451A——Game With Sticks - westbrook1998的博客 - CSDN博客





2018年09月03日 22:14:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[博弈																[找规律](https://so.csdn.net/so/search/s.do?q=找规律&t=blog)](https://so.csdn.net/so/search/s.do?q=博弈&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
After winning gold and silver in IOI 2014, Akshat and Malvika want to have some fun. Now they are playing a game on a grid made of n horizontal and m vertical sticks. 

  An intersection point is any point on the grid which is formed by the intersection of one horizontal stick and one vertical stick. 

  In the grid shown below, n = 3 and m = 3. There are n + m = 6 sticks in total (horizontal sticks are shown in red and vertical sticks are shown in green). There are n·m = 9 intersection points, numbered from 1 to 9. 
![这里写图片描述](https://odzkskevi.qnssl.com/07b8a409270a6befc783cc0083f6b515?v=1535980837)

  The rules of the game are very simple. The players move in turns. Akshat won gold, so he makes the first move. During his/her move, a player must choose any remaining intersection point and remove from the grid all sticks which pass through this point. A player will lose the game if he/she cannot make a move (i.e. there are no intersection points remaining on the grid at his/her move). 

  Assume that both players play optimally. Who will win the game? 

  Input 

  The first line of input contains two space-separated integers, n and m (1 ≤ n, m ≤ 100). 

  Output 

  Print a single line containing “Akshat” or “Malvika” (without the quotes), depending on the winner of the game. 

  Examples 

  Input 

  2 2 

  Output 

  Malvika 

  Input 

  2 3 

  Output 

  Malvika 

  Input 

  3 3 

  Output 

  Akshat
博弈 一次选择一个交点删除交点的边，直到没得删就输 

递推找规律 

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
//设f(a,b)为a横b竖时先手的胜负情况，1为胜，0为负
//由题意可知f(a,b)=~f(a-1,b-1)  且f(0,b)=f(a,0)=0
//简单打表可知判断min(n,m)奇偶即可
int main(void){
    int n,m;
    scanf("%d%d",&n,&m);
    if(min(n,m)%2){
        printf("Akshat\n");
    }
    else{
        printf("Malvika\n");
    }
    return 0;
}
```






