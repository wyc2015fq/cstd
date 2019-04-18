# 《Linux设备驱动程序》——并发和竞态 - 一世豁然的专栏 - CSDN博客





2014年11月07日 12:27:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：834








一、scull的缺陷

 1、竞态

   1）、竞态会导致对共享数据的非控制访问。

   2）、某些竞态经常会导致系统崩溃、数据被破坏或者产生安全问题。







二、并发及其管理

 1、Linux存在大量的并发来源，这些来源会导致可能的竞态：

   1）、正在运行的多个用户空间程序可能一种令人惊讶的组合方式访问我们的代码。

   2）、SMP系统甚至可在不同的处理器上同时执行我们的代码。

   3）、内核代码是可抢占的；因此驱动程序代码可能会在任何时候丢失对处理器的独占，而拥有处理器的进程可能调用驱动程序代码。

   4）、设备中断时异步事件，也会导致代码的并发运行。

   5）、内核还提供了许多可延迟代码执行的机制，例如：，工作队列，小任务以及定时器等。




 2、大部分竞态可通过使用内核的并发控制原语并应用几个基本的原理来避免。




 3、竞态通常作为对资源的共享访问结果而产生。

   1）、在设计自己的驱动程序时，第一个要记住的规则就是，只要有可能，就应该避免资源共享。这种思想的最明显应用就是避免使用全局变量。

   2）、共享是通常是必须的。




 4、资源共享的硬规则

   1）、在单个执行线程之外共享硬件或软件资源的任何时候，因为另一个线程可能产生对该资源的不一致观察，因此必须显式地管理对该资源的访问。

       I、访问管理的常见技术称为“锁定”或者“互斥”——确保一次只有一个执行线程可操作共享资源。

   2）、当内核代码创建了一个可能和其他内核部分共享的对象时，该对象必须在还有其他组件引用自己时保持存在（并正确工作）。

      I、该规则产生下面两个需求：

           在对象尚不能正确工作时，不能将其对内核可用。







