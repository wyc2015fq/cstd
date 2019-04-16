# HDU 5534 Partial Tree 完全背包 - 紫芝的博客 - CSDN博客





2018年10月12日 15:55:36[紫芝](https://me.csdn.net/qq_40507857)阅读数：17








In mathematics, and more specifically in graph theory, a tree is an undirected graph in which any two nodes are connected by exactly one path. In other words, any connected graph without simple cycles is a tree. 


You find a partial tree on the way home. This tree has nn nodes but lacks of n−1n−1edges. You want to complete this tree by adding n−1n−1 edges. There must be exactly one path between any two nodes after adding. As you know, there are nn−2nn−2 ways to complete this tree, and you want to make the completed tree as cool as possible. The coolness of a tree is the sum of coolness of its nodes. The coolness of a node is f(d)f(d), where ff is a predefined function and dd is the degree of this node. What's the maximum coolness of the completed tree?

Input

The first line contains an integer TT indicating the total number of test cases. 

Each test case starts with an integer nn in one line, 

then one line with n−1n−1 integers f(1),f(2),…,f(n−1)f(1),f(2),…,f(n−1). 


1≤T≤2015

2≤n≤2015

0≤f(i)≤10000



There are at most 1010 test cases with n>100

Output

For each test case, please output the maximum coolness of the completed tree in one line.

Sample Input

```
2
3
2 1
4
5 1 4
```

Sample Output

```
5
19
```

### 分析：

**因为n个点构成一棵树，有n-1条边，一共是2*n-2个度，而且确定的一点是每一个点都至少有1个度。**

**这样还剩下n-2个度。问题就转化成了n-2个度分给n个点（每个点拥有的度数可以是0）。**

**但是题目给了n-1个f(i)意思就是说，有n-1种物品，每一种物品的数量都是无限的（因为每个点的度数是可以相同的），并且每件物品的价值都是v【i】=1，并且每件物品的体积（重量）都是i（就是这个物品的度数），然后把这n-1种物品，正好装入一个容量为n-2的背包里，即完全背包问题**

**[完全背包模板](https://blog.csdn.net/qq_40507857/article/details/81217507)**

```cpp
//完全背包
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e4+10;
int n;//n种物品
int v;//背包容量
int c[maxn];
int w[maxn];
int dp[maxn];
int main()
{
    while(scanf("%d%d",&v,&n)!=EOF){
        for(int i=0;i<n;i++)
            scanf("%d",&c[i]);//每个物品的体积
 
        for(int i=0;i<n;i++)
            scanf("%d",&w[i]);//每物品的价值
 
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)//枚举所有的物品
        for(int j=c[i];j<=v;j++)//枚举背包容量
        dp[j]=max(dp[j],dp[j-c[i]]+w[i]);//状态转移方程
        printf("%d\n",dp[v]);
    }
    return 0;
}
```

**但是这道题还有个坑，就是我们默认了每个点都有了一个度了，所以实际上我们在枚举物品的时候，他们的度都是i+1（假设从1开始循环）**

```java
import java.math.*;
import java.util.*;
public class Main {
	static int maxn=2020;
	static int INF=(int)1e9;
	static int n,v;
	static int[] f=new int[maxn];
	static int[] dp=new int[maxn];
	
	static void DP() {
		for(int i=1;i<=n;i++)
			dp[i]=-INF;//求恰好装满背包时的最优解，初始化为—INF
		dp[0]=0;//背包容量为0，价值为0
		 v=n-2;//背包容量

		 for(int i=1;i<=n-1;i++)//枚举物品
			 for(int j=i;j<=v;j++)//枚举背包容量
				 dp[j]=Math.max(dp[j], dp[j-i]+f[i+1]-f[1]);//状态转移方程
	}
    public static void main (String[] args) {
    	 Scanner cin=new Scanner(System.in);
    	 int T=cin.nextInt();
    	 while((T--)!=0) {
    		 n=cin.nextInt();
    		 for(int i=1;i<=n-1;i++)
    			 f[i]=cin.nextInt();
    		 DP();
    		 System.out.println(dp[v]+f[1]*n);
    	 }
    	 cin.close();
      }
    }
```





