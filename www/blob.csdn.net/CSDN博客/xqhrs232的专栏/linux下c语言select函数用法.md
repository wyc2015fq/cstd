# linux下c语言select函数用法 - xqhrs232的专栏 - CSDN博客
2012年03月15日 16:29:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：862标签：[linux																[c																[语言																[keyboard																[struct																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://chen-xiao-mei2009-163-com.iteye.com/blog/464972](http://chen-xiao-mei2009-163-com.iteye.com/blog/464972)
Select在Socket编程中还是比较重要的，可是对于初学Socket的人来说都不太爱用Select写程序，他们只是习惯写诸如 
connect、accept、recv或recvfrom这样的阻塞程序（所谓阻塞方式block，顾名思义，就是进程或是线程执行到这些函数时必须等 
待某个事件的发生，如果事件没有发生，进程或线程就被阻塞，函数不能立即返回）。 
可是使用Select就可以完成非阻塞（所谓非阻塞方式non- 
block，就是进程或线程执行此函数时不必非要等待事件的发生，一旦执行肯定返回，以返回值的不同来反映函数的执行情况，如果事件发生则与阻塞方式相同，若事件没有发生则返回一个代码来告知事件未发生，而进程或线程继续执行，所以效率较高）方式工作的程序，它能够监视我们需要监视的文件描述符的变化情况——读写或是异常。
下面详细介绍一下！ 
Select的函数格式(我所说的是Unix系统下的伯克利socket编程，和windows下的有区别，一会儿说明)： 
int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
先说明两个结构体： 
第一，struct fd_set可以理解为一个集合，这个集合中存放的是文件描述符(filedescriptor)，即文件句柄，这可以是我们所说的普通意义的文件，当然Unix下任何设备、管道、FIFO等都是文件形式，全部包括在内，所以毫无疑问一个socket就是一个文件，socket句柄就是一个文件描述符。
fd_set集合可以通过一些宏由人为来操作，
比如 清空集合FD_ZERO(fd_set *)； 
将一个给定的文件描述符加入集合之中FD_SET(int ,fd_set *)； 
将一个给定的文件描述符从集合中删除FD_CLR(int ,fd_set*)； 
检查集合中指定的文件描述符是否可以读写FD_ISSET(int ,fd_set* )。一会儿举例说明。 
第二，struct timeval是一个大家常用的结构，用来代表时间值，有两个成员，一个是秒数，另一个是毫秒数。
具体解释select的参数： 
1>int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。
2>fd_set*readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读变化。
3>fd_set*writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的写变化的，即我们关心是否可以向这些文件中写入数据了，如果这个集合中有一个文件可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的写变化。
4>fd_set *errorfds同上面两个参数的意图，用来监视文件错误异常。
5>struct timeval *timeout是select的超时时间，这个参数至关重要，它可以使select处于三种状态，
第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止；
第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正值；
第三，timeout的值大于0，这就是等待的超时时间，即select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。
返回值： 
负值：select错误 正值：某些文件可读写或出错 0：等待超时，没有可读写或错误的文件 
在有了select后可以写出像样的网络程序来！举个简单的例子，就是从网络上接受数据写入一个文件中。 
例子： 
main() 
{ 
    int sock; 
    FILE *fp; 
    struct fd_set fds; 
    struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0 
    char buffer[256]={0}; //256字节的接收缓冲区 
    /* 假定已经建立UDP连接，具体过程不写，简单，当然TCP也同理，主机ip和port都已经给定，要写的文件已经打开 
    sock=socket(...); 
    bind(...); 
    fp=fopen(...); */ 
    while(1) 
   { 
        FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化 
        FD_SET(sock,&fds); //添加描述符 
        FD_SET(fp,&fds); //同上 
        maxfdp=sock>fp?sock+1:fp+1;    //描述符最大值加1 
        switch(select(maxfdp,&fds,&fds,NULL,&timeout))   //select使用 
        { 
            case -1: exit(-1);break; //select错误，退出程序 
            case 0:break; //再次轮询 
            default: 
                  if(FD_ISSET(sock,&fds)) //测试sock是否可读，即是否网络上有数据 
                  { 
                        recvfrom(sock,buffer,256,.....);//接受网络数据 
                        if(FD_ISSET(fp,&fds)) //测试文件是否可写 
                            fwrite(fp,buffer...);//写入文件 
                         buffer清空; 
                   }// end if break; 
          }// end switch 
     }//end while 
}//end main 
文章出处：DIY部落(http://www.diybl.com/course/6_system/linux/Linuxjs/20090308/159832.html)
linux c语言 select函数用法 
表头文件 
＃i nclude<sys/time.h> 
＃i nclude<sys/types.h> 
＃i nclude<unistd.h> 
定义函数 int select(int n,fd_set * readfds,fd_set * writefds,fd_set * exceptfds,struct timeval * timeout);
函数说明 select()用来等待文件描述词状态的改变。参数n代表最大的文件描述词加1，参数readfds、writefds 和exceptfds 称为描述词组，是用来回传该描述词的读，写或例外的状况。底下的宏提供了处理这三种描述词组的方式:
FD_CLR(inr fd,fd_set* set)；用来清除描述词组set中相关fd 的位 
FD_ISSET(int fd,fd_set *set)；用来测试描述词组set中相关fd 的位是否为真 
FD_SET（int fd,fd_set*set）；用来设置描述词组set中相关fd的位 
FD_ZERO（fd_set *set）； 用来清除描述词组set的全部位 
参数 timeout为结构timeval，用来设置select()的等待时间，其结构定义如下 
struct timeval 
{ 
time_t tv_sec; 
time_t tv_usec; 
}; 
返回值 如果参数timeout设为NULL则表示select（）没有timeout。 
错误代码 执行成功则返回文件描述词状态已改变的个数，如果返回0代表在描述词状态改变前已超过timeout时间，当有错误发生时则返回-1，错误原因存于errno，此时参数readfds，writefds，exceptfds和timeout的值变成不可预测。
EBADF 文件描述词为无效的或该文件已关闭 
EINTR 此调用被信号所中断 
EINVAL 参数n 为负值。 
ENOMEM 核心内存不足 
范例 常见的程序片段:
fs_set readset； 
FD_ZERO(&readset); 
FD_SET(fd,&readset); 
select(fd+1,&readset,NULL,NULL,NULL); 
if(FD_ISSET(fd,readset){……} 
下面是linux环境下select的一个简单用法 
＃i nclude <sys/time.h> 
＃i nclude <stdio.h> 
＃i nclude <sys/types.h> 
＃i nclude <sys/stat.h> 
＃i nclude <fcntl.h> 
＃i nclude <assert.h> 
int main () 
{ 
int keyboard; 
int ret,i; 
char c; 
fd_set readfd; 
struct timeval timeout; 
keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK); 
assert(keyboard>0); 
while(1) 
    { 
timeout.tv_sec=1; 
timeout.tv_usec=0; 
FD_ZERO(&readfd); 
FD_SET(keyboard,&readfd); 
ret=select(keyboard+1,&readfd,NULL,NULL,&timeout); 
if(FD_ISSET(keyboard,&readfd)) 
    { 
      i=read(keyboard,&c,1); 
          if('\n'==c) 
          continue; 
      printf("hehethe input is %c\n",c); 
       if ('q'==c) 
      break; 
      } 
} 
} 
用来循环读取键盘输入 
2007年9月17日，将例子程序作一修改，加上了time out,并且考虑了select得所有的情况： 
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <assert.h> 
int main () 
{ 
int keyboard; 
int ret,i; 
char c; 
fd_set readfd; 
struct timeval timeout; 
keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK); 
assert(keyboard>0); 
while(1) 
{ 
      timeout.tv_sec=5; 
      timeout.tv_usec=0; 
      FD_ZERO(&readfd); 
      FD_SET(keyboard,&readfd); 
      ret=select(keyboard+1,&readfd,NULL,NULL,&timeout); 
      //select error when ret = -1 
      if (ret == -1) 
          perror("select error"); 
      //data coming when ret>0 
      else if (ret) 
      { 
          if(FD_ISSET(keyboard,&readfd)) 
          { 
              i=read(keyboard,&c,1); 
              if('\n'==c) 
                  continue; 
              printf("hehethe input is %c\n",c); 
              if ('q'==c) 
              break; 
          } 
      } 
      //time out when ret = 0 
      else if (ret == 0) 
          printf("time out\n"); 
} 
} 
#include <string.h> 
#include <unistd.h> 
#include <sys/time.h> 
#include <sys/types.h> 
下面是我写的一个例程： 
在标准输入读取9个字节数据。 
用select函数实现超时判断！ 
int main(int argc, char ** argv) 
{ 
char buf[10] = ""; 
fd_set rdfds;// 
struct timeval tv; //store timeout 
int ret; // return val 
FD_ZERO(&rdfds); //clear rdfds 
FD_SET(1, &rdfds); //add stdin handle into rdfds 
tv.tv_sec = 3; 
tv.tv_usec = 500; 
ret = select(1 + 1, &rdfds, NULL, NULL, &tv); 
if(ret < 0) 
perror("\nselect"); 
else if(ret == 0) 
printf("\ntimeout"); 
else 
{ 
printf("\nret=%d", ret); 
} 
if(FD_ISSET(1, &rdfds)) 
{ 
printf("\nreading"); 
fread(buf, 9, 1, stdin); // read form stdin 
} 
// read(0, buf, 9); /* read from stdin */ 
// fprintf(stdout, "%s\n", buf); /* write to stdout */ 
write(1, buf, strlen(buf)); //write to stdout 
printf("\n%d\n", strlen(buf)); 
return 0; 
