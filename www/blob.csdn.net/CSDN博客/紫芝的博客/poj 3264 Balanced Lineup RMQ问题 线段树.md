# poj 3264 Balanced Lineup RMQ问题 线段树 - 紫芝的博客 - CSDN博客





2018年04月29日 20:27:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：40
个人分类：[线段树](https://blog.csdn.net/qq_40507857/article/category/7569707)









For the daily milking, Farmer John's *N* cows (1 ≤ *N* ≤ 50,000) always line up in the same order. One day Farmer John decides to organize a game of Ultimate Frisbee with some of the cows. To keep things simple, he will take a contiguous range of cows from the milking lineup to play the game. However, for all the cows to have fun they should not differ too much in height.

Farmer John has made a list of *Q* (1 ≤ *Q* ≤ 200,000) potential groups of cows and their heights (1 ≤ *height* ≤ 1,000,000). For each group, he wants your help to determine the difference in height between the shortest and the tallest cow in the group.

Input

Line 1: Two space-separated integers, *N* and *Q*. 

Lines 2.. *N*+1: Line *i*+1 contains a single integer that is the height of cow *i*

Lines *N*+2.. *N*+ *Q*+1: Two integers *A* and *B* (1 ≤ *A* ≤ *B* ≤ *N*), representing the range of cows from *A* to *B*inclusive.

Output

Lines 1.. *Q*: Each line contains a single integer that is a response to a reply and indicates the difference in height between the tallest and shortest cow in the range.

Sample Input

6 3
1
7
3
4
2
5
1 5
4 6
2 2
Sample Output

6
3
0

# RMQ（Range Minimum/Maximum Query），即区间最值查询

**RMQ是指这样一个问题：对于长度为n的数列A，回答若干次询问RMQ(i,j)，返回数列A中下标在区间[i,j]中的最小/大值。**

根据询问，修改，单点，区间，在线，离线的各种组合大概有8类问题

一般就是4种解决办法吧。

**1、线段树**

**2、树状数组**

**3、ST表**

**4、差分**

本文介绍一种比较高效的ST算法解决这个问题。

**ST（Sparse Table）算法可以在O(nlogn)时间内进行预处理，然后在O(1)时间内回答每个查询。**

**1）预处理**

设A[i]是要求区间最值的数列，F[i, j]表示从第i个数起连续2^j个数中的最大值。（DP的状态）

例如：

A数列为：3 2 4 5 6 8 1 2 9 7

F[1，0]表示第1个数起，长度为2^0=1的最大值，其实就是3这个数。

同理 F[1,1] = max(3,2) = 3, F[1，2]=max(3,2,4,5) = 5，F[1，3] = max(3,2,4,5,6,8,1,2) = 8;

并且我们可以容易的看出F[i,0]就等于A[i]。（DP的初始值）

我们把F[i，j]平均分成两段（因为F[i，j]一定是偶数个数字），从 i 到i + 2 ^ (j - 1) - 1为一段，i + 2 ^ (j - 1)到i + 2 ^ j - 1为一段(长度都为2 ^ (j - 1))。

**于是我们得到了状态转移方程：**

### F[i, j]=max（F[i，j-1], F[i + 2^(j-1)，j-1]）

**2)查询**

假如我们需要查询的区间为(i,j)，那么我们需要找到覆盖这个闭区间 **(左边界取i，右边界取j) **的最小幂（可以重复，比如查询1，2，3，4，5，我们可以查询1234和2345）。

因为这个区间的长度为j - i + 1,所以我们可以取k=log2( j - i + 1)，则有：**RMQ(i, j)=max{F[i , k], F[ j - 2 ^ k + 1, k]}**。

举例说明，要求区间[1，5]的最大值，k = log2（5 - 1 + 1）= 2，即求 **max(F[1, 2]，F[5 - 2 ^ 2 + 1, 2])=max(F[1, 2]，F[2, 2])**；

```cpp
void ST(int n) {
    for (int i = 1; i <= n; i++)
        dp[i][0] = A[i];
    for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 1; i + (1 << j) - 1 <= n; i++) {
            dp[i][j] = max(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
        }
    }
}
int RMQ(int l, int r) {
    int k = 0;
    while ((1 << (k + 1)) <= r - l + 1) k++;
    return max(dp[l][k], dp[r - (1 << k) + 1][k]);
}
```

## poj3264

## RMQ问题 ST算法 

```cpp
/*
poj3264 Balanced Lineup 
*/
#include <cstdio>  
#include <cmath>  
#include<algorithm>
#define MAXN (50000 + 10)  
using namespace std;
int cows[MAXN];  
int st_max[MAXN][20];  
int st_min[MAXN][20];  
int n, q, l, r;  
  
void initst_max()  
{  
    for (int i = 0; i < n; i++) st_max[i][0] = cows[i];  
      
    for (int j = 1; (1<<j) < n; ++j) {  
        for (int i = 0; i + (1<<j) <= n; ++i) {  
            st_max[i][j] = max(st_max[i][j-1], st_max[i+(1<<(j-1))][j-1]);  
        }  
    }  
}  
  
void initst_min()  
{  
    for (int i = 0; i < n; i++) st_min[i][0] = cows[i];  
      
    for (int j = 1; (1<<j) < n; ++j) {  
        for (int i = 0; i + (1<<j) <= n; ++i) {  
            st_min[i][j] = min(st_min[i][j-1], st_min[i+(1<<(j-1))][j-1]);  
        }  
    }  
}  
  
int queryst_max(int l, int r)  
{  
    int k = (int)(log(r-l+1.0)/log(2.0));  
    return max(st_max[l][k], st_max[r-(1<<k)+1][k]);  
}  
  
int queryst_min(int l, int r)  
{  
    int k = (int)(log(r-l+1.0)/log(2.0));  
    return min(st_min[l][k], st_min[r-(1<<k)+1][k]);  
}  
  
int main()  
{  
    while (scanf("%d %d", &n, &q) != EOF) {  
        for (int i = 0; i < n; ++i) scanf("%d", &cows[i]);  
        initst_max();  
        initst_min();  
        while (q--) {  
            scanf("%d %d", &l, &r);  
            if (l == r) printf("0\n");  
            else {  
                int a = queryst_max(l-1, r-1);  
                int b = queryst_min(l-1, r-1);  
                printf("%d\n", a - b);  
            }  
        }  
    }  
    return 0;  
}
```

