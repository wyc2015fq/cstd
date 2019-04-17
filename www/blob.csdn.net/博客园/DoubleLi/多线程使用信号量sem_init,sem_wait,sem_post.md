# 多线程使用信号量sem_init,sem_wait,sem_post - DoubleLi - 博客园






信号量的主要函数有：

```
int sem_init(sem_t *sem,int pshared,unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
```

sem_init用于对指定信号初始化，pshared为0，表示信号在当前进程的多个线程之间共享，value表示初始化信号的值。 
sem_wait可以用来阻塞当前线程，直到信号量的值大于0，解除阻塞。解除阻塞后，sem的值-1，表示公共资源被执行减少了。例如：如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，信号量的值将-1。当初始化value=0后，使用sem_wai会阻塞这个线程，这个线程函数就会等待其它线程函数调用sem_post增加了了这个值使它不再是0，才开始执行,然后value值-1。 
sem_post用于增加信号量的值+1，当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不在阻塞，选择机制由线程的调度策略决定。 
下面是例程：

```cpp
#include <stdio.h>
  #include <semaphore.h>

  sem_t sem;
  void* func1(void)
  {
      sem_wait(&sem);
      int *running=arg;
      printf("thread running1\n");
      printf("%d\n",*running);
  }

  void* func2(void* arg)
  {
      printf("pthread2 running\n");
      sem_post(&sem);
  }
  int main()
  {
      sem_init(&sem,0,0);
      pthread_t thread[2];
      int a=5;
      pthread_create(&(thread[0]),NULL,(void*)func,(void*)&a);
      printf("main thread running\n");
      sleep(10);
      pthread_create(&(thread[1]),NULL,(void*)func2,(void*)&a);
      printf("main thread running2\n");
      pthread_join(thread[0],NULL);
      pthread_join(thread[1],NULL);
      sem_destory(&sem);
  }
```



运行结果为： 
func1被阻塞，直到fun2把信号量+1，才开始执行。

```
main pthread running
main pthread running
pthread2 running
pthread1 running
5
```









