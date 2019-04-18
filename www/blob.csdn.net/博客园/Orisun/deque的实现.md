# deque的实现 - Orisun - 博客园







# [deque的实现](https://www.cnblogs.com/zhangchaoyang/articles/2277209.html)





deque相对于vector的两大特点：
- 可在常数时间内对首尾进行插入、删除操作。
- 没有capacity()，不需要reserce(size_type n)。这是因为deque由动态分配的连续空间组合而成，随时可以增加一段新的空间链接起来。它没有必要像vector那样“因旧空间不足而重新分配2倍的空间，然后复制元素，再释放旧空间”。

虽然deque也提供随机访问迭代器，但其复杂度不能和vector同日而语，这当然影响了各个运算层面。为提高效率，对deque进行排序时，可先将deque拷贝到vector中，再使用STL的sort算法，最后再复制回deque。

deque是由一段一段的定量连续空间构成。一旦有以要在deque的前端和尾端增加新空间，便配置一段定量连续空间，串在整个deque的头端或尾端。deque的最大任务就是在这些分段的连续空间上，维护其整体连续的假象，并提供随机存取的接口。避开了“重新配置、复制、释放”的轮回，代价是复杂的迭代器架构。

![](https://pic002.cnblogs.com/images/2011/103496/2011120609093324.png)

![](https://pic002.cnblogs.com/images/2011/103496/2011120609095016.png)

![](https://pic002.cnblogs.com/images/2011/103496/2011120609100496.png)

![](https://pic002.cnblogs.com/images/2011/103496/2011120609101432.png)

![](https://pic002.cnblogs.com/images/2011/103496/2011120609102454.png)












