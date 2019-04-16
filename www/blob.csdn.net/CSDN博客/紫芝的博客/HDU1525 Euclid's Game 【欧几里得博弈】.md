# HDU1525 Euclid's Game 【欧几里得博弈】 - 紫芝的博客 - CSDN博客





2018年11月06日 12:27:46[紫芝](https://me.csdn.net/qq_40507857)阅读数：776








有两个玩家,Stan 和 Ollie, 在玩游戏。初始有两个自然数。Stan是先手，每次把大的数字减去小的数字的任意倍数，但是不能使数字变成负数。然后Ollie进行同样的操作，直到有一个玩家使一个数字变为零。

例如，初始时数字为(25,7):

25 7

11 7

4 7

4 3

1 3

1 0

这样Stan赢

Input

输入数据包含多行，每行两个正整数

Output

对于每组数据，加入先手赢，输出Stan wins,否则输出Ollie wins。最后一行为0 0，这一行不要处理

Sample Input

34 12
15 24
0 0
Sample Output

Stan wins
Ollie wins

**题意： **

两个数，两人轮流操作，大数可以减去小数的整数倍（不能为负），先到0者为赢。

对某个状态(a, b) a >= b 

最简单的情况，如果a%b == 0, 则先手必赢。 

a > 2*b时，也是先手必赢，因为(a%b+b, b)状态必定与(a%b, b)相反，(a, b)可以转移到其中的必败态。 

a < 2*b时，只能转移到(a-b, b)，状态与其相反， 继续向下转移即可获知当前状态。

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[])
{
    int a, b;
    while(scanf("%d%d", &a, &b), a&&b)
    {
        if(a < b)
            swap(a, b);
        bool win = true;
        while(1)
        {
            if(a % b == 0 || a >= b << 1)
                break;
            a -= b;
            swap(a, b);
            win = !win;
        }
        if(win)
            puts("Stan wins");
        else
            puts("Ollie wins");
    }
    return 0;
}
```





