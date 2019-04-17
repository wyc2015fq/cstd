# 【&&】 0525_1001.Scenic Popularity Astar第二战_Failed - CD's Coding - CSDN博客





2014年05月25日 17:30:53[糖果天王](https://me.csdn.net/okcd00)阅读数：658标签：[Astar																[逻辑运算](https://so.csdn.net/so/search/s.do?q=逻辑运算&t=blog)](https://so.csdn.net/so/search/s.do?q=Astar&t=blog)
个人分类：[Coding Competition																[技巧记忆](https://blog.csdn.net/okcd00/article/category/1650755)](https://blog.csdn.net/okcd00/article/category/2133459)









# Scenic Popularity
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 60    Accepted Submission(s): 13**



Problem Description


　　临近节日，度度熊们最近计划到室外游玩公园，公园内部包括了很多的旅游景点区和休息区，由于旅游景点很热门，导致景点区和休息区都聚集了很多人。所以度度熊在旅游之前想通过百度地图查看一下公园内各个地方的热门程度。

　　假设所有景点区和休息区都是X轴直线上的一系列顶点，所对应的坐标Xi 保证唯一。每个景点区有个初始的热度值，而一个休息区(坐标为Xi)的热度值等于离它距离最近的景点区Xj的热度值（距离定义为|Xi-Xj|），如果此休息区与两个景点区的距离一样，则休息区的热度值选择两个景点区中的热度值最大值，如果两个热度值都一样，则随意选择其中一个。

　　度度熊在出门之前会经常去查看百度地图，每次查看前会有某些景点区的热度值已发生改变，从而也会导致周围的休息区的热度值发生改变，然后度度熊想知道当前热度值<=Rk的顶点(包括景点区和休息区)有多少个





Input


　　输入数据的第一行是测试Case的个数(T<=100）。

　　每个Case的第一行是N(0<N<=10000)，表示景点区和休息区的总数。

　　接着会有N行数据，每一列首先是顶点的X坐标Xi (0< Xi <=1e8)，第二列是一个整数Hi(0=<Hi <=100000)，如果Hi 不为0，则表示当前顶点为风景区且初始的热度值为Hi，否则表示当前顶点为休息区。这N行数据会按照坐标Xi递增的方式依次给出。

　　接着的一行数据是操作的次数K(K<=100)，最后会有K行数据，每一行的第一列要么是’U’或者’Q’，’U’表示当前操作为更改热度操作，’Q’表示当前操作为查询操作。如果是更改操作，接着会有两列数据，分别是热度值要改变的风景区的下标Lk(0<=Lk<N)以及改变后的热度值Vk(0< Vk<=100000)；如果是查询操作，第二列是要查询的热度范围Rk(0< Rk<=100000)





Output


　　对于第k组测试数据，第一行输出Case #k:，接下来对每次查询操作(即Q操作)会输出一个整数，表示满足条件的顶点数有多少个





Sample Input


1                                 
4                                 
10 0
20 3
30 0 
40 2
3
Q 3 
U 3 4 
Q 3






Sample Output


Case #1:
4
2






Source

[2014年百度之星程序设计大赛
 - 初赛（第二轮）](http://acm.hdu.edu.cn/search.php?field=problem&key=2014%C4%EA%B0%D9%B6%C8%D6%AE%D0%C7%B3%CC%D0%F2%C9%E8%BC%C6%B4%F3%C8%FC%20-%20%B3%F5%C8%FC%A3%A8%B5%DA%B6%FE%C2%D6%A3%A9&source=1&searchmode=source)









WA、MLE、RUNTIME_ERROR(Gameover)、TLE

各种尝了个遍……






```cpp
for(int lcnt=Ncnt-1;lcnt>=0&&posi[lcnt]>=0;lcnt--)
```
这样的句子想必都写过，然后就会吃到RE，程序一长根本不知道哪来的RE……



其实，在lcnt=-1的时候确实lcnt>=0判定为False了，但是它是会继续判断posi[lcnt]>=0的，这样就出现了RE……

坑死人不偿命，已死之人谨记于心。



```cpp
int heat[MAXM][4];//heat of the pos (View/ B: L/R) [TLE]
```
当MAX=1e8的时候，正好是HDU的MemoryLimit的两倍，开数组空间换时间是可以的，但是别开太大……






以及：都改疯了都不知道为何居然又TLE了的代码……



```cpp
#include<cmath>
#include<cstdio>
#include<string>
#include<vector>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
#define dbg cout<<"here"

typedef long long ll;
//const long long M=1000000007;
const int MAXM=100000001;
//int kind[MAXM];//kind ViewArea=1 BreakArea=2 [TLE]
int heat[10005][3]; //MLE_Link to Posi
//int heat[MAXM][3];//heat of the pos (View/ B: L/R) [TLE]
ll posi[10005];//position of ith point
ll posv[10005];//position of jth ViewArea

int getMax(int a,int b){return a>b?a:b;}

int getVcnt(int Ucnt) //Binsearch
{
	ll postmp=posi[Ucnt];
	int l=0,r=Ucnt;
	while(1)
	{
		if(l+1==r)return posi[l]==postmp?l:r;
		if(posv[(l+r)/2]>postmp) r=(l+r)/2;
		else if	(posv[(l+r)/2]<postmp) l=(l+r)/2;
		else return (l+r)/2;
	}
}

int getUcnt(int Vcnt)
{
	ll postmp=posv[Vcnt];
	int l=0,r=10000;
	while(1)
	{
		if(l+1==r)return posi[l]==postmp?l:r;
		if(posi[(l+r)/2]>postmp) r=(l+r)/2;
		else if	(posi[(l+r)/2]<postmp) l=(l+r)/2;
		else return (l+r)/2;
	}
}

int main()
{
	int T=0;
	scanf("%d",&T);
	for(int cnt=1;cnt<=T;cnt++) 
	{
		//memset(kind,0,sizeof(kind));
		memset(heat,0,sizeof(heat));
		memset(posi,MAXM+1,sizeof(posi));
		printf("Case #%d:\n",cnt);	
		int N=0,left=0,Vcnt=0;
		scanf("%d",&N);
		for(int Ncnt=0;Ncnt<N;Ncnt++)
		{
			ll pos=0;
			int h=0;
			scanf("%lld%d",&pos,&h);
			posi[Ncnt]=pos;
			if(h==0)
			{
				heat[Ncnt][1]= heat[left][0];
			}
			else
			{
				posv[Vcnt]=pos;
				heat[Ncnt][0]= h;
				if(Vcnt==0)
				{
					if(Ncnt>0) for(int lcnt=Ncnt-1;lcnt>=0;lcnt--)
					{
						if(posi[lcnt]>=0)break;
						heat[lcnt][2]=heat[Ncnt][0];
						//cout<<"Iamright";
						heat[lcnt][1]=-1;
					}
				}
				else for(int lcnt=Ncnt-1;lcnt>=0;lcnt--)
				{
					if(posi[lcnt]>posv[Vcnt-1])break;
					if((posi[Ncnt]-posi[lcnt])>(posi[lcnt]-posv[Vcnt-1]))
					{
						heat[lcnt][2]=heat[Ncnt][0];
						heat[lcnt][1]=-1;
					}
					else if((posi[Ncnt]-posi[lcnt])<(posi[lcnt]-posv[Vcnt-1]))
					{
						heat[lcnt][1]=heat[getUcnt(Vcnt-1)][0];
						heat[lcnt][2]=-1;
					}
					else
					{
						heat[lcnt][2]=heat[Ncnt][0];
					}
				}
				Vcnt++;
				left=Ncnt;
			}
			
		}
		posi[N+1]= -123;//endMark
		int K=0;
		scanf("%d",&K);
		for(int Kcnt=0;Kcnt<K;Kcnt++)
		{
			char sta='.';
			scanf("%c",&sta);//get end of line
			scanf("%c",&sta);
			if(sta=='Q') 
			{
				int Qlim=0,Qans=0;
				scanf("%d",&Qlim);
				for(int Qcnt=0;Qcnt<N;Qcnt++)
				{
					if(heat[Qcnt][0]!=0)Qans += heat[Qcnt][0]<=Qlim?1:0;
					else Qans += getMax(heat[Qcnt][1],heat[Qcnt][2])<=Qlim?1:0;
					//printf("%d %d %d\n",heat[Qcnt][0],heat[Qcnt][1],heat[Qcnt][2]);
				}
				printf("%d\n",Qans);
			}
			if(sta=='U')
			{
				int Ucnt=0,Uh=0;
				scanf("%d%d",&Ucnt,&Uh);
				heat[Ucnt][0]=Uh;
				if(Ucnt>0)
				for(int lfind=Ucnt-1;lfind>=0;lfind--)
				{
					if(getVcnt(Ucnt)!=0)if(posi[lfind]>posv[getVcnt(Ucnt)-1]) break;
					if(heat[lfind][2]!=-1)	heat[lfind][2]=Uh;
				}
				if(posi[Ucnt+1]!=-123)
				for(int rfind=Ucnt+1;rfind<=N-1&&posi[rfind]<posv[getVcnt(Ucnt)+1];rfind++)
				{
					if(heat[rfind][1]!=-1)
					heat[posi[rfind]][1]=Uh;
				}
			}
		}
	
	}
	
	return 0;
}
```


刚才作死的试了一下大数据会不会特别慢：

1

Case #1:

1234 0

523162 3

2341613 0

99999999 2

3

Q 2



--------------------------------

Process exited with return value 0

Press any key to continue . . .


这什么情况…… 




FAILED，存解题报告开始补差，开始学!





解题报告：From Kuangbin：


我的做法就是在修改的时候，暴力修改和这个点相关的。

找出景点会影响哪些休息区，然后修改的时候暴力，查询的时候树状数组求前n项和






```cpp
/* ***********************************************
Author        :kuangbin
Created Time  :2014/5/25 14:22:19
File Name     :E:\2014ACM\比赛\百度之星初赛2\A.cpp
************************************************ */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int MAXN = 100010;
long long c[MAXN];
int lowbit(int x)
{
    return x&(-x);
}
long long sum(int i)
{
    long long ret = 0;
    while(i > 0)
    {
        ret += c[i];
        i -= lowbit(i);
    }
    return ret;
}
void add(int i,long long val)
{
    while(i <= 100000)
    {
        c[i] += val;
        i += lowbit(i);
    }
}

int s1[MAXN];
int s2[MAXN];
int sz1,sz2;

vector<int>vec1[10010];
vector<int>vec2[10010];
int a[10010];
int b[10010];

void calc(int u)
{
    int id = lower_bound(s1,s1+sz1,s2[u]) - s1;
    if(id == 0)
    {
        vec1[0].push_back(u);
        vec2[u].push_back(0);
    }
    else if(id == sz1)
    {
        vec1[sz1-1].push_back(u);
        vec2[u].push_back(sz1-1);
    }
    else 
    {
        if(s2[u] - s1[id-1] < s1[id] - s2[u] )
        {
            vec1[id-1].push_back(u);
            vec2[u].push_back(id-1);
        }
        else if(s2[u] - s1[id-1] > s1[id] - s2[u])
        {
            vec1[id].push_back(u);
            vec2[u].push_back(id);
        }
        else
        {
            vec1[id-1].push_back(u);
            vec2[u].push_back(id-1);
            vec1[id].push_back(u);
            vec2[u].push_back(id);
        }
    }
}
int get(int u)
{
    if(vec2[u].size() == 1)
    {
        return a[vec2[u][0]];
    }
    else
    {
        return max(a[vec2[u][0]],a[vec2[u][1]]);
    }
}

int link[10010];

int main()
{
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int T;
    int iCase = 0;
    int n;
    scanf("%d",&T);
    while(T--)
    {
        iCase++;
        printf("Case #%d:\n",iCase);
        sz1 = 0;sz2 = 0;
        memset(c,0,sizeof(c));
        int u,v;
        scanf("%d",&n);
        for(int i = 0;i < n;i++)
        {
            scanf("%d%d",&u,&v);
            if(v == 0)
                s2[sz2++] = u;
            else
            {
                s1[sz1++] = u;
                link[i] = sz1-1;
                add(v,1);
                a[sz1-1] = v;
            }
            vec1[i].clear();
            vec2[i].clear();
        }
        for(int i = 0;i < sz2;i++)
            calc(i);
        for(int i = 0;i < sz2;i++)
        {
            b[i] = get(i);
            add(b[i],1);
        }
        char op[10];
        int m;
        scanf("%d",&m);
        while(m--)
        {
            scanf("%s",op);
            if(op[0] == 'Q')
            {
                scanf("%d",&u);
                printf("%d\n",(int)sum(u));
            }
            else
            {
                scanf("%d%d",&u,&v);
                u = link[u];
                add(a[u],-1);
                a[u] = v;
                add(a[u],1);
                for(int i = 0;i < vec1[u].size();i++)
                {
                    int p = get(vec1[u][i]);
                    add(b[vec1[u][i]],-1);
                    b[vec1[u][i]] = p;
                    add(p,1);
                }
            }
        }
    }
    return 0;
}
```




解题报告From：[你若乄成风卍](http://my.csdn.net/a1061747415)@CSDN


题目大意：


T组测试数据，每组首先一个n表示 旅游区和休息区 共有 n个。

接下来是n行介绍，每行两个数字，第一个表示位置，第二个表示热度，如果热度为0表示休息区，否则为景区，休息区的热度与最近景区的热度相同，如果（左边和右边）两个景区距离相同，休息区的热度取热度高的。

经接着m，表示m组询问，Q x ，表示 热度小于等于x的有多少个。

U 表示改变某个景区的热度。


解题思路：

对于Q操作的询问，很明显是维护的一个前缀和，因此想到用线段树维，但是U操作很复杂，要维护差量计算。



```cpp
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int maxn=110000;
int pos[maxn],value[maxn],lson[maxn],rson[maxn],n;

struct node{
    int l,r,sum;
}a[maxn*4];

void build(int l,int r,int k){
    a[k].l=l;
    a[k].r=r;
    a[k].sum=0;
    if(l<r){
        int mid=(l+r)/2;
        build(l,mid,2*k);
        build(mid+1,r,2*k+1);
    }
}

void insert(int l,int r,int k,int c){
    //if(k==1) cout<<"insert:"<<l<<" "<<r<<" "<<c<<" "<<endl;
    if(l<=a[k].l && a[k].r<=r){
        a[k].sum+=c;
    }else{
        int mid=(a[k].l+a[k].r)/2;
        if(r<=mid) insert(l,r,2*k,c);
        else if(l>=mid+1) insert(l,r,2*k+1,c);
        else{
            insert(l,mid,2*k,c);
            insert(mid+1,r,2*k+1,c);
        }
        a[k].sum=a[2*k].sum+a[2*k+1].sum;
    }
}

int query(int l,int r,int k){
    if(l<=a[k].l && a[k].r<=r){
        return a[k].sum;
    }else{
        int mid=(a[k].l+a[k].r)/2;
        if(r<=mid) return query(l,r,2*k);
        else if(l>=mid+1) return query(l,r,2*k+1);
        else return query(l,mid,2*k)+query(mid+1,r,2*k+1);
    }
}

void findlson(int k){
    if(k<=0) lson[k]=-1;
    else{
        if(value[k-1]==0) lson[k]=lson[k-1];
        else lson[k]=k-1;
    }
}

void findrson(int k){
    if(k>=n-1) rson[k]=-1;
    else{
        if(value[k+1]==0) rson[k]=rson[k+1];
        else rson[k]=k+1;
    }
}

void input(){
    build(0,110000,1);
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d%d",&pos[i],&value[i]);
        lson[i]=-1;
        rson[i]=-1;
    }
    for(int i=0;i<n;i++){
        if(value[i]==0) findlson(i);
    }
    for(int i=n-1;i>=0;i--){
        if(value[i]==0) findrson(i);
    }
    for(int i=0;i<n;i++){
        if(value[i]==0){
            if( lson[i]==-1 && rson[i]==-1 ) insert(0,0,1,1);
            if( lson[i]!=-1 && rson[i]==-1 ) insert(value[lson[i]],value[lson[i]],1,1);
            if( lson[i]==-1 && rson[i]!=-1 ) insert(value[rson[i]],value[rson[i]],1,1);
            if( lson[i]!=-1 && rson[i]!=-1 ){
                int l0=lson[i],r0=rson[i];
                if(pos[i]-pos[l0] <pos[r0]-pos[i] ) insert(value[l0],value[l0],1,1);
                else if(pos[i]-pos[l0] > pos[r0]-pos[i] ) insert(value[r0],value[r0],1,1);
                else{
                    insert(max(value[r0],value[l0]),max(value[r0],value[l0]),1,1);
                }
            }
        }else{
            insert(value[i],value[i],1,1);
        }
    }
}


void solve(){
    int m;
    scanf("%d",&m);
    while(m-- >0){
        char ch;
        cin>>ch;
        if(ch=='Q'){
            int c;
            scanf("%d",&c);
            cout<<query(0,c,1)<<endl;
        }else{
            int k,c;
            scanf("%d%d",&k,&c);
            insert(c,c,1,1);
            insert(value[k],value[k],1,-1);
            if(value[k]!=c){
                for(int i=k-1;i>=0;i--){
                    if(value[i]!=0) continue;
                    if(rson[i]!=k) break;
                    if(lson[i]==-1){
                        insert(c,c,1,1);
                        insert(value[k],value[k],1,-1);
                    }else{
                        int l0=lson[i],r0=rson[i];
                        if(pos[i]-pos[l0] > pos[r0]-pos[i] ){
                            insert(c,c,1,1);
                            insert(value[k],value[k],1,-1);
                        }
                        if(pos[i]-pos[l0] == pos[r0]-pos[i] ){
                            insert( max(value[r0],value[l0]) , max(value[r0],value[l0]) ,1,-1);
                            insert( max(c,value[l0]) , max(c,value[l0]) ,1,1);
                        }
                    }
                }
                for(int i=k+1;i<n;i++){
                    if(value[i]!=0) continue;
                    if(lson[i]!=k) break;
                    if(rson[i]==-1){
                        insert(c,c,1,1);
                        insert(value[k],value[k],1,-1);
                    }else{
                        int l0=lson[i],r0=rson[i];
                        if(pos[i]-pos[l0] < pos[r0]-pos[i] ){
                            insert(c,c,1,1);
                            insert(value[k],value[k],1,-1);
                        }
                        if(pos[i]-pos[l0] == pos[r0]-pos[i] ){
                            insert( max(value[r0],value[l0]) , max(value[r0],value[l0]) ,1,-1);
                            insert( max(c,value[r0]) , max(c,value[r0]) ,1,1);
                        }
                    }
                }
            }
            value[k]=c;
        }
    }
}

int main(){
    int t;
    scanf("%d",&t);
    for(int i=1;i<=t;i++){
        input();
        printf("Case #%d:\n",i);
        solve();
    }
    return 0;
}
```











