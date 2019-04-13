
# 怎样在Linux驱动中创建proc节点（示例） - 嵌入式Linux - CSDN博客

2014年12月29日 15:51:15[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4469


一、定义proc节点的读、写函数
static int tp_switch_writeproc(struct file *file,const char *buffer,
unsigned long count,void *data)
{
sscanf(buffer,"%d", &tp_dbg);
printk("tpd: proc-->tp_dbg = %d\n", tp_dbg);
return count;
}
static int tp_switch_readproc(char *page, char **start, off_t off,
int count,int *eof, void *data)
{
int len;
unsigned char tmp =tp_dbg&0x0F;
len = sprintf(page,"%c\n", tmp);
return 2;
}
二、驱动加载时创建proc节点的入口
\#include <linux/proc_fs.h>
static struct proc_dir_entry *tp_root;
static struct proc_dir_entry *debug_entry;
\#define USER_ROOT_DIR "tp_debug"
\#define USER_ENTRY1   "debug_switch"

staticint goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
… …
init_debug_port();   //创建proc调试节点
… …
}

static int init_debug_port(void)
{
pt_root =proc_mkdir(USER_ROOT_DIR, NULL);
if (NULL==pt_root)
{
printk(KERN_ALERT"Create dir /proc/%s error!\n", USER_ROOT_DIR);
return -1;
}
printk(KERN_INFO"Create dir /proc/%s\n", USER_ROOT_DIR);
// Create a test entryunder USER_ROOT_DIR
pt_entry1 =create_proc_entry(USER_ENTRY1, 0666, pt_root);
if (NULL ==pt_entry1)
{
printk(KERN_ALERT"Create entry %s under /proc/%s error!\n",
USER_ENTRY1,USER_ROOT_DIR);
goto err_out;
}
pt_entry1->write_proc= tp_switch_writeproc;
pt_entry1->read_proc =tp_switch_readproc;
printk(KERN_INFO"Create /proc/%s/%s\n",
USER_ROOT_DIR,USER_ENTRY1);
return 0;
err_out:
pt_entry1->read_proc =NULL;
pt_entry1->write_proc= NULL;
remove_proc_entry(USER_ROOT_DIR,pt_root);
return -1;
}
三、卸载驱动时删除proc节点
static void remove_debug_port(void)
{
remove_proc_entry(USER_ENTRY1,pt_root);
remove_proc_entry(USER_ROOT_DIR,NULL);
}
static int goodix_ts_remove(struct i2c_client *client)
{
… …
remove_debug_port();
… …
}

