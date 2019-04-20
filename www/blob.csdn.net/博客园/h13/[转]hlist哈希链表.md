# [转]hlist哈希链表 - h13 - 博客园
hlist哈希链表是内核中常用的一个数据结构，由于它不同于普通的链表，所以这里对hlist哈希链表进行一下分析，希望对大家有所帮助。
在include/Linux/list.h中有list链表与hlist哈希链表结构的定义，下面都列出它们的定义，可以对比一下：
```
struct list_head {
struct list_head *next, *prev;
};
```
struct hlist_head {
struct hlist_node *first;
};
struct hlist_node {
struct hlist_node *next, **pprev;
};
双头（next，prev）的双链表对于Hash表来说“过于浪费”，因而另行设计了一套Hash表专用的hlist数据结构——单指针表头双循环 链表，hlist的表头仅有一个指向首节点的指针，而没有指向尾节点的指针，这样在可能是海量的Hash表中存储的表头就能减少一半的空间消耗。
pprev因为hlist不是一个完整的循环链表而不得不使用。在list中，表头和节点是同一个数据结构，直接用prev没问题；在hlist中，表头 没有prev，也没有next，只有一个first。为了能统一地修改表头的first指针，即表头的first指针必须修改指向新插入的节点， hlist就设计了pprev。hlist节点的pprev不再是指向前一个节点的指针，而是指向前一个节点（可能是表头）中的next（对于表头则是 first）指针（struct list_head **pprev），从而在表头插入的操作可以通过一致的“*(node->pprev)”访问和修改前节点的next（或first）指针。
注：pprev是指向前一个节点中的next指针，next是指向hlist_node的指针，所以pprev是一个指向hlist_node的指针的指针。
![](http://blogimg.chinaunix.net/blog/upfile2/071227211225.jpg)
注意：
pprev可以理解成向list的prev一样，是一个指向hlist_node的指针，又由于hlist_node的第一个元素next是一个指向 hlist_node的指针，pprev也是一个指向next的指针，即pprev是一个指向hlist_node的指针的指针。
```
struct hlist_node Prev;
struct hlist_node *pprev = (struct hlist_node *) Prev = (struct hlist_node *) (struct hlist_node * next) = struct hlist_node ** next;
```
下面是hlist中常用的几个宏：
```
#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = { .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
#define INIT_HLIST_NODE(ptr) ((ptr)->next = NULL, (ptr)->pprev = NULL)
```
下面只列出hlist_add_before操作函数，其他hlist链表操作函数操作方法类似。这个函数中的参数next不能为空。它在next前面加入了n节点。函数的实现与list中对应函数类似。
```
static inline void __hlist_del(struct hlist_node *n)
{
struct hlist_node *next = n->next;
struct hlist_node **pprev = n->pprev;
*pprev = next;
if (next)
next->pprev = pprev;
}
```
static inline void hlist_add_before(struct hlist_node *n,struct hlist_node *next)
{
n->pprev = next->pprev;
n->next = next;
next->pprev = &n->next;
*(n->pprev) = n;
}
#define hlist_entry(ptr, type, member) container_of(ptr,type,member)
#define hlist_for_each(pos, head) \
for (pos = (head)->first; pos && ({ prefetch(pos->next); 1; }); \
pos = pos->next)
10、这两个宏的功能都是通过表头指针head依次获得指向各个节点的指针pos。
#define hlist_for_each(pos, head) /
        for (pos = (head)->first; pos && ({ prefetch(pos->next); 1; }); /
             pos = pos->next)
#define hlist_for_each_safe(pos, n, head) /
        for (pos = (head)->first; pos && ({ n = pos->next; 1; }); /
             pos = n)
首先，prefetch宏是一个空的实现，其值是1，因此for循环中的第二个表达式其实就等价于 pos。
其次，如果要删除pos所指向的节点，那么只能使用第二个宏hlist_for_each_safe(pos, n, head)，因为第一个宏在for循环的第三个表达式中对pos指针进行了解引用，如果pos所指向的节点已经被释放了这个操作就是非法的了。而safe版本的遍历不会有这个尴尬，因为用了一个中间变量n来暂时保存pos的next指针。
再强调一下，当我们要利用内核的哈希表实现自己的代码的时候，用到这两个宏来遍历我们的哈希链表的时候，记得：如果要遍历之后有删除的操作，一定要用safe版本。
11、以下这三个宏完成一样的功能：遍历哈希链表，并且获得其所在结构体的指针tpos。hlist_for_each_entry版本从表头开始遍历，而hlist_for_each_entry_continue版本从pos所指向的节点的下一个节点开始遍历，hlist_for_each_entry_from版本从pos所指向的那个节点开始遍历。
#define hlist_for_each_entry(tpos, pos, head, member) /
        for (pos = (head)->first; /
             pos && ({ prefetch(pos->next); 1;}) && /
                ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); /
             pos = pos->next)
#define hlist_for_each_entry_continue(tpos, pos, member) /
        for (pos = (pos)->next; /
             pos && ({ prefetch(pos->next); 1;}) && /
                ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); /
             pos = pos->next)
#define hlist_for_each_entry_from(tpos, pos, member) /
        for (; pos && ({ prefetch(pos->next); 1;}) && /
                ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); /
             pos = pos->next)
这两个宏的应用，同样要注意安全的问题，注意到for循环的第三个表达式都没有用到临时变量来保存pos的下一个指针变量，因此不能用这两个宏来遍历删除节点，要安全地遍历各个节点，需要用以下这个宏：
12、安全地从表头开始遍历各个节点。（这里的安全指的是遍历的过程可以删除该节点）
#define hlist_for_each_entry_safe(tpos, pos, n, head, member) /
        for (pos = (head)->first; /
             pos && ({ n = pos->next; 1; }) && /
                ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); /
             pos = n)
转自：[http://www.cnitblog.com/luofuchong/archive/2011/11/13/38986.html](http://www.cnitblog.com/luofuchong/archive/2011/11/13/38986.html)
