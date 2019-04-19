# Linux信号系统详解 - fanyun的博客 - CSDN博客
2016年10月30日 20:48:33[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：645
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
代码对应linux kernel v0.11
[Linux信号系统简介]
     在Linux系统中，信号机制是在软件层次上对中断机制的一种模拟。一个进程接收到信号之后，有相应的信号的处理程序，而一个进程也可以给另外一个（或一组）进程发送信号。在内核版本的0.11版本中，每个进程都有一个结构体sigaction的数组，类似于系统的中断描述表（也被称为中断向量表），这个数组可以被称为进程的信号描述表（信号向量表）。但信号向量表和中断向量表之间是有区别的，中断向量表所对应的中断处理例程一般是内核中的相关函数，但信号向量表中的处理程序一般来自用户进程。
     对信号的检测与响应总是发生在系统空间，通常发生在两种情况下：1、当进程由于系统调用、中断或异常而进入核心态，从核心态返回用户态之前；2、当前进程在核心态中由睡眠刚被唤醒的时候，由于信号的存在而提前返回到用户空间中。示意图如下
![](https://img-my.csdn.net/uploads/201211/18/1353248896_8767.jpg)
[进程信号量相关的数据描述]
在进程数据结构task_struct中，与进程相关的数据有包括三个部分，如下所示
longsignal;
structsigaction
 sigaction[32];
longblocked;        /*
 bitmap of masked signals */
这里的sigaction数组就是我们前面所说的信号向量表，数组中的每个元素就相当于一个“信号向量”。它指明了当进程收到一个信号时所要做的处理。
而struct sigaction的定义如下(在signal.h中)
structsigaction
 {
void(*sa_handler)(int);
                 sigset_t sa_mask;
intsa_flags;
void(*sa_restorer)(void);
};
其中sigset_t类型是有typedef定义而来，也在signal.h中定义，为  typedefunsignedintsigset_t;
  /* 32 bits */
Linux内核预先定义了两个信号处理函数
#defineSIG_DFL                 ((void(*)(int))0)   /*
 default signal handling */
#defineSIG_IGN                  ((void(*)(int))1)   /*
 ignore signal */
SIG_DFL为默认的信号处理函数，SIG_IGN表示不对信号做任何处理。SIG_DFL的值为0，因此当信号向量表为空时，所有的信号都用SIG_DFL来处理。
在sigaction结构体中，sa_handler为对应某个信号的信号处理函数。它可以为预先定义的两个值之一，或者是由用户自己定义的信号处理函数。sa_mask是信号处理的位图，sa_mask表示的32位二进制数中某一位为1时，表示当前位数的信号暂时被屏蔽，避免出现信号的嵌套处理，并且当前信号所对应的位图中的值必定为1。比如，当前正在处理信号向量表中第15个信号时，那么sa_mask&(1<<14)的结果肯定不为0。除非，sa_flags的值为SA_NOMASK。sa_flags用于控制对信号句柄的处理方式，它有三个可选的值，定义在signal.h头文件中
#defineSA_NOCLDSTOP 1
#defineSA_NOMASK       0x40000000
#defineSA_ONESHOT      0x80000000
SA_NOCLDSTOP表示进程处于停止状态，不对信号做处理，SA_NOMASK表示不启用位图屏蔽信号，即这个时候可以运行信号嵌套。SA_ONESHOT指明信号处理函数一旦被调用过就恢复到默认的信号处理函数去，即使得sa_handler=SIG_DFL
在进程中定义的与信号相关的数据还有long signal和long blocked。从数据的名字上看也容易猜出其具体用途。signal的二进制表示中的每一位表示一种信号，当它的二进制表示的某一位上的值为1时，表示收到了对应的信号。在内核中定义了22种信号。在signal.h文件中。具体的信号值的定义在后面说明使用的时候会给出。blocked指出进程在哪一位信号量上受到阻塞，blocked的二进制表示中的每一位被看成一个标识，当它的二进制表示中的某一位上的值为1时，表示进程在这一位对应的信号量上被阻塞，也就是说对应位上的信号暂时不用处理。
[信号系统相关函数解析] 
     在这一部分中，对Linux系统的信号系统的函数进行解析。Linux内核的0.11版本中信号系统部分的函数不多,signal.c总共120行左右的代码，所以这里对所有在signal.c中的信号相关的函数进行解析
intsys_sgetmask()
  
{
returncurrent->blocked;
}
syst_sgetmark返回当前进程受阻塞的情况，current是一个的类型是struct task_struct*的全局变量，总是指向当前正在执行的进程。
intsys_ssetmask(intnewmask)
{
intold=current->blocked;
                current->blocked = newmask & ~(1<<(SIGKILL-1));
returnold;
}
sys_ssetmask设置当前进程的阻塞掩码，同时返回进程旧的掩码值。SIGKILL定义在signal.h中，定义为#define SIGKILL 9。newmask&~(1<<SKILL-1))，操作可以使得结果的第8位（从第0位开始算起）为0，这个位对应的是强迫进程终止的信号.
staticinlinevoidsave_old(char*
 from,char* to)
{
inti;
                verify_area(to,sizeof(structsigaction));
    
for(i=0
 ; i<sizeof(structsigaction)
 ; i++) {
                                put_fs_byte(*from,to);
                                from++;
                                to++;
                }
}
在save_old函数中使用到了，verify_area和put_fs_byte两个函数，首先对这两个函数的作用进行说明。verify_area函数在fork.c中定义。它的原型是voidverify_area(void*
 addr,intsize)。对verify_area的具体定义涉及到对Linux内存管理方面的知识，这里不做详细解析，只需要了解verify_area的作用是对当前进程的地址addr到addr+size这一段进程空间以页为单位执行写操作前的检测。若页面是只读的，则执行共享检验和复制页面操作。puts_fs_byte函数的定义如下
externinlinevoidput_fs_byte(charval,char*addr)
{
__asm__ ("movb
 %0,%%fs:%1"::"r"(val),"m"(*addr));
}
是一个按字节的拷贝函数，将var字符拷贝到addr指定的地址中去。那么，save_old函数的作用就是将由from指向的一个sigaction结构体拷贝到to指定的地方去，即保存一个信号的处理例程。
staticinlinevoid get_new(char*
 from,char* to)
{
inti;
for(i=0
 ; i< sizeof(struct sigaction)
 ; i++)
                                *(to++) = get_fs_byte(from++);
}
将from指向的一个sigaction结构体拷贝到to指定的地方去。获取一个信号量的处理例程。get_fs_byte的定义与put_fs_byte类似。
intsys_signal(intsignum,longhandler,longrestorer)
{
structsigaction
 tmp;
  //Linux内核0.11版本中，限制一个进程能接受的最大信号量数是32个。信号量的编号是1到32。SIGKILL表示进程强制终止的信号量，是不允许直接对
 //进程设置强制终止的信号量的。
if(signum<1
 || signum>32 || signum==SIGKILL)  
return-1;
                tmp.sa_handler = (void(*)(int))
 handler;
                tmp.sa_mask = 0;  //sa_mask为0，表示不对任何一位上的信号量进行屏蔽
                tmp.sa_flags = SA_ONESHOT | SA_NOMASK; //信号一旦被处理之后将恢复到默认的处理函数，并且不启动信号屏蔽
                tmp.sa_restorer = (void(*)(void))
 restorer;   //恢复函数的设置
                handler = (long)
 current->sigaction[signum-1].sa_handler; //设置信号的处理函数
                current->sigaction[signum-1] = tmp;  //设置当前进程的信号向量表
returnhandler;
  
}
sys_signal函数设置当前进程某一个信号量的处理例程。有signum指定要设置的信号量，handler和restore分别指定信号的处理函数，和恢复函数。
intsys_sigaction(intsignum,conststructsigaction
 * action,
structsigaction
 * oldaction)
{
structsigaction
 tmp;
if(signum<1
 || signum>32 || signum==SIGKILL) 　　//同上一个函数的解释
return-1;
                tmp = current->sigaction[signum-1];   //获取旧的信号处理例程
                get_new((char*)
 action,
                                (char*)
 (signum-1+current->sigaction));    //保存新的信号处理例程
if(oldaction)
                                                                //假如指定了旧的保存地址，就把就的信号处理例程保存起来
                                save_old((char*)
 &tmp,(char*)
 oldaction);
if(current->sigaction[signum-1].sa_flags
 & SA_NOMASK)    //如果在新的处理例程中，设置了不启用信号屏蔽，那么把sa_mask设置成0，不屏蔽
                                                                                        //任何上的一位信号。否则就设置相应的屏蔽位。由此看来，除非显示第指定了不
                                                                                        //启用信号屏蔽表示，那么默认是要进行信号屏蔽的。不允许信号的嵌套处理。
                                current->sigaction[signum-1].sa_mask = 0;             
else
                                current->sigaction[signum-1].sa_mask |= (1<<(signum-1));
return0;
}
sys_sigaction的作用也是对进程的某一信号量的处理例程进行设置。signum指定要设置的信号量，action指向新的信号处理例程，oldaction指向用于保存此信号量的旧的处理例程的地址。这个函数和syg_signal的作用其实相差不多，只不过这个是使用完全自定义的sigaction，而sys_signal只是设置了对应信号量的handler和restore，其余的使用默认设置。
voiddo_signal(longsignr,longeax,longebx,longecx,longedx,
longfs,longes,longds,
longeip,longcs,longeflags,
unsignedlong*
 esp,longss)
{
unsignedlongsa_handler;
longold_eip=eip; 
structsigaction
 * sa = current->sigaction + signr - 1; //根据信号量获取信号处理例程
intlongs;
unsignedlong*
 tmp_esp;
                sa_handler = (unsignedlong)
 sa->sa_handler;    //获取信号处理函数
if(sa_handler==1)
 //信号处理函数为1，即表示sa_handler==SIG_IGN，说明要忽略此信号，不做任何处理
return;
if(!sa_handler)
 {     //sa_handler为0，即表示sa_handler!=SIG_DFL，说明要采用默认的信号处理动作，一般是退出进程的运行
if(signr==SIGCHLD)
 //SIGCHLD定义在signal.h中，这个信号量表示子进程停止或者被终止。当子进程调用do_exit退出的时候，给其
                                                       //父进程发送一个SIGCHILD的信号，表示自己停止运行了。
return;
else 
                             //否则，调用默认的信号处理动作，退出进程
                                                do_exit(1<<(signr-1));
                }
if(sa->sa_flags
 & SA_ONESHOT)  //如果设置了SA_ONESHOT，那么信号处理函数调用之后，要把信号的处理函数设置成默认值，即设为0
                                sa->sa_handler = NULL;
//下面的这段代码用信号量句柄替换内核堆栈中原用户程序的eip，同时也将sa_resotre，signr，SA_NOMASK，eax，ecx，edx作为参数以及原调用系统
//的程序返回指针及标志寄存器值压入堆栈。因此在本次调用中断返回用户程序时会首先执行用户的信号句柄程序，然后再继续执行用户程序。这样就
//实现了信号处理函数的嵌入动作
                *(&eip) = sa_handler;   //让EIP指向信号处理函数
                longs = (sa->sa_flags & SA_NOMASK)?7:8;
                *(&esp) -= longs;
                verify_area(esp,longs*4);
                tmp_esp=esp;
                put_fs_long((long)
 sa->sa_restorer,tmp_esp++);
                put_fs_long(signr,tmp_esp++);
if(!(sa->sa_flags
 & SA_NOMASK))
                                put_fs_long(current->blocked,tmp_esp++);
                put_fs_long(eax,tmp_esp++);
                put_fs_long(ecx,tmp_esp++);
                put_fs_long(edx,tmp_esp++);
                put_fs_long(eflags,tmp_esp++);
                put_fs_long(old_eip,tmp_esp++);
                current->blocked |= sa->sa_mask;   //插入完成后，设置进程在对应上的信号阻塞情况
}
下图画出了，插入之后的堆栈的示意图,设置了SA_NOMASK的情况。put_fs_long是向指定的地址写入一个4字节大小的数据，它的函数原型是
voidput_fs_long(unsignedlongval,unsignedlong*
 addr)，把val写到由addr指定的地址中去
 do_signal是具体的信号处理函数，根据当前进程的信号状态，做出相应的操作.
![](https://img-my.csdn.net/uploads/201211/18/1353248903_5193.jpg)
[进程信号使用的具体实例] 
这一部分解析一些进程之间使用信号量的具体实例
1、首先是信号量相关的初始化
在进程调用do_execv启动一个新的程序时，会设置进程信号量的初始值，是将信号向量表的每一项设为NULL
for(i=0 ;
 i<32 ; i++)
current->sigaction[i].sa_handler = NULL;
而前面提到过，创建一个新进程的时候，会把这个进程的signal设置0.
2、进程退出时，即进程调用了exit的系统调用
do_exit函数中有如下部分语句
for(i=0 ;
 i<NR_TASKS ; i++)
if(task[i]
 && task[i]->father == current->pid) {
                                                task[i]->father = 1;
if(task[i]->state
 == TASK_ZOMBIE)
/*
 assumption task[1] is always init */
                                                                (void)
 send_sig(SIGCHLD, task[1], 1);
                                }
这段代码作用是先找到当前要退出的进程的所有子进程，将这些进程的父进程设为进程1（一个内核常驻进程）。然后检查，这些子进程中，是否有进程状态为僵死状态的，如果有，就向其父进程（也就是进程1）发送一个子进程已僵死的信号。等待父进程的处理。
     在do_exit函数体的后面一点部分，有如下语句
if(current->leader)
       kill_session();
检查当前进程是否为其所在进程组的组长进程，如果是的，就结束会话。而kill_session函数的定义如下（在exit.c文件中）
staticvoidkill_session(void)
{
structtask_struct
 **p = NR_TASKS + task;
while(--p
 > &FIRST_TASK) {
if(*p
 && (*p)->session == current->session)
                                                (*p)->signal |= 1<<(SIGHUP-1);
                }
}
kill_session所做的事情，就是检查所有与当前进程在同一个会话组里面的进程，给这些进程发送一个SIGHUP信号，即会话已经结束，进程Hang起
在进程状态设置之后，需要通知这个进程的父进程，告知其一个子进程终止。于是有如下代码
current->state = TASK_ZOMBIE;
current->exit_code = code;
tell_father(current->father);
对tell_father的定义如下
staticvoidtell_father(intpid)
{
inti;
if(pid)
for(i=0;i<NR_TASKS;i++)
 {
if(!task[i])
continue;
if(task[i]->pid
 != pid)
continue;
                                                task[i]->signal |= (1<<(SIGCHLD-1)); //找到父进程，设置父进程的信号量
return;
                                }
/* if we don't find any fathers, we just release ourselves */
/* This is not really OK. Must change it to make father 1 */
                printk("BAD
 BAD - no father found\n\r");
                release(current);
}
 tell_father的作用就是，给其父进程发信号，告诉子进程已经终止。如果没找到父进程，就将自己释放
3、进程调度时的信号处理
在进程调度函数中，会检查进程的时钟信号
for(p
 = &LAST_TASK ; p > &FIRST_TASK ; --p)
if(*p)
 {
if((*p)->alarm
 && (*p)->alarm < jiffies) {
                                                                                (*p)->signal
 |= (1<<(SIGALRM-1));
                                                                                (*p)->alarm
 = 0;
                                                                }
if(((*p)->signal
 & ~(_BLOCKABLE & (*p)->blocked)) && 
                                                (*p)->state==TASK_INTERRUPTIBLE)
                                                                (*p)->state=TASK_RUNNING;
                                }
这段代码是刚进入schedule函数时首先执行的。先检查进程是否收到了时钟信号，如果收到就做设置信号量。然后在下面部分检查进程是否获得了所需的信号，并且是否可以现在处理这个信号，如果是，就将进程状态设置为运行。
