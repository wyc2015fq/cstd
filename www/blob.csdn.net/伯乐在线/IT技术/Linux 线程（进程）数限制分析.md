# Linux 线程（进程）数限制分析 - 文章 - 伯乐在线
原文出处： [瀚海书香](http://blog.chinaunix.net/uid-20662820-id-5690021.html)
### 1.问题来源
公司线上环境出现MQ不能接受消息的异常，运维和开发人员临时切换另一台服务器的MQ后恢复。同时运维人员反馈在出现问题的服务器上很多基本的命令都不能运行，出现如下错误：
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299507bbbZ.png)
### 2.   初步原因分析和解决
让运维的兄弟在服务上查看内存、CPU、网络、IO等基本信息都正常。于是自己到运维的服务器上看了一下，下面是slabtop –s c的运行结果，问题初步原因貌似出现了：
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299522pp3Q.png)
如果看到这个截图你看不出什么异常的话，下面的内容你可能不感兴趣，哈哈。。。
task_struct是内核对进程的管理单位，通过slub(slab的升级版，如果你对slub不了解也不影响下面的内容，只要了解slab就行了)进行节点的管理，正常负载的服务不应该出现task_struct的slub结构体占用内存最大的情况，这说明这台服务器上开启了大量的进程（Linux内核态对进程和线程都是一个单位，不要纠结这个，后面可能会进程、线程混用）。
通过这个信息，兄弟们发现这台服务器上有近3万个线程，同时也定位到出问题的网元（一个新同学的代码没有Review直接上线，里面有一个BUG触发了异常创建大量线程）。
问题貌似到这里就结束了，但是作为一个有情怀的程序员，这只是一个开始（哥的情怀白天都被繁琐的工作磨没了，只能在这深夜独享了。。。）
### 3.   Linux线程数的限制
#### 3.1     应用层测试代码


```
#define MEMSIZE (1024 * 1024 * 256)
 
void thread(void)
{
        sleep(100);
        return;
}
 
int main()
{
        pthread_t id;
        int ret;
        int num = 0;
        while (1) {
                ret = pthread_create(&id, NULL, (void*)thread, NULL);
                ++num;
                if (ret != 0)
                        break;
        }
        printf("pthread_create fail with ret=%d, total num=%d\n", ret, num);
        sleep(100);
        return 0;
}
```
通过strace跟踪，发现问题出现在copy_process函数，那剩下的工作就是分析copy_process返回异常的原因了。
#### 3.2     逆向分析
这个时候逆向分析最简单直接，可以直接定位到问题原因。
首先通过strace分析，查找出问题的系统调用是clone函数。
SYS_clone—>do_fork—>copy_process。内核态函数的分析工具这次试用了systemtap，下面就是没有任何美感的stap代码了，将就着看看吧


```
probe kernel.statement("*@kernel/fork.c:1184")
{
        printf("In kernel/fork.c 1184\n");
}
probe kernel.statement("*@kernel/fork.c:1197")
{
        printf("In kernel/fork.c 1197\n");
}
 
probe kernel.statement("*@kernel/fork.c:1206")
{
        printf("In kernel/fork.c 1206\n");
}
probe kernel.statement("*@kernel/fork.c:1338")
{
        printf("In kernel/fork.c 1338\n");
}
probe kernel.statement("*@kernel/fork.c:1342")
{
        printf("In kernel/fork.c 1342\n");
}
probe kernel.statement("*@kernel/fork.c:1363")
{
        printf("In kernel/fork.c 1363\n");
}
probe kernel.statement("*@kernel/fork.c:1369")
{
        printf("In kernel/fork.c 1369\n");
}
probe kernel.statement("*@kernel/fork.c:1373")
{
        printf("In kernel/fork.c 1373\n");
}
probe kernel.function("copy_process").return
{
        printf("copy_process return %d\n", $return)
}
 
 
function check_null_pid:long(addr:long)
{
        struct pid *p;
        p = (struct pid*)THIS->l_addr;
        if (p == NULL)
                THIS->__retvalue = 0;
        else
                THIS->__retvalue = 1;
}
probe kernel.function("alloc_pid")
{
        printf("alloc_pid init\n");
}
 
probe kernel.statement("*@kernel/pid.c:301")
{
        printf("alloc_pid 301\n");
}
probe kernel.statement("*@kernel/pid.c:312")
{
        printf("alloc_pid 312\n");
}
probe kernel.function("alloc_pid").return
{
        printf("alloc_pid return %ld\n", check_null_pid($return));
}
```
发现问题出在alloc_pid失败，分析内核代码，这个受限于kernel.pid_max参数。
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299555hnJq.png)
将参数调大到100000后，再次运行。
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299571ESue.png)
继续通过strace跟踪，这次发现问题出在了mprotect函数
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299613UjrD.png)
这个问题是由于当个线程的mmap个数限制，受限于vm.max_map_count参数。
![](http://blog.chinaunix.net/attachment/201603/30/20662820_1459299600DWDV.png)
将参数调大到100000后，再次运行，线程数明显增加了。
其实这里面还有一个参数kernel.threads-max限制，由于系统默认将这个参数设置为800000，非常大，所以这个参数的影响一直没有保留出来。
后面又犯贱把相关的参数都设置成800000，结果内存耗尽，系统直接没响应了。。。。
#### 3.3     正向分析
直接分析copy_process代码
copy_process
##### 3.3.1  内存限制
dup_task_struct–>alloc_task_struct_node/alloc_thread_info_node/arch_dup_task_struct–>kmme_cache_alloc_node(slub.c)–>slab_alloc_node–>
“CONFIG_MEMCG_KMEM” //这里也是一个坑，docker这种基于cgroup的也会影响，可能会因为分配给slub的内存不够用出现线程限制
具体函数：
alloc_pages—->__memcg_kmem_newpage_charge–>memcg_charge_kmem–>__res_counter_charge–>res_counter_charge_locked
##### 3.3.2  Threads-max参数限制
if (nr_threads >= max_threads) // threads-max 参数影响
##### 3.3.3  Pid_max参数限制
alloc_pid–>alloc_pidmap //pid_max参数影响
##### 3.3.4  单进程内存限制
单个进程的线程数，受限于vm.max_map_count限制
### 4.   总结
/proc/sys/kernel/pid_max #操作系统线程数限制
/proc/sys/kernel/thread-max  #操作系统线程数
max_user_process（ulimit -u） #系统限制某用户下最多可以运行多少进程或线程
/proc/sys/vm/max_map_count #单进程mmap的限制会影响当个进程可创建的线程数
/sys/fs/cgroup/memory/${cgroup}/memory.kmem #单个docker 内核内存的限制，可以影响task_struct等slab节点的申请，间接影响可创建的线程数
