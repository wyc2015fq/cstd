# 有名信号量sem_open和内存信号量sem_init创建信号量的区别 - maopig的专栏 - CSDN博客
2016年08月17日 09:18:22[maopig](https://me.csdn.net/maopig)阅读数：6476
[有名信号量sem_open和内存信号量sem_init创建信号量的区别](http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=16813896&id=4711204)
分类： C/C++
sem_t *sem
sem_open(const char *name, int oflag, .../*mode_t mode,unsinged int value) ;
int sem_init(sem_t *sem,int shared, unsigned int value);
区别：
1.创建有名信号量必须指定一个与信号量相关链的文件名称，这个name通常是文件系统中的某个文件。
  基于内存的信号量不需要指定名称
2.有名信号量sem 是由sem_open分配内存并初始化成value值
  基于内存的信号量是由应用程序分配内存，有sem_init初始化成为value值。如果shared为1，则分配的信号量应该在共享内存中。
3.sem_open不需要类似shared的参数，因为有名信号量总是可以在不同进程间共享的
  而基于内存的信号量通过shared参数来决定是进程内还是进程间共享，并且必须指定相应的内存
4.基于内存的信号量不使用任何类似于O_CREAT标志的东西，也就是说，sem_init总是初始化信号量的值，因此，对于一个给定的信号量，我们必须小心保证只调用sem_init一次，对于一个已经初始化过的信号量调用sem_init,结果是未定义的。
5.内存信号量通过sem_destroy删除信号量，有名信号量通过sem_unlink删除 
