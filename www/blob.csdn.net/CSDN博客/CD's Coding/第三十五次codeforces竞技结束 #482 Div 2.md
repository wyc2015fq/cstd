# 第三十五次codeforces竞技结束 #482 Div 2 - CD's Coding - CSDN博客





2018年05月15日 21:33:41[糖果天王](https://me.csdn.net/okcd00)阅读数：76








## 0x00 前言

感觉上次打了一场热身之后，脑子都转的灵活些了，果然一定要多写题防生锈， 

这次两个小时的CF Div2，打了一个半小时不到出了三题，第四题恕我看题意都看睡着了， 

然后抱着出了三题的喜悦去休息了，早上起来一看，FST了……QAQ 

原因居然是写完了代码之后脑子有问题的加了一个特判，当时脑子里想的是，加个速加个速，加毛线啊QAQ 

不要这个特判才是对的呀！
![emm](https://img-blog.csdn.net/20180515203109486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 0x01 A. Pizza, Pizza, Pizza!!!

> 
题目传送门：[http://codeforces.com/contest/979/problem/A](http://codeforces.com/contest/979/problem/A)


### 题目大意

有一张饼（假如是个蛋糕就难咯，给出题者们提个思路，改成蛋糕然后去Div1当A题）和N个小盆友， 

你想把这个饼平均分给小朋友们，自己也要吃，即（N+1）份， 

每次切一刀，要么切一个半径要么切一个直径，问最少切几刀可以完成。
### 解题思路

对于平均分一个圆这种事，简单来说就是平分一个360°的过程， 

当份数为奇数的时候，不可以任何作为直径的一刀，只能切（N+1）个半径把饼分成（N+1）份， 

特别的，如果N+1=1的时候，即只有你一个人就不用切了，是0刀， 

当份数为偶数的时候，切一刀直径平分饼为两半，此后每一刀都是切直径， 

其中一半被分为（N+1）/2份时，另一半也有这么多份。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-14
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int main()
{
    ll n=0; cin>>n; 
    if(n==0) cout<<0<<endl;
    else cout<<(n%2==1?(n+1)>>1:n+1)<<endl;
    return 0;
}
```

## 0x02 B. Treasure Hunt

> 
题目传送门：[http://codeforces.com/contest/979/problem/B](http://codeforces.com/contest/979/problem/B)


### 题目大意

三个人各有一个字符串，仅包含小写字符或大写字符， 

这个字符串的美丽程度表示为：相同子串出现次数的最大值， 

三个人可以修改N次，每次修改其中任一字符改成任一**其他**字符， 

问修改N次之后，谁的字符串的美丽程度最高，如果有并列最高，为平局。
### 解题思路

这个美丽程度的定义应该是令很多人迷惑的地方，题意描述中：

> 
For example,  

  the ribbon aaaaaaa has the beauty of 7 because its subribbon a appears 7 times,  

  and the ribbon abcdabc has the beauty of 2 because its subribbon abc appears twice.
即“abcdabc”的美丽程度为2，因为其中有两个“abc”子串， 

于是我有一小会儿也陷入了该如何判断一个字符串的美丽程度的纠结中， 

其实，美丽程度就是出现最多的字符的出现次数呀！
证： 

如果出现数最多的重复子串中包含重复字符，那么这个子串就不是出现数最多的重复子串， 

该重复字符的出现次数为（子串出现次数 $\times$ 该字符出现字数） 

如果出现数最多的重复子串不包含重复字符，那么这个子串的出现次数与其中任一字符出现次数相同。 

证毕。
那这道题所有人的修改目标就变为： 
**找到字符串中出现次数最多的字符，每次都把非该字符的一个改成该字符。**

若一个字符串 $s$ 的美丽程度为 $K$，那么 $N$ 次修改之后（其中$N > 1$）， 

美丽程度为 $min( len(s), K+N)$，这里为什么要单独把 $N = 1$ 排除开讨论呢？ 

当 $N > 1$ 的时候，对于一个字符，前面的 $N - 1$ 步随便换成什么总可以在最后一步换回原字符， 

但是如果只能换一次，会出现换不回来的情况，尤其是在源字符串本来就是类似 aaaaaa 这样全相同的时候， 

必须换一次反而会导致美丽程度减少，从6变成了5，考虑这个特例之后，正确计算几个字符串在N次变化后的美丽程度， 

最后判断大小即可。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-14
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int cnt[3][60] = {0};
int max_cnt[3] = {0};
string name[4] = {"Kuro", "Shiro", "Katie", "Draw"};

int parse_color(char c)
{
    return (int)(c-'A');
}


int main()
{
    int n; cin>>n;
    string ribbon[3];
    cin>>ribbon[0]>>ribbon[1]>>ribbon[2];

    int len = ribbon[0].length();

    //if(n>=len-1)
    //{
    //  cout<<"Draw"<<endl;
    //  return 0;
    //}  就是多写了这么些东西！去掉就AC了！

    for(int i=0;i<len;i++)
    {
        for(int j=0;j<3;j++)
        {
            int cur_color = parse_color(ribbon[j][i]);
            cnt[j][cur_color]++;
            max_cnt[j] = max(cnt[j][cur_color], max_cnt[j]);
        }
    }
    for (int j=0;j<3;j++)
    {
        if (max_cnt[j]==len && n==1)  max_cnt[j] = len-1;
        else max_cnt[j] = min(len, max_cnt[j]+n);       
    }

    int ans = 3;
    for (int i=0;i<3;i++)
    {
        int _ans = i;
        for (int j=0;j<3;j++)
        {
            if(i==j) continue;
            else if(max_cnt[i]<=max_cnt[j]) _ans = 3;   
        }
        if(_ans!=3) ans = _ans;
    }

    //cout<<max_cnt[0]<<"|"<<max_cnt[1]<<"|"<<max_cnt[2]<<endl;
    cout<<name[ans]<<endl;
    return 0;
}
```

## 0x03 C. Kuro and Walking Route

> 
题目传送门：[http://codeforces.com/contest/979/problem/C](http://codeforces.com/contest/979/problem/C)


### 题目大意

有 $N$ 个镇子，镇与镇之间有 $N-1$ 条双向边的路径将他们连接成一棵树状结构。 

其中一个镇子叫做鲜花镇，还有个镇子叫做蜜蜂镇，如果你从经过鲜花镇，就会带上花香， 

带着花香经过蜜蜂镇的时候，你会被蛰的很惨。 

问有多少个（从任意小镇A—>到任意小镇B）的途经过程中可以不被蛰？
### 解题思路

也就是说，不能经过鲜花镇之后再经过蜜蜂镇， 

以下图为例，即蓝色区域里的点（包括鲜花镇）为起点，到达黄色区域里的点（包括蜜蜂镇）的路径， 

这样的路径必然会被蛰，其他的路径就都安全。 

那么所有可以不被蛰的（镇To镇）的数量为： 


$N \times (N-1) - (count_{flower} - count_{both} ) \times (count_{bee} - count_{both} ) $

其中：$N$ 为小镇数量 
$count_{flower}$ 为可以不经过蜜蜂镇直达鲜花镇的小镇数量 
$count_{bee}$ 为可以不经过鲜花镇直达蜜蜂镇的小镇数量 
$count_{both}$ 为可以不经过蜜蜂镇直达鲜花镇且可以不经过鲜花镇直达蜜蜂镇的小镇数量
![pic](https://img-blog.csdn.net/20180515212604216?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

那么这些个count该如何获得呢？DFS或者BFS之类的都可以，毕竟无环图。 

我使用的是BFS，需要注意搜索过程中遇到另一个特殊镇就终止即可。

### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-14
# ==========================================================================*/

#include <map>
#include <queue>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
map<int, vector<int> > route;

int n,x,y; 
int cnt_f=0, cnt_b=0, cnt_both=0;
int vis_f[300008] = {0};
int vis_b[300008] = {0};

void bfs_f(int start, int stop)
{
    int cnt=0;
    queue<int> q;
    while(!q.empty())q.pop();
    q.push(start);
    while(!q.empty())
    {
        int node = q.front();
        q.pop();
        vis_f[node]=1;
        cnt_f++;
        for(vector<int>::iterator iter=route[node].begin(); iter!=route[node].end(); iter++)
        {
            int cur = *iter;
            if(cur==stop) continue;
            if(vis_f[cur]==1) continue;
            q.push(cur);
        }       
    }
}

void bfs_b(int start, int stop)
{
    int cnt=0;
    queue<int> q;
    while(!q.empty())q.pop();
    q.push(start);
    while(!q.empty())
    {
        int node = q.front();
        q.pop();
        vis_b[node]=1;
        cnt_b++;
        if(vis_f[node]==1) cnt_both++;
        for(vector<int>::iterator iter=route[node].begin(); iter!=route[node].end(); iter++)
        {
            int cur = *iter;
            if(cur==stop) continue;
            if(vis_b[cur]==1) continue;
            q.push(cur);
        }       
    }
}

int main()
{
    cin>>n>>x>>y;
    route.clear();
    vector<int> empty;
    for(int i=0;i<n;i++)
        route[i] = empty;

    for(int i=0;i<n-1;i++)
    {
        int ta, tb;
        scanf("%d %d", &ta, &tb);
        route[ta].push_back(tb);
        route[tb].push_back(ta);
    }

    bfs_f(x, y);
    bfs_b(y, x);
    cout<< (ll)n*(ll)(n-1)-(ll)(cnt_f-cnt_both)*(ll)(cnt_b-cnt_both) << endl;
    return 0;
}
```














