# 用户空间与内核空间数据交换的方式(3)------seq_file - h13 - 博客园
一般地，内核通过在procfs文件系统下建立文件来向用户空间提供输出信息，用户空间可以通过任何文本阅读应用查看该文件信息，但是procfs 有一个缺陷，如果输出内容大于1个内存页，需要多次读，因此处理起来很难，另外，如果输出太大，速度比较慢，有时会出现一些意想不到的情况， Alexander Viro实现了一套新的功能，使得内核输出大文件信息更容易，该功能出现在2.4.15（包括2.4.15）以后的所有2.4内核以及2.6内核中，尤其 是在2.6内核中，已经大量地使用了该功能。
要想使用seq_file功能，开发者需要包含头文件linux/seq_file.h，并定义与设置一个seq_operations结构（类似于file_operations结构）:  
```
struct seq_operations {
          void* (*start) (struct seq_file *m, loff_t *pos);
          void  (*stop) (struct seq_file *m, void *v);
          void* (*next) (struct seq_file *m, void *v, loff_t *pos);
          int   (*show) (struct seq_file *m, void *v);
  };
```
start函数用于指定seq_file文件的读开始位置，返回实际读开始位置，如果指定的位置超过文件末尾，应当返回NULL，start函数可以有一个特殊的返回SEQ_START_TOKEN，它用于让show函数输出文件头，但这只能在pos为0时使用，next函数用于把seq_file文件的当前读位置移动到下一个读位置，返回实际的下一个读位置，如果已经到达文件末尾，返回NULL，stop函数用于在读完seq_file文件后调用，它类似于文件操作close，用于做一些必要的清理，如释放内存等，show函数用于格式化输出，如果成功返回0，否则返回出错码。
Seq_file也定义了一些辅助函数用于格式化输出：
```
/*函数seq_putc用于把一个字符输出到seq_file文件*/
  int seq_putc(struct seq_file *m, char c);
  
 /*函数seq_puts则用于把一个字符串输出到seq_file文件*/
  int seq_puts(struct seq_file *m, const char *s);
  /*函数seq_escape类似于seq_puts，只是，它将把第一个字符串参数中出现的包含在第二个字符串参数
中的字符按照八进制形式输出，也即对这些字符进行转义处理*/
  int seq_escape(struct seq_file *, const char *, const char *);
  
 /*函数seq_printf是最常用的输出函数，它用于把给定参数按照给定的格式输出到seq_file文件*/
  int seq_printf(struct seq_file *, const char *, ...)__attribute__ ((format(printf,2,3)));
 /*函数seq_path则用于输出文件名，字符串参数提供需要转义的文件名字符，它主要供文件系统使用*/
  int seq_path(struct seq_file *, struct vfsmount *, struct dentry *, char *);
```
在定义了结构struct seq_operations之后，用户还需要把打开seq_file文件的open函数，以便该结构与对应于seq_file文件的struct file结构关联起来，例如，struct seq_operations定义为：
```
struct seq_operations exam_seq_ops = {
     .start = exam_seq_start,
     .stop = exam_seq_stop,
     .next = exam_seq_next,
     .show = exam_seq_show
  };
```
那么，open函数应该如下定义：
```
static int exam_seq_open(struct inode *inode, struct file *file)
  {
          return seq_open(file, &exam_seq_ops);
  };
```
注意，函数seq_open是seq_file提供的函数，它用于把struct seq_operations结构与seq_file文件关联起来。
最后，用户需要如下设置struct file_operations结构： 
```
struct  file_operations exam_seq_file_ops = {
          .owner   = THIS_MODULE,
          .open    = exm_seq_open,
          .read    = seq_read,
          .llseek  = seq_lseek,
          .release = seq_release
  };
```
注意，用户仅需要设置open函数，其它的都是seq_file提供的函数。
然后，用户创建一个/proc文件并把它的文件操作设置为exam_seq_file_ops即可：
```
struct proc_dir_entry *entry;
  entry =  create_proc_entry("exam_seq_file", 0, NULL);
  if (entry)
 entry->proc_fops = &exam_seq_file_ops;
```
对于简单的输出，seq_file用户并不需要定义和设置这么多函数与结构，它仅需定义一个show函数，然后使用single_open来定义open函数就可以，以下是使用这种简单形式的一般步骤：
1．定义一个show函数
```
int exam_show(struct seq_file *p, void *v)
  {
  …
  }
```
2. 定义open函数
```
int  exam_single_open(struct inode *inode, struct file *file)
  {
          return(single_open(file, exam_show,  NULL));
  }
```
注意要使用single_open而不是seq_open。
3. 定义struct file_operations结构  
```
struct file_operations exam_single_seq_file_operations = {
          .open           = exam_single_open,
          .read           = seq_read,
          .llseek         = seq_lseek,
          .release        = single_release,
  };
```
注意，如果open函数使用了single_open，release函数必须为single_release，而不是seq_release。 下面给出了一个使用seq_file的具体例子seqfile_exam.c，它使用seq_file提供了一个查看当前系统运行的所有进程的/proc接口，在编译并插入该模块后，用户通过命令"cat /proc/exam_esq_file"可以查看系统的所有进程。
```
//kernel module: seqfile_exam.c
#include <linux/config.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/percpu.h>
#include <linux/sched.h>
static struct proc_dir_entry *entry;
static void *l_start(struct seq_file *m, loff_t * pos)
{
        loff_t index = *pos;
        if (index == 0) {
                seq_printf(m, "Current all the processes in system:\n"
                           "%-24s%-5s\n", "name", "pid");
                return &init_task;
        }
        else {
                return NULL;
        }
}
static void *l_next(struct seq_file *m, void *p, loff_t * pos)
{
        task_t * task = (task_t *)p;
        task = next_task(task);
        if ((*pos != 0) && (task == &init_task)) {
                return NULL;
        }
        ++*pos;
        return task;
}
static void l_stop(struct seq_file *m, void *p)
{
}
static int l_show(struct seq_file *m, void *p)
{
        task_t * task = (task_t *)p;
        seq_printf(m, "%-24s%-5d\n", task->comm, task->pid);
        return 0;
}
static struct seq_operations exam_seq_op = {
        .start = l_start,
        .next  = l_next,
        .stop  = l_stop,
        .show  = l_show
};
static int exam_seq_open(struct inode *inode, struct file *file)
{
        return seq_open(file, &exam_seq_op);
}
static struct file_operations exam_seq_fops = {
        .open = exam_seq_open,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = seq_release,
};
static int __init exam_seq_init(void)
{
        entry = create_proc_entry("exam_esq_file", 0, NULL);
        if (entry)
                entry->proc_fops = &exam_seq_fops;
        return 0;
}
static void __exit exam_seq_exit(void)
{
        remove_proc_entry("exam_esq_file", NULL);
}
module_init(exam_seq_init);
module_exit(exam_seq_exit);
MODULE_LICENSE("GPL");
```
