# 第三十五次codeforces竞技结束 #483 Div 2 - CD's Coding - CSDN博客





2018年06月12日 10:12:12[糖果天王](https://me.csdn.net/okcd00)阅读数：79








## 0x00 前言

啥都不说了，居然出了四题， 

令我不仅产生了错觉：老选手也能有朝一日恢复到刚开始集训时的状态？ 

觉得浑身充满了活力QvQ！（可惜了这次用的不是自己的号QvQ，220名呀……） 

那么这次比赛得到了一个怎样的教训呢？（看见那些TLE了么） 

——老老实实给我用 `printf()` 和 `scanf()`！ 

——竞技编程除非字符串处理，没有 `cin>>` 和 `cout<<` ！
![yeah](https://img-blog.csdn.net/20180611164255599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180611175713518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 0x01 A. Game

> 
题目传送门：[http://codeforces.com/contest/984/problem/A](http://codeforces.com/contest/984/problem/A)


### 题目大意

有n个数字，两个人轮流取数， 

先A后B，取一个少一个， 

A想让最后剩下的数字最小， 

B想让最后剩下的数字最大， 

问：最后剩下的是什么数字？
### 解题思路

A想让最后剩下的数字最小，那么就每次都取最大的数字， 

B想让最后剩下的数字最大，那么就每次都取最小的数字， 

哦，中位数啊…… 

然后先取的是A，所以如果是偶数的话，是最中间两个数右边那个， 

考虑到C++里整数除法是没有余数的，所以下标直接取 $(n-1)/2$ 就行了。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-15
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
    int a[1024] = {0};
    int n; cin>>n;
    for(int i=0;i<n;i++)
        scanf("%d", &a[i]);
    sort(a, a+n);
    cout<<a[(n-1)/2]<<endl;
    return 0;
}
```

## 0x02 B. Minesweeper

> 
题目传送门：[http://codeforces.com/contest/984/problem/B](http://codeforces.com/contest/984/problem/B)


### 题目大意

扫雷大家玩过没？ 
`*`代表雷，`.`代表当前位置的相邻格（八方向）都没有雷，视作 `0`

数字表示当前位置的相邻格有对应数量的雷数，如3表示八方向加起来共有3个雷。 

给出扫雷游戏的矩阵形状即行列数：n行，m列， 

然后给出一个 $n \times m$ 的矩阵，问这张图是不是一张正确的扫雷图。 

（正确基本上可以理解为每个数字写的都是对的，包括为0的格子）
### 解题思路

首先把`.`都换成`0`，然后遍历所有的雷，把每个雷的八方向格子都减一， 

然后所有”不是雷的格子“（这一点很重要）都是0的话就说明这个图为正确。

### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-15
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int n, m;
int bomb[128][128] = {0};
int field[128][128] = {0};

void set_effects(int x, int y)
{
    bomb[x][y] = 1;
    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            if(x+i>=n or x+i<0) continue; 
            if(y+j>=m or y+j<0) continue;
            if(i==0 and j==0) continue;
            field[x+i][y+j]--;
        }
    }
}

int main()
{
    char c;
    cin>>n>>m;
    memset(bomb, 0, sizeof(bomb[0][0]));
    memset(field, 0, sizeof(field[0][0]));
    for(int i=0;i<n;i++)
    {
        scanf("%c", &c);
        for(int j=0;j<m;j++)
        {
            scanf("%c", &c);
            //cout<<c-'0'<<endl;
            if (c=='*') set_effects(i,j);
            if (c-'0'>=0 && c-'0'<=8) field[i][j] += (c-'0');
        }
    }
    string ans = "YES";
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(field[i][j]!=0 and bomb[i][j]==0)
            {
                //cout<<i<<"|"<<j<<endl;
                ans = "NO";
                break;
            }
        }
        if(ans == "NO") break;
    }
    cout<<ans<<endl;
    return 0;
}
```

## 0x03 C. Finite or not?

> 
题目传送门：[http://codeforces.com/contest/984/problem/C](http://codeforces.com/contest/984/problem/C)


### 题目大意

每一行给出 p,q,b 三个数，问在b进制下，$ \frac{p}{q} $ 能否表示为有限小数。 

如 $p=6, q=12, b=10$ 时： $\frac{6}{12} = \frac{1}{2} = 0.5_{(10)}$

如 $p=6, q=12, b=2$ 时： $\frac{6}{12} = \frac{1}{2} = 0.1_{(2)}$
### 解题思路

首先，先将分数 $ \frac{p}{q} $ 约分，化为最简分数后去掉整数部分， 

在b进制下， $ \frac{p}{q} $ 能被表现为有限小数的充要条件为： 

分母 $q$ 能被基数 $b$ 的 $n$ 次幂 $b^{k}$ 整除。 

那么，该问题就等价于计算上述问题。
解题过程中需要用到这一条定理来简化计算： 
$ GCD(x, y)=GCD(x, y / GCD(x, y)) $

### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-15
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

ll get_gcd(ll m, ll n)
{
    ll temp;  
    if(m<n)  
    {  
        swap(m,n);  
    }  
    while((temp=m%n)!=0)  
    {  
        m=n;  
        n=temp;  
    }  
    return n;  
}

int main()
{
    int n; cin>>n;
    ll p,q,b; 
    for(int i=0;i<n;i++)
    {
        scanf("%I64d %I64d %I64d", &p, &q, &b);
        if(p==0)
        {
            printf("Finite\n");
            continue;
        }
        q /= get_gcd(p,q);

        while(1) 
        {
            ll gcd = get_gcd(b,q);
            if(gcd==1) break;
            while(q%gcd==0) q/=gcd;
        }
        if (q==1) printf("Finite\n");
        else printf("Infinite\n");      
    }
    return 0;
}
```

## 0x04 D. XOR-pyramid

> 
题目传送门：[http://codeforces.com/contest/984/problem/D](http://codeforces.com/contest/984/problem/D)


### 题目大意

题意是，有一个金字塔，三角形的，杨辉三角那种感觉 

（其实题意里没有这么说，但是题目本身的金字塔给了一个很好的提示作用）， 

给你金字塔最底层的一行数，告诉你，对于长度为 $m$ 的数组 $b$，有函数： 


$f(b) = \begin{cases} b[1] & \quad \text{if } m = 1 \\ f(b[1] \oplus b[2],b[2] \oplus b[3],\dots,b[m-1] \oplus b[m]) & \quad \text{otherwise,} \end{cases}$

例如： 


$f(1,2,4,8)=f(1\oplus2,2\oplus4,4\oplus8)=f(3,6,12)=f(3\oplus6,6\oplus12)=f(5,10)=f(5\oplus10)=f(15)=15$
问，任意选择范围区间，需要回答这个区间内任意一个连续子数组的 $f$ 函数的最大值是多少。

### 解题思路

我的第一反应其实是：区间询问？树状数组？然而其实不是，比那简单多了—— 

通过简单手写几层，我们可以知道，这个式子的实质，就像求和三角（例如杨辉三角这种）一样， 

自底向上地，每个非底层节点都是自己下方两个节点的异或（杨辉三角/求和三角是下方两个节点的和）， 

那么，我们要获得整个异或金字塔的话，就从底层一层一层向上算即可，而每个节点上 $f$ 函数的最大值， 

就是该节点覆盖范围内（这里的覆盖范围指的是一直选左孩子和一直选右孩子的路径构成的三角形）最大的节点值， 

于是我们在自底向上计算的时候顺手维护该点的 $f$ 函数的最大值即可： 

因为，如果每个点都是该节点上 $f$ 函数的最大值， 

那么每个点上 $f$ 函数的最大值即为该点的值和其两个子节点的  $f$ 函数的最大值中，最大的那个。
### Source Code

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : main.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-05-15
# ==========================================================================*/

#include <map>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

const int N = 5005;
ll a[N][N] = {0};
ll m[N][N] = {0};

int main()
{
    int n; cin>>n;
    for(int i=0;i<n;i++)
    {
        scanf("%I64d", &a[0][i]);
        m[0][i]=a[0][i];
    }

    for(int i=1;i<n;i++)
    {
        for(int j=0;j<n-i;j++)
        {
            a[i][j] = a[i-1][j] ^ a[i-1][j+1];
            m[i][j] = max(a[i][j], max(m[i-1][j], m[i-1][j+1]));
        }
    }

    int q; cin>>q;
    int l,r;
    for(int i=0;i<q;i++)
    {
        scanf("%d %d", &l, &r);
        printf("%I64d\n", m[r-l][l-1]);
    }

    /*
    for(int i=1;i<n;i++)
    {
        printf("\n");
        for(int j=0;j<n-i;j++)
            printf("%d ",a[i][j]);
    }
    */

    return 0;
}
```












