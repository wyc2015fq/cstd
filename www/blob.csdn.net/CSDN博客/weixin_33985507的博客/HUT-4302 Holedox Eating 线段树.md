# HUT-4302 Holedox Eating 线段树 - weixin_33985507的博客 - CSDN博客
2012年07月20日 21:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
该题说的是一个小东西生活在管道内，长度为L，里面有一些蛋糕，现在动态的给定蛋糕的坐标，小东西的坐标初始话为0，当指令为吃蛋糕的时候，小东西将会去吃最临近的蛋糕。现在问你小东西在Q次询问下要走多远的距离。
这题用树状数组+二分查找超时了。。 构造出一棵线段树，每个节点保留区间中所有的蛋糕数以及左右蛋糕最近的坐标。维护好线段树即可。
代码如下：
```
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;
int L, Q;
struct Node
{
    int num, l, r, left, right;
}e[400005];
void build(int p, int l, int r)
{
    e[p].l = l, e[p].r = r;
    e[p].left = e[p].right = -1;  // 由于0为有效位，所以以-1特殊标记
    e[p].num = 0;
    if (l != r) {
        int mid = (l + r) >> 1;
        build(p<<1, l, mid);
        build(p<<1|1, mid+1, r);
    }
}
void update(int p)
{
    e[p].num = e[p<<1].num + e[p<<1|1].num;
    if (e[p<<1].left != -1) {
        e[p].left = e[p<<1].left;
    }
    else {
        e[p].left = e[p<<1|1].left;
    }
    if (e[p<<1|1].right != -1) {
        e[p].right = e[p<<1|1].right;
    }
    else {
        e[p].right = e[p<<1].right;
    } 
}
void modify(int p, int x, int val)
{
    if (e[p].l == e[p].r) {  // 一定就是我们要找的点
        e[p].num += val;
        if (e[p].num > 0) {
            e[p].left = e[p].right = e[p].l;
        }
        else {
            e[p].left = e[p].right = -1;
        }
        return;
    }
    else {
        int mid = (e[p].l + e[p].r) >> 1;
        if (x <= mid) {
            modify(p<<1, x, val);
        }
        else {
            modify(p<<1|1, x, val);
        }
    }
    update(p);
}
int rquery(int p, int l, int r)
{
    if (l == e[p].l && e[p].r == r) {
        return e[p].right;
    }
    int mid = (e[p].l + e[p].r) >> 1;
    if (r <= mid) {
        if (e[p<<1].num != 0) {
            return rquery(p<<1, l, r);
        }
        else return -1;
    }
    else if (l > mid) {
        if (e[p<<1|1].num != 0) {
            return rquery(p<<1|1, l, r);
        }
        else return -1;
    }
    else {
        int ans = rquery(p<<1|1, mid+1, r);
        if (ans == -1) {
            ans = rquery(p<<1, l, mid);
        }
        return ans;
    }
}
int lquery(int p, int l, int r)
{
    if (l == e[p].l && r == e[p].r) {
        return e[p].left;
    }
    int mid = (e[p].l + e[p].r) >> 1;
    if (r <= mid) {
        if (e[p<<1].num != 0) {
            return lquery(p<<1, l, r);
        }
        else return -1;
    }    
    else if (l > mid) {
        if (e[p<<1|1].num != 0) {
            return lquery(p<<1|1, l, r);
        }
        else return -1;
    }
    else {
        int ans = lquery(p<<1, l, mid);
        if (ans == -1) {
            ans = lquery(p<<1|1, mid+1, r);
        }
        return ans;
    }
}
int goleft(int &pos, int &dir, int left)
{
    int ans;
    dir = 0;
    ans = pos - left;
    pos = left;
    modify(1, left, -1);
    return ans;
}
int goright(int &pos, int &dir, int right)
{
    int ans;
    dir = 1;  // 向右走，更新方向和地址
    ans = right - pos;
    pos = right;
    modify(1, right, -1);
    return ans;
}
int solve(int p, int &pos, int &dir)
{
    int left = rquery(1, 0, pos);
    int right = lquery(1, pos, L);
    if (left == -1 && right == -1) {
        return 0;
    }
    if (left == -1 || right == -1) {
        // 有某一部分没有豆子，第一次一定是在这个区间内的
        if (right != -1) {
             return goright(pos, dir, right);
        }
        else if (left != -1) {
             return goleft(pos, dir, left);
        }
    }
    else {
        if (left == right) {
            modify(1, left, -1);
            return 0;
        }
        else {
            if (pos - left != right - pos) {
                if (pos - left < right - pos) {
                    return goleft(pos, dir, left);
                }
                else {
                    return goright(pos, dir, right);
                }
            }
            else {
                if (dir == 0) { // 玩左走
                    return goleft(pos, dir, left);
                }
                else {
                    return goright(pos, dir, right);
                }
            }
        }
    }
}
int main()
{
    int T, ca = 0, op, x, dir, pos, result;
    scanf("%d", &T);
    while (T--) {
        dir = pos = result = 0;  // 初始的方向并没有影响,0为左，1为右
        scanf("%d %d", &L, &Q);
        build(1, 0, L);
        while (Q--) {
            scanf("%d", &op);
            if (op == 0) {
                scanf("%d", &x);
                modify(1, x, 1);  // 对于该题而言只存在单点更新
            }
            else {
                result += solve(1, pos, dir);
            }
        }
        printf("Case %d: %d\n", ++ca, result);
    }
    return 0;
}
```
