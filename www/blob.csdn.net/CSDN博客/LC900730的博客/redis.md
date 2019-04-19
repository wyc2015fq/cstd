# redis - LC900730的博客 - CSDN博客
2017年05月16日 16:50:18[lc900730](https://me.csdn.net/LC900730)阅读数：147标签：[redis](https://so.csdn.net/so/search/s.do?q=redis&t=blog)
个人分类：[python](https://blog.csdn.net/LC900730/article/category/6832717)
#### SDS:simple dynamic string
每个sds.h/sdshdr结构中表示一个SDS的值：
```cpp
struct  sdshdr{
      //记录buf数组中已经使用的字节的数量
      //等于SDS中所保存的字符串的长度
     //len=5表示这个sds保存了一个5字节长度的字符串     
     //记录buf数组中未使用的字节的数量
    int free; //free=0表示这个sds没有分配任何未使用的空间
    //char buf[]; //字节数组，用于保存字符串，最后一个字节是'\0'，保存空字符的1字节不计算在sds的属性中
}
```
SDS以len属性的值而不是空字符来判断字符串是否已经结尾
#### adlist.h/listNode:链表节点
每个链表使用adlist.h/listNode结构来表示
```
typedef struct listNode{
    //前置节点
    struct listNode *prev;
    //后置节点
    struct listNode *next;
    //节点的值
    void *value;
}
```
虽然使用多个listNode结构就可以组成链表，但使用adlist.h/list来持有链表话，操作 起来会更加方便
```cpp
typedef struct list{
    //表头节点
    listNode *head；
    //表尾节点
    listNode *tail；
    //链表包含节点的数量
    unsigned long len;
    //节点值复制函数
    void *(*dup)(void *ptr);
    ////节点值复制函数
    void (*free)(void *ptr);
    //节点值对比函数
    int (*match)(void *ptr，void *key);
}list
```
list结构为链表提供了表头指针head，表尾指针tail，以及长度计数器len，而dup/free/match成员则是用于实现多态链表所需要的类型特定函数。 
dup函数用于复制链表节点所保存的值函数； 
free函数用于释放链表节点保存的值。 
match函数用于比较链表节点保存的值和另一个值是否相等； 
链表节点中使用void *保存节点的值，并且可以通过list结构的dup、free、match三个属性为节点值设置类型特定函数，所以链表可以用于保存不同类型的值。
#### Redis字典使用的哈希表
由dict.h/dictht定义：
```
typedef struct dicht{
    //哈希表数组
    dictEntry **table;
    //哈希表大小
    unsigned long size；
    //哈希表大小掩码，用于计算索引值，总是等于size-1
    unsigned long sizemask;
    //该哈希表已经有的节点数量
    unsigned long used;
}dictht
```
table属性是一个数组，每一个元素都指向dict.h/dictEntry结构的指针，每个dichEntry结构保存着一个键值对。
#### 哈希表节点dictEntry
每个dictEntry保存着一个键值对
```
typedef struct dictEntry{
//值
void *key;
union{
    void *val;
    uint64_tu64;
    int64_ts64;
}v;
//指向下个哈希表的节点，形成链表
struct dictEntry *next；
}dictEntry
```
key属性保存着键值对中的键，而v属性保存着键值对中的值，其中键值对的值可以是一个指针，或者是一个uint64_t整数或者int64_t； 
next属性是指向另一个哈希表节点的指真，这个指针将多个哈希值相同的键值对连接在一起，解决键冲突问题
