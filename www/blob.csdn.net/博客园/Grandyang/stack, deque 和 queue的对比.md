# stack, deque 和 queue的对比 - Grandyang - 博客园







# [stack, deque 和 queue的对比](https://www.cnblogs.com/grandyang/p/4553433.html)







stack, deque 和 queue这三个c++的STL的数据结构很类似但又各有不同。

stack是堆栈，没有迭代器，特点是后进先出。用push()将元素压入栈中，top()返回栈顶元素，pop()移除栈顶元素。

deque是双端队列，支持迭代器，使用push_back()在队尾添加元素，pop_back()移除队尾元素，这些跟vector差不多。不同的地方在于deque还可在队首添加和移除元素，使用pop_front()和push_front()。

queue是队列，特点是先进先出，不支持迭代器，使用push()将元素排入对中，front()返回队首元素,pop()移除队首元素。














