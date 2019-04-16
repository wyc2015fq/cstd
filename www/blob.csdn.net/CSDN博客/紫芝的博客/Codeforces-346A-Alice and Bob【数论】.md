# Codeforces-346A-Alice and Bob【数论】 - 紫芝的博客 - CSDN博客





2018年07月27日 17:33:03[紫芝](https://me.csdn.net/qq_40507857)阅读数：61








# A. Alice and Bob

time limit per test 2 seconds

memory limit per test  256 megabytes

input

standard input

output

standard output

It is so boring in the summer holiday, isn't it? So Alice and Bob have invented a new game to play. The rules are as follows. First, they get a set of *n* distinct integers. And then they take turns to make the following moves. During each move, either Alice or Bob (the player whose turn is the current) can choose two distinct integers *x* and *y* from the set, such that the set doesn't contain their absolute difference |*x* - *y*|. Then this player adds integer |*x* - *y*| to the set (so, the size of the set increases by one).

If the current player has no valid move, he (or she) loses the game. The question is who will finally win the game if both players play optimally. Remember that Alice always moves first.

## Input

The first line contains an integer *n* (2 ≤ *n* ≤ 100) — the initial number of elements in the set. The second line contains *n* distinct space-separated integers *a*1, *a*2, ..., *a**n* (1 ≤ *a**i* ≤ 109) — the elements of the set.

## Output

Print a single line with the winner's name. If Alice wins print "Alice", otherwise print "Bob" (without quotes).

Examples

## input

```
2
2 3
```

## output

```
Alice
```

## input

```
2
5 3
```

## output

```
Alice
```

## input

```
3
5 6 7
```

## output

```
Bob
```

Note

Consider the first test sample. Alice moves first, and the only move she can do is to choose 2 and 3, then to add 1 to the set. Next Bob moves, there is no valid move anymore, so the winner is Alice.

### 题意

A和B两个人进行游戏，每人轮流操作，最先无法进行操作的一方输。

> 
1.从这个序列中取两个数字

2.这两个数字的差值的绝对值temp，原先并不存在在序列中 

3.将temp放进序列


### 题目思路：

游戏结束的标志是无法取出两个数字，他们的差值不在序列中。也就是说，最终状态是一个首项等于公差的等差序列。

求出这个等差数列的项数-n，就是游戏进行的回合。所以我们要先求出首项。设首项为d，接下来就是d+d,d+2d….

后面几项都是首项的倍数，所以我们可以用gcd（a1,a2,a3…an）求出。ans = an / gcd(a1,a2..an) - n;

```cpp
#include <cstdio>
#include <iostream>
#include<set>
#include<bits/stdc++.h>
using namespace std;
int a[109];
int main()
{
        int n,x;
        cin>>n;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
        }
        sort(a,a+n);
        int g=a[0];
        for(int i=1;i<n;i++)
            g=__gcd(a[i],g);
        int ans=a[n-1]/g-n;
        if(ans&1)  cout<<"Alice\n";
        else cout<<"Bob\n";
    return 0;
}
```





