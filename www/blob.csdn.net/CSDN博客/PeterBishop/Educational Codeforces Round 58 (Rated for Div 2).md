# Educational Codeforces Round 58 (Rated for Div. 2) - PeterBishop - CSDN博客





2019年01月12日 17:18:57[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22
个人分类：[CodeForces](https://blog.csdn.net/qq_40061421/article/category/7796219)









昨晚太太天真题目虽然简单但是方法用的很蠢。。。。

### A（简单数学）

```cpp
#include<stdio.h>
int n,l,r,p;
main()
{
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d%d%d",&l,&r,&p);
		printf("%d\n",p<l?p:(r/p+1)*p);//这个机智的方法我怎么就想不到呢！！！
	}
}
```

### B（模拟，原来我的代码好像有细节问题） 

```cpp
#include<iostream>
using namespace std;

int main() {//先找到四个位置在之间数出来竖线就行了
	string s;
	int x,i,j,k=0;
	cin>>s;
	s=":]"+s+"[:";
	for(i=0;s[i]!='[';i++);
	for(j=s.size()-1;s[j]!=']';j--);
	for(++i;s[i]!=':';i++);
	for(--j;s[j]!=':';j--);
	if(i>=j)
		{
		cout<<"-1\n";
		return 0;
		}
	for(x=i+1;x<j;x++)
		k+=s[x]=='|';
	cout<<k+4<<endl;
	return 0;
}
```

### C() 

```cpp
#include<bits/stdc++.h>
#define MAXN 100005
#define INF 1000000000
#define MOD 1000000007
#define F first
#define S second
using namespace std;
typedef long long ll;
typedef pair<int,int> P;
int T,n,ans[MAXN];
struct node
{
    int l,r,id;
}a[MAXN];
bool cmp(node x,node y)
{
    if(x.l!=y.l) return x.l<y.l;
    return x.r<y.r;
}
int main()
{
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        for(int i=0;i<n;i++) 
        {
            scanf("%d%d",&a[i].l,&a[i].r);
            a[i].id=i;
        }
        sort(a,a+n,cmp);
        for(int i=0;i<n;i++) 
            ans[i]=2;
        int r=a[0].r;
        ans[a[0].id]=1;
        bool f=false;
        for(int i=1;i<n;i++)
        {
            if(a[i].l>r) 
            {
                f=true; 
                break;
            }
            r=max(r,a[i].r); 
            ans[a[i].id]=1;
        }
        if(!f) 
            puts("-1");
        else
        {
            for(int i=0;i<n;i++) 
                printf("%d ",ans[i]);
            puts("");
        }
    }
    return 0;
}
```