三、信号量和互斥体

 1、相关术语介绍

   1）、临界区

      I、在任意给定的时刻，代码只能被一个线程执行。

      II、并不是所有的临界区都是一样的，因此内核为不同的需求提供了不同的原语。

   2）、休眠

      I、当一个Linux进程到达某个时间点，从事它不能进行任何处理，它将进入休眠（或堵塞）状态，这将把处理器让其他执行线程知道将来它能够继续完成自己的处理为止。

           在等待I/O完成时，进程会进入休眠状态。

      II、可以是一种锁定机制，当进程在等待对临界区的访问时，从机制可让进程进入休眠状态。

      III、休眠可在任何时刻发生。在出现休眠的情况下，并不是所有的锁定机制都可用。

   3）、信号量

      I、一个信号量本质上是一个整数值，它和一堆函数联合使用，这一对函数通常被称为P和V。

      II、希望进入临界区的进程将在相关信号量上调用P；如果信号量大于零，则该值会减小一，二进程可以继续。相反，如果该信号量的值为零（或更小），进程必须等待直到

            去他进程释放这个信号量。

      III、对信号量的解锁通过调用V完成；该函数增加信号量的值，并在必要时唤醒等待的进程。

   4）、当信号量用于互斥体时，信号量的值应该初始化为1.这种信号量在任何时刻只能由单个进程或线程拥有。在此模式下，一个信号量有时也称为一个“互斥体”，踏实互斥的

             简称。




 2、Linux信号量的实现

   1）、要声明信号量，内核代码必须包括<asm/semaphore.h>。相关的类型是struct  semaphore；实际的信号量可通过几种途径来申明和初始化。

   2）、信号量的初始化

      I、直接创建信号量

          void  sema_init(struct  semaphore  *sem,  int  val);

          val是赋予一个信号量的初始值。

   3）、声明和初始化一个互斥体

          DECLARE_MUTEX(name);

          DECLARE_MUTEX_LOCKED(name);

          I、一个被称为name的信号量变量被初始化为1（DECLARE_MUTEX）或者0（DECLARE_MUTEX_LOCKED）。在后面一种情况下，互斥体的初始状态是锁定的，也就

               是说，在允许任何线程访问之前，必须显式的解锁该互斥体。

   4）、运行时初始化互斥体

          void  init_MUTEX(struct  semaphire  *sem);

          void  init_MUTEX_LOCKED(struct  semaphire  *sem);

   5）、在Linux世界中，P函数被称为down——或者这个名字的其他变种。down也许会将调用者置于休眠状态，然后等待信号量变得可用，之后授予调用者对被保护资源的访

             问。doen的三个版本：

          void  down(struct  semaphire  *sem);

          int  down_interruptible(struct  semaphire  *sem);

          int  down_trylock(struct  semaphire  *sem);

          I、down减少信号量的值，并在必要时一直等待。

          II、down_interruptible完成相同的工作，但操作是可断的。可断的版本几乎是我们始终要使用的版本，它允许等待在某个信号量上的用户空间进程可被用户中断。

                对down_interruptible的操作要小心，如果操作被中断，该函数就会返回非零值，而调用者不会拥有该信号量。对down_interruptible的正确使用始终要检查返回值，并

                作出相应的相应。

          III、down_trylock表示永远不会休眠；如果信号量在调用时不可获得，down_trylock会立即返回一个非零值。

          IV、当一个线程成功调用上述down的某个版本之后，就称为该进程拥有了改信号量。这样，该线程就被赋予访问由该信号量保护的临界区的权利。

   6）、当互斥操作完成后，必须返回该信号量。Linux等价于V的函数是up：

          void  up(struct  semaphore);

         I、调用up之后，调用者不再拥有信号量。

         II、任何拿到信号量的线程都必须通过一次（只有一次）对up的调用二释放该信号量。

         III、在出现错误的情况下，经常需要特别小心；如果在拥有一个信号量时发生错误，必须在讲错误状态返回给调用者之前释放该信号量。




 3、在scull使用信号量

   1）、正确使用锁定机制的关键是，明确指定需要保护的资源，并确保每一个对这些资源的访问使用正确的锁定。

   2）、struct_dev结构。

   3）、初始化scull信号量。其必须在scull设备对系统其他部分可用前辈初始化。

   4）、检查scull代码。

   5）、释放信号量。




 4、读取者/写入者信号量

   1）、内核提供了一种特殊的信号量——rwsem（读取者/写入者信号量）。

   2）、使用rwsem的代码必须包括<linux/rwsem.h>。读取者/写入者信号量相关的类型是struct  rw_semphore；一个rwsem对象必须在运行时通过下面的函数显式地初始化：

              void  init_rwsem(struct  rw_semaphore  *sem);

             新初始化的rwsem可用于其后出现的任务。

   3）、对只读访问，可用的接口如下：

             void  down_read(struct  rw_semaphore  *sem);

             int  down_read_trylock(struct  rw_semaphore  *sem);

             void  up_read(struct  rw_semaphore  *sem);

             I、对down_read的调用提供了对受保护资源的只读访问，可贺其他读取者并发地访问。注意，down_read可能会将调用进程置于不可中断的休眠。

             II、down_read_trylock不会再读取访问不可获得时等待；它在授予访问时返回非零，其它情况下返回零。注意，down_read_trylock的用法和其他大多数函数内核不同，

                   其他函数会在成功时返回零。

             III、由down_read获得的rwsem对象最终必须通过up_read被释放。

   4）、对只写访问，可用的接口如下：

             void  down_write(struct  rw_semaphore  *sem);

             int  down_write_trylock(struct  rw_semaphore  *sem);

             void  up_write(struct  rw_semaphore  *sem);

             void  downgrade_write(struct  rw_semaphore  *sem);

             I、down_write、down_write_trylock和up_write与读取者的对应函数行为相同。

             II、当某个快速改变获得了写入者锁，而其后是更长时间的只读访问的话，可以在结束修改之后调用downgrade_write，来允许其他读取者的访问。

   5）、最好在很少需要写访问且写入者只会短暂拥有信号量的时候使用rwsem。







