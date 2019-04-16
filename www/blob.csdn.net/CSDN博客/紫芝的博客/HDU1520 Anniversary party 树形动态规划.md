# HDU1520 Anniversary party 树形动态规划 - 紫芝的博客 - CSDN博客





2018年05月02日 11:39:02[紫芝](https://me.csdn.net/qq_40507857)阅读数：51
个人分类：[动态规划](https://blog.csdn.net/qq_40507857/article/category/7565832)









# HDU1520 Anniversary party 树形动态规划 

# Anniversary party

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 15486    Accepted Submission(s): 5958****

Problem Description

There is going to be a party to celebrate the 80-th Anniversary of the Ural State University. The University has a hierarchical structure of employees. It means that the supervisor relation forms a tree rooted at the rector V. E. Tretyakov. In order to make the party funny for every one, the rector does not want both an employee and his or her immediate supervisor to be present. The personnel office has evaluated conviviality of each employee, so everyone has some number (rating) attached to him or her. Your task is to make a list of guests with the maximal possible sum of guests' conviviality ratings.

Input

Employees are numbered from 1 to N. A first line of input contains a number N. 1 <= N <= 6 000. Each of the subsequent N lines contains the conviviality rating of the corresponding employee. Conviviality rating is an integer number in a range from -128 to 127. After that go T lines that describe a supervisor relation tree. Each line of the tree specification has the form:L K

It means that the K-th employee is an immediate supervisor of the L-th employee. Input is ended with the line 0 0

Output

Output should contain the maximal sum of guests' ratings.

Sample Input

```
7
1 
1 
1 
1 
1 
1 
1 
1 3
2 3
6 4
7 4
4 5
3 5
0 0
```

Sample Output
`5`
Source

[Ural State University Internal Contest October'2000 Students Session](http://acm.hdu.edu.cn/search.php?field=problem&key=Ural+State+University+Internal+Contest+October%272000+Students+Session&source=1&searchmode=source)

Recommend

linle

## 题意：

在一棵有根树上每个节点有一个权值，相邻的父亲和孩子只能选择一个，求总权值之和最大

## 思路：

这个DFS过程大家可能不太理解，dfs(son)貌似没有写终止条件，可是却没引起无限递归，为什么呢？递归是怎么终止的？

题目大意是，在一棵有根树上，每个节点有一个权值，相邻的父亲和孩子只能选择一个，

求总权值之和最大.

首先，我们用a【i】存放节点i的权值，father【i】表示节点i的祖先，G【i】是一个vector动态数组，存放i的所有子孙节点，

接下来我们寻找树的根节点，从根节点开始搜索，遍历根节点的子孙节点，递归深度即数组G【root】的大小，

所以dfs(son)貌似没有写终止条件，却没引起无限递归

dp[i][0]表示第i个人参加，dp[i][1]表示第i个人不参加 

### 状态转移方程： 

**dp[root][0]+=max(dp[son][1],dp[son][0]);**

当前这个点不选，他的孩子可选、可不选，取最大值 
**dp[root][1]+=dp[son][0];**

当前这个点选择，它的孩子就不能选择





```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=6005;
int father[maxn];//标记根节点
int n,a[maxn];//每个节点的权值
vector<int>G[maxn];//存放子节点
int dp[maxn][2];
void dfs(int root)
{
	dp[root][1]=a[root];
	for(int i=0;i<G[root].size();i++){
		int son=G[root][i];
		dfs(son);
		dp[root][0]+=max(dp[son][1],dp[son][0]);
		//当前这个人不选，取选孩子和不选孩子的较大者
		dp[root][1]+=dp[son][0];
		//当前这个人选，不选他的孩子
	}
}
int main()
{
	ios::sync_with_stdio(0);
	int n;
	while(cin>>n){
		//memset(father,-1,sizeof(father));
		for(int i=1;i<=n;i++){
		cin>>a[i];
		G[i].clear();//清空
		dp[i][0]=dp[i][1]=0;
		father[i]=-1;
		}

		int l,k;
		while(cin>>l>>k,l+k){//l是k的孩子
			father[l]=k;//l的祖先为k
			G[k].push_back(l);
		}
		int root=1;
		while(father[root]!=-1)//寻找根节点
		root=father[root];
		dfs(root);
		cout<<max(dp[root][1],dp[root][0])<<endl;
	}
	return 0;
}
```







