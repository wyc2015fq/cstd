# 牛客网——C列一列 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/71/C](https://www.nowcoder.net/acm/contest/71/C)
来源：牛客网



## 题目描述


小W在计算一个数列{An},其中A1=1,A2=2,An+2=An+1+An。尽管他计算非常精准，但很快他就弄混了自己的草稿纸，他找出了一些他计算的结果，但他忘记了这些都是数列中的第几项。

## 输入描述:

每行包括数列中的一项Ak(k<=100000)。

总行数T<=30。

## 输出描述:

对于每一项Ak，输出一行包括一个正整数k表示输入中数是数列的第几项。


示例1



## 输入

2
3
5
8
13



## 输出

2
3
4
5
6
```
#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
typedef long long  ll;

const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=1000010;
ull a[MAXN];
int main()
{
    a[1] = 1;
    a[2] = 2;
    for(int i=3;i < MAXN;i++) a[i] = a[i-1] + a[i-2];
    char str[MAXN];
    while(~scanf("%s",str))
    {
        ll len = strlen(str);
        ull B = 0;
        for(int i=0;i < len;i++) B = B*10 + (str[i]-'0');

        for(int i=1;i < MAXN;i++)
        {
            if(a[i] == B)
            {
                cout << i << endl;
                break;
            }
        }
    }
    return 0;
}
```

用map方便查找

```
#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
typedef long long  ll;

const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=1000010;
ll a[MAXN];
map<ll,int> mm;
int main() {
    a[1] = 1,a[2] = 2;
    mm[1] = 1,mm[2] = 2;
    for( int i=3; i<=MAXN; i++ ) //打表
    {
        a[i] = a[i-1] + a[i-2];
        mm[a[i]] = i;            //用map使值和对应的位置形成键值对方便查找
    }
    string s;
    while( cin >> s ) 
    {
        ll sum = 0;
        for( int i=0; i<s.length(); i++ ) 
        {
            sum =  sum * 10 + s[i] - '0';
        }
        cout << mm[sum] << endl;
    }
    return 0;
}
```


















