# 西安电子科技大学2017年ACM－ICPC校赛 - 紫芝的博客 - CSDN博客





2018年11月18日 11:19:52[紫芝](https://me.csdn.net/qq_40507857)阅读数：311








### 2017 Xidian Programming Contest  Onsite

![](https://img-blog.csdnimg.cn/20181118173157961.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-5
#define ll long long
using namespace std;
const int maxn=1e6;
int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        ll sum=0;
        for(int i=0;i<n-1;i++)
            sum+=i;
        printf("%lld\n",sum);
    }
}
```

![](https://img-blog.csdnimg.cn/20181118173400342.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-5
#define long long LL
using namespace std;
const int maxn=1e3+10;
int a[maxn];
int b[maxn];
int cnt[maxn*10];
bool vis[maxn*10];
int main(){
   int n;
   scanf("%d",&n);
   memset(cnt,0,sizeof cnt);
   memset(vis,0,sizeof vis);
   for(int i=0;i<n;i++){
    scanf("%d",&a[i]);
    b[i]=a[i];
    cnt[a[i]]++;
   }
   sort(a,a+n);
   int nMIN=a[1],nMAX=a[n-2];
   for(int i=0;i<n;i++)
    printf("%d%c",a[i],i==n-1?'\n':' ');

   int num=unique(a,a+n)-a;
   set<int> s;
   s.clear();
   for(int i=0;i<num;i++)
    {
        s.insert(a[i]);
    }

    int k=0;
   for(int i=0;i<n;i++){
    if(!vis[b[i]]&&s.find(b[i])!=s.end()){
        printf("%d%c",b[i],(k+1)==num?'\n':' ');
        vis[b[i]]=true;
        k++;
    }
   }

   printf("%d %d\n",nMAX,nMIN);
   int MAX=*max_element(cnt,cnt+maxn*10);
   vector<int> v;
   for(int i=0;i<maxn*10;i++)
    if(cnt[i]>=MAX)
    v.push_back(i);
    int len=v.size();
    for(int i=0;i<len;i++)
        printf("%d%c",v[i],i==len-1?'\n':' ');
}
```

