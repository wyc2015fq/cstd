# Billboard(线段树) - weixin_33985507的博客 - CSDN博客
2016年01月12日 20:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# Billboard
**Time Limit: 20000/8000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others) Total Submission(s): 16755    Accepted Submission(s): 7089 **
Problem Description
At the entrance to the university, there is a huge rectangular billboard of size h*w (h is its height and w is its width). The board is the place where all possible announcements are posted: nearest programming competitions, changes in the dining room menu, and other important information.
On September 1, the billboard was empty. One by one, the announcements started being put on the billboard.
Each announcement is a stripe of paper of unit height. More specifically, the i-th announcement is a rectangle of size 1 * wi.
When  someone puts a new announcement on the billboard, she would always  choose the topmost possible position for the announcement. Among all  possible topmost positions she would always choose the leftmost one.
If  there is no valid location for a new announcement, it is not put on the  billboard (that's why some programming contests have no participants  from this university).
Given the sizes of the billboard and the  announcements, your task is to find the numbers of rows in which the  announcements are placed.
Input
There are multiple cases (no more than 40 cases).
The  first line of the input file contains three integer numbers, h, w, and n  (1 <= h,w <= 10^9; 1 <= n <= 200,000) - the dimensions of  the billboard and the number of announcements.
Each of the next n lines contains an integer number wi (1 <= wi <= 10^9) - the width of i-th announcement.
Output
For  each announcement (in the order they are given in the input file)  output one number - the number of the row in which this announcement is  placed. Rows are numbered from 1 to h, starting with the top row. If an  announcement can't be put on the billboard, output "-1" for this  announcement.
Sample Input
3 5 5
2
4
3
3
3
Sample Output
1
2
1
3
-1
 题解：意思是一块广告版，往上面贴广告条，规格1*w；广告条要尽量往高处放，给你一系列广告条规格，让你判断高度；如果放不成就输出-1；
方法就是用线段树，代表1-h间的最大宽度值（初始w），逐渐往里面插广告条；尽量往左边插；
代码：
```
#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
using namespace std;
#define mem(x,y) memset(x,y,sizeof(x))
#define SI(x) scanf("%d",&x)
#define SL(x) scanf("%lld",&x)
#define PI(x) printf("%d",x)
#define P_ printf("")
#define PL(x) printf("%lld",x)
typedef long long LL;
const int INF=0x3f3f3f3f;
#define ll root<<1
#define rr root<<1|1
#define lson ll,l,mid
#define rson rr,mid+1,r
const int MAXN=200100;
int tree[MAXN<<2];
#define V(x) tree[x]
int w;
int ans;
void pushup(int root){
	V(root)=max(V(ll),V(rr));
}
void build(int root,int l,int r){
	V(root)=w;
	if(l==r)return;
	int mid=(l+r)>>1;
	build(lson);
	build(rson);
}
void query(int root,int l,int r,int v){
	if(l==r){
		V(root)-=v;
		ans=l;return;
	}
	int mid=(l+r)>>1;
	if(V(ll)>=v)query(lson,v);
	else query(rson,v);
	pushup(root);
}
int main(){
	int h,n;
	while(~scanf("%d%d%d",&h,&w,&n)){
		if(h>n)h=n;
		build(1,1,h);
		while(n--){
			int x;
			SI(x);
			ans=INF;
			if(x>V(1)){
				puts("-1");continue;
			}
			query(1,1,h,x);
			printf("%d\n",ans);
		}
	}
	return 0;
}
```
