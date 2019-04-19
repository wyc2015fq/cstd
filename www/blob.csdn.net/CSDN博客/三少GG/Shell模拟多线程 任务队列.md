# Shell模拟多线程/任务队列 - 三少GG - CSDN博客
2013年08月07日 15:54:16[三少GG](https://me.csdn.net/scut1135)阅读数：2633
[版权声明](http://creativecommons.org/licenses/by/3.0/deed.zh)：转载时请以超链接形式标明文章原始出处和作者信息及[本声明](http://bangzhuzhongxin.blogbus.com/logs/11205960.html)
[http://luobeng.blogbus.com/logs/123290553.html](http://luobeng.blogbus.com/logs/123290553.html)
#!/bin/bash 
SEND_THREAD_NUM=13 
tmp_fifofile="/tmp/$$.fifo" 
mkfifo "$tmp_fifofile" 
exec 6<>"$tmp_fifofile" 
for((i=0; i < $SEND_THREAD_NUM; i++)); do 
    echo 
done >&6 
for i in `seq 1 100`; do 
    read -u6 
    { 
        echo $i 
        sleep 3 
        echo >&6 
    } & 
    pid=$! 
    echo $pid 
done 
wait 
exec 6>&-
[http://findingcc.blog.51cto.com/1045158/287417](http://findingcc.blog.51cto.com/1045158/287417)
上面是一篇详细的注解。
上面这段脚本相当于模拟了一个线程数为13的线程池，实在是绝妙。
首先，建立一个fifo文件，然后写入13个空行，代表了13个线程。然后，把fifo文件的输入输出重定向到文件描述符6，
read -u6开始从6号文件描述符读取空行，每读取一个空行返回（相当于得到了创建一个线程的令牌)，执行一段后台脚本
（相当于开始一个线程的任务），脚本执行完后，重新把空行写入到fifo文件（相当于把令牌归还）。这样就保证了空行的总数不变
（即线程总数不变），如果read没有独到空行，read会阻塞 等待其他模拟线程结束，并把空行写入到fifo文件。
另外一份代码，类似
#!/bin/bash
function pinghost { 
ping $1 -c 1 -w 10 |grep rtt|cut -d “/” -f6 
}
tmp_fifofile=”/tmp/$.fifo”   # 脚本运行的当前进程ID号作为文件名 
mkfifo $tmp_fifofile         # 新建一个随机fifo管道文件 
exec 6<>$tmp_fifofile         # 定义文件描述符6指向这个fifo管道文件 
rm $tmp_fifofile
thread=10 
for ((i=0;i<$thread;i++));do   # for循环 往 fifo管道文件中写入10个空行 
echo 
done >&6 
while read domain 
do 
read -u6                  # 从文件描述符6中读取行（实际指向fifo管道) 
{ 
pinghost ${domain};      # 执行pinghost函数 
echo >&6                      # 再次往fifo管道文件中写入一个空行。 
}&                                  # 放到后台执行
done
wait                          #因为之前的进程都是后台执行，因此要有wait来等待所有的进程都执行完毕后才算整个脚本跑完。 
exec 6>&-                #删除文件描述符6 
exit 0 
说明:{} 这部分语句被放入后台作为一个子进程执行，这部分几乎是同时完成的，当fifo中10个空行读完后 while循环 
继续等待 read 中读取fifo数据，当后台的10个子进程后，按次序排队往fifo输入空行，这样fifo中又有了数据，for语句继续执行
-------------------------------------------------------------------------------------------------------------------------------------
## linux shell 实现多线程
在bash中，使用后台任务来实现任务的“多进程化”。在不加控制的模式下，不管有多少任务，全部都后台执行。也就是说，在这种情况下，有多少任务就有多少“进程”在同时执行。我们就先实现第一种情况：
实例一：正常情况脚本
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)#!/bin/bash
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)for ((i=0;i<5;i++));do
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)        {
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)sleep3;echo 1>>aa && echo "done!"
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)        } 
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)wait
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)cat aa|wc -l
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)rm aa
这种情况下，程序顺序执行，每个循环3s，共需15s左右。
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)$ time bash test.sh 
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)5
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)real    0m15.030s
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)user    0m0.002s
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)sys     0m0.003s
实例二：“多进程”实现
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)#!/bin/bash
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)for ((i=0;i<5;i++));do
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)        {
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)sleep3;echo 1>>aa && echo "done!"
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)        } &
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)wait
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)cat aa|wc -l
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)rm aa
这个实例实际上就在上面基础上多加了一个后台执行&符号，此时应该是5个循环任务并发执行，最后需要3s左右时间。
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)$ time bash test.sh 
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done!
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)5
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)real    0m3.011s
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)user    0m0.002s
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)sys     0m0.004s
效果非常明显。
这里需要说明一下wait的左右。wait是等待前面的后台任务全部完成才往下执行，否则程序本身是不会等待的，这样对后面依赖前面任务结果的命令来说就可能出错。例如上面wc -l的命令就报错：不存在aa这个文件。
wait命令的官方解释如下：
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)       wait [n]
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)              Wait for the specified process and return its termination status.  n may be a process ID or  a  job  specification; if a job spec is given, all processes in that job's pipeline are waited for.  If n is not given, all currently active child processes are waited for, and the return status is zero.  If  n  specifies  a  non-existent
 process  or job, the return status is 127.  Otherwise, the return status is the exit status of the last processor job waited for.