四、completion

 1、completion是一种轻量级的机制，它允许一个线程告诉另一个线程某个工作已经完成。为了使用completion，代码必须包含<linux/completion>。




 2、初始化completio

   1）、静态初始化

      I、DECLARE_COMPLETION(my_completion);

   2）、动态初始化

      I、struct  completion  my_completion;

           init_completion(&my_completion);




 3、等待completion

     void  wait_for_completion(struct  completion  *c);

     I、该函数执行一个非中断的等待。如果代码调用了wait_for_completion且没有人会完成任务，则产生一个不可杀的进程。




 4、触发实际completion事件

     void  completion(struct  completion  *c);

     void  completion_all(struct  completion  *c);

     I、  completion只会唤醒一个进程，而completion_all会唤醒所有进程。

     II、一个completion通常是单词设备，也就说，它只会被使用一然后被丢弃。但是，如果仔细处理，completion结构也可以被重复利用。

           如果没有使用completion_all，则可以重复使用一个completion结构，只要那个将要触发的时间是明确而不含糊的，就不会带来问题。

           如果使用了completion_all，则必须在重复使用该结构后重新初始化它。

           INIT_COMPLETION(struct  completion  c);




 5、completion实例。




 6、comlpetion机制的典型使用时模块退出时的内核线程终止。在这种原型中，某些驱动程序的内部工作由一个内核线程在while(1)完成。当内核准备清除该模块时，exit函数会

       告诉该线程退出并等待completion。为饿了实现这个目标，内核包含了可用于这种线程的一个特殊函数：

       void  completion_an_exit(struct  completion  *c,  long  retval);







五、自旋锁


 1、自旋锁介绍

   1）、一个自旋锁是一个互斥设备，它只能有两个值；“锁定”和“解锁”。

   2）、它通常实现为某个整数值中的某单个位，希望获得某特定锁的代码测试相关的位。如果锁可用，则“锁定”位置被设置，而代码继续进入临界区；相反，如果所被其他人获

             得，则代码进入忙循环并重复检查这个锁，知道该锁可用为止。




 2、自旋锁API介绍

   1）、自旋锁原语包含的文件是<linux/spinlock.h>。实际的锁具有spinlock_t类型。

   2）、自旋锁初始化

       spinlock_t  my_lock = SPIN_LOCK_UNLOCKED；

       或运行下面的函数：

       void  spin_lock(spinlock_t  *lock);

   3）、进入临界区之前，必须获取锁：

       void  spin_lock(spinlock_t  *lock);

   4）、注意，所有的自旋锁等待在本质上都是不可中断的。一旦调用了spin_lock，在获得锁之前将一直处于自旋状态。

   5）、释放自旋锁

       void  spin_unlock(spinlock_t  *lock);




 3、自旋锁和原子上下文

   1）、适用于自旋锁的核心规则：任何拥有自旋锁的代码都必须是原子的。他不能休眠，事实上，它不能因为任何原因放弃处理器，除了服务中断以外（在某些情况下也不能

              放弃处理器）。

   2）、内核抢占的情况由自旋锁代码本身处理。

             在拥有锁的时候有时候很难做到；许多内核函数可以休眠。

             在拥有自旋锁时禁止中断（仅在本地CPU上），用于禁止中断的自旋锁函数有许多变种。

             自旋锁必须在可能的最短时间内拥有。




 4、自旋锁函数

   1）、锁定一个自旋锁的函数

       void  spin_lock(spinlock_t  *lock);

       void  spin_lock_irqsave(spinlock_t  *lock,  unsigne  long  flags);

       void  spin_lock_irq(spinlock_t  *lock);

       void  spin_lock_bh(spinlock_t  *lock);

       I、spin_lock_irqsave会在获得自旋锁之前禁止中断（只在本地处理器上），而先前的中断状态保存在flags中。

       II、如果我们能够确保没有任何其他代码禁止本地处理器的中断，额可以使用spin_lock_irq，而无需跟踪标志。

       III、spin_lock_bh在获得锁之前禁止软件中断，但是会让硬件中断保持打开。

       IV、如果有一个自旋锁，它可以运行在中断上下文中的代码获得，则必须使用某个中断的spin_lock形式。

       V、如果我么我们不在硬件中断中访问自旋锁，但可能在软件中断中访问，则应该使用spin_lock_bh。

   2）、释放自旋锁

        void  spin_unlock(spinlock_t  *lock);

        void  spin_unlock_irqrestore(spinlock_t  *lock,  unsigned  long  flags);

        void  spin_unlock_irq(spinlock_t  *lock);

        void  spin_unlock_bh(spinlock_t  *lock);

        I、spin_unlock的变种都会撤销对应的spin_lock函数所做的工作。

        II、传递到spin_unlock_irqrestore的flags参数必须是传递给spin_lock_irqsave的同一个变量。

        III、必须在同一函数中调用spin_lock_irqsave和spin_unlock_irqrestore，否则代码可能在某些架构上出现问题。

   3）、非堵塞的自旋锁操作

        int  spin_trylock(spinlock_t  *lock);

        int  spin_trylock_bh(spinlock_t  *lock);




 5、读取者/写入者自旋锁

   1）、读取者/写入者自旋锁允许任一数量的读取者同时进入临界区，但写入者必须互斥访问。读取者/写入者自旋锁具有rwlock_t类型，在<linux/spinlock.h>中定义。

   2）、声明和初始化读取者/写入者自旋锁

       rwlock_t  my_rwlock = RW_LOCK_UNLOCK;

       rwlock_t  my_rwlock;


       rwlock_init(&my_rwlock);

   3）、读取者函数

       void  read_lock(rwlock_t  *lock);

       void  read_lock_irqsave(spinlock_t  *lock,  unsigne  long  flags);

       void  read_lock_irq(spinlock_t  *lock);

       void  read_lock_bh(spinlock_t  *lock);

       void  read_unlock(spinlock_t  *lock);

       void  read_unlock_irqrestore(spinlock_t  *lock,  unsigned  long  flags);

       void  read_unlock_irq(spinlock_t  *lock);

       void  read_unlock_bh(spinlock_t  *lock);

   4）、写入者函数

        void  write_lock(rwlock_t  *lock);

       void  write_lock_irqsave(spinlock_t  *lock,  unsigne  long  flags);

       void  write_lock_irq(spinlock_t  *lock);

       void  write_lock_bh(spinlock_t  *lock);

       void  write_unlock(spinlock_t  *lock);

       void  write_unlock_irqrestore(spinlock_t  *lock,  unsigned  long  flags);

       void  write_unlock_irq(spinlock_t  *lock);

       void  write_unlock_bh(spinlock_t  *lock);







