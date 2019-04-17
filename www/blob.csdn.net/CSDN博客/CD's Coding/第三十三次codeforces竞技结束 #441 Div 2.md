# 第三十三次codeforces竞技结束 #441 Div 2 - CD's Coding - CSDN博客





2017年10月17日 18:14:54[糖果天王](https://me.csdn.net/okcd00)阅读数：257








## 0x00 前言

哇，真的是好久好久没有打Codeforces了呢…… 

距离上一次（[2015-04-14 23:47](http://blog.csdn.net/okcd00/article/details/45049565)）竟然已经过了两年半之久… 

嘛，总之……还凑合QvQ……
![codeforces](https://img-blog.csdn.net/20171017095231031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 0x01 A. Trip For Meal

> 
题目传送门：[http://codeforces.com/contest/876/problem/A](http://codeforces.com/contest/876/problem/A)


### 题目大意

有三个小伙伴，姑且称作A、B、C吧~  

A家到B家为a米，A家到C家要走b米，B家到C家要走c米； 

小熊维尼喜欢吃饭，要吃n顿饭，每一顿都会吃空当前所在的小伙伴家的全部蜂蜜，然后小熊会到另一个小伙伴家去吃下一顿，其中第一顿已经决定在A家（兔子家）吃着了； 

在某个小伙伴家吃饭的时候，另外两个小伙伴会把家里的食物补足（真是好盆友，感动到落泪）； 

小熊想知道，把n顿饭吃完，又想要走最短的路，那最短要走多少米呢？ 

（其中，n,a,b,c 取值范围均为 1~100）
### 解题思路

**DP: 状态dist[n][place]为第n顿饭在某个place吃的时候，所走的最短路程**


$ dist[n][place] = max(dist[n-1][other\_places]), place \in \{0,1,2\} $


$ dist[0][0]= 0,~and~dist[0][1]=dist[0][2]=+\infty $
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2017-10-16
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int dist[101][3]={0};

int main()
{
    ios::sync_with_stdio(false);
    int n,a,b,c;
    cin>>n>>a>>b>>c;
    dist[0][0] = 0;
    dist[0][1] = dist[0][2] = 10024;
    for(int i=1;i<n;i++)
    {
        dist[i][0] = min( dist[i-1][1] + a, dist[i-1][2] + b );
        dist[i][1] = min( dist[i-1][0] + a, dist[i-1][2] + c );
        dist[i][2] = min( dist[i-1][0] + b, dist[i-1][1] + c );
        //cout<< dist[i][0] << dist[i][1] << dist[i][2] <<endl;
    } 
    cout<< min(dist[n-1][0], min(dist[n-1][1], dist[n-1][2])) <<endl;
    return 0;
}
```

## 0x02 B. Divisiblity of Differences

> 
题目传送门：[http://codeforces.com/contest/876/problem/B](http://codeforces.com/contest/876/problem/B)


### 题目大意

给了n个整数，要求你取出其中的k个，使得这k个数中任意两两之间的差值可以被m整除。 

问能不能取出来，如果能，输出取出的k个数字（如果有多种取法，任选） 

（n,k,m的取值范围为1e5，这n个数字的取值范围为1e9）
### 解题思路

**这k个数中任意两两之间的差值可以被m整除** 等价于 **取k个对m取模相等的数字**

所以这道题就转化成了，是否存在某个模数，这个模数包含的数字个数不小于k，并输出。 

那么首先我们依次读入整数，每读入一个数字X： 

对m取模（X%m）的计数加一，同时记录模数（X%m）包含数X，X的个数加一； 

读完之后我们对每个模数进行检查（此处为防止TLE，使用STL中的map做稀疏处理）总个数是否足够k 

如果某模数足够k个了，则开始遍历这个模数下包含的数字，依次重复输出该数字直至用完，总数到k个停止。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2017-10-16
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef map<ll, int> mli;

class T
{
public:
    ll count;
    mli record;
    void __init(){
        this->count = 0;
        this->record.clear();
    }
};

typedef map<ll, T> mlt;

int main()
{
    ios::sync_with_stdio(false);
    mli a, rest;
    mlt rec;

    int n, k, m;
    cin>>n>>k>>m;

    ll tmp;
    for(int i=0;i<n;i++)
    {
        cin>>tmp;
        ll res = (tmp%m+m)%m;

        rest[res] += 1; 
        rec[res].count += 1;
        rec[res].record[tmp] += 1;
    }

    for(mlt::iterator it=rec.begin();it!=rec.end();++it)
    {
        ll c = it->first;
        T cur = it->second;

        if (cur.count < k) continue;
        mli r = cur.record;
        cout<<"Yes"<<endl;
        int kk=0;
        for (mli::iterator mit=r.begin(); mit!=r.end(); ++mit)
        {
            ll num = mit->first;
            int cnt = mit->second;
            // cout<<num<<"#"<<cnt<<endl;
            for(int _t=0;_t<cnt;_t++)
            {
                cout<<num<<" "; 
                kk++;
                if(k-kk==0) return 0;
            }
        }
    }
    cout<<"No"<<endl;
    return 0;
}
```

## 0x03 C. Classroom Watch

> 
题目传送门：[http://codeforces.com/contest/876/problem/C](http://codeforces.com/contest/876/problem/C)


### 题目大意

问：对于输入的正整数X，是否存在数字A，A的各位数字之和加上A等于X， 

如果没有，则输出0，如果有，输出个数，以及所有满足该条件的数字A。 

（X的取值范围是 1~1e9）
### 解题思路

鉴于X的范围在1~1e9（我错了一次就是因为看成了1e8，哭(┬＿┬)） 

九位数的各位数字之和加起来也不会超过81，所以我们反过来等价分析： 

是否存在数字B（1~81），X减去B的差各位数字之和恰好等于B。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2017-10-16
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int getDigitSum(ll x)
{
    ll ret = 0;
    while(x)
    {
        ret += x%10;
        x/=10;
    } 
    return ret;
}

int main()
{
    ios::sync_with_stdio(false);
    ll x; cin>>x;
    int ans = 0, answer[81] = {0};
    for(int vsum=1;vsum<=81;vsum++)
    {
        ll num = x-vsum;
        if(num <= 0) continue;
        if (vsum == getDigitSum(num))
        {
            answer[ans] = num;
            ans++;
        }
    }
    cout<<ans<<endl;
    sort(answer, answer+ans);
    for(int i=0;i<ans;i++)
    {
        cout<<answer[i]<<endl;
    }
    return 0;
}
```

## 0x04 D. Sorting the Coins

> 
题目传送门：[http://codeforces.com/contest/876/problem/D](http://codeforces.com/contest/876/problem/D)


### 题目大意

首先我们需要定义一个概念——**order难度系数**

对于一个硬币的排列，我们定义**order难度系数**如下： 

从左到右看一次当前硬币排列（计数+1）； 

每次看到当前硬币是反面朝上的而它右边的数字不是反面朝上的时候，交换这俩硬币的位置， 

如：  

XOXO -> OXXO -> OXOX（第一轮结束）->OOXX（第二轮结束）->（第三轮：check finished）；  

XOOO -> OOOX（第一轮结束）->（第二轮：check finished）； 

当某一次从头看到尾都没有交换硬币位置操作时（这次看的操作也是计数的），结束，return 总计数。
（话说回来，这难道不是只有0/1的冒泡排序么） 

HINT：大家是不是没看懂呀~那么这么理解，只有0和1的数列， 

做冒泡排序，经过n轮操作后排序完成，返回n+1， 

因为就算成功排序了，你还得再过一遍整个数列才知道排序完成了嘛。
有n个硬币，最初都是正面朝上的， 

给n个数字，其中每个数字a为其中一枚硬币的位置，即该次将第a枚硬币翻成反面朝上， 

问，每一次翻硬币后（最初全正面朝上时视作第0次翻硬币后）的**order难度系数**

（n的取值范围为 1~300k）
### 解题思路

**XOXO -> OXXO -> OXOX**
**OXOX -> OOXX**

实际上，我们看如上例子，就能发现： 

每一轮，实际上都是把一个距离右侧最近的X贴上队伍的过程， 

哈哈可能有点抽象，我是这么想的： 

硬币们的最右侧视为一个打饭窗口（吃货），X是小人，O是空地， 

那么每一轮实际上是一个还不在队伍里的小人走完空地贴上空地队尾的过程。
那么，我们就来解题吧~ 

最初，没有小人，自然order难度系数是1（看一遍，不操作，over）， 

每当增加一个小人，如果这个小人不在队伍里（即所在的小人序列不连通到最右端）， 

那自然就要多一次跑到队尾的过程，order难度加一， 

但如果这个小人直接就在队伍里了，或者和先前有的人构成了一个队伍， 

那么他们都变成了不用跑过来打饭的人，每一个新入队的会导致order难度减一。
理解了这一点之后这道题就容易多啦，看，代码是不是也特别浅显易懂~

### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2017-10-16
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int a[300033]={0};
int m[300033]={0};

int main()
{
    ios::sync_with_stdio(false);
    int n; scanf("%d", &n);
    int tmp, ans = 1, rig = n;

    for(int i=1;i<=n;i++)
        scanf("%d", &a[i]);
    printf("1 ");
    for(int i=1;i<=n;i++)
    {
        m[a[i]]=1;
        ans++;

        if (i==n)
        {
            printf("1");
            break;
        }

        while(m[rig]==1)
        {
            rig--;
            ans--;
        }
        printf("%d ", ans);
    }
    return 0;
}
```















