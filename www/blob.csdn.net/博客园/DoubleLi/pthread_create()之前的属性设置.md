# pthread_create()之前的属性设置 - DoubleLi - 博客园






一.pthread_create()之前的属性设置
1．线程属性设置
我们用pthread_create函数创建一个线程，在这个线程中，我们使用默认参数，即将该函数的第二个参数设为NULL。的确，对大多数程序来说，使用默认属性就够了，但我们还是有必要来了解一下线程的有关属性。
属性结构为pthread_attr_t，它同样在头文件pthread.h中定义，属性值不能直接设置，须使用相关函数进行操作，初始化的函数为![](http://writeblog.csdn.net/Editor/FCKeditor/editor/images/spacer.gif)**pthread_attr_init**，这个函数必须在pthread_create函数之前调用。属性对象主要包括是否绑定、是否分离、
堆栈地址、堆栈大小、优先级。默认的属性为非绑定、非分离、缺省的堆栈、与父进程同样级别的优先级。

2．绑定
关于线程的绑定，牵涉到另外一个概念：轻进程（LWP：Light Weight Process）。轻进程可以理解为内核线程，它位于用户层和系统层之间。系统对线程资源的分配、对线程的控制是通过轻进程来实现的，一个轻进程可以控制一个或多个线程。默认状况下，启动多少轻进程、哪些轻进程来控制哪些线程是由系统来控制的，这种状况即称为非绑定的。绑定状况下，则顾名思义，即某个线程固定的"绑"在一个轻进程之上。被绑定的线程具有较高的响应速度，这是因为CPU时间片的调度是面向轻进程的，绑定的线程可以保证在需要的时候它总有一个轻进程可用。通过设置被绑定的轻进程的优先级和调度级可以使得绑定的线程满足诸如实时反应之类的要求。
　　设置线程绑定状态的函数为 pthread_attr_setscope，它有两个参数，第一个是指向属性结构的指针，第二个是绑定类型，它有两个取值： PTHREAD_SCOPE_SYSTEM（绑定的）和PTHREAD_SCOPE_PROCESS（非绑定的）。下面的代码即创建了一个绑定的线程。
＃i nclude 
pthread_attr_t attr;
pthread_t tid;
/*初始化属性值，均设为默认值*/
**pthread_attr_init**(&attr); 
pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
pthread_create(&tid, &attr, (void *) my_function, NULL);


3．线程分离状态

   线程的分离状态决定一个线程以什么样的方式来终止自己。非分离的线程终止时，其线程ID和退出状态将保留，直到另外一个线程调用pthread_join.分离的线程在当它终止时，所有的资源将释放，我们不能等待它终止。

    设置线程分离状态的函数为 pthread_attr_setdetachstate（pthread_attr_t *attr, int detachstate）。第二个参数可选为PTHREAD_CREATE_DETACHED（分离线程）和 PTHREAD _CREATE_JOINABLE（非分离线程）。这里要注意的一点是，如果设置一个线程为分离线程，而这个线程运行又非常快，它很可能在 pthread_create函数返回之前就终止了，它终止以后就可能将线程号和系统资源移交给其他的线程使用，这样调用pthread_create的线程就得到了错误的线程号。要避免这种情况可以采取一定的同步措施，最简单的方法之一是可以在被创建的线程里调用 pthread_cond_timewait函数，让这个线程等待一会儿，留出足够的时间让函数pthread_create返回。设置一段等待时间，是在多线程编程里常用的方法。



4．优先级

    它存放在结构sched_param中。用函数pthread_attr_getschedparam和函数 pthread_attr_setschedparam进行存放，一般说来，我们总是先取优先级，对取得的值修改后再存放回去。下面即是一段简单的例子。

＃i nclude 
＃i nclude 
pthread_attr_t attr; pthread_t tid;
sched_param param;
int newprio=20; 
/*初始化属性*/
**pthread_attr_init**(&attr); 
/*设置优先级*/
pthread_attr_getschedparam(&attr, ¶m);  
param.sched_priority=newprio;
pthread_attr_setschedparam(&attr, ¶m);
pthread_create(&tid, &attr, (void *)myfunction, myarg);


二．线程数据处理


和进程相比，线程的最大优点之一是数据的共享性，各个进程共享父进程处沿袭的数据段，可以方便的获得、修改数据。但这也给多线程编程带来了许多问题。我们必须当心有多个不同的进程访问相同的变量。许多函数是不可重入的，即同时不能运行一个函数的多个拷贝（除非使用不同的数据段）。在函数中声明的静态变量常常带来问题，函数的返回值也会有问题。因为如果返回的是函数内部静态声明的空间的地址，则在一个线程调用该函数得到地址后使用该地址指向的数据时，别的线程可能调用此函数并修改了这一段数据。在进程中共享的变量必须用关键字volatile来定义，这是为了防止编译器在优化时（如gcc中使用-OX参数）改变它们的使用方式。为了保护变量，我们必须使用信号量、互斥等方法来保证我们对变量的正确使用。

1．线程数据

   在单线程的程序里，有两种基本的数据：全局变量和局部变量。但在多线程程序里，还有第三种数据类型：线程数据（TSD: Thread-Specific Data）。它和全局变量很象，在线程内部，各个函数可以象使用全局变量一样调用它，但它对线程外部的其它线程是不可见的。例如我们常见的变量errno，它返回标准的出错信息。它显然不能是一个局部变量，几乎每个函数都应该可以调用它；但它又不能是一个全局变量，否则在 A线程里输出的很可能是B线程的出错信息。要实现诸如此类的变量，我们就必须使用线程数据。我们为每个线程数据创建一个键，它和这个键相关联，在各个线程里，都使用这个键来指代线程数据，但在不同的线程里，这个键代表的数据是不同的，在同一个线程里，它代表同样的数据内容。
　　和线程数据相关的函数主要有4个：创建一个键；为一个键指定线程数据；从一个键读取线程数据；删除键。
　　创建键的函数原型为：
　　int pthread_key_create __P ((pthread_key_t *__key,void (*__destr_function) (void *)));                                                                                                                                                 第一个参数为指向一个键值的指针，第二个参数指明了一个destructor函数，如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。这个函数常和函数pthread_once ((pthread_once_t*once_control, void (*initroutine) (void)))一起使用，为了让这个键只被创建一次。函数pthread_once声明一个初始化函数，第一次调用pthread_once时它执行这个函数，以后的调用将被它忽略。
int pthread_key_delete(pthread_key_t *key);
该函数用于删除一个由pthread_key_create 函数调用创建的键。调用成功返回值为0，否则返回错误代码。
在下面的例子中，我们创建一个键，并将它和某个数据相关联。我们要定义一个函数 createWindow，这个函数定义一个图形窗口（数据类型为Fl_Window *，这是图形界面开发工具FLTK中的数据类型）。由于各个线程都会调用这个函数，所以我们使用线程数据。
/* 声明一个键*/
pthread_key_t myWinKey;
/* 函数 createWindow */
void createWindow ( void ) {
Fl_Window * win;
static pthread_once_t once= PTHREAD_ONCE_INIT;
/* 调用函数createMyKey，创建键*/
pthread_once ( & once, createMyKey) ;
/*win指向一个新建立的窗口*/
win=new Fl_Window( 0, 0, 100, 100, "MyWindow");
/* 对此窗口作一些可能的设置工作，如大小、位置、名称等*/
setWindow(win);
/* 将窗口指针值绑定在键myWinKey上*/
pthread_setpecific ( myWinKey, win);
}
/* 函数 createMyKey，创建一个键，并指定了destructor */
void createMyKey ( void ) {
pthread_keycreate(&myWinKey, freeWinKey);
}
/* 函数 freeWinKey，释放空间*/
void freeWinKey ( Fl_Window * win){
delete win;
}
这样，在不同的线程中调用函数createMyWin，都可以得到在线程内部均可见的窗口变量，这个变量通过函数 pthread_getspecific得到。在上面的例子中，我们已经使用了函数pthread_setspecific来将线程数据和一个键绑定在一起。这两个函数的原型如下：

int pthread_setspecific __P ((pthread_key_t __key,__const void *__pointer)); 该函数设置一个线程专有数据的值，赋给由pthread_key_create 创建的键，调用成功返回值为0，否则返回错误代码。
void *pthread_getspecific __P ((pthread_key_t __key));                                                  该函数获得绑定到指定键上的值。调用成功，返回给定参数key 所对应的数据。如果没有数据连接到该键，则返回NULL。

　　这两个函数的参数意义和使用方法是显而易见的。要注意的是，用pthread_setspecific为一个键指定新的线程数据时，必须自己释放原有的线程数据以回收空间。这个过程函数pthread_key_delete用来删除一个键，这个键占用的内存将被释放，但同样要注意的是，它只释放键占用的内存，并不释放该键关联的线程数据所占用的内存资源，而且它也不会触发函数pthread_key_create中定义的destructor函数。线程数据的释放必须在释放键之前完成。


2．互斥锁

  假设各个现成向同一个文件顺序写入数据，最后得到的结果是不可想象的。所以用互斥锁来保证一段时间内只有一个线程在执行一段代码。
  使用int pthread_mutex_lock锁住互斥锁，使用int pthread_mutex_unlock解琐。
如果我们试图为一个已被其他线程锁住的互斥锁加锁，程序便会阻塞直到该互斥对象解锁。
如果在共享内存中分配一个互斥锁，我们必须在运行时调用ptgread_mutex_init函数尽心初始化。
void reader_function ( void );
void writer_function ( void ); 
char buffer;
int buffer_has_item=0;
pthread_mutex_t mutex;
struct timespec delay;
void main ( void ){
pthread_t reader;
/* 定义延迟时间*/
delay.tv_sec = 2;
delay.tv_nec = 0;
/* 用默认属性初始化一个互斥锁对象*/
pthread_mutex_init (&mutex,NULL);
pthread_create(&reader, pthread_attr_default, (void *)&reader_function), NULL);
writer_function( );
}
void writer_function (void){
while(1){
/* 锁定互斥锁*/
pthread_mutex_lock (&mutex);
if (buffer_has_item==0){
buffer=make_new_item( );
buffer_has_item=1;
}
/* 打开互斥锁*/
pthread_mutex_unlock(&mutex);
pthread_delay_np(&delay);
}
}
void reader_function(void){
while(1){
pthread_mutex_lock(&mutex);
if(buffer_has_item==1){
consume_item(buffer);
buffer_has_item=0;
}
pthread_mutex_unlock(&mutex);
pthread_delay_np(&delay);
}
}
函数 pthread_mutex_init用来生成一个互斥锁。NULL参数表明使用默认属性。如果需要声明特定属性的互斥锁，须调用函数 pthread_mutexattr_init。函数pthread_mutexattr_setpshared和函数 pthread_mutexattr_settype用来设置互斥锁属性。前一个函数设置属性pshared，它有两个取值， PTHREAD_PROCESS_PRIVATE和PTHREAD_PROCESS_SHARED。前者用来不同进程中的线程同步，后者用于同步本进程的不同线程。在上面的例子中，我们使用的是默认属性PTHREAD_PROCESS_ PRIVATE。后者用来设置互斥锁类型，可选的类型有PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_ERRORCHECK、 PTHREAD_MUTEX_RECURSIVE和PTHREAD _MUTEX_DEFAULT。它们分别定义了不同的上所、解锁机制，一般情况下，选用最后一个默认属性。
需要注意的是在使用互斥锁的过程中很有可能会出现死锁：两个线程试图同时占用两个资源，并按不同的次序锁定相应的互斥锁，例如两个线程都需要锁定互斥锁1和互斥锁2，a线程先锁定互斥锁1，b线程先锁定互斥锁2，这时就出现了死锁。此时我们可以使用函数 pthread_mutex_trylock，它是函数pthread_mutex_lock的非阻塞版本，当它发现死锁不可避免时，它会返回相应的信息，程序员可以针对死锁做出相应的处理。另外不同的互斥锁类型对死锁的处理不一样，但最主要的还是要程序员自己在程序设计注意这一点。
3．条件变量
互斥锁一个明显的缺点是它只有两种状态：锁定和非锁定。而条件变量通过允许线程阻塞和等待另一个线程发送信号的方法弥补了互斥锁的不足，它常和互斥锁一起使用。使用时，条件变量被用来阻塞一个线程，当条件不满足时，线程往往解开相应的互斥锁并等待条件发生变化。一旦其它的某个线程改变了条件变量，它将通知相应的条件变量唤醒一个或多个正被此条件变量阻塞的线程。这些线程将重新锁定互斥锁并重新测试条件是否满足。一般说来，条件变量被用来进行线承间的同步。
条件变量的结构为pthread_cond_t，函数pthread_cond_init（）被用来初始化一个条件变量。它的原型为：

