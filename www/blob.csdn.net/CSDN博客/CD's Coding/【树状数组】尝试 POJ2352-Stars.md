# 【树状数组】尝试 POJ2352-Stars - CD's Coding - CSDN博客





2015年03月31日 00:07:23[糖果天王](https://me.csdn.net/okcd00)阅读数：389标签：[二进制																[algorithm																[树状数组																[poj2352](https://so.csdn.net/so/search/s.do?q=poj2352&t=blog)
个人分类：[算法记忆](https://blog.csdn.net/okcd00/article/category/1650753)








Stars

|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 35197||**Accepted:** 15277|




Description


Astronomers often examine star maps where stars are represented by points on a plane and each star has Cartesian coordinates. Let the level of a star be an amount of the stars that are not higher and not to the right of the given star. Astronomers want to know
 the distribution of the levels of the stars. 
![](http://poj.org/images/2352_1.jpg)

For example, look at the map shown on the figure above. Level of the star number 5 is equal to 3 (it's formed by three stars with a numbers 1, 2 and 4). And the levels of the stars numbered by 2 and 4 are 1. At this map there are only one star of the level
 0, two stars of the level 1, one star of the level 2, and one star of the level 3. 


You are to write a program that will count the amounts of the stars of each level on a given map.

Input


The first line of the input file contains a number of stars N (1<=N<=15000). The following N lines describe coordinates of stars (two integers X and Y per line separated by a space, 0<=X,Y<=32000). There can be only one star at one point of the plane. Stars
 are listed in ascending order of Y coordinate. Stars with equal Y coordinates are listed in ascending order of X coordinate. 


Output


The output should contain N lines, one number per line. The first line contains amount of stars of the level 0, the second does amount of stars of the level 1 and so on, the last line contains amount of stars of the level N-1.

Sample Input
5
1 1
5 1
7 1
3 3
5 5
Sample Output
1
2
1
1
0
Hint


This problem has huge input data,use scanf() instead of cin to read data to avoid time limit exceed.

Source

[Ural Collegiate Programming Contest 1999](http://poj.org/searchproblem?field=source&key=Ural+Collegiate+Programming+Contest+1999)


是时候捡起来线段树和树状数组了……




题目大意：

在坐标上有n个星星，如果某个星星坐标为(x, y), 它的左下位置为：(x0,y0)，x0<=x 且y0<=y。如果左下位置有a个星星，就表示这个星星属于level x

按照y递增，如果y相同则x递增的顺序给出n个星星，求出所有level水平的数量。





首先这道题看了网上的解题报告：

线段树解法：



```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#define mid ((left+right)>>1)
#define lson rt<<1,left,mid
#define rson rt<<1|1,mid+1,right
using namespace std;

const int MAXN = 32005;

int sum[MAXN<<2],level[MAXN<<2];

void update(int rt,int left,int right,int data){
    ++sum[rt];
    if(left==right) return;
    if(data <= mid) update(lson,data);
    else update(rson,data);
}
int query(int rt,int left,int right,int l,int r){
    if(left==l && right==r) {
        return sum[rt];
    }
    int m = mid;
    if(r <= m) return query(lson,l,r);
    else if(l > m) return query(rson,l,r);
    else return query(lson,l,m)+query(rson,m+1,r);
}

int main(){
    int n,x,y;
    while(~scanf("%d",&n)){
        memset(sum, 0, sizeof(sum));
        memset(level, 0, sizeof(level));
        for(int i=0; i<n; ++i){
            scanf("%d%d",&x,&y);
            ++x;
            ++level[query(1,1,MAXN,1,x)];
            update(1,1,MAXN,x);
        }
        for(int i=0; i<n; ++i)
            printf("%d\n",level[i]);
    }
    return 0;
}
```


树状数组解法：



```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;

const int MAXN = 32005;

int c[MAXN],level[MAXN],n;

int lowbit(int x){return x & (-x);}

// 求前n项的和
int sum(int n){
    int sum = 0;
    while(n > 0){
        sum += c[n];
        n -= lowbit(n);
    }
    return sum;
}
// 增加某个元素的大小
void add(int x){
    while(x <= MAXN){
        ++c[x];
        x += lowbit(x);
    }
}

int main(){
    int n,x,y;
    while(~scanf("%d",&n)){
        memset(level, 0, sizeof(level));
        memset(c, 0, sizeof(c));
        for(int i=0; i<n; ++i) {
            scanf("%d%d",&x,&y);
            ++x;
            level[sum(x)]++;
            add(x);
        }
        for(int i=0; i<n; ++i)
            printf("%d\n",level[i]);
    }
    return 0;
}
```


相对的我还是比较喜欢树状数组的，今天看懂的一些小部分还是记录一下的好：

首先，lowbit(x)是很常见的操作，作用为取当前数在二进制表示中最低位的1，比如10111000取lowbit就是1000，定义上则是 return x&(-x)




C1 = A1


C2 = A1 + A2


C3 = A3


C4 = A1 + A2 + A3 + A4


C5 = A5


C6 = A5 + A6


C7 = A7


C8 = A1 + A2 + A3 + A4 + A5 + A6 + A7 + A8




咱们来用这个来说明下意思：




设节点编号为x，那么这个节点管辖的区间为2^k（其中k为x二进制末尾0的个数）个元素。因为这个区间最后一个元素必然为Ax，


所以很明显：Cn = A(n – 2^k + 1) + ... + An




使用lowbit的时候，一般有向上和向下（即加减）之分：

求sum的时候，把区间一个接着一个的加起来，就是 i=x;i>0;i-=lowbit(i) 

n = n – lowbit(n)这一步实际上等价于将n的二进制的最后一个1减去，即一个接着一个的获取这个区间内的和，把这些和加起来就得到了所需要的和。

当我们要把其中某个点加1的时候，是不是应该要把包含有它的所有区间记录也加一呢，那么问题来了，哪些有呢，那就是i=x;i<=maxn;i+=lowbit(i)

知道了这些至少这道题就能解了，线段树神马的……咱以后再说好啦~ 赶紧去写作业哦~~~




啊然后这是这道题我的Code：



```cpp
#include<cstdio>
#include<iostream>
#include<algorithm>
using namespace std; 
#define maxn 32001

int arr[maxn]={0};

int lowbit(int i){return (i&-i);}
void update(int x,int val){for(int i=x;i<=maxn;i+=lowbit(i))arr[i]+=val;}

int getsum(int x)
{
    int temp=0;
    for(int i=x;i>0;i-=lowbit(i)) temp+=arr[i];
    return temp;
}

int main()
{
	int stars[maxn]={0};
	int n;	scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		int x,y;	scanf("%d%d",&x,&y);
		int l=getsum(x+1);		stars[l]++;
		update(x+1,1);
	}
	for(int i=0;i<n;i++)		printf("%d\n",stars[i]);
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=树状数组&t=blog)](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)](https://so.csdn.net/so/search/s.do?q=二进制&t=blog)




