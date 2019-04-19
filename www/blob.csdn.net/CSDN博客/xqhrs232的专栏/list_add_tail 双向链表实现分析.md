# list_add_tail 双向链表实现分析 - xqhrs232的专栏 - CSDN博客
2018年08月09日 14:37:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：57
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[https://blog.csdn.net/qingkongyeyue/article/details/76089257](https://blog.csdn.net/qingkongyeyue/article/details/76089257)
相关文章
1、list_add_tail()----[https://blog.csdn.net/kernel_details/article/details/1481390](https://blog.csdn.net/kernel_details/article/details/1481390)
转自[http://www.xuebuyuan.com/1389026.html](http://www.xuebuyuan.com/1389026.html)
在看内核v4l2示例[代码](http://www.xuebuyuan.com/)driver/media/video/vivi.c时 ，看到list_add_tail()函数，现在对其进行分析：
- 
`struct list_head {`
- 
`struct list_head *next, *prev;`
- 
`};`
- 
- 
`list_add_tail(&buf->vb.queue, &vid->active);`
- 
`/**`
- 
`* list_add_tail - add a new entry`
- 
`* @new: new entry to be added`
- 
`* @head: list head to add it before`
- 
`*`
- 
`* Insert a new entry before the specified head.`
- 
`* This is useful for implementing queues.`
- 
`*/`
- 
`static __inline__ void list_add_tail(struct list_head *_new, struct list_head *head)`
- 
`{`
- 
`__list_add(_new, head->prev, head);`
- 
`}`
- 
- 
`/*`
- 
`* Insert a new entry between two known consecutive entries.`
- 
`*`
- 
`* This is only for internal list manipulation where we know`
- 
`* the prev/next entries already!`
- 
`*/`
- 
`static __inline__ void __list_add(struct list_head * _new,`
- 
`struct list_head * prev,`
- 
`struct list_head * next)`
- 
`{`
- 
`next->prev = _new;`
- 
`_new->next = next;`
- 
`_new->prev = prev;`
- 
`prev->next = _new;`
- 
`}`
- 
很多地方说：这个函数完成的功能就是添加一个新的结点在head的左边，其实不然，它是从右向左在head->priv和head两个节点之间插入_new。
假设刚开始建立链表，只有struct list_head *head，
那么前两句话有用：将next->prev = _new;
                            _new->next = next;
这就是将new节点添加到head 节点的左边，那么接 下来两句没用：   _new->prev = prev;  prev->next = _new;
如果head左边已近有了其他节点，那么调用list_add_tail（）函数后，前边两句的功能一样，都是把新的节点添加在head左边，而后两句就是把新节点添加在原来head之前节点(head->priv)右边，这样就串起来了。
那list_add就反过来，把新的节点添加在head和head之后的节点(head->next)之间；
关于list_add和list_add_tail建立栈和FIFO：
list_add和list_add_tail都是在head两边插入新的节点，所以list_add先插入的节点向右移，head->next是最后插入的节点，list_add_tail先插入的节点向左移，head->next是最先插入的节点；
遍历链表都是从head开始向下，所以用list_add建立的链表先访问的是最后插入的节点，类似于栈；list_add_tail建立的链表先访问的是最先插入的节地点，类似于FIFO。