int pthread_cond_init __P ((pthread_cond_t *__cond,__const pthread_condattr_t *__cond_attr));

其中cond是一个指向结构pthread_cond_t的指针，cond_attr是一个指向结构pthread_condattr_t的指针。结构 pthread_condattr_t是条件变量的属性结构，和互斥锁一样我们可以用它来设置条件变量是进程内可用还是进程间可用，默认值是 PTHREAD_ PROCESS_PRIVATE，即此条件变量被同一进程内的各个线程使用。注意初始化条件变量只有未被使用时才能重新初始化或被释放。
在pthread中，条件变量是一个pthread_cond_t类型的变量，条件变量使用下面两个函数：

pthread_cond_wait函数用于阻塞，线程可以被函数pthread_cond_signal和函数    pthread_cond_broadcast唤醒，但是要注意的是，条件变量只是起阻塞和唤醒线程的作用，具体的判断条件还需用户给出，例如一个变量是否为0等等，这一点我们从后面的例子中可以看到。线程被唤醒后，它将重新检查判断条件是否满足，如果还不满足，一般说来线程应该仍阻塞在这里，被等待被下一次唤醒。这个过程一般用while语句实现。
另一个用来阻塞线程的函数是pthread_cond_timedwait（）它比函数pthread_cond_wait（）多了一个时间参数，经历abstime段时间后，即使条件变量不满足，阻塞也被解除。
函数pthread_cond_signal（）用来释放被阻塞在条件变量cond上的一个线程。
函数pthread_cond_broadcast（pthread_cond_t *cond）用来唤醒所有被阻塞在条件变量cond上的线程。这些线程被唤醒后将再次竞争相应的互斥锁，所以必须小心使用这个函数。
下面是使用函数pthread_cond_wait（）和函数pthread_cond_signal（）的一个简单的例子:
pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
decrement_count　() {
pthread_mutex_lock (&count_lock);
while(count==0) 
pthread_cond_wait( &count_nonzero, &count_lock);
count=count -1;
pthread_mutex_unlock (&count_lock);
}

increment_count(){
pthread_mutex_lock(&count_lock);
if(count==0)
pthread_cond_signal(&count_nonzero);
count=count+1;
pthread_mutex_unlock(&count_lock);
}
count值为0时， decrement函数在pthread_cond_wait处被阻塞，并打开互斥锁count_lock。此时，当调用到函数 increment_count时，pthread_cond_signal（）函数改变条件变量，告知decrement_count（）停止阻塞。










