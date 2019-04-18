# Linux内核中的进程组及会话 - ljx0305的专栏 - CSDN博客
2010年06月21日 17:11:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：589标签：[linux内核																[struct																[终端																[session																[signal																[each](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
### [Linux内核中的进程组及会话](http://writeblog.csdn.net/blog/187085)
将阐述Linux内核中的如下几个概念 
1) 进程组 
2) 会话 
3) 控制终端 
前面的概念来源于前人，我只是站在前人的肩膀上结合内核中的实现加深概念理解。 
1.概念： 
a)进程组 
Shell 上的一条命令行形成一个进程组 
每个进程属于一个进程组 
每个进程组有一个领头进程 
进程组的生命周期到组中最后一个进程终止, 或加入其他进程组为止 
getpgrp: 获得进程组 id, 即领头进程的 pid 
setpgid: 加入进程组和建立新的进程组 
前台进程组和后台进程组 
=============================================================================== 
#include <unistd.h> 
int setpgid (pid_t pid, pid_t pgid); 
pid_t getpgid (pid_t pid); 
int setpgrp (void); 
pid_t getpgrp (void); 
------------------------------------------------------------------------------- 
进程只能将自身和其子进程设置为进程组 id. 
某个子进程调用 exec 函数之后, 就不能再将该子进程的 id 作为进程组 id. 
=============================================================================== 
b)会话 
一次登录形成一个会话 
一个会话可包含多个进程组, 但只能有一个前台进程组. 
setsid 可建立一个新的会话 
=============================================================================== 
#include <unistd.h> 
pid_t setsid(void); 
------------------------------------------------------------------------------- 
如果调用进程不是进程组的领头进程, 该函数才能建立新的会话. 
调用 setsid 之后, 进程成为新会话的领头进程. 
进程成为新进程组的领头进程. 
进程失去控制终端 
=============================================================================== 
c)控制终端 
会话的领头进程打开一个终端之后, 该终端就成为该会话的控制终端 (SVR4/Linux) 
与控制终端建立连接的会话领头进程称为控制进程 (session leader) 
一个会话只能有一个控制终端 
产生在控制终端上的输入和信号将发送给会话的前台进程组中的所有进程 
终端上的连接断开时 (比如网络断开或 Modem 断开), 挂起信号将发送到控制进程(session leader) 
2. Linux中的实现举例，用以验证上述规则: 
asmlinkage long sys_getpgid(pid_t pid) 
{ 
if (!pid) { 
return current->pgrp; 
} else { 
int retval; 
struct task_struct *p; 
read_lock(&tasklist_lock); 
p = find_task_by_pid(pid); 
retval = -ESRCH; 
if (p) 
retval = p->pgrp; 
read_unlock(&tasklist_lock); 
return retval; 
} 
} 
/* 
* This needs some heavy checking ... 
* I just haven't the stomach for it. I also don't fully 
* understand sessions/pgrp etc. Let somebody who does explain it. 
* 
* OK, I think I have the protection semantics right.... this is really 
* only important on a multi-user system anyway, to make sure one user 
* can't send a signal to a process owned by another. -TYT, 12/12/91 
* 
* Auch. Had to add the 'did_exec' flag to conform completely to POSIX. 
* LBT 04.03.94 
*/ 
asmlinkage long sys_setpgid(pid_t pid, pid_t pgid) 
{ 
struct task_struct * p; 
int err = -EINVAL; 
if (!pid) 
pid = current->pid; 
if (!pgid) 
pgid = pid; 
if (pgid < 0) 
return -EINVAL; 
/* From this point forward we keep holding onto the tasklist lock 
* so that our parent does not change from under us. -DaveM 
*/ 
read_lock(&tasklist_lock); 
/*第一前提: 先要验证要设定的进程是否存在,不存在的话不能做事*/ 
err = -ESRCH; 
p = find_task_by_pid(pid); 
if (!p) 
goto out; 
/* 第二前提: 先要检查做这个操作的权限: 
当前进程只能将自身和其子进程设置为进程组id,并且 
当前进程和其子进程必须属于同一次会话 
(同组的进程一定属于同一次会话) 
*/ 
if (p->p_pptr == current || p->p_opptr == current) 
{ 
err = -EPERM; 
/*如果不属于同一次会话(同一次控制台),不可以*/ 
if (p->session != current->session) 
goto out; 
err = -EACCES; 
/*某个子进程调用 exec 函数之后, 就不能再将该子进程的 id 作为进程组 id*/ 
if (p->did_exec) 
goto out; 
} 
else if (p != current) 
goto out; 
err = -EPERM; 
/*boolean value for session group leader */ 
/*如果是一次会话的leader,也不可以 
注意进程组的首领进程也是可以改变组id的*/ 
if (p->leader) 
goto out; 
/*好!几个前提条件全满足了,要做正事了: 
但是是不是组号的合法性还没有验证?见后话!*/ 
/*要设进程号不是要设定的组号,如果是,直接设,因为这 
意味着是增加了以自己的pid作为新的组号的进程组,这个 
进程也将成为新进程组的首领进程,所以在此根本不用比较 
会话号,自己对自己肯定是同一次会话.如果条件不满足,则 
要做这些判断*/ 
if (pgid != pid) 
{ 
struct task_struct * tmp; 
for_each_task (tmp) 
{ 
/*能不能找到一个进程,组号正好是要设定的组号, 
并且和要设定的进程属于同一个控制台(同一个会话) 
找到才可以设定,其实这里就是要判定组号的合法性, 
即必须是一个已经存在的组,而且和当前同一次会话才 
可以操作,这个也不能忘记,其实就是说:同组的进程 
一定属于同一次会话*/ 
if (tmp->pgrp == pgid && 
tmp->session == current->session) 
goto ok_pgid; 
} 
goto out; 
} 
ok_pgid: 
p->pgrp = pgid; 
err = 0; 
out: 
/* All paths lead to here, thus we are safe. -DaveM */ 
read_unlock(&tasklist_lock); 
return err; 
} 
asmlinkage long sys_getsid(pid_t pid) 
{ 
if (!pid) { 
return current->session; 
} else { 
int retval; 
struct task_struct *p; 
read_lock(&tasklist_lock); 
p = find_task_by_pid(pid); 
retval = -ESRCH; 
if(p) 
retval = p->session; 
read_unlock(&tasklist_lock); 
return retval; 
} 
} 
asmlinkage long sys_setsid(void) 
{ 
struct task_struct * p; 
int err = -EPERM; 
read_lock(&tasklist_lock); 
for_each_task(p) 
{ 
/*如果当前进程是一个进程组的首领进程, 
则不能建立一个新的会话*/ 
if (p->pgrp == current->pid) 
goto out; 
} 
/*将新创建会话的leader设定为创建者就是当前进程*/ 
current->leader = 1; 
/*清楚看见一个新的进程组诞生了 
当前进程成为新进程组的首领进程 
新会话的id 是当前进程号,也是新会话的leader 
*/ 
current->session = current->pgrp = current->pid; 
/*当前进程失去控制终端*/ 
current->tty = NULL; 
current->tty_old_pgrp = 0; 
err = current->pgrp; 
out: 
read_unlock(&tasklist_lock); 
return err; 
}
引用:http://shake863.javaeye.com/blog/187085
