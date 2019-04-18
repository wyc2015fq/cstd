# vector,map,list,queue区别 - nosmatch的专栏 - CSDN博客
2012年05月14日 18:08:33[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：636标签：[vector																[list																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
**1。**vector  （连续的空间存储,可以使用[]操作符）快速的访问随机的元素，快速的在末尾插入元素，但是在序列中间岁间的插入，删除元素要慢，而且如果一开始分配的空间不够的话，有一个重新分配更大空间，然后拷贝的性能开销.
**2。**deque （小片的连续，小片间用链表相连，实际上内部有一个map的指针，因为知道类型，所以还是可以使用[]，只是速度没有vector快）快速的访问随机的元素，快速的在开始和末尾插入元素，随机的插入，删除元素要慢，空间的重新分配要比vector快,重新分配空间后，原有的元素不需要拷贝。对deque的排序操作，可将deque先复制到vector，排序后在复制回deque。
**3。**list   （每个元素间用链表相连）访问随机元素不如vector快，随机的插入元素比vector快，对每个元素分配空间，所以不存在空间不够，重新分配的情况
**4。**set 内部元素唯一，用一棵平衡树结构来存储，因此遍历的时候就排序了，查找也比较快的哦。
**5。**map 一对一的映射的结合，key不能重复。
**6。**stack 适配器，必须结合其他的容器使用，stl中默认的内部容器是deque。先进后出，只有一个出口，不允许遍历。
**7。**queue 是受限制的deque，内部容器一般使用list较简单。先进先出，不允许遍历。
**下面是选择顺序容器类型的一些准则  **
1.如果我们需要随机访问一个容器则vector要比list好得多 。
2.如果我们已知要存储元素的个数则vector 又是一个比list好的选择。  
3.如果我们需要的不只是在容器两端插入和删除元素则list显然要比vector好  
4.除非我们需要在容器首部插入和删除元素否则vector要比deque好。
5.如果只在容易的首部和尾部插入数据元素，则选择deque.
6.如果只需要在读取输入时在容器的中间位置插入元素，然后需要随机访问元素，则可考虑输入时将元素读入到一个List容器，接着对此容器重新拍学，使其适合顺序访问，然后将排序后的list容器复制到一个vector容器中
