# 怎样运用pipe更为便捷地实现线程池 - Likes的博客 - CSDN博客
2019年03月18日 11:31:40[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：11标签：[pipe																[线程池																[便捷](https://so.csdn.net/so/search/s.do?q=便捷&t=blog)](https://so.csdn.net/so/search/s.do?q=线程池&t=blog)](https://so.csdn.net/so/search/s.do?q=pipe&t=blog)
个人分类：[网络](https://blog.csdn.net/songchuwang1868/article/category/8235292)
先讨论一下pipe的原子性问题，具体而言就是多个写线程写一个pipe，同时多个读线程读这个pipe
- read pipe
只要pipe中有数据，read就不会阻塞，不会阻塞就不会在读的中途被换出，也就能确保原子性。
要达到上述目的，一个简单的做法就是write的数据量是read的数据量的倍数或相同。这样read就不会在读到一半的数据时被阻塞。
- write pipe
只要pipe中未满，write就不会阻塞，不会阻塞就不会再写的中途被换出，也就能确保原子性。
但是如果read不读取数据，pipe始终会被写满，而且很可能write写到中途，pipe满，被阻塞换出。
但实际上**内核能确保写 pipe的原子性**，只要一次写的大小小于PIPE_SIZE，写pipe一定是原子的，发生上述问题write虽然被换出，但当pipe不为空时，内核确保换入的线程是刚才换出的还没有写完的线程。
所以在使用pipe时会遇到read和write的原子性问题，但是只要确保读写数据量相同，就不会存在读写到一半就会被换出的问题。
所以我们也可以利用pipe便捷地实现线程池。
线程池一般分为两个部分：多个计算线程和一个任务队列。对于任务队列的实现，一般常用的做法是使用条件变量。我们也可以使用pipe简单的地实现一个任务队列。
我们用pipe传递指向任务的指针，由于指针始终是4个字节，读写管道的字节数相同，确保了read管道的原子性，且4<PIPE_SIZE，所以write管道的原子性能够保障。