![](https://img-blog.csdnimg.cn/20181118174031593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
//poj 3468
#include <iostream>
#include <cstdio>
#define __int64 long long
using namespace std;
const int N = 1e5+10;
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
 
__int64 sum[N<<2],add[N<<2];
struct Node
{
    int l,r;
    int mid()
    {
        return (l+r)>>1;
    }
} tree[N<<2];
 
void PushUp(int rt)
{
    sum[rt] = sum[rt<<1] + sum[rt<<1|1];
}
 
void PushDown(int rt,int m)
{
    if(add[rt])
    {
        add[rt<<1] += add[rt];
        add[rt<<1|1] += add[rt];
        sum[rt<<1] += add[rt] * (m - (m>>1));
        sum[rt<<1|1] += add[rt] * (m>>1);
        add[rt] = 0;
    }
}
 
void build(int l,int r,int rt)
{
    tree[rt].l = l;
    tree[rt].r = r;
    add[rt] = 0;
    if(l == r)
    {
        sum[rt]=0;
        return ;
    }
    int m = tree[rt].mid();
    build(lson);
    build(rson);
    PushUp(rt);
}
 
void update(int c,int l,int r,int rt)
{
    if(tree[rt].l == l && r == tree[rt].r)
    {
        add[rt] += c;
        sum[rt] += (__int64)c * (r-l+1);
        return;
    }
    if(tree[rt].l == tree[rt].r) return;
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    if(r <= m) update(c,l,r,rt<<1);
    else if(l > m) update(c,l,r,rt<<1|1);
    else
    {
        update(c,l,m,rt<<1);
        update(c,m+1,r,rt<<1|1);
    }
    PushUp(rt);
}
 
__int64 query(int l,int r,int rt)
{
    if(l == tree[rt].l && r == tree[rt].r)
    {
        return sum[rt];
    }
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    __int64 res = 0;
    if(r <= m) res += query(l,r,rt<<1);
    else if(l > m) res += query(l,r,rt<<1|1);
    else
    {
       res += query(l,m,rt<<1);
       res += query(m+1,r,rt<<1|1);
    }
    return res;
}
 
int main()
{
    int n,m;
    while(~scanf("%d %d",&n,&m))
    {
        build(1,n,1);
        for(int i=1;i<=m;i++)
        {
            int x;
            scanf("%d",&x);
            update(1,1,x,1);
        }
        for(int i=1;i<=n;i++)
            printf("%lld\n",query(i,i,1));
    }
    return 0;
}
```

![](https://img-blog.csdnimg.cn/20181118174427894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
#include<cstdio>
#include<cmath>
#include<cstdlib>
using namespace std;
typedef struct
{
    int x;
    int y;
}  square;
square sq[1010];
int n;
bool find1(int a,int b)
{
    for(int i=0;i<n;i++)
    {
        if(sq[i].x==a&&sq[i].y==b)
        return true;
    }
    return false;
}
int main()
{
    int t,x,y,i,j,k,cnt;
    scanf("%d",&t);
    while(t--)
    {
        cnt=0;
        scanf("%d",&n);
        for(i=0;i<n;i++)
        {
            scanf("%d%d",&sq[i].x,&sq[i].y);
        }
        for(i=0;i<n;i++)
        {
            for(j=i+1;j<n;j++)
            {
                if(abs(sq[i].x-sq[j].x)==abs(sq[i].y-sq[j].y)&&find1(sq[i].x,sq[j].y)&&find1(sq[j].x,sq[i].y))
                cnt++;
            }
        }
        printf("%d\n",cnt/2);
    }
    return 0;
}
```

![](https://img-blog.csdnimg.cn/20181118174630964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

答案显然和R、G、B的数量有关
- 对于R＞ｎ，必然有两个两个R处于同一列
- 对于２＜R＜＝ｎ，用构造法可以证明是可以的
- 对于R＝２，只有【R，G】或者【R，B】（B和G不能同时存在）两种显然是可以，R必须用一条斜线将B和G划分为两个区域，显然G，B都是奇数
- 对于R＜２，只有【R，G】或者【R，B】（B和G不能同时存在）两种显然是可以

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-5
#define long long LL
using namespace std;
const int maxn=1e4+10;
char str[2][maxn];
int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        scanf("%s%s",str[0],str[1]);
    /*
        if(n==1){
            if(str[0][0]=='R'&&str[1][0]=='R'||
               str[0][0]=='G'&&str[1][0]=='B'||
               str[0][0]=='B'&&str[1][0]=='G')
                printf("NO\n");
            else
                printf("YES\n");
            continue;
        }
    */
        //统计个数
        int R=0,G=0,B=0;
        for(int i=0;i<n;i++){
                if(str[0][i]=='R')  R++;
                if(str[0][i]=='G')  G++;
                if(str[0][i]=='B')  B++;

                if(str[1][i]=='R')  R++;
                if(str[1][i]=='G')  G++;
                if(str[1][i]=='B')  B++;
        }
        //R>n，必然有两个R相邻
        if(R>n){
            printf("NO\n");
            continue;
        }
        //用构造法可以证明
        if(R>2&&R<=n){
            printf("YES\n");
            continue;
        }
        //R=2，只有【R，G】或者【R，B】两种显然是可以的
        //R必须用一条斜线将B和G划分为两个区域，显然G，B都是奇数
        if(R==2){
            if(G==0||B==0||(G&1)&&(B&1))
                printf("YES\n");
            else
                printf("NO\n");
            continue;
        }
        //R=0，显然只有B或者G
        //R=1，显然只有B或者G
        if(R<2){
            if(G&&B)
                printf("NO\n");
            else
                printf("YES\n");
            continue;
        }
    }
    return 0;
}
```

![](https://img-blog.csdnimg.cn/20181118174704891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```cpp
//I
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
typedef pair<int,int> p;
bool cmp(p a,p b){
    return a.second<b.second;
}
p num[maxn];
int Rank[maxn];
int pre[maxn];//par[i]表示i的根节点
//初始化
void init(int n)
{
    for(int i=0;i<n;i++){
        Rank[i]=0;
        pre[i]=i;
    }
}
//并查集查找函数
int findset(int x){
    if(pre[x]==x)
        return pre[x];
    return pre[x]=findset(pre[x]);
}
//并查集合并函数
void unionset(int x,int y){
    int fx=findset(x),fy=findset(y);
    if(fx==fy)
        return ;
    if(Rank[x]<Rank[y]){
        pre[fx]=fy;
    }else{
        pre[fy]=fx;
        if(Rank[x]==Rank[y])
            Rank[x]++;
    }
}
int main(){
    int n,T;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            int x;
            scanf("%d",&x);
            num[i]=p(i,x);
        }
        sort(num,num+n,cmp);
        init(n);
        for(int i=0;i<n;i++)
            unionset(i,num[i].first);
        int ans=0;
        set<int> s;
        //set<int>::iterator it;
        for(int i=0;i<n;i++){
            int father=findset(i);
            if(s.find(father)!=s.end())
                ans++;
            else
                s.insert(father);
        }
        printf("%d\n",ans);
    }
    return 0;
}
```





