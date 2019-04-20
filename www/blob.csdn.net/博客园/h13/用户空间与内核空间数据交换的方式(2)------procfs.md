# 用户空间与内核空间数据交换的方式(2)------procfs - h13 - 博客园
procfs是比较老的一种用户态与内核态的数据交换方式，内核的很多数据都是通过这种方式出口给用户的，内核的很多参数也是通过这种方式来让用户方便设置的。除了sysctl出口到/proc下的参数，procfs提供的大部分内核参数是只读的。实际上，很多应用严重地依赖于procfs，因此它几乎是必不可少的组件。本节将讲解如何使用procfs。
Procfs提供了如下API：  
```
struct proc_dir_entry *create_proc_entry(const char *name, mode_t mode, struct proc_dir_entry *parent)
```
该函数用于创建一个正常的proc条目，参数name给出要建立的proc条目的名称，参数mode给出了建立的该proc条目的访问权限，参数 parent指定建立的proc条目所在的目录。如果要在/proc下建立proc条目，parent应当为NULL。否则它应当为proc_mkdir 返回的struct proc_dir_entry结构的指针。  
```
extern void remove_proc_entry(const char *name, struct proc_dir_entry *parent)
```
该函数用于删除上面函数创建的proc条目，参数name给出要删除的proc条目的名称，参数parent指定建立的proc条目所在的目录。 
```
struct  proc_dir_entry *proc_mkdir(const char * name, struct proc_dir_entry *parent)
```
该函数用于创建一个proc目录，参数name指定要创建的proc目录的名称，参数parent为该proc目录所在的目录。  
```
extern struct proc_dir_entry *proc_mkdir_mode(const char *name, mode_t mode, struct proc_dir_entry *parent)
 struct proc_dir_entry *proc_symlink(const char * name, struct proc_dir_entry* parent, const char *dest)
```
该函数用于建立一个proc条目的符号链接，参数name给出要建立的符号链接proc条目的名称，参数parent指定符号连接所在的目录，参数dest指定链接到的proc条目名称。
```
struct  proc_dir_entry *create_proc_read_entry(const char *name, mode_t mode, struct proc_dir_entry *base,
            read_proc_t *read_proc, void * data);
```
该函数用于建立一个规则的只读proc条目，参数name给出要建立的proc条目的名称，参数mode给出了建立的该proc条目的访问权限，参 数base指定建立的proc条目所在的目录，参数read_proc给出读去该proc条目的操作函数，参数data为该proc条目的专用数据，它将 保存在该proc条目对应的struct file结构的private_data字段中。
```
struct  proc_dir_entry *create_proc_info_entry(const char *name, mode_t mode, struct proc_dir_entry *base,
            get_info_t *get_info);
```
该函数用于创建一个info型的proc条目，参数name给出要建立的proc条目的名称，参数mode给出了建立的该proc条目的访问权限， 参数base指定建立的proc条目所在的目录，参数get_info指定该proc条目的get_info操作函数。实际上get_info等同于 read_proc，如果proc条目没有定义个read_proc，对该proc条目的read操作将使用get_info取代，因此它在功能上非常类似于函数create_proc_read_entry。 
```
struct  proc_dir_entry *proc_net_create(const char *name, mode_t mode, get_info_t *get_info)
```
该函数用于在/proc/net目录下创建一个proc条目，参数name给出要建立的proc条目的名称，参数mode给出了建立的该proc条目的访问权限，参数get_info指定该proc条目的get_info操作函数。
```
struct  proc_dir_entry *proc_net_fops_create(const char *name, mode_t mode, struct file_operations *fops)
```
该函数也用于在/proc/net下创建proc条目，但是它也同时指定了对该proc条目的文件操作函数。 
```
void proc_net_remove(const char *name)
```
该函数用于删除前面两个函数在/proc/net目录下创建的proc条目。参数name指定要删除的proc名称。
除了这些函数，值得一提的是结构struct proc_dir_entry，为了创建一了可写的proc条目并指定该proc条目的写操作函数，必须设置上面的这些创建proc条目的函数返回的指针 指向的struct proc_dir_entry结构的write_proc字段，并指定该proc条目的访问权限有写权限。
为了使用这些接口函数以及结构struct proc_dir_entry，用户必须在模块中包含头文件linux/proc_fs.h。
在源代码包中给出了procfs示例程序procfs_exam.c，它定义了三个proc文件条目和一个proc目录条目，读者在插入该模块后应当看到如下结构：
```
$ ls /proc/myproctest
  aint  astring  bigprocfile
  $
```
读者可以通过cat和echo等文件操作函数来查看和设置这些proc文件。特别需要指出，bigprocfile是一个大文件（超过一个内存
页），对于这种大文件，procfs有一些限制，因为它提供的缓存，只有一个页，因此必须特别小心，并对超过页的部分做特别的考虑，处理起来比较复杂并且
很容易出错，所有procfs并不适合于大数据量的输入输出，后面一节seq_file就是因为这一缺陷而设计的，当然seq_file依赖于 procfs的一些基础功能。
```
//kernel module: procfs_exam.c
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#define STR_MAX_SIZE 255
static int int_var;
static char string_var[256];
static char big_buffer[65536];
static int big_buffer_len = 0;
static struct proc_dir_entry * myprocroot;
static int first_write_flag = 1;
int int_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
count = sprintf(page, "%d", *(int *)data);
return count;
}
int int_write_proc(struct file *file, const char __user *buffer,unsigned long count, void *data)
{
unsigned int c = 0, len = 0, val, sum = 0;
int * temp = (int *)data;
while (count) {
if (get_user(c, buffer)) //从用户空间中得到数据
 return -EFAULT;
len++;
buffer++;
count--;
if (c == 10 || c == 0)
break;
val = c - '0';
if (val > 9)
return -EINVAL;
sum *= 10;
sum += val;
}
* temp = sum;
return len;
}
int string_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
count = sprintf(page, "%s", (char *)data);
return count;
}
int string_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
if (count > STR_MAX_SIZE) {
count = 255;
}
copy_from_user(data, buffer, count);
return count;
}
int bigfile_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
if (off > big_buffer_len) {
* eof = 1;
return 0;
}
if (count > PAGE_SIZE) {
count = PAGE_SIZE;
}
if (big_buffer_len - off < count) {
count = big_buffer_len - off;
}
memcpy(page, data, count);
*start = page;
return count;
}
int bigfile_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
char * p = (char *)data;
if (first_write_flag) {
big_buffer_len = 0;
first_write_flag = 0;
}
if (65536 - big_buffer_len < count) {
count = 65536 - big_buffer_len;
first_write_flag = 1;
}
copy_from_user(p + big_buffer_len, buffer, count);
big_buffer_len += count;
return count;
}
static int __init procfs_exam_init(void)
{
#ifdef CONFIG_PROC_FS
struct proc_dir_entry * entry;
myprocroot = proc_mkdir("myproctest", NULL);
entry = create_proc_entry("aint", 0644, myprocroot);
if (entry) {
entry->data = &int_var;
entry->read_proc = &int_read_proc;
entry->write_proc = &int_write_proc; 
}
entry = create_proc_entry("astring", 0644, myprocroot);
if (entry) {
entry->data = &string_var;
entry->read_proc = &string_read_proc;
entry->write_proc = &string_write_proc; 
}
entry = create_proc_entry("bigprocfile", 0644, myprocroot);
if (entry) {
entry->data = &big_buffer;
entry->read_proc = &bigfile_read_proc;
entry->write_proc = &bigfile_write_proc; 
}
#else
printk("This module requires the kernel to support procfs,\n");
#endif
return 0;
}
static void __exit procfs_exam_exit(void)
{
#ifdef CONFIG_PROC_FS
remove_proc_entry("aint", myprocroot);
remove_proc_entry("astring", myprocroot);
remove_proc_entry("bigprocfile", myprocroot);
remove_proc_entry("myproctest", NULL);
#endif
}
module_init(procfs_exam_init);
module_exit(procfs_exam_exit);
MODULE_LICENSE("GPL");
```
