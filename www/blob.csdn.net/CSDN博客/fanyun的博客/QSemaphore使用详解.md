# QSemaphore使用详解 - fanyun的博客 - CSDN博客
2018年02月25日 22:12:56[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：204
   QSemaphore类提供了强壮的整数信号量。
      QSemaphore也可以被用来使线程的执行顺序化，和[QMutex](http://www.kuqin.com/qtdocument/qmutex.html)的方法相似。信号量和互斥量的不同在于，信号量可以在同一时间被多于一个的线程访问。
       假设我们有一个应用程序把数据存储到一个大的树型结构中。应用程序创建了10个线程（通常被称作线程池）来执行树中的搜索。当应用程序搜索树中的一小片数据，它在每个基本节点上使用一个线程来执行搜索。一个信号量就可以用来确保两个线程不必在同一时间内试图对树的同一个分支进行搜索。
        一个信号量的非计算实例是在餐馆就餐。信号量被初始化为最大值等于餐馆内的椅子数。当人们来了，他们想要一个座位。当座位满了，信号量就被访问，每次一个人。当人们离开了，访问就被释放，允许更多的人进入。如果一个10人的聚会想坐下，但是这里只有9个座位，那么这10个人就必须等，但是一个4人的聚会就可以坐下了（剩下的座位就是5了，那么那10个人就得等更长的时间了）。
当信号量被创建时，它被给定为一个它所允许的同时访问的最大数值。这个数量可以使用[operator++](http://www.kuqin.com/qtdocument/qsemaphore.html#operator++)()、[operator--](http://www.kuqin.com/qtdocument/qsemaphore.html#operator--)()、[operator+=](http://www.kuqin.com/qtdocument/qsemaphore.html#operator+-eq)()和[operator-=](http://www.kuqin.com/qtdocument/qsemaphore.html#operator--eq)()来改变。允许访问的数字可以使用[available](http://www.kuqin.com/qtdocument/qsemaphore.html#available)()重新得到，并且总量可以通过[total](http://www.kuqin.com/qtdocument/qsemaphore.html#total)()得到。注意如果没有足够多的可使用的访问，增加函数将会阻塞。如果你需要得到不阻塞的访问，请使用[tryAccess](http://www.kuqin.com/qtdocument/qsemaphore.html#tryAccess)()。
