# 线段树之延时标记（区间修改）及lazy思想 - 紫芝的博客 - CSDN博客





2018年04月12日 23:25:23[紫芝](https://me.csdn.net/qq_40507857)阅读数：52
个人分类：[线段树](https://blog.csdn.net/qq_40507857/article/category/7569707)









### **暴力求解**

**1.最简单的方法是：在主函数中添加一个循环 **

进行 r-l+1次单点修改实现区间修改，对于单个元素修改时间复杂度为 O(log2(n))

所以对于单个区间修改的时间复杂度为 O(n*log(n))，甚至比朴素的模拟算法还慢

```cpp
for(int p=l;p<=r;++p)
UpdateTree(p,value);
```

**2.延时标记 lazy tag**

延时标记就是在递归的过程中，如果当前区间被需要，修改的区间完全覆盖，那么就要停止递归，并且在上面做一个标记 


3.但是这个信息没有更新到每个元素（即叶子结点），下次查询的时候可能没法得到足够的信息。之前在一个区间上打了一个标记，这个标记不仅是这个结点的性质，此性质作用于整个子树中，假设我们另一个查询中包含了，当前区间的子孙区间，这个标记也要对之后的查询产生影响

# lazy tag 

1.如果对线段树进行单点更新，都是在叶子节点中实现，不会对后续节点产生影响


2.如果当前区间被需要修改的目标区间完全覆盖，打一个标记 

如果下一次的查询或更改包含此区间，那么将这个标记分解，并传递给左右儿子


3.延时标记在需要时，才向下传递信息，如果没有用到，则不再进行操作

为了完成这种操作，可以在结构体中增加一个add数组存储区间的延时变化量


4.通俗的解释 lazy的意思，比如现在需要对[a,b]区间进行加 C操作，

那么就从根节点[1,n]开始调用update函数进行操作，如果刚好执行到一个子节点

它的节点标记为 rt，这时 **tree[rt].l==l&&tree[rt].r==r**这时我们可以进一步

更新此时rt节点的sum[rt]的值，**sum[rt]+=(ll)c*(r-l+1);**


5.关键的地方：如果此时按照常规的线段树的update操作，这时候还应该更新

rt子节点的sum[]值，而lazy思想恰恰是暂时不更新rt子节点的sum[]值，

到此时就 return，直到下一次需要用到rt子节点的值得时候才去更新，

这样避免许多无用的操作，从而节省时间


6.PushUp(rt);通过当前节点rt把值递归向上更新到根节点
PushDown(rt);通过当前节点rt递归向下去更新rt子节点的值

rt表示当前子树的根，也就是当前所在的节点 

## 解题模板

1.定义结构体来存储每个节点的子节点数值的总和，

add用来记录该节点的每个数值应该加多少

tree[i].l,tree[i].r分别表示某个节点的左右区间，都是闭区间 

```cpp
__int64 sum[N<<2],add[N<<2];
struct Node
{
    int l,r;
    int mid()
    {
        return (l+r)>>1;
    }
} tree[N<<2];
```

2.PushUp(rt);通过当前节点rt把值递归向上更新到根节点

```cpp
void PushUp(int rt)
{
    sum[rt] = sum[rt<<1] + sum[rt<<1|1];
}
```

3.PushDown(rt);通过当前节点rt递归向下去更新rt子节点的值

rt表示当前子树的根，也就是当前所在的节点 

```cpp
void PushDown(int rt,int m)
{
    if(add[rt])
    {
        add[rt<<1] += add[rt];
        add[rt<<1|1] += add[rt];
        sum[rt<<1] += add[rt] * (m - (m>>1));
        sum[rt<<1|1] += add[rt] * (m>>1);
        add[rt] = 0;
    }
}
```

4.建立线段树

```cpp
void build(int l,int r,int rt)
{
    tree[rt].l = l;
    tree[rt].r = r;
    add[rt] = 0;
    if(l == r)
    {
        scanf("%I64d",&sum[rt]);
        return ;
    }
    int m = tree[rt].mid();
    build(lson);
    build(rson);
    PushUp(rt);
}
```

5.update函数，lazy思想主要用于这里

通俗的解释 lazy的意思，比如现在需要对[a,b]区间进行加 C操作，

那么就从根节点[1,n]开始调用update函数进行操作，如果刚好执行到一个子节点

它的节点标记为 rt，这时 tree[rt].l==l&&tree[rt].r==r这时我们可以进一步

更新此时rt节点的sum[rt]的值，sum[rt]+=(ll)c*(r-l+1);


关键的地方：如果此时按照常规的线段树的update操作，这时候还应该更新

rt子节点的sum[]值，而lazy思想恰恰是暂时不更新rt子节点的sum[]值，

到此时就 return，直到下一次需要用到rt子节点的值得时候才去更新，

这样避免许多无用的操作，从而节省时间

```cpp
void update(int c,int l,int r,int rt)
{
    if(tree[rt].l == l && r == tree[rt].r)
    {
        add[rt] += c;
        sum[rt] += (__int64)c * (r-l+1);
        return;
    }
    if(tree[rt].l == tree[rt].r) return;
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    if(r <= m) update(c,l,r,rt<<1);
    else if(l > m) update(c,l,r,rt<<1|1);
    else
    {
        update(c,l,m,rt<<1);
        update(c,m+1,r,rt<<1|1);
    }
    PushUp(rt);
}
```

6.query函数，也就是用这个函数来求区间和

第一个if还是区间的判断和前面update的一样，到这里就可以知道答案了，所以就直接return。

接下来的查询就需要用到rt子节点的值了，由于我们用了Lazy操作，这段的数值还没有更新，因此我们需要调用PushDown函数去更新之，满足if(add[rt])就说明还没有更新。

```cpp
__int64 query(int l,int r,int rt)
{
    if(l == tree[rt].l && r == tree[rt].r)
    {
        return sum[rt];
    }
    PushDown(rt,tree[rt].r - tree[rt].l + 1);
    int m = tree[rt].mid();
    __int64 res = 0;
    if(r <= m) res += query(l,r,rt<<1);
    else if(l > m) res += query(l,r,rt<<1|1);
    else
    {
       res += query(l,m,rt<<1);
       res += query(m+1,r,rt<<1|1);
    }
    return res;
}
```

### 线段树 区间更新 区间查询模板题  [POJ3468](https://blog.csdn.net/qq_40507857/article/details/81879739)





