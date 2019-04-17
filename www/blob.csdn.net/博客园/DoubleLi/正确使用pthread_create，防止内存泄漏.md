# 正确使用pthread_create，防止内存泄漏 - DoubleLi - 博客园






近日，听说pthread_create会造成内存泄漏，觉得不可思议，因此对posix(nptl)的线程创建和销毁进行了分析。



分析结果：**如果使用不当，确实会造成内存泄漏。**

产生根源**：pthread_create默认创建的线程是非detached的。**

预防方式：**要么创建detached的线程，要么线程线程的start_routine结束之前detached，要么join**



分析过程如下：



**  1.查看pthread_create源代码，核心代码如下（nptl/pthread_create.c）：**




点击(此处)折叠或打开


- int
- __pthread_create_2_1 (newthread, attr, start_routine, arg)
-      pthread_t *newthread;
-      const pthread_attr_t *attr;
-      void *(*start_routine) (void *); 
-      void *arg;
- {
-   STACK_VARIABLES;
- 
-   const struct pthread_attr *iattr = (struct pthread_attr *) attr;
-   if (iattr == NULL)
-     /* Is this the best idea? On NUMA machines this could mean
-        accessing far-away memory. */
-     iattr = &default_attr;
- 
-   struct pthread *pd = NULL;
- int err = ALLOCATE_STACK (iattr, &pd);//为tcb分配内存
-   if (__builtin_expect (err != 0, 0)) 
-     /* Something went wrong. Maybe a parameter of the attributes is
-        invalid or we could not allocate memory. */
-     return err;
- //……
- err = create_thread (pd, iattr, STACK_VARIABLES_ARGS);//正式创建线程
- 




**2.查看createthread.c(nptl/sysdeps/pthread/createthread.c)**




点击(此处)折叠或打开


- static int 
- create_thread (struct pthread *pd, const struct pthread_attr *attr,
-            STACK_VARIABLES_PARMS)
- {
- #ifdef TLS_TCB_AT_TP
-   assert (pd->header.tcb != NULL);
- #endif
- //……
- 
- 
- int res = do_clone (pd, attr, clone_flags, **start_thread**,
-                   STACK_VARIABLES_ARGS, 1);//clone一个进程
- 


**3.接着看start_thread（nptl/pthread_create.c）做了什么**




点击(此处)折叠或打开


- static int
- start_thread (void *arg)
- {
-   struct pthread *pd = (struct pthread *) arg;
- //……
- 
-       /* Run the code the user provided. */
- #ifdef CALL_THREAD_FCT
-       THREAD_SETMEM (pd, result, CALL_THREAD_FCT (pd)); 
- #else
- THREAD_SETMEM (pd, result, pd->start_routine (pd->arg)); //正式启动线程的执行，并等待执行完成
- #endif
- //……
- 
- if (IS_DETACHED (pd))
-     /* Free the TCB.  */
-     __free_tcb (pd);//如果设置detached标志，则释放tcb占用的内容，否则直接返回
-   else if (__builtin_expect (pd->cancelhandling & SETXID_BITMASK, 0))
-     {
-       /* Some other thread might call any of the setXid functions and expect
-      us to reply.  In this case wait until we did that.  */
-       do
-     lll_futex_wait (&pd->setxid_futex, 0, LLL_PRIVATE);
-       while (pd->cancelhandling & SETXID_BITMASK);
- 
-       /* Reset the value so that the stack can be reused.  */
-       pd->setxid_futex = 0;
-     }
- 



从上面的过程，我们可以看到，如果在创建线程的时候，如果没有设置detached标志，则tcb内存永远不会释放



**接下来，我们看看pthread_detach(npth/pthread_detach.c)做了什么**




点击(此处)折叠或打开


- int
- pthread_detach (th)
-      pthread_t th; 
- {
-   struct pthread *pd = (struct pthread *) th; 
- 
-   /* Make sure the descriptor is valid. */
-   if (INVALID_NOT_TERMINATED_TD_P (pd))
-     /* Not a valid thread handle. */
-     return ESRCH;
- 
-   int result = 0;
- 
-   /* Mark the thread as detached. */
-   if (atomic_compare_and_exchange_bool_acq (&pd->joinid, pd, NULL))
-     { 
-       /* There are two possibilities here. First, the thread might
-      already be detached. In this case we return EINVAL.
-      Otherwise there might already be a waiter. The standard does
-      not mention what happens in this case. */
-       if (IS_DETACHED (pd))
-     result = EINVAL;
-     } 
-   else
-     /* Check whether the thread terminated meanwhile. In this case we
-        will just free the TCB. */
-     if ((pd->cancelhandling & EXITING_BITMASK) != 0)
-       /* Note that the code in __free_tcb makes sure each thread
-      control block is freed only once. */
-  __free_tcb (pd);//经过一系列的容错判断，直接释放tcb占用的内存
- 
-   return result;
- }


**最后，我们看一下pthread_join(nptl/pthread_join.c)做了什么**




点击(此处)折叠或打开


- int
- pthread_join (threadid, thread_return)
-      pthread_t threadid;
-      void **thread_return;
- {
-   struct pthread *pd = (struct pthread *) threadid;
- 
-   /* Make sure the descriptor is valid. */
-   if (INVALID_NOT_TERMINATED_TD_P (pd))
-     /* Not a valid thread handle. */
-     return ESRCH;
- 
-   /* Is the thread joinable?. */
-   if (IS_DETACHED (pd))
-     /* We cannot wait for the thread. */
-     return EINVAL;
- 
-   struct pthread *self = THREAD_SELF;
-   int result = 0;
- 
-   /* During the wait we change to asynchronous cancellation. If we
-      are canceled the thread we are waiting for must be marked as
-      un-wait-ed for again. */
-   pthread_cleanup_push (cleanup, &pd->joinid);
- 
-   /* Switch to asynchronous cancellation. */
-   int oldtype = CANCEL_ASYNC ();
- 
-   if ((pd == self
-        || (self->joinid == pd
-      && (pd->cancelhandling
-      & (CANCELING_BITMASK | CANCELED_BITMASK | EXITING_BITMASK
-          | TERMINATED_BITMASK)) == 0))
-       && !CANCEL_ENABLED_AND_CANCELED (self->cancelhandling))
-     /* This is a deadlock situation. The threads are waiting for each
-        other to finish. Note that this is a "may" error. To be 100%
-        sure we catch this error we would have to lock the data
-        structures but it is not necessary. In the unlikely case that
-        two threads are really caught in this situation they will
-        deadlock. It is the programmer's problem to figure this
-        out. */
-     result = EDEADLK;
-   /* Wait for the thread to finish. If it is already locked something
-      is wrong. There can only be one waiter. */
-   else if (__builtin_expect (atomic_compare_and_exchange_bool_acq (&pd->joinid,
-                                  self,
-                                  NULL), 0))
-     /* There is already somebody waiting for the thread. */
-     result = EINVAL;
-   else
-     /* Wait for the child. */
-     lll_wait_tid (pd->tid);
- 
- 
-   /* Restore cancellation mode. */
-   CANCEL_RESET (oldtype);
- 
-   /* Remove the handler. */
-   pthread_cleanup_pop (0);
- 
- 
-   if (__builtin_expect (result == 0, 1))
-     {
-       /* We mark the thread as terminated and as joined. */
-       pd->tid = -1;
- 
-       /* Store the return value if the caller is interested. */
-       if (thread_return != NULL)

-     *thread_return = pd->result;//设置返回值

- 
- 
-       /* Free the TCB. */

-       __free_tcb (pd);/释放TCB占用内存
-     }
- 
-   return result;
- }




综上，如果要保证创建线程之后，确保无内存泄漏，必须采用如下方法来规范pthread_create的使用：

**方法一、创建detached的线程**




点击(此处)折叠或打开


- void run() { 
-     return;
- } 
- 
- int main(){ 
-     pthread_t thread; 
-     pthread_attr_t attr; 
-     pthread_attr_init( &attr ); 
-     pthread_attr_setdetachstate(&attr,1); 
-     pthread_create(&thread, &attr, run, 0); 
- 
-     //...... 
-     return 0; 
- }




**方法二、要么线程线程的start_routine结束之前detached**




点击(此处)折叠或打开


- void run() { 
-     pthread_detach(pthread_self()); 
- } 
- 
- int main(){ 
-     pthread_t thread;  
-     pthread_create(&thread, NULL, run, 0); 
- 
-     //...... 
-     return 0; 
- }




**方法三、主线程使用pthread_join**




点击(此处)折叠或打开


- void run() { 
-     return;
- } 
- 
- int main(){ 
-     pthread_t thread; 
-     pthread_create(&thread, NULL, run, 0);  
- 
-     //...... 
-     pthread_join(thread,NULL);
-     return 0; 
- }












