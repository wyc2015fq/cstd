# 腾讯校招笔试题之贪吃的小Q - curryche的博客 - CSDN博客





2018年07月16日 21:00:08[curryche](https://me.csdn.net/whwan11)阅读数：1532








## 题目

来源：牛客网 

链接： 
[https://www.nowcoder.com/questionTerminal/d732267e73ce4918b61d9e3d0ddd9182](https://www.nowcoder.com/questionTerminal/d732267e73ce4918b61d9e3d0ddd9182)
小Q的父母要出差N天，走之前给小Q留下了M块巧克力。小Q决定每天吃的巧克力数量不少于前一天吃的一半，但是他又不想在父母回来之前的某一天没有巧克力吃，请问他第一天最多能吃多少块巧克力  

输入描述: 

每个输入包含一个测试用例。 

每个测试用例的第一行包含两个正整数，表示父母出差的天数N(N<=50000)和巧克力的数量M(N<=M<=100000)。
输出描述: 

输出一个数表示小Q第一天最多能吃多少块巧克力。 

示例1 

输入 

3 7 

输出 

4
## 解答

思路 

1. 因为每天至少要吃1块巧克力，所以第1天最多吃M-(N-1)块巧克力 

2. 从M-(N-1)到1，计算该吃法是否满足条件，满足条件即跳出循环
代码

```cpp
#include<iostream>
#include<stdlib.h>
#include <math.h>
using namespace std;



int main()
{
    int N, M, i, j, sum_eat, current_eat, last_eat;

    cin >> N >> M;

    for ( i = M - N + 1; i >=1; i--)
    {
        sum_eat = i;
        j = 1;
        last_eat = i;

        while (j<N)
        {
            if (last_eat <= 2)
            {
                current_eat = 1;
            }
            else
            {
                current_eat = last_eat / 2 + (last_eat % 2);
            }

            sum_eat += current_eat;
            if (sum_eat > M)
            {
                break;
            }

            last_eat = current_eat;
            j++;
        }
        if (j == N)
        {
            break;
        }
    }
    cout << i << endl;

    system("pause");
    return 0;
}
```







