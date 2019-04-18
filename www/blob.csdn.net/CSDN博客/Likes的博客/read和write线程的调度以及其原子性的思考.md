# read和write线程的调度以及其原子性的思考 - Likes的博客 - CSDN博客
2019年03月18日 11:18:33[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：13标签：[read																[write																[系统调用																[原子性](https://so.csdn.net/so/search/s.do?q=原子性&t=blog)](https://so.csdn.net/so/search/s.do?q=系统调用&t=blog)](https://so.csdn.net/so/search/s.do?q=write&t=blog)](https://so.csdn.net/so/search/s.do?q=read&t=blog)
个人分类：[操作系统](https://blog.csdn.net/songchuwang1868/article/category/8768253)
常有文章说系统调用都具有原子性，这是有待商榷的。
一个read操作读取一个fd，在读取部分数据后，fd暂时没有数据，read会被阻塞，CPU自然会将其换出，调度其他进程进行操作。而很可能换入的进程也对同一个fd进行操作，这就导致了最开始的read读取的不是连续的数据。所以read的原子性在这种情况下是不能被保证的。

