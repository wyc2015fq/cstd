# 2015 ACM-ICPC 上海赛区邀请赛 解题报告 (Silver) - CD's Coding - CSDN博客





2015年05月31日 21:41:16[糖果天王](https://me.csdn.net/okcd00)阅读数：2283








## 前言

ACM/ICPC(Association for Computing Machinery – International Collegiate Programming Contest, 美国计算机协会-国际大学生程序设计竞赛)是由国际计算机界历史悠久、颇具权威性的组织ACM主办的年度竞赛活动，是世界上公认规模最大、水平最高的国际大学生程序设计竞赛。经ACM/ICPC授权，华东理工大学于2015年5月23、24日举办“2015ACM国际大学生程序设计竞赛上海大都会赛”。


![](https://img-blog.csdn.net/20150530131052278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


正在敲的那个就是我啦~ 话说居然就给了我个背影 不开心！

学校新闻网给我们写的新闻~

http://news.cqu.edu.cn/news/article/show.php?itemid=68397





这次于2015年5月23日至24日在华东理工大学举行。来自重庆大学、上海交通大学、北京航空航天大学、复旦大学、中国科技大学等91个高校的190余支队伍参加此项赛事。

咳咳，那么严肃多不好玩，啦啦啦我们拿了银牌~~ 我们总共出了BEFGJ五道题，其中[吾辈](http://codeforces.com/profile/Royalmiki)出了BEF三道，G题出自@**[suyuxuan](http://codeforces.com/profile/SYX)**，J题出自@**[KingLizard](http://codeforces.com/profile/CST14-Kinglizard)**，他们的代码也将贴在下方，并辅以解释。虽然还远远不及前面巨巨们伟岸的身影，但我们也继续努力着，期望以后能获得更好的成绩。（如若下文中有说得不对的地方，请予以指出，我会在看到后第一时间进行修正，谢谢^_^）




# 解题报告

啊话说代码啥的我放到GitHub上了，也可以去那里看以及下载代码：

https://github.com/okcd00/ACM_Road/tree/master/ACM_Contest/Shanghai_20150525








## A. Article

QWZeng学长的解题报告：[传送门](http://blog.csdn.net/qingwen_z/article/details/46293513)




### 题意：



“*这是一道概率dp+贪心题；*

*建立状态dp[i]， 表示敲出i个字符的期望次数， 那么有 dp[i] = dp[i-1] + p*(1 + dp[i]) + (1-p);*

*解释一下： 敲出i个字符， 首先得敲出i-1个字符， 所以有第一部分的dp[i-1]； 然后敲下一个字符时， 有两种可能， p概率会丢失， （1-p)概率不会丢失, 对于丢失的情况就还得重新敲dp[i]次了（期望次数）， 不丢失的情况就只有一次就成功了， 所以是(1-p) * 1。*

*解出 dp[i] = （dp[i-1] + 1) / ( 1- p), dp部分就搞定了;*

*接下来是贪心部分； 我们能够看出来dp[i]的导数是递增的， 也就是说dp[i]随i的增大， dp[i]增大的越快。 所以如果对于i个字符分两部分来完成的话， 两个部分尽量均匀才是最优的(高中数学， 画个图像也很容易理解)； 所以我们可以枚举保存的次数k, 保存k次就相当于把n个字符分成k部分来完成， 由上面结论可知， 这k部分尽量均匀。*”   ——QWZeng

代码如下： By @[QWZeng](http://blog.csdn.net/qingwen_z)

### Code：



```cpp
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<queue>

using namespace std;
#define ll long long
#define inf 0x3f3f3f3f
const int maxn = 110010;

int n, x;
double p, dp[maxn];

double cal(int k) {
    int len = n/k;
    double ans = 0;
    if(n%k) {
        ans = (dp[len + 1] + x) * (n%k) + (dp[len] + x) * (k - n%k);
    }
    else {
        ans = (dp[len] + x) * k;
    }
    return ans;
}

int main() {
    int t; scanf("%d", &t);
    for(int cas=1; cas<=t; cas++) {
        printf("Case #%d: ", cas);
        scanf("%d%lf%d", &n, &p, &x);
        for(int i=1; i<=n+x; i++) dp[i] = (dp[i-1] + 1) / (1-p);

        int k = 1;
        for(; k<n; k++) {
            if(dp[k+1] <= (dp[k] + x + dp[1])) continue;
            else break;
        }
        double ans = dp[n] + x;
        for(int i=2; i<=n; i++) ans = min(ans, cal(i));
        printf("%.6f\n", ans);
    }
    return 0;
}
```





## B. Base64

### 题意：

这道题是一个编码题，以现行的Base64加密方法（似乎是隔壁信息安全班似乎密码学课最简单的作业之一），将给定的字符串str，加密k次，输出得到的结果。

关于Base64，实际上是这么个意思(默默说一句很多下载地址都是用这个方式加密的)：

在给定字符串中每次取三个字符，他们用ASCII码表示是3个8位的二进制位，分成4个六位的小段落，每个小段代表的数字在下表中找到对应的，这四个字符就是这三个字符的Base64加密后的字符，然而字符串最末如果不足3个字符，先将有的字符转换成二进制位，六个二进制位作为一个字符，如果某一个小段不足六位补0，从下一个小段起补‘=’号，所以Base64密文的长度一定是4的倍数。

![](https://img-blog.csdn.net/20150530133059168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


代码如下：

### Code：



```cpp
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
typedef long long ll;

const string codec = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int trans(string s)
{
	int ret=0;
	for(int i=0;i<s.length();i++)
	{
		ret=ret<<8;
		int t=(int)s[i];
		ret+=t;
	}
	return ret;
}

string Base64(string str, int k)
{
	if(k==0) return str;
	int i=0,vplus=str.length()%3,zero=0;
	if(vplus) for(int v=0;v<3-vplus;v++) str+=(char)zero;
	string ret="";
	for(i=0; i+3<=str.length(); i+=3)
	{
		int t=trans(str.substr(i,3));
		string tmp="";
		for(int j=0;j<4;j++)
		{
			int code=t%(1<<6);
			t/=(1<<6);
			tmp=codec[code]+tmp;
		}
		ret+=tmp;
	}
	if(vplus==0) return Base64(ret,k-1);
	else if(vplus==1) return Base64(ret.substr(0,ret.length()-2)+"==",k-1);
	else return Base64(ret,substr(0,ret.length()-1+"=",k-1);
}

int main()
{
	int T=0;	cin>>T;
	for(int _T=1;_T<=T;_T++)
	{
		int k;
		string str;
		scanf("%d",&k);
		getchar();
		getline(cin,str);
		cout<<"Case #"<<_T<<": "<<Base64(str,k)<<endl;
	}
	return 0;
}
```



这里我是直接不够的位数都补成0，然后根据字符串长度mod3的结果来将最后的1或2位置换为‘=’号

啊对了，以及少量的位操作。


## E. Exam

### 题意：

啊啦，这个是个大家都深有体会的考前复习算法（噗，这也能叫算法）。

以当前时刻起，告知有n场考试，每场考试有3个属性：r (Review: 通过这场考试所需要花费的时间)，e (Exam: 考试开始时间)，l (Last_time: 考试持续时间)

问：能不能通过所有的考试

这很明显是贪心吧（复习中谁用的不是贪心算法告诉我=-=）

我们用rest代表我们所有的空闲时间，（每场考试开始时间减去前一场考试的结束时间，第一场考试视作第零场考试结束于0时）无考试的时间为空闲，加入rest中，和这场考试所需要的复习时间比较，如果够了则在rest中减去所需要的复习时间，反之，这场考试挂科妥妥的。

代码如下：

### Code：



```cpp
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
typedef long long ll;

struct testBag
{
	ll r;
	ll e;
	ll l;
}test[100086];

int cmp(const void *_a, const coid *_b)
{
	struct testBag *a = (testBag *)_a;
	struct testBag *b = (testBag *)_b;
	return a->e - b->e;
}

int main()
{
	int T;	cin>>T;
	for(int _T=1;_T<=T;_T++)
	{
		int n=0;
		scanf("%d",&n);
		ll last=0, rest=0;
		int flag=1;	//can pass;
		for(int i=0;i<n;i++)
		{
			scanf("%lld%lld%lld",&test[i].r,&test[i].e,&test[i].l);
		}	qsort(test,n,sizeof(test[0]),cmp);
		for(int i=0;i<n;i++)
		{
			ll r,e,l;	//review,exam,last
			r=test[i].r;
			e=test[i].e;
			l=test[i].l;
			if(flag!=1) continue;
			rest += (e-last);
			if(rest<r) flag=0;
			else
			{
				rest-=r;
				last=e+l;
			}
		}
		printf("Case #%d: ",_T);
		if(flag==1) printf("YES\n");
		else printf("NO\n);
	}
	return 0;
}
```
用到了结构体排序，啊啦啊啦第一次在正式比赛用结构体排序，心里还是略微有点抖~~~



结构体排序是个很方便的东西，大家可以记录下来以后用~




## F. Friends



### 题意：

给了一个惊天地泣鬼神的Venn图，问有多少种满足这个Venn图的取值。

纠结了半天之后发现是32的n次方。

啊啦大数据走起，JAVA里的BigInteger大家要会用才行哦~

代码如下：

### Code：



```java
import java.util.Scanner;
import java.math.BigInteger;

public class Main
{
	public static void main(String[] args)
	{
		Scanner sc=new Scanner(System.in);
		int t=sc.nextInt();
		if(k==0) System.out.println("Case #"+z+": "+"1");
		else
		{
			BigInteger ans=BigInteger.valueOf(1);
			for(int i=1;i<=k;i++) ans=ans.multiply(BigInteger.valueOf(32));
			System.out.println("Case #"+z+": "+ans);
		}
	}
}
```





## G. Game

### 题意：



"题意就是一棵树，然后每个点上有一个值，每个值只能取一次


 然后进行k此搜索，每次只能从树根往下，不能回头


 问你最大能够取多少"——[Suyuxuan](http://my.csdn.net/suyuxuan94)





“解法的话，对于每个叶子节点设立一个潜力值，这个值等于从树根一直走到它能够取到的值





 根据潜力值给叶子进行倒叙排序


 之后，再从叶子向根上回溯即可，注意走过的点做标记，碰到标记了的点就停止


 再取前k大的求和即可”  ——[Suyuxuan](http://my.csdn.net/suyuxuan94)



代码如下（@Suyuxuan）

### Code:



```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#define rep(i,a,b) for (int i=a;i<((b)+1);i++)
#define Rep(i,a,b) for (int i=a;i>=b;i--)
#define foreach(e,x) for (__typeof(x.begin()) e=x.begin();e!=x.end();e++)
#define mid ((l+r)>>1)  //segement tree
#define lson (k<<1)     //segement tree
#define rson (k<<1|1)   //segement tree
#define MEM(a,x) memset(a,x,sizeof a)
#define L ch[r][0]                  //splay tree
#define R ch[r][1]                  //splay tree
#define keyvalue ch[ch[root][1]][0] //splay tree
using namespace std;
const int N=1000050;
const long long Mod=1e9+7;
const int inf=0x3f3f3f3f;
typedef pair<int, int> pii;
typedef long long ll;
int pnt[N],head[N],nxt[N],cnt,fa[N],a[N],n,m,k;
bool vis[N];
struct P{
    int u;ll w;
    P(){}
    P(int _u,ll _w) {
        u=_u;w=_w;
    }
};
void addedge(int u,int v) {
    pnt[cnt]=v;nxt[cnt]=head[u];head[u]=cnt++;
}
vector<P>   vt;
bool cmp(const P &a,const P &b) {
    return a.w>b.w;
}
void dfs(int u,int f,ll w) {
    bool flag=true;
    fa[u]=f;
    for (int i=head[u];i!=-1;i=nxt[i]) {
        int v=pnt[i];
        if (v==f)   continue;
        flag=false;
        dfs(v,u,w+a[v]);
    }
    if (flag)   vt.push_back(P(u,w));
}
int main(int argc, const char * argv[]) {
    int T;
    scanf("%d",&T);
    rep(z,1,T) {
        scanf("%d%d",&n,&k);
        MEM(head,-1);MEM(vis,false);
        cnt=0;
        rep(i,1,n)  scanf("%d",&a[i]);
        rep(i,1,n-1) {
            int u,v;
            scanf("%d%d",&u,&v);
            addedge(u, v);
            addedge(v, u);
        }
        dfs(1,1,a[1]);
        sort(vt.begin(),vt.end(),cmp);
        ll ans=0;
        priority_queue<ll,vector<ll>,less<ll> > p;
        rep(i,0,vt.size()-1) {
            P cur=vt[i];
            ll tmp=0;int u=cur.u;
            while (!vis[u]) {
                tmp+=a[u];
                vis[u]=true;
                u=fa[u];
            }
            p.push(tmp);
        }
        rep(i,1,k) {
            if (p.empty())  break;
            ans+=p.top();p.pop();
        }
        printf("Case #%d: %lld\n",z,ans);
    }
    return 0;
}
```











## J. Joyful

### 题意：





关于算法，KingLizard这样说：

“算每个点被选中的期望，然后加起来即可：对每个点，先算出不能覆盖该点的概率，然后求出k次都不能覆盖该点的概率，然后用1减”



代码如下（@KingLizard）

### Code:



```cpp
#include<cstdio>
#include<iostream>
#include<algorithm>
#define rep(i,a,b) for(int i=a;i<b+1;i++)
#define lson (k<<1)
#define rson (k<<1|1)
#define mid ((l+r)>>1)
using namespace std;
typedef long long ll;
int n,m,k;

double pow(double a,int b)
{
	double res=1.0;
	while(b>0)
	{
		if(b&1) res=res*a;
		a=a*a;
		b>>=1;
	}
	return res;
}

int main()
{
	int T;
	scanf("%d",&T);
	rep(z,1,T)
	{
		scanf("%d%d%d",&n,&m,&k);
		double ans=0.0;
		for(double x=1.0; x<=(double)n; x+=1)
		for(double y=1.0; y<=(double)m; y+=1)
		{
			double temp=0.0;
			temp+=(x-1)/n*(x-1)/n + (y-1)/m*(y-1)/m + (n-x)/n*(n-x)/n + (m-y)/m*(m-y)/m;
			temp-=(x-1)/n*(y-1)/m*(x-1)/n*(y-1)/m + (x-1)/n*(m-y)/m*(x-1)/n*(m-y)/m;
			temp-=(n-x)/n*(y-1)/m*(n-x)/n*(y-1)/m + (n-x)/n*(m-y)/m*(n-x)/n*(m-y)/m;
			double tmp=pow(temp,k);	 
			ans+=1-tmp;
		}
		printf("Case #%d: %.f\n",z,ans);
	}
	return 0;
}
```










