# I/O多路复用之select - 建建的博客 - CSDN博客
2017年02月20日 16:33:36[纪建](https://me.csdn.net/u013898698)阅读数：171
# [I/O多路复用之select](http://www.cnblogs.com/zengzy/p/5113910.html)
1、什么是I/O多路复用
       关于什么是I/O多路复用，在知乎上有个很好的回答，可以参考[罗志宇前辈的回答](https://www.zhihu.com/question/32163005)。
　　这里记录一下自己的理解。我认为要理解这个术语得从两方面去出发，一是：多路是个什么概念？二是：复用的什么东西？先说第一个问题。多路指的是多条独立的i/o流，i/o流可以这么理解：读是一条流（称之为读流，比如输入流），写是一条流（称之为写流，比如输出流），异常也是一条流（称之为异常流），每条流用一个文件描述符来表示，同一个文件描述符可以同时表示读流和写流。再来看第二个方面，复用的是什么东西？复用的是线程，复用线程来跟踪每路io的状态，然后用一个线程就可以处理所有的io。
       当然，不提什么I/O多路复用也能在一个线程就处理完所有的io流，用个while循环挨个处理一次不就解决了嘛？那为什么还要提出这个技术呢？原因就是刚才我们想的方法（轮询）效率太低了，资源利用率也不高。试想一下，如果某个io被设置成了阻塞io，那么其他的io将被卡死，也就浪费掉了其他的io资源。另一方面，假设所有io被设置成非阻塞，那cpu一天到晚也不用干别的事了，就在这不停的问，现在可以进行io操作了吗，直到有一个设备准备好环境才能进行io，也就是在设备准备io环境的这一段时间，cpu是没必要瞎问的，问了也没结果。
       随后硬件发展起来了，有了多核的概念，也就有了多线程。这个时候可以这样做，来一条io我开一个线程，这样的话再也不用轮询了。然而，管理线程是要耗费系统资源的，程序员也开始头疼了，线程之间的交互是十分麻烦的。这样一来程序的复杂性蹭蹭蹭地往上涨，io效率是可能提高了，但是软件的开发效率却可能减低了。
       所以也就有了I/O多路复用这一技术。简单来说，就是一个线程追踪多条io流（读，写，异常），但不使用轮询，而是由设备本身告知程序哪条流可用了，这样一来就解放了cpu，也充分利用io资源，下文主要讲解如何实现这一技术，linux下这一技术有三个实现，select，poll，epoll。今天主要记录自己对select的理解，从接口到原理再到实现。
# 2、select接口
#include <sys/select.h>
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
- readfds，读流集合，也就是程序员希望从这些描述符中读内容
- writefds，写流集合，也就是程序员希望向这些描述符中写内容
- exceptfds，异常流集合，也就是中间过程发送了异常
- nfds，上面三种事件中，最大的文件描述符+1
- timeout，程序员的容忍度，可等待的时间
struct timeval{
　　long tv_sec;//second
　　long tv_usec;//minisecond
}
 timeout有三种取值：
- NULL，select一直阻塞，知道readfds、writefds、exceptfds集合中至少一个文件描述符可用才唤醒
- 0，select不阻塞
- timeout_value，select在timeout_value这个时间段内阻塞
如果非得与“多路”这个词关联起来，那就是readfds+writefds+exceptfds的数量和就是路数。
另外，还有一组与fd_set 有关的操作
- FD_SET(fd, _fdset)，把fd加入_fdset集合中
- FD_CLR(fd, _fdset)，把fd从_fdset集合中清除
- FD_ISSET(fd, _fdset)，判定fd是否在_fdset集合中
- FD_ZERO(_fdset)，清除_fdset有描述符
# 3、select实现原理
　　select的实现依赖于设备的驱动函数poll，poll的功能是检查设备的哪条条流可用（一个设备一般有三条流，读流，写流，设备发生异常的异常流），如果其中一条流可用，返回一个mask（表示可用的流），如果不可用，把当前进程加入设备的流等待队列中，例如读等待队列、写等待队列，并返回资源不可用。
　　select正是利用了poll的这个功能，首先让程序员告知自己关心哪些io流（用文件描述符表示，也就是上文的readfds、writefds和exceptfds），并让程序员告知自己这些流的范围（也就是上文的nfds参数）以及程序的容忍度（timeout参数），然后select会把她们拷贝到内核，在内核中逐个调用流所对应的设备的驱动poll函数，当范围内的所有流也就是描述符都遍历完之后，他会检查是否至少有一个流发生了，如果有，就修改那三个流集合，把她们清空，然后把发生的流加入到相应的集合中，并且select返回。如果没有，就睡眠，让出cpu，直到某个设备的某条流可用，就去唤醒阻塞在流上的进程，这个时候，调用select的进程重新开始遍历范围内的所有描述符。
　　直接看这个步骤可能会好理解些
- 1、拷贝nfds、readfds、writefds和exceptfds到内核
- 2、遍历[0,nfds)范围内的每个流，调用流所对应的设备的驱动poll函数
- 3、检查是否有流发生，如果有发生，把流设置对应的类别，并执行4，如果没有流发生，执行5。或者timeout=0，执行4
- 4、select返回
- 5、select阻塞当前进程，等待被流对应的设备唤醒，当被唤醒时，执行2。或者timeout到期，执行4
　　然后补充一副select在内核中的流程图
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160109104726278-1829044619.png)
# 4、select实现
select的核心实现是do_select，所以下面看一下do_select的源码，非完整源码，只保留了关键部分
```
int do_select(int n, fd_set_bits *fds, s64 *timeout)
{
         retval = 0;        //retval用于保存已经准备好的描述符数，初始为0
         for (;;) {
                   unsigned long *rinp, *routp, *rexp, *inp, *outp, *exp;
                   long __timeout;
                   set_current_state(TASK_INTERRUPTIBLE);    //将当前进程状态改为TASK_INTERRUPTIBLE，可中断
                   inp = fds->in; outp = fds->out; exp = fds->ex;
                   rinp = fds->res_in; routp = fds->res_out; rexp = fds->res_ex;
 
                   for (i = 0; i < n; ++rinp, ++routp, ++rexp) { //遍历每个描述符
                            unsigned long in, out, ex, all_bits, bit = 1, mask, j;
                            unsigned long res_in = 0, res_out = 0, res_ex = 0;
                            const struct file_operations *f_op = NULL;
                            struct file *file = NULL;
 
                            in = *inp++; out = *outp++; ex = *exp++;
                            all_bits = in | out | ex;
                            if (all_bits == 0) {
                                     i += __NFDBITS;       //all_bits的类型是unsigned long int ,大小为4个字节32位，all_bits=0，说明连续32个描述符（流）不在readdfs、writedfs、execptdfs集合中，所以i+=32,而__NFDBITS=32。
                                     continue;
                            }
 
                            for (j = 0; j < __NFDBITS; ++j, ++i, bit <<= 1) {     //遍历每个长字里的每个位
                                     int fput_needed;
                                     if (i >= n)
                                               break;
                                     if (!(bit & all_bits))
                                               continue;
                                     file = fget_light(i, &fput_needed);
                                     if (file) {
                                               f_op = file->f_op;
                                               MARK(fs_select, "%d %lld", i, (long long)*timeout);
                                               mask = DEFAULT_POLLMASK;
                                               if (f_op && f_op->poll)
                                                        mask = (*f_op->poll)(file, retval ? NULL : wait);//调用设备的驱动poll函数
                                               fput_light(file, fput_needed);
                                               if ((mask & POLLIN_SET) && (in & bit)) {
                                                        res_in |= bit; //如果是这个描述符可读, 将这个位置位
                                                        retval++;  //返回描述符个数加1
                                               }
                                               if ((mask & POLLOUT_SET) && (out & bit)) {
                                                        res_out |= bit;
                                                        retval++;
                                               }
                                               if ((mask & POLLEX_SET) && (ex & bit)) {
                                                        res_ex |= bit;
                                                        retval++;
                                               }
                                     }
                            }
                            if (res_in)
                                     *rinp = res_in;
                            if (res_out)
                                     *routp = res_out;
                            if (res_ex)
                                     *rexp = res_ex;
                   }
                   wait = NULL;
                   if (retval || !*timeout || signal_pending(current))//如果retval!=0，也就是有readdfs、writedfs、execptdfs至少有一个发生，跳出循环
                            break;
                   /*以下处理timeout参数*/
　　　　　　　　　　  __timeout = schedule_timeout(__timeout);
                   if (*timeout >= 0)
                            *timeout += __timeout;
         }
         __set_current_state(TASK_RUNNING);
　　　　　return retval;
}
```
