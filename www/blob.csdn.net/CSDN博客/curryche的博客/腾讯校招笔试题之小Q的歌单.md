# 腾讯校招笔试题之小Q的歌单 - curryche的博客 - CSDN博客





2018年07月17日 17:49:19[curryche](https://me.csdn.net/whwan11)阅读数：1933








## 题目

来源：牛客网 

链接： 
[https://www.nowcoder.com/questionTerminal/f3ab6fe72af34b71a2fd1d83304cbbb3](https://www.nowcoder.com/questionTerminal/f3ab6fe72af34b71a2fd1d83304cbbb3)
小Q的歌单 

热度指数：1624   时间限制：1秒 空间限制：32768K

小Q有X首长度为A的不同的歌和Y首长度为B的不同的歌，现在小Q想用这些歌组成一个总长度正好为K的歌单，每首歌最多只能在歌单中出现一次，在不考虑歌单内歌曲的先后顺序的情况下，请问有多少种组成歌单的方法。  

输入描述: 

每个输入包含一个测试用例。 

每个测试用例的第一行包含一个整数，表示歌单的总长度K(1<=K<=1000)。 

接下来的一行包含四个正整数，分别表示歌的第一种长度A(A<=10)和数量X(X<=100)以及歌的第二种长度B(B<=10)和数量Y(Y<=100)。保证A不等于B。
输出描述: 

输出一个整数,表示组成歌单的方法取模。因为答案可能会很大,输出对1000000007取模的结果。 

示例1 

输入 

5 

2 3 3 3 

输出 

9
## 解答

一开始想用暴力求解来做，提交之后发现运行时间超过限制，只通过了60%的test cases。后来想到了动态规划，这应该是一道很典型的动态规划题目，具体思路如下。

思路 

1. 用数组dp来存储结果，dp[i][j]表示用前j首歌表示长度为i的歌单的方法数，i=1…K，j=1,…,na+nb。na是第1种歌的数量，nb是第2种歌的数量 

2. lens[j]表示第j首歌的长度，如果i>=lens[j]，那么由前j首歌组成长度为i的歌单的方法数可分为两部分，第一部分是dp[i][j-1]，即由前j-1首歌组成长度为i的歌单的方法数，第二部分是dp[i-lens[j]][j-1]，即由j-1首歌组成长度为i-lens[j]的歌单的方法数，因为第j首歌已经占据了lens[j]的长度。 

3. 如果$i<lens[j]$，那么dp$[i][j]$等于dp$[i] [j-1]$。
代码

```cpp
#include<iostream>
#include<stdlib.h>
#include <math.h>
using namespace std;

int main()
{
    int i, j, K, la, lb, na, nb,result;
    cin >> K;
    cin >> la >> na >> lb >> nb;
    if (K <= 0 || la <= 0 || na <= 0 || lb <= 0 || nb <= 0)
    {
        cout << "error!";
        return 0;
    }

    int* lens = new int[na+nb+1];
    for (i = 1; i <= na; i++)
    {
        lens[i] = la;
    }
    for (i = 1; i <= nb; i++)
    {
        lens[i+na] = lb;
    }

    int** dp= new int*[K+1];
    for (i = 0; i < K + 1; i++)
    {
        dp[i] = new int[na + nb + 1];
    }

    for (j = 0; j < na + nb + 1; j++)
    {
        dp[0][j] = 1;
    }

    for (i = 1; i <= K ; i++)
    {
        if (lens[1] != i)
        {
            dp[i][1] = 0;
        }
        else
        {
            dp[i][1] = 1;
        }
    }

    for (i = 1; i <= K; i++)
    {
        for (j = 2; j < na + nb + 1; j++)
        {
            if (i >= lens[j])
            {
                dp[i][j] = (dp[i][j-1]+dp[i-lens[j]][j-1]) % 1000000007;
                //dp[i][j] = (dp[i][j - 1] + dp[i - lens[j]][j - 1]);
            }
            else
            {
                dp[i][j] = dp[i][j - 1] % 1000000007;
            }
        }
    }

    cout << dp[K][na+nb] << endl;

    system("pause");
    return 0;
}
```







