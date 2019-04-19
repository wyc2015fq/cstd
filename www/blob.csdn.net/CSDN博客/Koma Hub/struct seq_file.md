# struct seq_file - Koma Hub - CSDN博客
2019年02月27日 21:36:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：29
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
**Table of Contents**
[struct seq_file](#struct%20seq_file)
[一个例子](#%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90)
# struct seq_file
```cpp
struct seq_file {
    char *buf;       //在seq_open中分配，大小为4KB
    size_t size;     //4096
    size_t from;     //struct file从seq_file中向用户态缓冲区拷贝时相对于buf的偏移地址
    size_t count;    //可以拷贝到用户态的字符数目
    loff_t index;    //从内核态向seq_file的内核态缓冲区buf中拷贝时start、
                       //next的处理的下标pos数值，即用户自定义遍历iter
    loff_t read_pos; //当前已拷贝到用户态的数据量大小，即struct file中拷贝到用户态的数据量
    u64 version; 
    struct mutex lock; //保护该seq_file的互斥锁结构
    const struct seq_operations *op; //seq_start,seq_next,set_show,seq_stop函数结构体
    void *private;
};
struct seq_operations {
    void * (*start) (struct seq_file *m, loff_t *pos);
    void (*stop) (struct seq_file *m, void *v);
    void * (*next) (struct seq_file *m, void *v, loff_t *pos);
    int (*show) (struct seq_file *m, void *v);
};
```
# 一个例子
```cpp
/*
 * Simple demonstration of the seq_file interface.
 *
 * $Id: seq.c,v 1.3 2004/09/26 07:02:43 gregkh Exp $
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
MODULE_AUTHOR("Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");
/*
 * The sequence iterator functions.  The position as seen by the
 * filesystem is just the count that we return.
 */
static void *ct_seq_start(struct seq_file *s, loff_t *pos)
{
	loff_t *spos = kmalloc(sizeof(loff_t), GFP_KERNEL);
	if (!spos)
		return NULL;
	*spos = *pos;
	return spos;
}
static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	loff_t *spos = (loff_t *) v;
	*pos = ++(*spos);
	return spos;
}
static void ct_seq_stop(struct seq_file *s, void *v)
{
	kfree (v);
}
/*
 * The show function.
 */
static int ct_seq_show(struct seq_file *s, void *v)
{
	loff_t *spos = (loff_t *) v;
	seq_printf(s, "%Ld\n", *spos);
	return 0;
}
/*
 * Tie them all together into a set of seq_operations.
 */
static struct seq_operations ct_seq_ops = {
	.start = ct_seq_start,
	.next  = ct_seq_next,
	.stop  = ct_seq_stop,
	.show  = ct_seq_show
};
/*
 * Time to set up the file operations for our /proc file.  In this case,
 * all we need is an open function which sets up the sequence ops.
 */
static int ct_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &ct_seq_ops);
};
/*
 * The file operations structure contains our open function along with
 * set of the canned seq_ ops.
 */
static struct file_operations ct_file_ops = {
	.owner   = THIS_MODULE,
	.open    = ct_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};
	
	
/*
 * Module setup and teardown.
 */
static int ct_init(void)
{
	struct proc_dir_entry *entry;
	entry = create_proc_entry("sequence", 0, NULL);
	if (entry)
		entry->proc_fops = &ct_file_ops;
	return 0;
}
static void ct_exit(void)
{
	remove_proc_entry("sequence", NULL);
}
module_init(ct_init);
module_exit(ct_exit);
```
[https://www.cnblogs.com/Wandererzj/archive/2012/04/16/2452209.html](https://www.cnblogs.com/Wandererzj/archive/2012/04/16/2452209.html)
