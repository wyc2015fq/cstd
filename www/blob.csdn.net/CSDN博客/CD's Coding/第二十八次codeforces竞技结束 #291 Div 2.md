# 第二十八次codeforces竞技结束 #291 Div 2 - CD's Coding - CSDN博客





2015年02月22日 22:35:26[糖果天王](https://me.csdn.net/okcd00)阅读数：682











![](http://st.codeforces.com/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.ru/contest/514/problem/A)|[Chewbaсca and Number](http://codeforces.ru/contest/514/problem/A)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x3704|
|[B](http://codeforces.ru/contest/514/problem/B)|[Han Solo and Lazer Gun](http://codeforces.ru/contest/514/problem/B)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2790|
|[C](http://codeforces.ru/contest/514/problem/C)|[Watto and Mechanism](http://codeforces.ru/contest/514/problem/C)standard input/output3 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x895|
|[D](http://codeforces.ru/contest/514/problem/D)|[R2D2 and Droid Army](http://codeforces.ru/contest/514/problem/D)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x744|





这次在C题上纠结的有点多了浪费了不少时间……

[](http://codeforces.ru/blog/entry/16398)


Codeforces Round #291 (Div. 2)













Luke Skywalker gave Chewbacca an integer number *x*. Chewbacca isn't good at numbers but he loves inverting digits in them. Inverting digit *t* means
 replacing it with digit 9 - *t*.


Help Chewbacca to transform the initial number *x* to the minimum possible positive number
 by inverting some (possibly, zero) digits. The decimal representation of the final number shouldn't start with a zero.




Input


The first line contains a single integer *x*(1 ≤ *x* ≤ 1018) —
 the number that Luke Skywalker gave to Chewbacca.




Output


Print the minimum possible positive number that Chewbacca can obtain after inverting some digits. The number shouldn't contain leading zeroes.




Sample test(s)




input
27




output
22




input
4545




output
4444












关于这道题我只想说“哈哈哈哈哈哈我居然WA了4次哈哈哈哈…… ”



其实读题真的是很重要的事情，The number shouldn't contain leading zeroes 意思是，数字不含前导零…… 没看见……

题意是说，给你一个数字，你可以将其中任意数位的数字变成（9-当前数字），需要获得一个最小的数（但是不含前导零！）。

所以基本上就是一个把大于等于5的都变成9和当前数字的差，然后注意首位如果是0请改成9，即可…… 叹气，不审题害死人呐

### Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	string s;	cin>>s;
	for(int i=0;i<s.length();i++)
	{
		int now=s[i]-'0';
		if(i==0 && (now==0 || now==9)) cout<<9;
		else cout<<Min(now,9-now);
	}
	return 0;
}
```






There are *n* Imperial stormtroopers on the field. The battle field is a plane with Cartesian coordinate system. Each stormtrooper is associated with his
 coordinates (*x*, *y*) on this plane.


Han Solo has the newest duplex lazer gun to fight these stormtroopers. It is situated at the point (*x*0, *y*0).
 In one shot it can can destroy all the stormtroopers, situated on some line that crosses point (*x*0, *y*0).


Your task is to determine what minimum number of shots Han Solo needs to defeat all the stormtroopers.


The gun is the newest invention, it shoots very quickly and even after a very large number of shots the stormtroopers don't have enough time to realize what's happening and change their location.




Input


The first line contains three integers *n*, *x*0 и *y*0 (1 ≤ *n* ≤ 1000,  - 104 ≤ *x*0, *y*0 ≤ 104)
 — the number of stormtroopers on the battle field and the coordinates of your gun.


Next *n* lines contain two integers each *x**i*, *y**i* ( - 104 ≤ *x**i*, *y**i* ≤ 104)
 — the coordinates of the stormtroopers on the battlefield. It is guaranteed that no stormtrooper stands at the same point with the gun. Multiple stormtroopers can stand at the same point.




Output


Print a single integer — the minimum number of shots Han Solo needs to destroy all the stormtroopers.




Sample test(s)




input
4 0 0
1 1
2 2
2 0
-1 -1




output
2




input
2 1 2
1 1
1 0




output
1






Note


Explanation to the first and second samples from the statement, respectively:
![](http://espresso.codeforces.com/ff3de9a248db659e5c5faf87282bbfa75c573e12.png)






这道题比起A对于参赛者而言倒是最简单AC的一个，告诉你雷射炮的坐标和敌人的坐标，问你多少枪能把他们全干掉。

题意解析的话大概是这么个情况，从炮台坐标开始画多少条直线能穿透所有已知点。

再优化一下就相对坐标系一下，就是把炮台所在点视为原点，那么斜率相同的（没有斜率的也互相称作相同）即为在同一次攻击内被干掉。

一旦有除法要考虑精度浮点数判断相等的时候就很头疼，于是我用了分数表示斜率的方法，用map存一个<int,int>来代表 n/m 这样一个分数的斜率，化为最简分数即可。

我的这种方法需要考虑到的为以下问题：

1、 其中有一个为0的时候，因为0和非0数组成的分数，要么就是无斜率的0分母斜率，要么就是求最大公约数没办法把各种分之0化为相同的。于是一旦遇到一个为0，另一个为非0，我们就把他变成 0/1 和 1/0 的形式存即可。

2、 都为0的时候，您是不是怎么打都会死……

3、 这里我用的是map，很棒的STL，如果有不懂的可以这么理解，map是一个数组，但是下标可以为任何的东西，此处我用的下表是点对，就是分数。m[make_pair(a,b)]就是下标为<a,b>的意思。

### Code：



```cpp
#include <map>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int x[1001]={0};
int y[1001]={0};
map<pair<int,int>,int> m;

int main()
{
	int n;	cin>>n;
	cin>>x[0]>>y[0];
	for(int ni=1;ni<=n;ni++) 
	{
		scanf("%d%d",&x[ni],&y[ni]);
		x[ni]=x[ni]-x[0],y[ni]=y[ni]-y[0];
		int g=__gcd(x[ni],y[ni]);
		if(g==0) g=Max(x[ni],y[ni]);
		m[make_pair(x[ni]/g,y[ni]/g)]++;
	}
	cout<<m.size();
	return 0;
}
```














Watto, the owner of a spare parts store, has recently got an order for the mechanism that can process strings in a certain way. Initially the memory of the mechanism is filled with *n* strings.
 Then the mechanism should be able to process queries of the following type: "Given string *s*, determine if the memory of the mechanism contains string *t* that
 consists of the same number of characters as *s* and differs from *s* in
 exactly one position".


Watto has already compiled the mechanism, all that's left is to write a program for it and check it on the data consisting of *n* initial lines and *m* queries.
 He decided to entrust this job to you.




Input


The first line contains two non-negative numbers *n* and *m* (0 ≤ *n* ≤ 3·105, 0 ≤ *m* ≤ 3·105)
 — the number of the initial strings and the number of queries, respectively.


Next follow *n* non-empty strings that are uploaded to the memory of the mechanism.


Next follow *m* non-empty strings that are the queries to the mechanism.


The total length of lines in the input doesn't exceed 6·105.
 Each line consists only of letters 'a', 'b','c'.




Output


For each query print on a single line "YES" (without the quotes), if the memory of the mechanism contains the required string, otherwise print "NO"
 (without the quotes).




Sample test(s)




input
2 3
aaaaa
acacaca
aabaa
ccacacc
caaac




output
YES
NO
NO












题意的话是在说，先给你n个字符串，然后进行m次询问，每次询问一个字符串，问之前给出的n个字符串内有没有字符串可以在刚好改动一个字母的情况下获得当前字符串。



这道题，出题人给的解题方法是Hash，我个人原先的想法是字典树建一棵树，每次询问从root开始向下找目标节点，有一次非相同机会，如果找不到已有节点则使用，并在当前节点bfs，但这种方法有问题，比如在某节点其实应该认作不同，但字典树中刚好有另一个字符串的此位置有这个字母，就这辈子都找不到了……果然不能这样。那么Hash应该怎么做呢？

解题报告是这么说的：

While adding a string to the set, let's count its polynomial hash and add it to an array. Then let's sort this array.
 Now, to know the query answer, let's try to change every symbol in the string and check with binary search if its hash can be found in the array (recounting hashes with O(1) complexity).
 If the hash is found in the array, the answer is "YES", otherwise "NO".

简单的说，hash，排序，询问的字符串每个字母都变，找hash是否存在。over

### Code：



```cpp
#include <bits/stdc++.h>
typedef long long int lnt;
typedef double dou;
using namespace std;
#define N 600514
int nx[N*2][3],spt;
int newnode(){
	for(int i=0;i<3;i++)nx[spt][i]=0;
	return spt++;
}
void add(char*s,int p,int*st,int&top){
	top=0;
	st[top++]=p;
	for(int i=0;s[i];i++){
		int a=s[i]-'a';
		if(nx[p][a]==0)nx[p][a]=newnode();
		p=nx[p][a];
		st[top++]=p;
	}
}
set<pair<int,int> >mp[3];
int n,m;
char s[N];
int r1,r2;
int st1[N],t1;
int st2[N],t2;
int solve(){
	if(scanf("%d %d",&n,&m)==EOF)return 0;
	spt=1;
	r1=newnode();
	r2=newnode();
	for(int i=0;i<n;i++){
		scanf("%s",s);
		add(s,r1,st1,t1);
		int l=0;for(;s[l];l++);
		for(int j=0;j+j<l;j++)swap(s[j],s[l-1-j]);
		add(s,r2,st2,t2);
		for(int j=0;j<t1-1;j++){
			//printf("%d %d %d\n",st1[j],st2[t1-2-j],s[j]-'a');
			mp[s[l-1-j]-'a'].insert(pair<int,int>(st1[j],st2[t1-2-j]));
		}
	}
	////////////////////
	for(int i=0;i<m;i++){
		scanf("%s",s);
		add(s,r1,st1,t1);
		int l=0;for(;s[l];l++);
		for(int j=0;j+j<l;j++)swap(s[j],s[l-1-j]);
		add(s,r2,st2,t2);
		int flag=0;
		for(int j=0;j<t1-1;j++){
			int a=s[l-1-j]-'a';
			//printf("(%d %d %d)\n",st1[j],st2[t1-2-j],a);
			if(a!=0&&mp[0].find(pair<int,int>(st1[j],st2[t1-2-j]))!=mp[0].end()){flag=1;break;}
			if(a!=1&&mp[1].find(pair<int,int>(st1[j],st2[t1-2-j]))!=mp[1].end()){flag=1;break;}
			if(a!=2&&mp[2].find(pair<int,int>(st1[j],st2[t1-2-j]))!=mp[2].end()){flag=1;break;}
		}
		puts(flag?"YES":"NO");
	}
	mp[0].clear();
	mp[1].clear();
	mp[2].clear();
	return 1;
}
int main(){
	while(solve());
	return 0;
}
```


字典树的话其实也是行得通的，建立字典树进行dfs即可（我比赛的时候为啥要用bfs……哭）

### Code：



```cpp
#include<bits/stdc++.h>
const int maxnode=6e5+100;
const int sigma_size=3;
using namespace std;
int n,m;
int ch[maxnode][sigma_size],val[maxnode];
int sz;
void init(){
    memset(ch,0,sizeof ch);
    memset(val,0,sizeof(val));
    sz=1;
}
void insert(char *s)
{
    int u=0,l=strlen(s);
    for(int i=0; i<l; ++i) {
        int c=s[i]-'a';
        if(!ch[u][c]) {
            ch[u][c]=sz++;
        }
        u=ch[u][c];
    }
    val[u]=1;
}
char s[maxnode];
bool dfs(char *s,int rt,int p,int d,int l)
{
    if(d>1) return false;
    if(p==l){
        if(d==1&&val[rt]) return true;
        return false;
    }
    for(int i=0;i<sigma_size;++i){
        if(ch[rt][i]){
            if(dfs(s,ch[rt][i],p+1,d+(i!=s[p]-'a'),l)) return true;
        }
    }
    return false;
}

int main()
{
    init();
    scanf("%d%d",&n,&m);
    for(int i=0; i<n; ++i) {
        scanf("%s",s);
        insert(s);
    }
    for(int i=0; i<m; ++i) {
        scanf("%s",s);
        if(dfs(s,0,0,0,strlen(s))) printf("YES\n");
        else printf("NO\n");
    }

    return 0;
}
```















An army of *n* droids is lined up in one row. Each droid is described by *m* integers *a*1, *a*2, ..., *a**m*,
 where *a**i*is
 the number of details of the *i*-th type in this droid's mechanism. R2-D2 wants to destroy the sequence of consecutive droids of maximum length. He has *m* weapons,
 the *i*-th weapon can affect all the droids in the army by destroying one detail of the *i*-th
 type (if the droid doesn't have details of this type, nothing happens to it).


A droid is considered to be destroyed when all of its details are destroyed. R2-D2 can make at most *k*shots. How many shots from the weapon of what type
 should R2-D2 make to destroy the sequence of consecutive droids of maximum length?




Input


The first line contains three integers *n*, *m*, *k* (1 ≤ *n* ≤ 105, 1 ≤ *m* ≤ 5, 0 ≤ *k* ≤ 109)
 — the number of droids, the number of detail types and the number of available shots, respectively.


Next *n* lines follow describing the droids. Each line contains *m* integers *a*1, *a*2, ..., *a**m* (0 ≤ *a**i* ≤ 108),
 where *a**i* is
 the number of details of the *i*-th type for the respective robot.




Output


Print *m* space-separated integers, where the *i*-th
 number is the number of shots from the weapon of the *i*-th type that the robot should make to destroy the subsequence of consecutive droids of the maximum
 length.


If there are multiple optimal solutions, print any of them.


It is not necessary to make exactly *k* shots, the number of shots can be less.




Sample test(s)




input
5 2 4
4 0
1 2
2 1
0 2
1 3




output
2 2




input
3 2 4
1 2
1 3
2 2




output
1 3






Note


In the first test the second, third and fourth droids will be destroyed.


In the second test the first and second droids will be destroyed.












关于D……没来的及看，当时卡C太狠了，但是似乎是个队列的问题，先存个代码和解题报告备看




To destroy all the droids on a segment of *l* to *r*,
 we need to make sum_m(Max_r cnt[i][j])  shots, where *cnt*[*i*][*j*] —
 number of *j*-type
 details in *i*-th
 droid. Let's support two pointers — on the beginning and on the end of the segment, which we want to destroy all the droids on. If we can destroy droids on current segment, let's increase right border of the segment, otherwise increase left border, updating
 the answer after every segment change. Let's use a queue in order to find the segment maximum effectively.


### Code：



```cpp
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#define ls rt << 1
#define rs rt << 1 | 1
#define lson ls, l, m
#define rson rs, m + 1, r
#define getm int m = (l + r) >> 1
#define LL long long
#define ULL unsigned long long
#define pii pair <LL, LL>

using namespace std;

const int N = 100005, mod = 1e9 + 7, M = 100;
int n, m, k;
int q[N][6], a[N][6], r[6], f[6], e[6];

inline void inq (int id, int x) {
    while (f[id] <= e[id] && a[x][id] >= a[q[e[id]][id]][id]) e[id]--;
    q[++e[id]][id] = x;
}

inline void outq (int id, int x) {
    if (q[f[id]][id] <= x) f[id]++;
}

inline int jug () {
    int sum = 0;
    for (int i = 1; i <= m; i++) {
        sum += a[q[f[i]][i]][i];
    }
    return sum > k;
}

inline int read() {
    int x=0; char ch=getchar();
    while(ch<'0'||ch>'9'){ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x;
}

int main () {
//    freopen ("in.txt", "r", stdin);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            a[i][j] = read ();
        }
    }
    for (int i = 1; i <= m; i++) f[i] = 0, e[i] = -1;
    int res = 0;
    int pre = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            inq (j, i);
        }
        while (pre <= i && jug ()) {
            for (int j = 1; j <= m; j++) outq(j, pre);
            pre++;
        }
        if (res < i - pre + 1) {
            res = i - pre + 1;
            for (int j = 1; j <= m; j++) r[j] = a[q[f[j]][j]][j];
        }
    }
    for (int i = 1; i <= m; i++) {
        cout << r[i] << ' ';
    }
}
```










