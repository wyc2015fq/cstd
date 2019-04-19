# 《UNIX环境高级编程》笔记——线程 - yixianfeng41的专栏 - CSDN博客
2015年12月30日 20:09:22[第2梦](https://me.csdn.net/yixianfeng41)阅读数：840标签：[linux 线程																[线程](https://so.csdn.net/so/search/s.do?q=线程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux 线程&t=blog)
个人分类：[Linux/Unix](https://blog.csdn.net/yixianfeng41/article/category/6006179)
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
## 一、线程与进程概念
### 进程是具有独立功能的程序在某个数据集合上的一次运行活动，它是系统进行资源分配和调度的一个独立单位；
### 线程是进程的一个实体，一个进程可以包含一个或多个线程，线程是CPU调度和分派的基本单位。
## 二、使用多线程的好处
1） 简化处理异步事件的代码。通过为每种事件类型的处理分配单独的线程，能够简化处理异步事件的代码。
2）共享方便。多个线程自动地可以访问相同的存储地址空间和文件描述符。
3）提高程序吞吐量。在只有一个控制线程的情况下，单个进程需要多个任务时，实际上需要把这些任务串行化；有了多个控制线程，相互独立的任务的处理就可以交叉运行。
4） 改善程序的响应时间。交互的程序可以使用多线程来改善程序响应时间，多线程可以把程序中处理用户输入输出的部分与其他部分分开。
## 三、线程的创建
与进程有进程ID一样，线程也有线程ID.进程ID在整个系统中是唯一的，但线程ID不同，线程ID只在它所属的进程环境中有效。进程ID用pid_t数据类型来表示；线程ID用pthread_t数据类型来表示。
新增线程，可以通过调用pthread_create函数来创建。
```cpp
#include <pthread.h> 
      int   pthread_create ( pthread_t *tid, const pthread_attr_t  *attr,
      void * (*func) (void *),  void *arg);
```
参数含义：
tid指向的内存单元被设置为新创建线程的线程ID；
attr参数用于定制各种不同的线程属性；
新创建的线程从func函数的地址开始运行，该函数只有 一个无类型指针参数arg，如果要向func函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构的地址作为arg参数传入。
线程创建并不能保证是新建线程先运行还是调用线程先运行。
新创建的线程有权访问它所在进程的地址空间，他将继承父进程的浮点环境以及信号屏蔽字，不过，已经处于阻塞队列的信号将被清空。具体，
同一进程的线程共享：
1）进程指令；
2）大多数数据；
3）打开的文件（即描述符）；
4）信号处理函数和信号处理；
5）当前工作目录；
6）用户ID和组ID;
每个线程独有的：
1）线程ID;
2）寄存器集合，包括程序计数器和栈指针；
3）栈（用于存放局部变量和返回地址）；
4）errno;
5）优先级。
新创建的线程默认以分离模式运行，也就是说，当线程结束时 ，系统将自动回收期资源，并扔掉期结束状态。当然，我们也可以通过设置其线层属性来使线程以非分离模式运行，此环境下，线程结束时，必须有其他线程调用join函数来释放其资源并获取其结束状态。
## 四、线程的终止
 如果进程中任何一个线程调用了exit,_Exit或者_exit，那么整个进程就会终止。与其类似，如果信号的默认动作是终止进程，那么信号发送到线程会终止整个进程。
单个线程可以通过下列三种方式退出：
1）线程只是从启动例程中返回，返回值是线程的退出码；
2）线程可以被同一进程的其他线程取消；
3）线程调用pthread_exit。
```cpp
void pthread_exit（void *rval_ptr）；
```
其中rval_ptr是一个无类型指针，与传递给启动例程的单个参数类型。进程中的其他例程可以通过调用pthread_join函数访问到这个 指针。 
```cpp
int pthread_join(pthread_t thread ,void  ** rval_ptr);
```
现在来看看pthread_join这个函数，这个函数跟进程里waitpid这个函数有点类似。
pthread_join的功能是等待线程终止。调用了pthread_join后，调用线程将一直被阻塞，直到指定的线程调用pthread_exit、从启动例程返回或者被取消。可以通过调用pthread_join自动把线程置于分离状态，这样资源就可以恢复。如果对线程的返回值并不感兴趣，可以把rval_ptr置为null。在这种情况下，调用pthread_join函数将指定的线程终止，但并不获取线程的种子状态。
### 线程的分离状态   
在任何一个时间点上，线程是可结合的（joinable），或者是分离的（detached）。
一个可结合的线程能够被其他线程收回其资源和杀死；在被其他线程回收之前，它的存储器资源（如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或杀死的，它的存储器资源在它终止时由系统自动释放。
线程的分离状态决定一个线程以什么样的方式来终止自己。在默认情况下线程是非分离状态的，这种情况下，原有的线程等待创建的线程结束。只有当pthread_join（）函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。而分离线程不是这样子的，它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放   系统资源。程序员应该根据自己的需要，选择适当的分离状态。所以如果我们在创建线程时就知道不需要了解线程的终止状态，则可以pthread_attr_t结构中的detachstate 线程属性，让线程以分离状态启动。
把指定的进程变为分离状态：
```cpp
int pthread_detach(pthread_t tid);
```
如果把自己变为分离的，则可以这样：
```cpp
pthread_de_tatch(pthread_self);
```
### **线程退出调用函数**
线程可以安排它退出时需要调用的函数，这与进程可以调用atexit函数安排进程退出时需要调用的函数是类似的。这样的函数称为线程清理处理程序。当这几种情况时会调用清理函数：
1）调用pthread_exit；
2）响应取消请求；
3）用非零execute参数调用pthread_cleanup_pop时。 注意当你从启动例程等情况，即便有清理函数，也不会调用的。
```cpp
void  pthread_cleanup(void(*rtn)(void*),void * arg);
   void pthread_cleanup_pop(int execute);
```
## 五、线程同步
当多个控制线程共享相同的内存时，需要确保每一个线程看到一致的数据视图，因此必须进行线程同步。怎么实现同步了，unix中有三种同步机制：互斥、读写锁、以及条件变量。
#### 1、互斥量
互斥量从本质上来说是一把锁，在访问共享资源前对互斥量进行加锁，在访问完成后释放互斥量的锁。对互斥量加锁后，任何其他试图再次对互斥量加锁的线程将会被阻塞直到当前线程释放该互斥锁。
互斥量用pthread_mutex_t数据类型来表示，在使用互斥量以前，必须首先对它进行初始化。静态互斥量可以初始化为常量PTHREAD_MUTEX_INITIALIZER ；动态的分配互斥量可以在运行时通过调用pthread_mutex_init函数进行初始化。  如果动态的分配互斥量（例如通过调用malloc函数），那么在释放内存前需要调pthread_mutex_destroy。
对互斥量进行加锁，需要调用pthread_mutex_lock，如果互斥量已经上锁，调用线程将阻塞直到互斥量被解锁。对互斥量解锁，需要调用pthread_mutex_unlock.
     #include <pthread.h>  
     int pthread_mutex_lock(pthread_mutex_t  *mptr);  
     int pthread_mutex_trylock (pthread_mutex_t  *mptr);   
     int pthread_mutex_unlock (pthread_mutex_t  *mptr);
如果线程不希望被阻塞，它可以使用pthread_mutex_trylock尝试对互斥量进行加锁。如果调用pthread_mutex_trylock时互斥量处于未锁住状态,那么pthread_mutex_trylock将锁住互斥量，不会出现阻塞并返回0，否则pthread_mutex_trylock会失败，不能锁住互斥量，而返回失败。
在加锁时，需要注意的时，不要产生死锁，可以通过小心地控制互斥量加锁的顺序来避免产生死锁。
#### 2、读写锁
读写锁可以有三种状态：读模式下加锁状态，写模式下加锁状态，不加锁状态。一次只有一个线程可以占有写模式的读写锁，但是多个线程可以同时占有读模式的读写锁。与互斥量一样，读写锁在使用前必须初始化，在释放他们的底层前必须销毁。
```cpp
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destory(pthread_rwlock_t *rwlock);
```
要在读模式下锁定读写锁，需要调用pthread_rwlock_rdlock；要在写模式下锁定读写锁，需要调用pthread_rwlock_wrlock；不管以何种方式锁住读写锁，都可以调用pthread_rwlock_unlock进行解锁。
```cpp
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); 
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock); 
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```
为了避免产生死锁等错误情况，读写锁也定义了有条件原语。
```cpp
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 
int pthread_rwlock_tryrwlock(pthread_rwlock_t *rwlock);
```
#### 3、条件变量
条件变量给多个线程提供了一个回合的场所。条件变量与互斥量一起使用时，允许线程以无竞争的方式等待特定的条件发生。条件本身是由互斥量保护的。
条件变量使用之前必须先进行初始化，pthread_cond_t数据结构代表的条件变量可以用两种方式进行初始化，可以把常量PTHREAD_COND_INITIALIZER赋给静态分配的条件变量，但是如果条件变量是动态分配的，可以使用pthread_cond_init函数初始化。在释放底层的内存空间之前，可以使用pthread_cond_destroy函数对条件变量进行去除初始化。
```cpp
int pthread_cond_init(pthread_cond_t *restrict cond, pthread_condattr_t *restrict attr); 
int pthread_cond_destroy(pthread_cond_t *cond);
```
使用pthread_cond_wait等待条件变成真，传递给pthread_cond_wait的互斥量对条件进行保护，调用者把锁住的互斥量传给函数。函数把调用线程放到等待条件的线程列表上，然后对互斥量解锁，这两个操作时原子操作，这样就关闭了条件检查和线程进入休眠状态等待条件改变这两个操作之间的时间通道，这样线程就不会错过条件的任何变化，pthread_cond_wait 返回时，互斥量再次被锁住。
```cpp
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
int pthread_cond_timewait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex,const struct timespec *restrict timeout);
```
pthread_cond_timedwait函数的工作方式与pthread_cond_wait函数相似，只是多了一个timeout，timeout值指定了等待的时间。
通常waits在条件变量上的使用的code框架是这样的：
```cpp
struct {
pthread_mutex_t	mutex;
pthread_cond_t	cond;
/* variables for maintaining the condition */
  	} varx = {
     PTHREAD_MUTEX_INITIALIZER, ç initialize mutex
     PTHREAD_COND_INITIALIZER,…ç initialize condition var
     };
 
pthread_mutex_lock(&var.mutex);
while (condition is false)
pthread_cond_wait(&var.cond, &var.mutex);
           …modify condition….
pthread_mutex_unlock(&var.mutex);
```
有两个函数可以用于通知线程条件已经满足。pthread_cond_signal函数将唤醒等待该线程的某个线程，而pthread_cond_broadcast函数将唤醒等待该条件的所有线程。调用pthread_cond_signal或者pthread_cond_broadcast，也称为向线程或条件发送信号。必须注意一定要在改变状态以后再发给线程发送信号。
```cpp
int pthread_cond_siganl(pthread_cond_t *cond); 
int pthread_cond_broadcast(pthread_cond_t *cond);
```
具体怎么用了，如下：
```cpp
pthread_mutex_lock(&mutex);
     condition_is_false=false;
     pthread_cond_signal(&cond)
pthread_mutex_unlock(&mutex)
```
### 六、线程控制
#### 1. 重入
重入一般可以理解为一个函数同时被多个程序调用；可重入指得是函数在任何时候都可以被中断，而一段时间之后又可以恢复运行，而相应的数据不会破坏或者丢失。
如果一个函数在同一时刻可以被多个线程安全调用，就称该函数是线程安全的。如果一个函数对多个线程来说是可重入的，则说这个函数是线程安全的，但并不能说明对信号处理程序来说该函数是可重入的。如果函数对异步信号处理程序的重入是安全的，那么就可以说函数是异步-信号安全的。
#### 2. 线程和信号
每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。这意味着尽管按个线程可以阻止某些信号，但当线程修改了与某个信号的处理行为后，所有的线程都必须共享这个处理行为的改变。

