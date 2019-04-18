# pthread_create() 给创建的线程传递参数 - 文章 - 伯乐在线
原文出处： [梦醒潇湘love，2012-12-10](http://blog.chinaunix.net/uid-26548237-id-3434555.html)
对于pthread_create为创建线程传递参数的问题，先前只知道传递单个参数的方法，现在需要传递多个参数。
对于新手的我比较纠结，因此，上网查找资料，发现CU里面的一个帖子（帖子地址后面给出）。
下面把帖子中认为对自己有用的知识摘录下来，以后经常看看。
帖子内容：
1、传递int、short、char等，大小少于一个指针长度类型的，强制转换为(void*)，然后在线程函数里转换回来即可；
2、传递多个参数的时候，创建线程的函数内malloc()一个结构体，把参数都放进去，然后传指针。等线程函数结束后，在线程内部free()；
3、前面同2，但是在主线程中把子线程join回来，然后在主线程中free()；
但是，对于第二种情况，一个函数malloc()，另一个函数free()，感觉不爽；
对于第三种情况，虽然做到同一个函数内申请释放了，但是子线程无法detach，且主线程必须等待所有子线程返回后才能退出，也不爽。
有啥好办法没？
下面是其中部分回复：
1、对于C++，经常的做法是：线程函数是类的静态函数，传递的参数时类的成员；
2、用外部变量（全局变量），定义成数组，然后每个线程编号做下标；也就是，传句柄/下标比传指针安全，可以多一次有效性检查；
3、直接在创建函数里用临时变量创建一个结构也可以
这种方法是错误的，创建函数启动线程后不会阻塞，直接就返回了，临时变量也就失效了，等新线程再访问该临时变量的地址，会遭遇野指针。
4、关于传递多个参数，一种比较通用的方式是为pthread_create做一次函数包装，由包装函数完成一些琐碎的事情，下面举例说明扩展成3个参数的例子，之后调用my_thread_create()即可处理多于一个参数的例子。


```
typedef void * (*FUNCALLBACK)(void * arg1, void * arg2, void * arg3);
typedef stuct {
   FUNCALLBACK callback;
   void * arg1;
   void * arg2;
   void * arg3;
} ARGS;
void * sub_thread_entry(void * arg)
{
    ARGS args;
    
    /* retrieve args */
    args.callback = ((ARGS *)arg)->callback;
    args.arg1 = ((ARGS *)arg)->arg1;
    args.arg2 = ((ARGS *)arg)->arg2;
    args.arg3 = ((ARGS *)arg)->arg3;
    free(arg);
    
    return args.callback(args.arg1, args.arg2, args.arg3);
} 
int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        FUNCALLBACK start_routine, void *arg1, void * arg2, void * arg3)
{
    ARGS * args;
    
    if (NULL == (args=(ARGS *)malloc(sizeof(ARGS))))
        return -1;
     
    /* push args into buffer */
    args->callback = start_routine;
    args->arg1 = arg1;
    args->arg2 = arg2;
    args->arg3 = arg3;
    
    return pthread_create(pthread, attr, sub_thread_entry, args);
}
```
帖子地址：http://bbs.chinaunix.net/thread-1742995-1-1.html
感谢各位提供的方法，谢谢。
