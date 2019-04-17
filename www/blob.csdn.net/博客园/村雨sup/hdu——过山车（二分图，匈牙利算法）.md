# hdu——过山车（二分图，匈牙利算法） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# 过山车

**Time Limit: 1000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 26112    Accepted Submission(s): 11310**



Problem Description

RPG girls今天和大家一起去游乐场玩，终于可以坐上梦寐以求的过山车了。可是，过山车的每一排只有两个座位，而且还有条不成文的规矩，就是每个女生必须找个个男生做partner和她同坐。但是，每个女孩都有各自的想法，举个例子把，Rabbit只愿意和XHD或PQK做partner，Grass只愿意和linle或LL做partner，PrincessSnow愿意和水域浪子或伪酷儿做partner。考虑到经费问题，boss刘决定只让找到partner的人去坐过山车，其他的人，嘿嘿，就站在下面看着吧。聪明的Acmer，你可以帮忙算算最多有多少对组合可以坐上过山车吗？





Input

输入数据的第一行是三个整数K , M , N，分别表示可能的组合数目，女生的人数，男生的人数。0<K<=1000
1<=N 和M<=500.接下来的K行，每行有两个数，分别表示女生Ai愿意和男生Bj做partner。最后一个0结束输入。





Output

对于每组数据，输出一个整数，表示可以坐上过山车的最多组合数。





Sample Input


6 3 3
1 1
1 2
1 3
2 1
2 3
3 1
0






Sample Output


3






Author

PrincessSnow





Source

[RPG专场练习赛](http://acm.hdu.edu.cn/search.php?field=problem&key=RPG%D7%A8%B3%A1%C1%B7%CF%B0%C8%FC&source=1&searchmode=source)





```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=510;

int line[MAXN][MAXN],used[MAXN],nxt[MAXN];

int k,n,m;
bool Find(int x)
{
    for(int i=1;i <= m;i++)
    {
        if(line[x][i]&&!used[i])
        {
            used[i] = 1;
            if(nxt[i] == 0||Find(nxt[i]))
            {
                nxt[i] = x;
                return true;
            }
        }
    }
    return false;
}

int match()
{
    int sum = 0;
    for(int i=1;i <= n;i++)
    {
        memset(used,0,sizeof(used));
        if(Find(i))sum++;
    }
    return sum;
}


int main()
{
    ios::sync_with_stdio(false);
    while(cin >> k&&k)
    {
        cin >> n >> m;
        memset(line,0, sizeof(line));
        memset(nxt,0,sizeof(nxt));
        while(k--)
        {
            int u,v;
            cin >> u >> v;
            line[u][v] = 1;
        }
        cout << match() << endl;
    }

    return 0;
}
```

https://www.bilibili.com/video/av16362938/

b站上的大佬讲的不错诶

记一个小坑：如果你用相同的字符定义了一个全局变量和局部变量时，编译器不会报错，但在oj上可能会出错。











