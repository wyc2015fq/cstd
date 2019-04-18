# LeetCode Unique Binary Search Trees （DP） - xcw0754 - 博客园
# [LeetCode Unique Binary Search Trees （DP）](https://www.cnblogs.com/xcw0754/p/4921618.html)
题意：
　　一棵BST有n个节点，每个节点的key刚好为1～n。问此树有多少种不同形态？
思路：
　　提示是动态规划。
　　考虑一颗有n个节点的BST和有n-1个节点的BST。从n-1到n只是增加了一个点n，那么点n可以放的地方并不多，而且有一些规律。由于n是最大的，所以必定是在最右边，但是它的上面和下面也可以有一些点，假设点n的上面有k个点，下面则为n-k-1个。观察到点n的上面的点必定是[1, n-k-1]，下面的点是[n-k, n-1]，而点数<n的BST的形态数都已经求出，那么枚举这个k值就行了。
```
1 class Solution {
 2 public:
 3     int numTrees(int n)
 4     {
 5         vector<int> que(2,1);
 6         for(int i=2; i<=n; i++)
 7         {
 8             int sum=0;
 9             for(int j=0; j<i; j++)
10                 sum+=que[j]*que[i-j-1];    
11             que.push_back(sum);
12         }
13         return que[n];
14     }
15 };
AC代码
```