线段树C++

```cpp
#include <iostream>
#include <cstdio>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn = (5e4+10);

struct node{
	int l,r,max,min;
};

int a[maxn];
node tree[maxn<<2];

static int max(int a,int b) {
    return a>b?a:b;
}

static int min(int a,int b) {
    return a<b?a:b;
}

static void pushup(int rt) {
    tree[rt].max=max(tree[rt<<1].max, tree[rt<<1|1].max);
    tree[rt].min=min(tree[rt<<1].min, tree[rt<<1|1].min);
}
static void build(int rt,int l,int r) {
    tree[rt].l=l;
    tree[rt].r=r;
    if(l==r) {
        tree[rt].max=tree[rt].min=a[l];
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    pushup(rt);
}
static int queryMax(int rt,int l,int r) {
    if(tree[rt].l==l&&tree[rt].r==r) {
        return tree[rt].max;
    }
    int mid=(tree[rt].l+tree[rt].r)>>1;
    if(r<=mid)
        return queryMax(rt<<1,l,r);
    else if(l>mid)
        return queryMax(rt<<1|1,l,r);
    else
        return max(queryMax(rt<<1,l,mid),queryMax(rt<<1|1,mid+1,r));
}
static int queryMin(int rt,int l,int r) {
    if(tree[rt].l==l&&tree[rt].r==r) {
        return tree[rt].min;
    }
    int mid=(tree[rt].l+tree[rt].r)>>1;
    if(r<=mid)
        return queryMin(rt<<1,l,r);
    else if(l>mid)
        return queryMin(rt<<1|1,l,r);
    else
        return min(queryMin(rt<<1,l,mid),queryMin(rt<<1|1,mid+1,r));
}

int main() {
int q,n;
    while(scanf("%d%d",&n,&q)!=EOF) {

        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        build(1,1,n);
        while((q--)!=0) {
            int l;
            int r;
            scanf("%d%d",&l,&r);
            printf("%d\n",queryMax(1,l,r)-queryMin(1,l,r));
        }
    }

}
```

线段树单点查询，Java

```cpp
import java.util.*;
import java.math.*;
class node{
	int l,r,max,min;
	node(int l,int r){
		this.l=0;
		this.r=0;
		this.max=0;
		this.min=0;
	}
}

public class Main{
	static int maxn=(int)(5e4+10);
	static int[] a=new int[maxn];
	static node[] tree=new node[maxn<<2];
	
	static void init() {
		for(int i=0;i<(maxn<<2);i++) {
			tree[i]=new node(0,0);
		}
	}
	static int max(int a,int b) {
		return a>b?a:b;
	}
	
	static int min(int a,int b) {
		return a<b?a:b;
	}
	
	static void pushup(int rt) {
		tree[rt].max=max(tree[rt<<1].max, tree[rt<<1|1].max);
		tree[rt].min=min(tree[rt<<1].min, tree[rt<<1|1].min);
	}
	static void build(int rt,int l,int r) {
		tree[rt].l=l;
		tree[rt].r=r;
		if(l==r) {
			tree[rt].max=tree[rt].min=a[l];
			return;
		}
		int mid=(l+r)>>1;
		build(rt<<1,l,mid);
		build(rt<<1|1,mid+1,r);
		pushup(rt);
	}
	static int queryMax(int rt,int l,int r) {
		if(tree[rt].l==l&&tree[rt].r==r) {
			return tree[rt].max;
		}
		int mid=(tree[rt].l+tree[rt].r)>>1;
		if(r<=mid)
			return queryMax(rt<<1,l,r);
		else if(l>mid)
			return queryMax(rt<<1|1,l,r);
		else
			return max(queryMax(rt<<1,l,mid),queryMax(rt<<1|1,mid+1,r));
	}
	static int queryMin(int rt,int l,int r) {
		if(tree[rt].l==l&&tree[rt].r==r) {
			return tree[rt].min;
		}
		int mid=(tree[rt].l+tree[rt].r)>>1;
		if(r<=mid)
			return queryMin(rt<<1,l,r);
		else if(l>mid)
			return queryMin(rt<<1|1,l,r);
		else
			return min(queryMin(rt<<1,l,mid),queryMin(rt<<1|1,mid+1,r));
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		init();
		while(cin.hasNext()) {
			int n=cin.nextInt();
			int q=cin.nextInt();
			for(int i=1;i<=n;i++)
				a[i]=cin.nextInt();
			build(1,1,n);
			while((q--)!=0) {
				int l=cin.nextInt();
				int r=cin.nextInt();
				System.out.println(queryMax(1,l,r)-queryMin(1,l,r));
			}
		}
		cin.close();
	}
}
```





