# pthread_cleanup_push/pop - nosmatch的专栏 - CSDN博客
2012年01月09日 11:09:53[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：2814
void pthread_cleanup_push(void (*routine)(void*), void *arg);
void pthread_cleanup_pop(int execute);//这里的int参数，0是不执行push的内容，非0是执行。
原型很简单，功能跟atexit(）差不多，只不过一个是线程一个是进程。
用来设置在push/pop内线程退出时要做的事情。
需要注意的问题有几点：
1，push与pop一定是成对出现的，其实push中包含"{"而pop中包含"}"，少一个不行。
2，push可以有多个，同样的pop也要对应的数量，遵循"先进后出原则"。
push进去的函数可能在以下三个时机执行：
1，显示的调用pthread_exit();
或
2，在cancel点线程被cancel。
或
3，pthread_cleanup_pop()的参数不为0时。
以上动作都限定在push/pop涵盖的代码内。
前面的2个比较好理解，关键是pthread_cleanup_pop参数问题，其实int那是因为c没有bool，这里的参数只有0与非0的区别，对pthread_cleanup_pop，参数是5和10都是一样的，都是非0。
我们经常会看到这样的代码：
void child(void *t)
{
pthread_cleanup_push(pthread_mutex_unlock,&mutex);
pthread_mutex_lock(&mutex);
..............
pthread_mutex_unlock(&mutex);
pthread_cleanup_pop(0);
}
为啥pthread_cleanup_pop是0呢，他根本就不会执行push进来的函数指针指向的函数，没错，是不执行，真要执行了就麻烦了。
那为啥还得写这句呢？
那是因为push和pop是必须成对出现的，不写就是语法错误。
这么写的目的主要是为了保证mutex一定可以被unlock，因为，在pthread_mutex_lock和 pthread_mutex_unlock之间可能发生任何事情，比如，存在N个cancel点导致线程被main或者其他线程给cancel，而 cancel动作是不会释放互斥锁的，这样就锁死啦。
通过pthread_cleanup_push加入一个函数pthread_mutex_unlock，参照上面执行时机的说明，在线程被cancel的时候，就可以作释放锁的清理动作。如果线程正常执行，知道运行到pthread_cleanup_pop，那么锁已经被中间代码里的 pthread_mutex_unlock给释放了，这时候如果pthread_cleanup_pop的参数不为0，则会再次释放，错就错在这里了，释放了两次。
所以，pthread_cleanup_pop(0)是必须的，因为，首先要成对出现，其次，我们不希望他真的执行到这里释放两次。
同样道理：
void *exit1(void *t)
{
printf("exit1\n");
}
void *child(void *t)
{
pthread_cleanup_push(exit1,NULL);
.....
pthread_exit(NULL);
pthread_cleanup_pop(0);
}
exit1函数是在pthread_exit(NULL)的时候执行的，pthread_cleanup_pop的参数是不是0没有关系，因为根本执行不到这里。
而换成这样：
pthread_cleanup_push(exit1,NULL);
......
pthread_cleanup_pop(0);
pthread_exit(NULL);
则exit1不会执行，因为pop的参数是0，如果把pop的参数修改为1则会执行exit1，那会执行两次吗？NO，因为pthread_exit在push/pop block的外面，他不会触发exit1.
pthread_cleanup_push(exit1,NULL);
pthread_cleanup_push(exit2,NULL);
........
pthread_cleanup_pop(0);
pthread_cleanup_pop(1);
那0和1分别控制的是谁？配对原则，从外到里一对一对的拔掉就可以了，显然，0控制的是exit2. 
**pthread_cleanup_push(void(*fun)(void* arg),void* arg);**
**pthread_cleanup_pop(int);**
**注意pthread_cleanup_pop(int)用法**
**如果pthread_cleanup_push 和pthread_cleanup_pop 包含的代码正常执行了,那么在遇到**
**pthread_cleanup_pop这句时，如果参数为0,将不会执行push进的函数，否则执行**
