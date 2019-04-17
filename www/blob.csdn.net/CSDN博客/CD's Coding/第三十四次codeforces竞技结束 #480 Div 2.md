# 第三十四次codeforces竞技结束 #480 Div 2 - CD's Coding - CSDN博客





2018年05月10日 00:58:36[糖果天王](https://me.csdn.net/okcd00)阅读数：86








## 0x00 前言

时隔好久，又想着忙里偷闲打一场codeforces活动下快要生锈的脑袋 

emmmm，虽然手忙脚乱只出了三题，但实在是手生也还凑合了…… 

D题也有思路，日后再补上吧，把赛中做出的三题写一写，聊以纪录。 

这次的教训就是：以后打codeforces不要作死加 `ios::sync_with_stdio(false);`
![emm](https://img-blog.csdn.net/20180510002751175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 0x01 A. Links and Pearls

> 
题目传送门：[http://codeforces.com/contest/980/problem/A](http://codeforces.com/contest/980/problem/A)


### 题目大意

有一串珍珠项链，由珍珠`o`和链条`-`组成， 

问是否可以通过交换珍珠、链条的位置，令项链完美的达到这样一种平衡： 

每两个珍珠间的链条数相等（没有链条算每两个珍珠间都是0条，整个项链都没有珍珠也算平衡）
### 解题思路

有P个珍珠，L个链条…… 

P个珍珠可以把整个项链分成P段，要每段相等， 

那就等于在问L能不能被P整除，下一题。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2017 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-08
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
    ios::sync_with_stdio(false);
    int l=0, p=0;
    string necklace;
    cin>>necklace;
    for(int i=0;i<necklace.length();i++)
    {
        (necklace[i]=='o'?p:l)++;
    }
    cout<<((p==0 or l%p==0)?"YES":"NO")<<endl;
    return 0;
}
```

## 0x02 B. Marlin

> 
题目传送门：[http://codeforces.com/contest/980/problem/B](http://codeforces.com/contest/980/problem/B)


### 题目大意

一张4行N列（N是奇数）的表格，其中需要放K个地方不让通过（最外层一圈不让放），问： 

是否满足一种放法，左下到右上的最短路条数和右下到最上的最短路条数相等， 

如果有的话，输出YES并且随便打印出一种放法来。
### 解题思路

首先，我们可以知道，所谓最短路，因为最外圈不让有阻拦，所以贴边走一定是其中一条最短路了， 

也就是说，对于左下出发的最短路，必须每一步只能向上或向右，同理对于右下出发的最短路每一步只能向上或向左， 

即：只要你摆的是个轴对称图形（无论对称轴是上下方向还是左右方向的均可），就一定满足题意了。 

然后，$4 \times N$ 的格子……最外面一圈不让放，即可以放的区域： 

（其实题面上是说放酒店的区域，不明所以）的可选区域也只有 $2 \times (N-2)$

那不是就好办了，如果K是偶数，第二列开始起，我从左到右一列一列的摆完即可，必然上下两半轴对称； 

如果K是奇数，首先我把最中间一列（这里的K是奇数，必然有中间列）上面的那个放上酒店， 

中间列的下面那个就不放了（因为放满是偶数，只要是奇数必然放不满，下面那个空着即可）， 

剩下需要放置的酒店数量就是偶数了，然后以中轴为中心，沿着左右两个方向，轴对称的两个两个放。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-08
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int grid[100][200]={0};

int main()
{
    //ios::sync_with_stdio(false);
    int r=4, n=0, k=0;
    cin>>n>>k;
    memset(grid, 0, sizeof(grid[0]));
    int mid = n / 2; 
    int cnt = k / 2;
    int res = k % 2;
    grid[1][mid] = (res==1?1:0);
    if(res%2==1)
    {
        for(int i=1;i<=cnt;i++)
        {
            if(mid-i>0)
            {
                grid[1][mid-i] = 1;
                grid[1][mid+i] = 1; 
            }
            else if(i<n-2)
            {
                int d = n-2-i;
                //cout<<d<<endl;
                grid[2][mid-d] = 1;
                grid[2][mid+d] = 1; 
            }
            else
            {
                grid[2][mid] = 1;
            }
        }
    }
    else
    {
        for(int i=1;i<=cnt;i++)
        {
            grid[1][i] = 1;
            grid[2][i] = 1;
        }
    }

    cout<<"YES"<<endl;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout<<(grid[i][j]==1?"#":".");
        }
        cout<<endl;
    }
    return 0;
}

}
```

## 0x03 C. Posterized

> 
题目传送门：[http://codeforces.com/contest/980/problem/C](http://codeforces.com/contest/980/problem/C)


### 题目大意

灰度数据是0-255范围的整数，共256个， 

现在需要把他们分为若干组，每组的数必须连续且数量不得超过K个， 

组数不限，但长度受限，比如K为200的时候，我可以分成 $[0, 127]$ 和 $[128, 255]$， 

也可以分成 $[0, 199]$ 和 $[200, 255]$ ，或是 $[0, 100]$、$[101, 200]$ 和 $[201, 255]$， 

每一个组中随便选出一个数来表示这整组的值，如 $[0, 127]$ 中的每一个灰度值都可以用 $0$ 来表示 

给你N个灰度数据（0-255的整数），通过分组将该灰度数据的序列变成字典序最小的序列。
### 解题思路

首先，字典序最小？那不就是贪心了嘛！ 

字典序是指：两个序列相比，先比第一个，第一个谁小就是字典序较小的，如果一样，比第二个，以此类推。 

所以每次读入一个数我们就立刻输出满足条件的可以表示这个数的值就行啦~ 

那么需要注意哪几点呢？  

1） 可以表示当前数 $X$ 的最小数为 $max(0, X-K+1)$

2） 如果这个数在已经分好的区间里了，那就没有操作空间了，就是那个区间的表示数 

3） 对于当前数 $X$，在区间 $[X-K+1, X]$里如果有已经分好的区间，看能否容纳进去： 

3.1） 能容纳的要求是，该区间的最小值 $Y \in [X-K+1, X]$，则扩展原区间为 $[Y, X]$

3.2） 若不能容纳，则在区间 $[X-K+1, X]$里寻找最小的尚未被划入区间的数$S$，构造区间$[S, X]$，该区间的表示数为$S$
每次划分或扩展一个区间时，需要在一个hash表（我用的是list，0~255作下标）中进行记录 

便于 $O(1)$ 的复杂度检测步骤 2，每次扫到一个数就如此判断，输出该数所属区间或新开区间的最小值即可。

### Source Code

```cpp
#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int key[300] = {0};

int main()
{
    //ios::sync_with_stdio(false);
    memset(key, 0, sizeof(key[0]));
    int i,j;
    int n=0, k=0, p=0, ans=0;
    cin>>n>>k;

    for(i=0;i<300;i++)
    {
        key[i] = -1;
    }

    for(i=0; i<n; i++)
    {
        scanf("%d", &p);
        if(key[p] != -1)
        {
            ans = key[p];
        }
        else
        {
            int cur_key = 0;
            for(j=max(k,0);j>=0;j--)
            {
                int cur_pos = max(p-j+1, 0);
                if(key[cur_pos] != -1)
                {
                    int kval = key[cur_pos], kpos = cur_pos;
                    if(kpos-kval == k-1) continue;
                    else if(p-kval < k)
                    {
                        cur_key = kval;
                        break;
                    }
                }
                else
                {
                    cur_key = cur_pos;
                    break;
                }
            }
            ans = max(cur_key, 0);
            for(j=ans; j<=p; j++)
            {
                //cout<<"Marked "<<j<<" as "<<ans[i]<<endl;
                key[j] = ans;
            }
        }
        cout<<ans<<" ";
    }
    return 0;
}
```