以上所讲的实例都是进程数目不可控制的情况，下面描述如何准确控制并发的进程数目。
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)#!/bin/bash
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# 2006-7-12, by wwy
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)#———————————————————————————–
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# 此例子说明了一种用wait、read命令模拟多线程的一种技巧
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# 此技巧往往用于多主机检查，比如ssh登录、ping等等这种单进程比较慢而不耗费cpu的情况
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# 还说明了多线程的控制
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)#———————————————————————————–
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)function a_sub { # 此处定义一个函数，作为一个线程(子进程)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)sleep3# 线程的作用是sleep 3s
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)}
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)tmp_fifofile="/tmp/$$.fifo"
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)mkfifo $tmp_fifofile# 新建一个fifo类型的文件
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)exec6<>$tmp_fifofile# 将fd6指向fifo类型
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)rm $tmp_fifofile
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)thread=15# 此处定义线程数
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)for ((i=0;i<$thread;i++));do
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)echo
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done >&6# 事实上就是在fd6中放置了$thread个回车符
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)for ((i=0;i<50;i++));do# 50次循环，可以理解为50个主机，或其他
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)read -u6 
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# 一个read -u6命令执行一次，就从fd6中减去一个回车符，然后向下执行，
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)# fd6中没有回车符的时候，就停在这了，从而实现了线程数量控制
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif){ # 此处子进程开始执行，被放到后台
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)      a_sub && { # 此处可以用来判断子进程的逻辑
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)       echo "a_sub is finished"
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)      } || {
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)       echo "sub error"
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)      }
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)      echo >&6# 当进程结束以后，再向fd6中加上一个回车符，即补上了read -u6减去的那个
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)} &
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)done
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)wait# 等待所有的后台子进程结束
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)exec6>&- # 关闭df6
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)exit0
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)
sleep 3s，线程数为15，一共循环50次，所以，此脚本一共的执行时间大约为12秒
即：
15×3=45, 所以 3 x 3s = 9s
(50-45=5)<15, 所以 1 x 3s = 3s
所以 9s + 3s = 12s
$ time ./multithread.sh >/dev/null
real        0m12.025s
user        0m0.020s
sys         0m0.064s
而当不使用多线程技巧的时候，执行时间为：50 x 3s = 150s。
此程序中的命令
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)mkfifo tmpfile
和linux中的命令
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)mknod tmpfile p
效果相同。区别是mkfifo为POSIX标准，因此推荐使用它。该命令创建了一个先入先出的管道文件，并为其分配文件标志符6。管道文件是进程之间通信的一种方式，注意这一句很重要
![linux <wbr>shell <wbr>实现多线程](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)exec6<>$tmp_fifofile# 将fd6指向fifo类型
如果没有这句，在向文件$tmp_fifofile或者&6写入数据时，程序会被阻塞，直到有read读出了管道文件中的数据为止。而执行了上面这一句后就可以在**程序运行期间**不断向fifo类型的文件写入数据而不会阻塞，并且数据会被保存下来以供read程序读出。
**除非注明，本站文章均为原创，转载请以链接形式标明本文地址**
------------------------------------------------------------------------------------------------------------------------------------
**多任务队列：**
1. 
## [shell 任务队列](http://blog.greashes.com/?p=21001)五月 22nd, 2013
实现了一个简单的shell的任务队列。
调用create_task.sh（生产者）创建一个任务并放入任务队列，同时唤醒任务处理进程（消费者）处理任务队列。
任务处理进程（消费者）会一直处理完所有任务后退出。有任务时任务处理进程将再次被启动。
logs目录下的 task.log存储任务执行过程中的所有输出，包括错误信息。
[下载](http://blog.greashes.com/media/agpzfnh1bHNibG9ncg0LEgVNZWRpYRihnAEM/shell_task_queue.zip?a=download)
稍微改改代码应该就能支持多个任务队列实例。
2.
[https://github.com/huangyun/multirun](https://github.com/huangyun/multirun)
**生产者-消费者 模型：**
[http://blog.csdn.net/program_think/article/details/4022087](http://blog.csdn.net/program_think/article/details/4022087)
