# 【DFS_双调旅行商_Tri树_dp】网络搜集： AStar2014资格赛_解题报告 - CD's Coding - CSDN博客





2014年05月25日 18:02:26[糖果天王](https://me.csdn.net/okcd00)阅读数：612标签：[Astar																[dfs																[动态规划																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[Coding Competition																[算法记忆																[技巧记忆](https://blog.csdn.net/okcd00/article/category/1650755)







[A.Energy Conversion(HDU4823)](http://acm.hdu.edu.cn/showproblem.php?pid=4823)


DFS深搜，和我的解法完全一样= =……



```cpp
include <stdio.h>
#include <string.h>

const int maxn = 10000;

int dfs(long long n,long long m,long long v,long long k,int tim){
    if(n<=m)return tim;
    if(m<v || (m-v)*k<=m)return -1;
    return dfs(n,(m-v)*k,v,k,tim+1);
}

int main(){
    int L,T;
    long long n,m,v,k;
    //freopen("input.txt","r",stdin);
    //freopen("output.txt","w",stdout);
    scanf("%d",&T);
    for(L=1;L<=T;L++){
        scanf("%I64d%I64d%I64d%I64d",&n,&m,&v,&k);
        printf("%d\n",dfs(n,m,v,k,0));
    }
    return 0;
}
```
这是我AC的代码：



```cpp
#include<cmath>
#include<cstdio>
#include<iostream>
#include<algorithm>
using namespace std;

typedef long long ll;

int solve(ll n,ll m,ll v,ll k,int cnt)
{
	if(m>=n) return cnt;
	return (m-v)*k<=m?-1:solve(n,(m-v)*k,v,k,cnt+1) ;
} 

int main()
{
	int cs;
	scanf("%d",&cs);
	for(int i=1;i<=cs;i++)
	{
		long long n=0,m=0,v=0,k=0;
		int cnt=0;
		scanf("%lld%lld%lld%lld",&n,&m,&v,&k);
		printf("%d\n",solve(n,m,v,k,cnt));
	}
	return 0;
}
```




[B.Disk Schedule(HDU4824)](http://acm.hdu.edu.cn/showproblem.php?pid=4824)   From:[晓风残月xj](http://my.csdn.net/xj2419174554) @CSDN



```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
using namespace std;

const int INF=100*1000*800;
const int MAXN=1000+10;
struct point
{
    int x,y;
};
point vert[MAXN];
int res[MAXN][MAXN];


int dis(int i,int j)
{
	int t=abs(vert[i].x-vert[j].x)*400;
	int x1,x2;
	if(vert[i].y<vert[j].y)
	{
		x1=vert[i].y;
		x2=vert[j].y;
	}
	else
	{
		x1=vert[j].y;
		x2=vert[i].y;
	}
	int l=x2-x1;
	int r=360-x2+x1;
	return (l<r?l:r)+t;
}

int min(int a,int b)
{
    return a < b ? a : b;
}

int work(int n)
{
    int i,j;
    int s,Min=INF;
    res[2][1]=dis(2,1);
    for(i=2;i<=n;i++)
    {
        for(j=1;j<i;j++)
        {
            res[i][j]=min(res[i][j],res[i-1][j]+dis(i-1,i));
            res[i][i-1]=min(res[i][i-1],res[i-1][j]+dis(j,i));
        }
    }
    for(i=1;i<n;i++)
    {
        s=dis(i,n);
        if(Min > res[n][i] + s) Min = res[n][i] + s;
    }
    return Min;
}

int main()
{
    int i,j,n,cas;
	cin>>cas;
    while(cas--)
    {
		scanf("%d",&n);
		vert[1].x=0;
		vert[1].y=0;
        for(i=2;i<=n+1;i++)
            scanf("%d%d",&vert[i].x,&vert[i].y);

        for(i=1;i<=n+1;i++)
            for(j=1;j<=n+1;j++) res[i][j]=INF;
   
        printf("%d\n",work(n+1)+n*10);
    }
    return 0;
}
```

Another From:[czz5242199](http://my.csdn.net/czz5242199)@CSDN





本次最难的一题，相当于一个有约束条件的旅行商问题，这个约束条件就是你只能一开始先一直往右到达最右边，然后再一直往左回到起点。


由于移动轨道的时间和读取时间是固定的（400*2*MaxTrack + n*10)，所以你需要的就是找到最小的为了读取所有数据的轨道转动时间。


用dist[i, j]表示第i个轨道和第j个轨道之间的差，则dist[i, j] = Min( abs(a[i]-a[j]), 360-abs(a[i]-a[j]))


之后考虑如何用动态规划来进行状态的表示，用f[i, j]（i<=j）表示我们从i走到最右边的n，然后再走回j，期间刚好把[i, n]之间的所有点刚好都走一次，可以看出f[0][0]即为要求的目标


状态转移分为三种情况：


当i = j是，首先需要从i走到k(k>i)，然后再遍历，所以f[j, j] = Min(dist[j, k] + f[k, j])  i<k<=n


当i = j-1时类似，f[j-1, j] = Min(dist[j-1, k] + f[k, j])


当i < j-1时，i如果不先走到i+1，则i+1就无法被遍历到了，因为之后往左的过程最多走到j，而i+1<j，所以f[i, j] = dist[i, i+1]+f[i+1, j]



```cpp
#include <iostream>
#include <cmath>
using namespace std;

int n, maxl, t, a[1001], ans, f[1001][1001], dist[1001][1001];

int min(int a, int b) {
	return a<b?a:b;
}

int main()
{
	cin >> t;
	while (t--) {
		cin >> n;

		a[0] = 0;
		for (int i=1; i<=n; i++) cin >> maxl >> a[i];

		for (int i=0; i<=n; i++)
			for (int j=i; j<=n; j++)
				dist[i][j] = min(abs(a[i]-a[j]), 360-abs(a[i]-a[j]));

		f[n][n] = 0;
		for (int i=n-1; i>=0; i--)
			f[i][n] =  f[i+1][n]+dist[i][i+1];

		for (int j=n-1; j>=0; j--) {
			f[j][j] = dist[j][n] + f[j][n];
			if (j>0) f[j-1][j] = dist[j-1][n] + f[j][n];
			for (int i=j+1; i<n; i++) {
				f[j][j] = min(f[j][j], dist[j][i]+f[j][i]);
				if (j>0) f[j-1][j] = min(f[j-1][j], dist[j-1][i]+f[j][i]);
			}
			for (int i=j-2; i>=0; i--)
				f[i][j] = dist[i][i+1]+f[i+1][j];
		}
		cout << maxl*800+n*10+f[0][0] << endl;

	}
	return 0;
}
```








[C.Xor Sum(HDU 4825)](http://acm.hdu.edu.cn/showproblem.php?pid=4825) From:[洛阳纸迷](http://my.csdn.net/u010052353)@CSDN




```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#define LENGTH 32
#define MAXN 100000
using namespace std;
typedef struct node{
        node *next[2];
        bool flag;
};

node Head;
node arr[LENGTH * MAXN];
int nodeCount;

node *GetNewNode(){
        if (nodeCount >= LENGTH * MAXN) {
                printf("ERROR : node count full!");
                exit(1);
        }
        arr[nodeCount].next[0] = NULL;
        arr[nodeCount].next[1] = NULL;
        arr[nodeCount].flag    = false;
        return &(arr[nodeCount++]);
}

void Init(){
        nodeCount = 0;
        Head.next[0] = NULL;
        Head.next[1] = NULL;
        Head.flag    = false;
}

void insertNode(unsigned int num){
        node *p = &Head;
        int temp;
        for (int i = LENGTH - 1; i>=0; i--){
                if (((1<<i)&num) != 0){
                        temp = 1;
                } else {
                        temp = 0;
                }
                
                if (p->next[temp] == NULL){
                        p->next[temp] = GetNewNode();
                }
                
                p = p->next[temp];
        }
        p->flag = true;
}

unsigned int searchNode(unsigned int s){
        unsigned int k;
        node *p = &Head;
        int temp;
        k = 0;
        for (int i = LENGTH - 1; i>=0 ;i--){
                if (((1<<i)&s) != 0){
                        temp = 0;
                } else {
                        temp = 1;
                }
                
                if (p->next[temp] == NULL){
                        k = (k << 1) + (temp ^ 1);
                        p = p->next[temp ^ 1];
                } else {
                        k = (k << 1) + temp;
                        p = p->next[temp];
                }
        }
        return k;
}

int main(){
        int n , m ;
        unsigned int k , s;
        Init();
        int case_count = 0;
        scanf("%d", &case_count);
        int case_index = 0;
        while ((case_count--) && scanf("%d%d" , &n , &m) != EOF){
                case_index++;
                printf("Case #%d:\n", case_index);
                Init();
                for (int i = 0; i < n; i++){
                        scanf("%u",&k);
                        insertNode(k);
                }
                for (int i = 0; i < m; i++){
                        scanf("%u" , &s);
                        k = searchNode(s);
                        printf("%u\n" , k);
                }
        }
        return 0;
}
```

Another From:[czz5242199](http://my.csdn.net/czz5242199)@CSDN




给出N个数(N<=10W)，然后给出M个数(M<=10W)，对于每个数p在之前的N个数中找出q使得 p xor q最大


一步一步考虑即可，首先选择使得p在二进制表示下最高位（第31位）是1的那些数，如果没有再选择会使得最高位为0的数


之后再之后选择的数中选择能够使得第30位为1的数


.......


直到最后


数据结构就选择trie树即可




```cpp
#include <stdio.h>

struct node {
	struct node *zero, *one;
	node() {
		zero = NULL;
		one = NULL;
	}
};

node *head;

long long power[32];

void add(node* &h, int dep, long long p) {
	if (h == NULL) h = new node();
	if (dep == -1) return;
	if (p&power[dep]) add(h->one, dep-1, p); else add(h->zero, dep-1, p);
}

long long getAnswer(long long p) {
	long long ans = 0;
	node* h = head;
	for (int i=31; i>=0; i--) {
		if (p&power[i]) {
			if (h->zero) {
				h = h->zero;
			} else {
				ans |= power[i];
				h = h->one;
			}
		} else {
			if (h->one) {
				ans |= power[i];
				h = h->one;
			} else {
				h = h->zero;
			}
		}
	}
	return ans;
}

int main() {
	power[0] = 1;
	for (int i=1; i<=31; i++) power[i] = power[i-1] << 1;

	int T, n, m;
	scanf("%d",&T); 
	for (int test = 0; test < T; ++test) {
		scanf("%d%d",&n, &m);
		head = NULL;
		for (int i = 0; i < n; ++i) {
			long long temp;
			scanf("%I64d",&temp);
			add(head, 31, temp);
		}

		printf("Case #%d:\n", test+1);
		for (int i=0; i<m; i++) {
			long long temp;
			scanf("%lld",&temp);
			printf("%I64d\n", getAnswer(temp));
		}
	}
	return 0;
}
```





[D.Labyrinth (HDU4826)](http://acm.hdu.edu.cn/showproblem.php?pid=4826) From:[czz5242199](http://blog.csdn.net/czz5242199/article/details/26104413)@CSDN





给一个矩阵，一开始你在左上角，每个格子都有一个数，你可以选择向右，向下，向上走，但不能走重复，问你走到右上角的时候积累的和最大是多少


不能向左走是一个重要的特点，让我们可以使用动态规划


用f[i, j]表示走到第i行j列的时候最多能积累的和


首先f[i,j] = f[i,j-1]+a[i,j] （从左边走过来）


之后由于还可以上下走，所以对当前列上下各用一次动态规划即可，最后答案是f[0, M-1]




```cpp
#include <iostream>
#include <stdio.h>
using namespace std;


int T, n, m;
int a[100][100], f[100][100], t[100];

int max(int a, int b) {
	return a>b?a:b;
}

int main() {
	cin >> T;
	for (int test=1; test<=T; test++) {
		cin >> n >> m;
		for (int i=0; i<n; i++)
			for (int j=0; j<m; j++) 
				cin >> a[i][j];
		cout << "Case #" << test << ":\n";

		f[0][0] = a[0][0];
		for (int i=1; i<n; i++) 
			f[i][0] = f[i-1][0]+a[i][0];
		for (int j=1; j<m; j++) {
			for (int i=0; i<n; i++)
				f[i][j] = f[i][j-1]+a[i][j];

			t[0] = f[0][j];
			for (int i=1; i<n; i++)
				t[i] = max(f[i][j], t[i-1]+a[i][j]);

			for (int i=n-2; i>=0; i--) 
				f[i][j] = max(f[i][j], f[i+1][j]+a[i][j]);

			for (int i=0; i<n; i++)
				f[i][j] = max(f[i][j], t[i]);
		}

		cout << f[0][m-1] << endl;
	}
}
```](https://blog.csdn.net/okcd00/article/category/1650753)](https://blog.csdn.net/okcd00/article/category/2133459)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)](https://so.csdn.net/so/search/s.do?q=Astar&t=blog)




