# Linux下的C编程实战之三进程控制 - nosmatch的专栏 - CSDN博客
2011年07月26日 23:22:39[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：449
**1.Linux进程**
　　Linux进程在内存中包含三部分数据：代码段、堆栈段和数据段。代码段存放了程序的代码。代码段可以为机器中运行同一程序的数个进程共享。堆栈段存放的是子程序（函数）的返回地址、子程序的参数及程序的局部变量。而数据段则存放程序的全局变量、常数以及动态数据分配的数据空间（比如用malloc函数申请的内存）。与代码段不同，如果系统中同时运行多个相同的程序，它们不能使用同一堆栈段和数据段。
　　Linux进程主要有如下几种状态：用户状态（进程在用户状态下运行的状态）、内核状态（进程在内核状态下运行的状态）、内存中就绪（进程没有执行，但处于就绪状态，只要内核调度它，就可以执行）、内存中睡眠（进程正在睡眠并且处于内存中，没有被交换到SWAP设备）、就绪且换出（进程处于就绪状态，但是必须把它换入内存，内核才能再次调度它进行运行）、睡眠且换出（进程正在睡眠，且被换出内存）、被抢先（进程从内核状态返回用户状态时，内核抢先于它，做了上下文切换，调度了另一个进程，原先这个进程就处于被抢先状态）、创建状态（进程刚被创建，该进程存在，但既不是就绪状态，也不是睡眠状态，这个状态是除了进程0以外的所有进程的最初状态）、僵死状态（进程调用exit结束，进程不再存在，但在进程表项中仍有记录，该记录可由父进程收集）。
　　下面我们来以一个进程从创建到消亡的过程讲解Linux进程状态转换的“生死因果”。
　　（1）进程被父进程通过系统调用fork创建而处于创建态；
　　（2）fork调用为子进程配置好内核数据结构和子进程私有数据结构后，子进程进入就绪态（或者在内存中就绪，或者因为内存不够而在SWAP设备中就绪）；
　　（3）若进程在内存中就绪，进程可以被内核调度程序调度到CPU运行；
　　（4）内核调度该进程进入内核状态，再由内核状态返回用户状态执行。该进程在用户状态运行一定时间后，又会被调度程序所调度而进入内核状态，由此转入就绪态。有时进程在用户状态运行时，也会因为需要内核服务，使用系统调用而进入内核状态，服务完毕，会由内核状态转回用户状态。要注意的是，进程在从内核状态向用户状态返回时可能被抢占，这是由于有优先级更高的进程急需使用CPU，不能等到下一次调度时机，从而造成抢占；
　　（5）进程执行exit调用，进入僵死状态，最终结束。
**　　2.进程控制**
　　进程控制中主要涉及到进程的创建、睡眠和退出等，在Linux中主要提供了fork、exec、clone的进程创建方法，sleep的进程睡眠和exit的进程退出调用，另外Linux还提供了父进程等待子进程结束的系统调用wait。
　　fork
　　对于没有接触过Unix/Linux操作系统的人来说，fork是最难理解的概念之一，它执行一次却返回两个值，完全“不可思议”。先看下面的程序：
int main()
{
　int i;
　if (fork() == 0)
　{
　　for (i = 1; i < 3; i++)
　　　printf("This is child process\n");
　}
　else
　{
　　for (i = 1; i < 3; i++)
　　　printf("This is parent process\n");
　}
}
　　执行结果为：
This [is](http://www.mydown.com/soft/network/chat/475/444475.shtml) child process
This is child process
This is parent process
This is parent process
　　fork在英文中是“分叉”的意思，这个名字取得很形象。一个进程在运行中，如果使用了fork，就产生了另一个进程，于是进程就“分叉”了。当前进程为父进程，通过fork()会产生一个子进程。对于父进程，fork函数返回子程序的进程号而对于子程序，fork函数则返回零，这就是一个函数返回两次的本质。可以说，fork函数是Unix系统最杰出的成就之一，它是七十年代Unix早期的开发者经过理论和实践上的长期艰苦探索后取得的成果。
　　如果我们把上述程序中的循环放的大一点：
int main()
{
　int i;
　if (fork() == 0)
　{
　　for (i = 1; i < 10000; i++)
　　　printf("This is child process\n");
　}
　else
　{
　　for (i = 1; i < 10000; i++)
　　　printf("This is parent process\n");
　}
}
　　则可以明显地看到父进程和子进程的并发执行，交替地输出“This is child process”和“This is parent process”。
　　此时此刻，我们还没有完全理解fork()函数，再来看下面的一段程序，看看究竟会产生多少个进程，程序的输出是什么？
int main()
{
　int i;
　for (i = 0; i < 2; i++)
　{
　　if (fork() == 0)
　　{
　　　printf("This is child process\n");
　　}
　　else
　　{
　　　printf("This is parent process\n");
　　}
　}
}
　　exec
　　在Linux中可使用exec函数族，包含多个函数（execl、execlp、execle、execv、execve和execvp），被用于启动一个指定路径和文件名的进程。
　　exec函数族的特点体现在：某进程一旦调用了exec类函数，正在执行的程序就被干掉了，系统把代码段替换成新的程序（由exec类函数执行）的代码，并且原有的数据段和堆栈段也被废弃，新的数据段与堆栈段被分配，但是进程号却被保留。也就是说，exec执行的结果为：系统认为正在执行的还是原先的进程，但是进程对应的程序被替换了。
　　fork函数可以创建一个子进程而当前进程不死，如果我们在fork的子进程中调用exec函数族就可以实现既让父进程的代码执行又启动一个新的指定进程，这实在是很妙的。fork和exec的搭配巧妙地解决了程序启动另一程序的执行但自己仍继续运行的问题，请看下面的例子：
char command[MAX_CMD_LEN];
void main()
{
　int rtn; /* 子进程的返回数值 */
　while (1)
　{
　　/* 从终端读取要执行的命令 */
　　printf(">");
　　fgets(command, MAX_CMD_LEN, stdin);
　　command[strlen(command) - 1] = 0;
　　if (fork() == 0)
　　{
　　　/* 子进程执行此命令 */
　　　execlp(command, command);
　　　/* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/
　　　perror(command);
　　　exit(errorno);
　　}
　　else
　　{
　　　/* 父进程，等待子进程结束，并打印子进程的返回值 */
　　　wait(&rtn);
　　　printf(" child process return %d\n", rtn);
　　}
　}
}
　　这个函数基本上实现了一个shell的功能，它读取用户输入的进程名和参数，并启动对应的进程。
　　clone
　　clone是Linux2.0以后才具备的新功能，它较fork更强（可认为fork是clone要实现的一部分），可以使得创建的子进程共享父进程的资源，并且要使用此函数必须在编译内核时设置clone_actually_works_ok选项。
　　clone函数的原型为：
int clone(int (*fn)(void *), void *child_stack, int flags, void *arg);
　　此函数返回创建进程的PID，函数中的flags标志用于设置创建子进程时的相关选项，具体含义如下表：
|标志|含义|
|----|----|
|CLONE_PARENT|创建的子进程的父进程是调用者的父进程，新进程与创建它的进程成了“兄弟”而不是“父子”|
|CLONE_FS|子进程与父进程共享相同的文件系统，包括root、当前目录、umask|
|CLONE_FILES|子进程与父进程共享相同的文件描述符（file descriptor）表|
|CLONE_NEWNS|在新的namespace启动子进程，namespace描述了进程的文件hierarchy|
|CLONE_SIGHAND|子进程与父进程共享相同的信号处理（signal handler）表|
|CLONE_PTRACE|若父进程被trace，子进程也被trace|
|CLONE_VFORK|父进程被挂起，直至子进程释放虚拟内存资源|
|CLONE_VM|子进程与父进程运行于相同的内存空间|
|CLONE_PID|子进程在创建时PID与父进程一致|
|CLONE_THREAD|Linux 2.4中增加以支持POSIX线程标准，子进程与父进程共享相同的线程群|
　　来看下面的例子：
int variable, fd; 
int do_something() {
　variable = 42;
　close(fd);
　_exit(0);
}
int main(int argc, char *argv[]) {
　void **child_stack;
　char tempch;
　variable = 9;
　fd = open("test.file", O_RDONLY);
　child_stack = (void **) malloc(16384);
　printf("The variable was %d\n", variable);
　clone(do_something, child_stack, CLONE_VM|CLONE_FILES, NULL);
　sleep(1); /* 延时以便子进程完成关闭文件操作、修改变量　*/
　printf("The variable is now %d\n", variable);
　if (read(fd, &tempch, 1) < 1) {
　　perror("File Read Error");
　　exit(1);
　}
　printf("We could read from the file\n");
　return 0;
}
　　运行输出：
　　The variable is now 42
　　File Read Error
　　程序的输出结果告诉我们，子进程将文件关闭并将变量修改（调用clone时用到的CLONE_VM、CLONE_FILES标志将使得变量和文件描述符表被共享），父进程随即就感觉到了，这就是clone的特点。
　　sleep
　　函数调用sleep可以用来使进程挂起指定的秒数，该函数的原型为：　　
unsigned int sleep(unsigned int seconds);
　　该函数调用使得进程挂起一个指定的时间，如果指定挂起的时间到了，该调用返回0；如果该函数调用被信号所打断，则返回剩余挂起的时间数（指定的时间减去已经挂起的时间）。
　　exit
　　系统调用exit的功能是终止本进程，其函数原型为：
void _exit(int status);
　　_exit会立即终止发出调用的进程，所有属于该进程的文件描述符都关闭。参数status作为退出的状态值返回父进程，在父进程中通过系统调用wait可获得此值。
　　wait
　　wait系统调用包括：
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
　　wait的作用为发出调用的进程只要有子进程，就睡眠到它们中的一个终止为止； waitpid等待由参数pid指定的子进程退出。
