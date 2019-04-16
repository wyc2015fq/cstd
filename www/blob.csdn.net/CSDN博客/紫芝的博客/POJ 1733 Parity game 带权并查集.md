# POJ 1733 Parity game 带权并查集 - 紫芝的博客 - CSDN博客





2018年04月07日 20:22:14[紫芝](https://me.csdn.net/qq_40507857)阅读数：29
个人分类：[并查集](https://blog.csdn.net/qq_40507857/article/category/7569705)









# Parity game
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 11358||**Accepted:** 4388|

Description

Now and then you play the following game with your friend. Your friend writes down a sequence consisting of zeroes and ones. You choose a continuous subsequence (for example the subsequence from the third to the fifth digit inclusively) and ask him, whether this subsequence contains even or odd number of ones. Your friend answers your question and you can ask him about another subsequence and so on. Your task is to guess the entire sequence of numbers. 


You suspect some of your friend's answers may not be correct and you want to convict him of falsehood. Thus you have decided to write a program to help you in this matter. The program will receive a series of your questions together with the answers you have received from your friend. The aim of this program is to find the first answer which is provably wrong, i.e. that there exists a sequence satisfying answers to all the previous questions, but no such sequence satisfies this answer.

Input

The first line of input contains one number, which is the length of the sequence of zeroes and ones. This length is less or equal to 1000000000. In the second line, there is one positive integer which is the number of questions asked and answers to them. The number of questions and answers is less or equal to 5000. The remaining lines specify questions and answers. Each line contains one question and the answer to this question: two integers (the position of the first and last digit in the chosen subsequence) and one word which is either `even' or `odd' (the answer, i.e. the parity of the number of ones in the chosen subsequence, where `even' means an even number of ones and `odd' means an odd number).

Output

There is only one line in output containing one integer X. Number X says that there exists a sequence of zeroes and ones satisfying first X parity conditions, but there exists none satisfying X+1 conditions. If there exists a sequence of zeroes and ones satisfying all the given conditions, then number X should be the number of all the questions asked.

Sample Input

10
5
1 2 even
3 4 odd
5 6 even
1 6 even
7 10 odd
Sample Output

3
Source

[CEOI 1999](http://poj.org/searchproblem?field=source&key=CEOI+1999)

[点击打开链接](http://poj.org/problem?id=1733)



题意：

有一个长度已知的 01串，给出[l,r]这个区间中 1的个数是奇数还是偶数

给出一些语句，问前几个是正确的？.



思路：

1.一类经典的并查集题目，经典模型就是将 [l,r]这个区间转换为(l,r], 

那么 1的个数就可以表示为 sum[r]-sum[l-1],也就确定了奇数还是偶数



2.可以用 r[]数组表示这个端点到它的根节点的 1的奇偶 

这个区间就是 (i,root[i]) 0代表偶数，1代表奇数



3.对于每个输入的区间，查找他们的根节点是否相同

①如果相同，证明这个区间的奇偶性在之前已经得知，那么直接判断即可

②如果不同，那么就是u-1与v此时不在同一个集合中，

可以知道(u-1,root([u-1])]区间、(v,root([v])]区间 和 (u-1,v]区间 1的奇偶

那么就可以算出(root([u-1]),root([v])]区间的属性，进而合并两者

③在合并时，根节点 r[root(u)]=r(u)^r(v)^r(u-1,v],

在路径压缩过程中, r[i]=r[i]^r[root(i)]


4.异或运算符：相同为0，不同为 1 ; a^b = (!a)&&b || a&&(!b) 



5.点的最大值为 10^9,语句有 5000条，所以需要离散化



```cpp
#include<cstdio>
#include<algorithm>
using namespace std;
#define maxn 5005
struct Node{
	int u,v;
	char str[10];
}que[maxn];
int a[maxn<<1],n,q,cnt;
int pre[maxn<<1],r[maxn<<1];
int Find(int x)
{
	if(x!=pre[x])
	{
		int f=pre[x];
		pre[x]=Find(pre[x]);
		r[x]=r[x]^r[f];
	}
	return pre[x];
}


int main()
{
	while(scanf("%d",&n)!=EOF){
		scanf("%d",&q);
		cnt=0;
		for(int i=0;i<q;i++)
		{
			scanf("%d%d%s",&que[i].u,&que[i].v,que[i].str);
			que[i].u--;
			a[cnt++]=que[i].u;
			a[cnt++]=que[i].v;
		}
		sort(a,a+cnt);
		cnt=unique(a,a+cnt)-a;
		for(int i=0;i<cnt;i++)
		{
			pre[i]=i;
			r[i]=0;
		}
		int ans=0;
		for(int i=0;i<q;i++)
		{
			int u=lower_bound(a,a+cnt,que[i].u)-a;
			int v=lower_bound(a,a+cnt,que[i].v)-a;
			int ra=Find(u);
			int rb=Find(v);
			if(ra==rb){
				if(r[u]==r[v]&&que[i].str[0]=='o')
				break;
				if(r[u]!=r[v]&&que[i].str[0]=='e')
				break;
				ans++;
			}else{
				if(que[i].str[0]=='o'){
					pre[ra]=rb;
					r[ra]=r[u]^r[v]^1;
				}else{
					pre[ra]=rb;
					r[ra]=r[u]^r[v];
				}
				ans++;
			}
		}
		printf("%d\n",ans);
		
	}return 0;
}
```







