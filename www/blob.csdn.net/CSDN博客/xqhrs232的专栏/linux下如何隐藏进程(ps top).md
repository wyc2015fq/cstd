# linux下如何隐藏进程(ps/top) - xqhrs232的专栏 - CSDN博客
2016年07月14日 09:26:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4540
原文地址::[http://blog.csdn.net/sanbailiushiliuye/article/details/7561869](http://blog.csdn.net/sanbailiushiliuye/article/details/7561869)
相关文章
1、Linux系统上对其他用户隐藏进程的简单方法----[http://www.jb51.net/LINUXjishu/347787.html](http://www.jb51.net/LINUXjishu/347787.html)
2、[linux下根据进程名称隐藏进程的PID](http://blog.chinaunix.net/uid-26585427-id-5077452.html) ----[http://blog.chinaunix.net/uid-26585427-id-5077452.html](http://blog.chinaunix.net/uid-26585427-id-5077452.html)
3、linux中实现进程隐藏的一种新方法----[http://www.doc88.com/p-6741895256548.html](http://www.doc88.com/p-6741895256548.html)
今天看了一篇外国的blog，讲的是如何在2.4内核中隐藏进程。
其核心思想就是把task->pid变成0，就成了0号进程。而在ps，top命令中，是不显示0号进程的相关信息。这么一来，在/proc/文件夹下就不会有该进程的相关信息了。
于是把代码稍微做了一点改动，在2.6.18-308.4.1.el5 内核中还是好使的。
上代码：
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JASON.LIN.YU");
static pid_t pid = 0;
static pid_t old_pid = 0;
static struct task_struct *task = NULL;
//static struct task_struct *old_task = NULL;
static char * mystring = NULL;
module_param(pid, int, 0);
module_param(mystring, charp, 0);
MODULE_PARM_DESC(pid, "The pid to hide");
MODULE_PARM_DESC(mystring, "The process's name");
int start_module(void)
{
        task = find_task_by_pid(pid);
        if(NULL == task){
                return 1;
        }
        old_pid = task->pid;
task->pid = 0;//关键在于改变pid=0
        return 0;
}
void clean_module(void)
{
        struct list_head* list;
        list_for_each(list, ¤t->tasks){
                task = list_entry(list, struct task_struct, tasks);
                if( 0 == memcmp(mystring, task->comm, strlen(mystring)+1)){
                        task->pid = old_pid;//替换pid
                        break;
                }
        }
        return;
}
rmmod后，proc文件夹下该进程有重新出现。
注意：使用这个方法不能ctrl+C，或者exit（）调用，否则系统oops。
====================================================================================================================================
另外，还有一种比较简便的方法，就是把int main(int argc, char*argv[])中的参数变成0，那么就在单纯的ps命令中就不会显示进程相关信息，但是/proc/文件夹下，还会存在该进程的相关信息。
代码如下：
/*test.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
  int i = 0;
  memset(argv[i], 0, strlen(argv[i]));
  for(;;);
  return 0;
}
gcc -o hide test.c
./hide
ps aux| grep hide
>>>root      2437  0.0  0.0   4016   700 pts/1    R+   13:26   0:00 grep hide
但是top命令会显示出hide进程
以及使用ps -u username -U username会把进程信息出卖。
