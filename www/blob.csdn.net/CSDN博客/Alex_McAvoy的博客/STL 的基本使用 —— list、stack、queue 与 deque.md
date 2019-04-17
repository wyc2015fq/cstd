# STL 的基本使用 —— list、stack、queue 与 deque - Alex_McAvoy的博客 - CSDN博客





2019年01月13日 15:49:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70








# 【概述】

list、stack 和 queue 是在程序设计中经常会用到的数据容器，STL 提供了方便的链表、堆栈、队列的实现，准确地说，STL 中的 list、stack 与 queue 不同于 pair、vector 等容器，而是对列表、栈、队列这三个数据结构的重新包装。

# 【list 的使用】

list 定义在 <list> 头文件中，其提供了双链表的全部功能，即是一个线性链表结构，无需分配内存且可任意伸缩。

定义：**list<type> listName**
- l.front()：访问列表首元素
- l.back()：访问列表尾元素
- l.empty()：判断列表是否为空，当列表为空时返回 true，否则返回 false
- l.size()：访问列表中元素的个数
- l.begin()：返回指向第一个元素的迭代器
- l.end()：返回指向最后一个元素之后的迭代器
- l.erase(it)：删除迭代器 it 所指元素并返回下一元素迭代器
- l.erase(it_begin,it_end)：删除迭代器 [it_begin,it_end] 区间的元素并返回最后一个被删除的元素的下一元素迭代器
- l.insert(it,value)：在迭代器 it 位置插入一个值为 value 的元素并返回其迭代器，原 it 及以后的元素后移
- l.insert(it,num,value)：在迭代器 it 位置插入 num 个值为 value 的元素并返回其迭代器，原 it 及以后的元素后移

# 【stack 的使用】

stack 定义在 <stack> 头文件中，其提供了堆栈的全部功能，即实现了先进后出的数据结构。

定义：** stack<type> stackName**

stack 的基本操作有：
- s.push(x)：将元素 x 入栈
- s.pop()：将栈顶元素删除
- s.top()：访问栈顶元素
- s.empty()：判断栈是否为空，当栈为空时返回 true，否则返回 false
- s.size()：访问栈中元素的个数

# 【queue 的使用】

queue 定义在 <queue> 头文件中，其提供了队列的全部功能，即实现了先进先出的数据结构。

定义：**queue<type> queueName**

queue 的基本操作有：
- q.push(x)：入队，将 x 存入队列末端
- q.pop()：出队，将队列的第一个元素弹出
- q.front()：访问队首元素
- q.back()：访问队尾元素
- q.empty()：判断队列是否为空，当队列为空时返回 true，否则返回 false
- q.size()：访问队列中元素的个数

# 【deque 的使用】

deque 定义在 <deque> 头文件中，是一种具有队列和栈的性质的数据结构，双端队列中的元素可以从两端弹出，其限定插入和删除操作在表的两端进行。

定义：**deque<type> dequeName**

deque 的基本操作有：
- q.push_back(x)：入队，将 x 存入队列末端
- q.push_front(x)：入队，将 x 存入队列首端
- q.pop_back()：出队，将队列的末端元素弹出
- q.pop_front()：出队，将队列的首段元素弹出
- q.front()：访问队首元素
- q.back()：访问队尾元素
- q.empty()：判断队列是否为空，当队列为空时返回 true，否则返回 false
- q.size()：访问队列中元素的个数