六、锁陷阱


1、不明确的规则

   1）、恰当的锁定模式需要清晰和明确的规则。当我们创建了一个可被执并行访问的对象时，应该同时定义用来控制访问的锁。锁定模式一定在一开始就安排好，否则后面额

             改进将会非常困难。

   2）、如果获得锁的函数要调用其他同样试图获取这个锁的函数，代码就会死锁。

   3）、为了让锁定正常工作，则不得不编写一些函数，这些函数假定调用者已经获取了相关的锁。通常，内部的静态函数可通过这种方式编写，而提供给外部调用的函数则必

             须显式地处理锁定。




 2、锁的顺序规则

   1）、拥有两个锁的解决办法：在必须获取多个锁时，应该时钟以相同的顺序获得。

   2）、拥有多个锁的规则：

      I、如果我们必须获得一个局部锁，以及一个属于内核更中心位置的锁，则应该首先获得自己的局部锁。

      II、如果我们拥有信号量和自旋锁的组合，则必须首先获得信号量；在拥有自旋锁时调用down是个严重的错误。

      III、最好的办法是避免出现需要多个锁的情况。




 3、细粒度所和粗粒度锁的对比

   1）、现代的内核可包含数千个锁，每个锁保护一个小的资源。这种类型的细粒度锁具有良好的伸缩性；它允许每个处理器在执行特定任务时无需和其他处理器正在使用的锁

             竞争。细粒度锁本身有其成本。

   2）、作为通常额规则，我们应该在最初使用粗粒度锁，除非有真正的原因相信竞争会导致问题。我们需要抑制自己过早考虑优化的欲望，因为真正的性约束通常出现在非预

              期的情况下。







