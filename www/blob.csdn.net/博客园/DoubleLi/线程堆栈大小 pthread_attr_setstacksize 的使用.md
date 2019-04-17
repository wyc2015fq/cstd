# 线程堆栈大小 pthread_attr_setstacksize 的使用 - DoubleLi - 博客园






pthread_create 创建线程时，若不指定分配堆栈大小，系统会分配默认值，查看默认值方法如下：

# ulimit -s
8192
#

上述表示为8M；单位为KB。

也可以通过# ulimit -a 其中 stack size 项也表示堆栈大小。ulimit -s  value 用来重新设置stack 大小。

一般来说 默认堆栈大小为 8388608; 堆栈最小为 16384 。 单位为字节。

堆栈最小值定义为 PTHREAD_STACK_MIN ，包含#include <limits.h>后可以通过打印其值查看。对于默认值可以通过pthread_attr_getstacksize (&attr, &stack_size); 打印stack_size来查看。

尤其在嵌入式中内存不是很大，若采用默认值的话，会导致出现问题，若内存不足，则 pthread_create 会返回 12，定义如下：

#define EAGAIN 11

#define ENOMEM 12 /* Out of memory */

上面了解了堆栈大小，下面就来了解如何使用 pthread_attr_setstacksize 重新设置堆栈大小。先看下它的原型：

#include <pthread.h>
int pthread_attr_setstacksize(pthread_attr_t **attr*, size_t *stacksize*);

attr 是线程属性变量；stacksize 则是设置的堆栈大小。 返回值0，-1分别表示成功与失败。

这里是使用方法

  pthread_t thread_id;

  int ret ,stacksize = 20480; /*thread 堆栈设置为20K，stacksize以字节为单位。*/

  pthread_attr_t attr;
  ret = pthread_attr_init(&attr); /*初始化线程属性*/

  if (ret != 0)

      return -1;
  ret = pthread_attr_setstacksize(&attr, stacksize);

  if(ret != 0)

      return -1;
  ret = pthread_create (&thread_id, &attr, &func, NULL);

  if(ret != 0)

      return -1;
  ret = pthread_attr_destroy(&attr); /*不再使用线程属性，将其销毁*/

  if(ret != 0)

    return -1;









