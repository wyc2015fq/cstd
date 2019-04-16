# HDU4825 Xor Sum 01字典树（Tire Tree） - 紫芝的博客 - CSDN博客





2018年07月29日 17:41:01[紫芝](https://me.csdn.net/qq_40507857)阅读数：62








# Xor Sum

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 132768/132768 K (Java/Others)Total Submission(s): 4251    Accepted Submission(s): 1872****

**Problem Description**

Zeus 和 Prometheus 做了一个游戏，Prometheus 给 Zeus 一个集合，集合中包含了N个正整数，随后 Prometheus 将向 Zeus 发起M次询问，每次询问中包含一个正整数 S ，之后 Zeus 需要在集合当中找出一个正整数 K ，使得 K 与 S 的异或结果最大。Prometheus 为了让 Zeus 看到人类的伟大，随即同意 Zeus 可以向人类求助。你能证明人类的智慧么？

**Input**

输入包含若干组测试数据，每组测试数据包含若干行。

输入的第一行是一个整数T（T < 10），表示共有T组数据。

每组数据的第一行输入两个正整数N，M（<1=N,M<=100000），接下来一行，包含N个正整数，代表 Zeus 的获得的集合，之后M行，每行一个正整数S，代表 Prometheus 询问的正整数。所有正整数均不超过2^32。

**Output**

对于每组数据，首先需要输出单独一行”Case #?:”，其中问号处应填入当前的数据组数，组数从1开始计算。

对于每个询问，输出一个正整数K，使得K与S异或值最大。

**Sample Input**

```
2 
3 2 
3 4 5 
1 
5 
4 1 
4 6 5 6 
3
```

**Sample Output**

```
Case #1: 
4 
3 
Case #2: 
4
```

**Source**

[2014年百度之星程序设计大赛 - 资格赛](http://acm.hdu.edu.cn/search.php?field=problem&key=2014%C4%EA%B0%D9%B6%C8%D6%AE%D0%C7%B3%CC%D0%F2%C9%E8%BC%C6%B4%F3%C8%FC+-+%D7%CA%B8%F1%C8%FC&source=1&searchmode=source)

**Recommend**

liuyiding   |   We have carefully selected several similar problems for you:  [6318](http://acm.hdu.edu.cn/showproblem.php?pid=6318)[6317](http://acm.hdu.edu.cn/showproblem.php?pid=6317)[6316](http://acm.hdu.edu.cn/showproblem.php?pid=6316)[6315](http://acm.hdu.edu.cn/showproblem.php?pid=6315)[6314](http://acm.hdu.edu.cn/showproblem.php?pid=6314)

### 思路：

建立一棵01字典树，将一个数字用二进制位建立一棵二叉树，左边是0，右边是1，根据位运算进行建树。

因为只要是数值不同，对应的二进制数也不同，所以不存在覆盖问题。对于查询，求的是异或的最大值，

那么就肯定尽量让每一位都与查询的值尽量不同。（前提是有这样的分支存在）就这样一直走到叶子节点

就是所求的异或的最大值。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
unsigned int number;
unsigned int tree[32*maxn][2];
int val[32*maxn];//存储n个数字
void build(int a)
{
    int u=0;
    for(int i=31;i>=0;--i){//从右向左建树
        int c=(a>>i)&1;
        if(!tree[u][c])
            tree[u][c]=number++;
        u=tree[u][c];
    }
    val[u]=a;
}
unsigned int query(int k)
{
    int u=0;
    for(int i=31;i>=0;--i){
        int c=(k>>i)&1;
        if(tree[u][c^1])
            u=tree[u][c^1];
        else
            u=tree[u][c];
    }
    return val[u];
}
int main()
{
    int t,n,m,a,k;
    scanf("%d",&t);
    for(int i=1;i<=t;++i){
        memset(tree,0,sizeof(tree));
        number=1;
        scanf("%d%d",&n,&m);
        for(int j=0;j<n;++j){
            scanf("%d",&a);
            build(a);
        }
        printf("Case #%d:\n",i);
        for(int j=0;j<m;++j){
            scanf("%d",&k);
            printf("%d\n",query(k));
        }
    }
    return 0;
}
```





