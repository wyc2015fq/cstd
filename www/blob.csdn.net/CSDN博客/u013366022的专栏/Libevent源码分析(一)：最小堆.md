# Libevent源码分析(一)：最小堆 - u013366022的专栏 - CSDN博客
2016年11月30日 15:03:03[slitaz](https://me.csdn.net/u013366022)阅读数：404
Libevent中的timeout事件是使用最小堆来管理维护的.代码位于<minheap-internal.h>.
看函数命名和代码风格应该是一个C++程序员,函数名都挺好懂的,只是下面这个结构体变量命名比较坑....
```
typedef struct min_heap
{
    struct event** p;
    unsigned n, a;//n队列元素的多少,a代表队列空间的大小.
} min_heap_t;
```
注释是我加的,这命名,n啊a啊的,鬼知道啥意思....必须吐槽一下.
先来说说什么是最小堆：
1.堆是一个二叉树
2.最小堆：父节点的值总是小于等于子节点
如下图：
![](http://images.cnitblog.com/blog/583030/201411/172144430195991.jpg)
上图圆圈旁边的数字代表其在数组中的下标.堆一般是用数组来存储的,也就是说实际存储结构是连续的,只是逻辑上是一棵树的结构.这样做的好处是很容易找到堆顶的元素,对Libevent来说,很容易就可以找到距当前时间最近的timeout事件.
 现在想想看我们要插入一个元素,我们要怎么移动数组中元素的位置,使其逻辑上仍然是一个小堆？结合下图很容易看出来：
1.假设我们要插入的元素为6大于其父节点的值2.则把元素放在数组相应的index上,插入完成.
![](http://images.cnitblog.com/blog/583030/201411/172152403944050.jpg)
2.假设我们要插入的为2小于其父节点的值3.则交换该节点与其父节点的值.对于下图来说,交换完毕后插入就算完成了.那要是交换完后发现index=2的元素还小于其父节点index=0的呢?就又得在一次交换,如此循环,直到达到根节点或是其不大于父节点.
![](http://images.cnitblog.com/blog/583030/201411/172152595197507.jpg)![](http://images.cnitblog.com/blog/583030/201411/172153117071385.jpg)
到了这里我们看看libevent里的实现代码就很清楚了
```
int min_heap_push(min_heap_t* s, struct event* e)
{
    if (min_heap_reserve(s, s->n + 1))
        return -1;
    min_heap_shift_up_(s, s->n++, e);
    return 0;
}
//分配队列大小.n代表队列元素个数多少.
int min_heap_reserve(min_heap_t* s, unsigned n)
{
    if (s->a < n)    //队列大小不足元素个数,重新分配空间.
    {
        struct event** p;
        unsigned a = s->a ? s->a * 2 : 8;  //初始分配8个指针大小空间,否则原空间大小翻倍.
        if (a < n)
            a = n;   //翻倍后空间依旧不足,则分配n.
        if (!(p = (struct event**)mm_realloc(s->p, a * sizeof *p))) //重新分配内存
            return -1;
        s->p = p; //重新赋值队列地址及大小.
        s->a = a; //
    }
    return 0;
}
void min_heap_shift_up_(min_heap_t* s, unsigned hole_index, struct event* e)
{
    unsigned parent = (hole_index - 1) / 2;
    while (hole_index && min_heap_elem_greater(s->p[parent], e)) //比父节点小或是到达根节点.则交换位置.循环.
    {
        (s->p[hole_index] = s->p[parent])->ev_timeout_pos.min_heap_idx = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    (s->p[hole_index] = e)->ev_timeout_pos.min_heap_idx = hole_index;
}
```
 实际上作者写了一个比较通用的函数min_heap_shift_up(),与之相对应的还有min_heap_shift_down()
```
void min_heap_shift_down_(min_heap_t* s, unsigned hole_index, struct event* e)
{
    unsigned min_child = 2 * (hole_index + 1);
    while (min_child <= s->n)
    {
    //找出较小子节点
    min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1]);
    //比子节点小正常.不需要再交换位置,跳出循环.
    if (!(min_heap_elem_greater(e, s->p[min_child])))
        break;
    //比子节点大,要交换位置
    (s->p[hole_index] = s->p[min_child])->ev_timeout_pos.min_heap_idx = hole_index;
    hole_index = min_child;
    min_child = 2 * (hole_index + 1);
    }
    (s->p[hole_index] = e)->ev_timeout_pos.min_heap_idx = hole_index;
}
```
这里的hole_index是我们要填入某个值的下标,e是要填入的值.还是画图比较好理解:
![](http://images.cnitblog.com/blog/583030/201411/172225150665254.png)
当这个值(下标为hole_Index=1)比其父节点1(index = 0)小时,要向上移动调整.
当这个值(下标为hole_Index=1)比其最小子节点6(index = 3)还大时,要向下移动调整.
Libevent里是这么使用他们的：
```
int min_heap_erase(min_heap_t* s, struct event* e)
{
    if (-1 != e->ev_timeout_pos.min_heap_idx)
    {
        struct event *last = s->p[--s->n];//把最后一个值作为要填入hole_index的值
        unsigned parent = (e->ev_timeout_pos.min_heap_idx - 1) / 2;
        /* we replace e with the last element in the heap.  We might need to
           shift it upward if it is less than its parent, or downward if it is
           greater than one or both its children. Since the children are known
           to be less than the parent, it can't need to shift both up and
           down. */
        if (e->ev_timeout_pos.min_heap_idx > 0 && min_heap_elem_greater(s->p[parent], last))
            min_heap_shift_up_(s, e->ev_timeout_pos.min_heap_idx, last);
        else
            min_heap_shift_down_(s, e->ev_timeout_pos.min_heap_idx, last);
        e->ev_timeout_pos.min_heap_idx = -1;
        return 0;
    }
    return -1;
}
```
```
struct event* min_heap_pop(min_heap_t* s)
{
    if (s->n)
    {
        struct event* e = *s->p;
        min_heap_shift_down_(s, 0u, s->p[--s->n]);
        e->ev_timeout_pos.min_heap_idx = -1;
        return e;
    }
    return 0;
}
```
最后总结一下,由于堆这种结构在逻辑上的这种二叉树的关系,其插入也好,删除也好,就是一个与父节点或是子节点比较然后调整位置,这一过程循环往复直到达到边界条件的过程.记住这一点,就不难写出代码了.
二叉树节点i:父节点为(i-1)/2.子节点为2i+1,2(i+1)。
