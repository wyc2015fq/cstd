# 《Linux程序设计》——进程间通信：管道 - 一世豁然的专栏 - CSDN博客





2014年10月16日 17:07:15[一世豁然](https://me.csdn.net/Explorer_day)阅读数：953








一、管道

 1、管道：当从一个进程链接数据到另一个进程时，我们使用术语管道。通常把一个进程的输出通过管道连接到另一个进程的输入。




 2、进程管道

   1）、popen函数

**#include<stdio.h>**

**         FILE  *popen(const  char  *cpmmand,  const  char  *popen);**

       I、popen函数允许一个程序将另一个程序作为新进程来启动，并可以传递数据给它或者通过它接受数据。command字符串是要运行的程序名和相应的函数。popen_mode必

           须是“r”或者“w”。

       II、每个popen调用都必须指定“r”或“w”，在popen函数的标准实践中不支持任何其他选项。这意味着我怕们不能调用另一个程序并同时对它进行读写操作。popen函数在失败时

            返回一个空指针。


    2）、pclose函数

**   #include<stdio.h>**

**         int  pclose(FILE  *stream_to_close);**

       I、用popen启动的进程结束时，我们可以用pclose函数关闭与之关联的文件流。pclose只在popen启动的进程结束后才返回。如果调用pclose时它任然运作，pclose调用

           将等待该进程的结束。

      II、pclose调用的返回值通常是它所关闭的文件流所在进程的退出码。如果调用进程在调用pclose之间执行了一个wait语句，被调用进程的退出状态就会丢失，因为被调用进

            程已经结束。此时pclose将返回-1并设置errno为ECHILD。




 3、将输出送往popen（代码P445-446）




 4、传递更多数据（代码P446-447）




 5、popen函数的实现（代码P448）




 6、pipe调用

** #include<unistd.h>**

**     int  pipe(int  file_descriptor[2]);**

    I、此函数在两个程序之间传递数据不需要启动一个shell来解释请求命令。它还提供了对读写数据的更多控制。

    II、此函数的参数是一个由两个参数类型的文件描述符组成的数组的指针。该函数在数组中填上两个新的文件描述符后返回0，如果失败则返回-1并设置errno来表明错误的原

          因。两个返回的文件描述符以一种特殊的方式连接起来。写到file_descriptor[1]的所有数据都可以从file_descriptor[0]都出来。数据基于先进先出的原则。







二、父进程与子进程

 1、代码——数据消费者和数据生产者（P452-453）




 2、管道关闭后读操作




 3、把管道用作标准输入和标准输出

   1）、用管道连接两个进程更简洁的方法：把其中一个管道文件描述符设置为一个已知值，一般是标准输入0或标准输出1。这样做我们可以调用程序，即那些不需要以文件描


             述 符为参数的程序。

   2）、**#incldue<unistd.h>**

**             int  dup(int  file_descriptor);**

**             int  dup2(int  file_descriptor_one,  int  file_descriptor_two);**

           I、dup调用的目的是打开一个新的文件描述符。它与open调用的区别是，dup调用创建的新文件描述符与作为它的参数的那个已有文件描述符指向同一个文件（或管

               道）。

         II、对于dup2函数来说，新的文件描述符总是取最小的可用值。而对于dup2来说，它所创建的新文件描述符或者与参数file_descriptor_two相同。

         III、dup实现在两个进程传递数据的方式：标准输入的文件描述符总是0，而dup返回的新的文件描述符又总是使用最小可用的数字。







三、命名管道：FIFO

**    #include<sys/types.h>**

**     #include<sys/stat.h>**

**     int  mkfifo(const  char  *filename,  mode_t  mode);**

**     int  mknod(const  char  *filename,  mode_t  mode  |  S_FIFO,  (dev_t)  0);**

     I、与mknod命令一样，我们可以是mknod函数简历许多特殊类型的文件。要用mknod函数创建一个可移植的命名管道惟一方法是使用一个dev_t类型的值0，并将文件访问模

         式与S_FIFO按位与或

 1、访问FIFO文件（P458实验）




 2、使用open打开FIFO文件

   1）、打开FIFO的一个主要限制是：程序不能以O_RDWR模式打开FIFO文件进行读写操作。通常使用FIFO只是为了单向传递数据，所以没有必要使用O_RDWR模式。

   2）、打开FIFO文件和打开普通文件的另一个区别是，对open_flag（open函数的第二个参数）的O_NONBLOCK选项的用法。使用这个选项不仅改变open调用处理方式，还

            会改变对这次open调用返回的文件描述符进行的读写请求的处理方式。

   3）、O_RDONLY、O_WRONLY和O_NONBLOCK标志四种合法的组合方式：

          I、**open（const  char  *path，  O_RDONLY）；**

               在这种情况下，open调用剑阻塞，除非有一个进程以写方式打开同一个FIFO，否则它不会返回。

          II、**open(const  char  *path,  O_RDONLY | O_NONBLOCK);**

                即使没有其它进程以写方式打开FIFO，这个open调用也将成功并立刻返回。

          III、**open(const  char  *path,  O_WRONLY);**

                 在这种情况下，open调用剑阻塞，除非有一个进程以读方式打开同一个FIFO，否则它不会返回。

          IV、**open(const  char  *path,  O_WRONLY | O_NONMLOCK);**

                 此函数调用总是立刻返回，但如果没有进程以读方式打开FIFO文件，open调用将返回一个错误-1并且FIFO也不会被打开。如果有一个进程以读方式打开FIFO文件，我

                 们可以通过它返回的文件描述符对这个FIFO文件进行写操作。

   4）、不带O_NONBLOCK标志的O_RDONLY和O_WRONLY。

   5）、带O_NONBLOCK标志的O_RDONLY和不带该标识的O_WRONLY。

   6）、对FIFO进行读写操作

         使用O_NONBLOCK模式会影响到对FIFO的read和write调用：

         I、对一个空的、阻塞的FIFO（既没有用O_NONBLOCK标志打开）的read调用将等待，直到数据可以读时才继续执行。与此相反，对一个空的非阻塞的FIFO的read调用将

              立刻返回0字节。

         II、对一个满的、阻塞的FIFO的write调用将等待，直到数据可以被写入时才继续执行。如果FIFO不能接收所有写入的数据，它会按下面的规则执行：

               *如果请求写入的数据的长度小于等于PIPE_BUF字节，调用失败，数据不能写入。

              *如果请求写入的数据的长度小于PIPE_BUF字节，将写入部分数据，返回实际写入的字节数，返回值也可能是0。

   7）、系统对任一时刻在一个FIFO中可以存在到的数据长度是有限制的。它由#define PIPE_BUF语句定义，通常可以在头文件limit.h中找到。




 3、高级主题：使用FIFO的客户/服务器应用程序（代码465-467）










