# POJ  3258  River Hopscotch（二分） - HJ - CSDN博客
2017年02月02日 01:39:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：249
Description
Every year the cows hold an event featuring a peculiar version of hopscotch that involves carefully jumping from rock to rock in a river. The excitement takes place on a long, straight river with a rock at the start and another rock at the end, L units away from the start (1 ≤ L ≤ 1,000,000,000). Along the river between the starting and ending rocks, N (0 ≤ N ≤ 50,000) more rocks appear, each at an integral distance Di from the start (0 < Di < L).
To play the game, each cow in turn starts at the starting rock and tries to reach the finish at the ending rock, jumping only from rock to rock. Of course, less agile cows never make it to the final rock, ending up instead in the river.
Farmer John is proud of his cows and watches this event each year. But as time goes by, he tires of watching the timid cows of the other farmers limp across the short distances between rocks placed too closely together. He plans to remove several rocks in order to increase the shortest distance a cow will have to jump to reach the end. He knows he cannot remove the starting and ending rocks, but he calculates that he has enough resources to remove up to M rocks (0 ≤ M ≤ N).
FJ wants to know exactly how much he can increase the shortest distance *before* he starts removing the rocks. Help Farmer John determine the greatest possible shortest distance a cow has to jump after removing the optimal set of M rocks.
Input 
Line 1: Three space-separated integers: L, N, and M 
Lines 2..N+1: Each line contains a single integer indicating how far some rock is away from the starting rock. No two rocks share the same position.
Output 
Line 1: A single integer that is the maximum of the shortest distance a cow has to jump after removing M rocks
Sample Input
25 5 2 
2 
14 
11 
21 
17
Sample Output
4
Hint 
Before removing any rocks, the shortest jump was a jump of 2 from 0 (the start) to 2. After removing the rocks at 2 and 14, the shortest required jump is a jump of 4 (from 17 to 21 or from 21 to 25).
Source 
USACO 2006 December Silver
大致题意：
一条河长度为 L，河的起点(Start)和终点(End)分别有2块石头，S到E的距离就是L。
河中有n块石头，每块石头到S都有唯一的距离
问现在要移除m块石头（S和E除外），每次移除的是与当前最短距离相关联的石头，要求移除m块石头后，使得那时的最短距离尽可能大，输出那个最短距离。
解题思路：
经典的二分（要理解题目意思）
可以想象成从N块石头里挑出N-M块，使得最小间距最大。在实现上，需要贪心地选取比间距d稍微大那么一点（尽量小）的作为编号小的石头，所以需要排个序再去二分搜索。
```
#include <iostream>  
    #include <cstdio>  
    #include <algorithm>  
    using namespace std;  
    int L, N, M;  
    int d[50005];  
    bool cmp(int a, int b) {  
        return a < b;  
    }  
    int BSearch(int l, int h, int k) {  
        int m, last, cnt;  
        while (l <= h) {  
            m = (l + h) >> 1;  
            last = cnt = 0;  
            for (int i = 1; i <= N + 1; i++) {  
                if (m >= d[i] - d[last]) cnt++;  
                else last = i;  
            }  
            if (cnt > k) h = m - 1;  
            else l = m + 1;  
        }  
        return l;  
    }  
    int main()  
    {  
        scanf ("%d%d%d", &L, &N, &M);  
        d[0] = 0;  
        d[N+1] = L;  
        for (int i = 1; i <= N; i++)  
            scanf ("%d", &d[i]);  
        sort(d+1, d+1+N, cmp);  
        printf ("%d\n", BSearch(0, L, M));  
        return 0;  
    }
```