七、除了锁之外的方法


 1、免锁算法

   1）、经常用于免锁的生产者/消费者任务的数据结构之一是循环缓冲区。在这个算法中，一个生产者将数据放入数组的结尾，而消费者从数组的另一端移走数据。在达到数组

             尾部的时候，生产者绕回到数组的头部。

   2）、如果仔细实现，在没有多个生产者或消费者的情况下，循环缓冲区不需要锁。




 2、原子变量

   1）、内核提供了一种原子的整数类型，称为atomic_t,定义在<asm/atomic.h>中。

   2）、原子操作函数（P127）

   3）、注意事项

       I、atomic_t数据项只能通过原子函数来访问。如果读者将原子变量传递给了需要整数类型的函数，则会遇到编译错误。

       II、只有原子变量的数目是原子的，atomic_t变量才能工作。需要多个atomic_t变量的操作，仍然需要考虑某种类型的锁。




 3、位操作

   1）、原子位操作非常快，只要底层硬件允许，这种操作就可以使用单个机器指令来执行，并且不需要禁止中断。

       I、这些函数依赖于具体的架构，因此在<asm/bitops.h>中声明。

       II、这些函数使用的数据类型也是依赖于具体架构。

   2）、可用的位操作函数（P128）

       I、当这些函数用来访问和修改一个共享标志时，除了调用它们之外，我们不需要做其他任何事情——它们会以原子方式执行操作。

       II、使用位操作来管理一个锁变量以控制对某个共享变量的访问，则相对复杂并值得讨论。




 4、seqlock

   1）、当要保护的资源很小、很简单，会很频繁被访问而且写入访问很少发生且必须快速时，就可以使用seqlock。

       I、从本质上讲，seqlock会允许读取者对资源额自由访问，但需要读取者检查是否和写入者发生冲突，当这种情况发生时，就需要重试对资源的访问。

       II、seqlock通常不能用于保护包含有指针的数据结构，因为在写入者修改该数据的同时，读取者可能会追随一个无效的指针。

   2）、seqlock在<linux/seqlock.h>中定义，通常初始化seqlock的方法有如下两种：

       seqelock_t  lock1 = SEQLOCK_UNLOCKED;

        seqlock_t  lock2;

        seqlock_init(&lock2);

   3）、读取访问通过一个整数顺序值而进入临界区。在退出时，该顺序会和当前值比较；如果不相等，则必须重试读取访问。

   4）、如果在中断处理例程中使用seqlock，则应该使用IRQ安全的版本。

      unsigned  int  read_seqbegin_irqsave(seqlock_t  *lock,  unsigned  long  flags);

      int  read_seqretry_urqrestore(seqlock_t  *lock,  unsigned  int  seq,  unsigned  long  flags);

   5）、写入者必须在进入seqlock保护的临界区时获得一个互斥体。为此，需调用下面的函数：

      void  write_seqlock(seqlock_t  *lock);

   6）、写入者使用自旋锁实现，因此自旋锁的常见限制也适用于写入锁。做如下调用可释放该锁：

       void  write_sequnlock(seqlock_t  *lock);

   7）、因为自旋锁用来控制写入的访问，因此自旋锁的常见变种都可以使用：

       void  write_seqlock_irqsave(seqlock_t  *lock,  unsigned  long  flags);

       void  write_seqlock_irq(seqlock_t  *lock);

       void  write_seqlock_bh(seqlock_t  *lock);

       void  write_sequnlock_irqsave(seqlock_t  *lock,  unsigned  long  flags);

       void  write_sequnlock_irq(seqlock_t  *lock);

       void  write_sequnlock_bh(seqlock_t  *lock);




 5、读取-复制-更新


   1）、读取-复制-更新（RCU）也是一种高级的互斥机制。在正确使用的情况下，可以获得搞得性能。

       I、RCU对它可以保护的数据结构做了一些限定。它针对经常发生读取而很少写入的情形做了优化。

       II、被保护的资源应该通过指针访问，而对这些资源的引用必须仅有原子带阿门拥有。

       III、在需要修改该数据结构时，写入线程首先复制，然后修改副本，之后用新的版本替代相关的指针。

   2）、使用RCU的代码应包含<linux/rcupdate.h>。

       I、在读取端，阿迪吗使用受RCU保护的数据结构时，必须引用数据结构的代码包括在rcu_read_lock和rcu_read_unlock调用之间。

       II、用来修改受保护结构的带阿门必须在一个步骤中完成。第一步很简单，制醋分配一个新的结构，如果必要则从老的结构中复制数据，然后将读去代码能看到的指针替换

             掉。此时，读取端回假定修改已经完成，任何进入临界区的代码将看待数据的新版本。

       III、余下的工作就是释放老的结构。

   3）、修改受RCU保护的数据结构的代码必须分哦欸一个struct  rcu_head数据结构来获得清除yonder回调函数，但并不需要用什么方式来初始化这个结构。通常，此结构内嵌

              在由RCU保护的打资源中。再修改完资源后，应该做如下调用：

              void  call_rcu(struct  rcu_head  *head,  void  ( *func) (void  *arg),  void  *arg);

              I、可安全释放该资源时，给定的func会被调用，传递到call_rcu的相同参数也会传递给这个函数。通常func要做的唯一工作就是调用kfree。




