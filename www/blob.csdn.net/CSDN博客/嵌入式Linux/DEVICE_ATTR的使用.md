
# DEVICE_ATTR的使用 - 嵌入式Linux - CSDN博客

2015年12月02日 14:43:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：882


使用DEVICE_ATTR，可以在sys fs中添加“文件”，通过修改该文件内容，可以实现在运行过程中动态控制device的目的。
类似的还有DRIVER_ATTR，BUS_ATTR，CLASS_ATTR。
这几个东东的区别就是，DEVICE_ATTR对应的文件在/sys/devices/目录中对应的device下面。
而其他几个分别在driver，bus，class中对应的目录下。
这次主要介绍DEVICE_ATTR，其他几个类似。
在documentation/driver-model/Device.txt中有对DEVICE_ATTR的详细介绍，这儿主要说明使用方法。
先看看DEVICE_ATTR的原型：
DEVICE_ATTR(_name, _mode, _show, _store)
_name：名称，也就是将在sys fs中生成的文件名称。
_mode：上述文件的访问权限，与普通文件相同，UGO的格式。
_show：显示函数，cat该文件时，此函数被调用。
_store：写函数，echo内容到该文件时，此函数被调用。
看看我们怎么填充这些要素：
名称可以随便起一个，便于记忆，并能体现其功能即可。
模式可以为只读0444，只写0222，或者读写都行的0666。当然也可以对User\Group\Other进行区别。
显示和写入函数就需要实现了。
显示函数的一般实现：
static ssize_t xxx_show(struct device *dev,
struct device_attribute *attr, char *buf)
{
return scnprintf(buf, PAGE_SIZE, "%d\n", dma_dump_flag);
}
实现相对简单，调用了个很阳春的scnprintf，把数据放到buf中，就算大功告成了。
至于buf中的内容怎么显示出来，这个先略过。
写入函数的一般实现：
static ssize_t xxx_store(struct device *dev,
struct device_attribute *attr, const char *buf, size_t count)
{
unsigned long num;
if (strict_strtoul(buf, 0, &num))
return -EINVAL;
if (num < 0)
return -EINVAL;
mutex_lock(&xxx_lock);
dma_dump_flag = num;
mutex_unlock(&xxx_lock);
return count;
}
也挺直白，就不细说了。
其中加了个lock进行互斥。
函数名中的后缀_show和_store当然不是必须的。
只是便于标识。
DEVICE_ATTR的定义例子：
static DEVICE_ATTR(xxx, 0666, xxx_show, xxx_store);
该代码可以防止文件的任何位置，只要别引起编译错误！
是不是这样就搞定了？
当然没有，还需要调用函数device_create_file来传教sys fs中的文件。
调用方法：
device_create_file(&pdev->dev, &dev_attr_xxx);
文件不是创建在某个device目录下么，pdev->dev就是该device。
dev_attr_xxx就是在xxx前加上dev_attr_，好像是废话，不过现实就是这样。
开始还找了半天，dev_attr_xxx在哪儿定义？
最终发现这儿就是它唯一出现的地方。
device_create_file的调用例子：
ret = device_create_file(&pdev->dev, &dev_attr_xxx);
if (ret != 0) {
dev_err(&pdev->dev,
"Failed to create xxx sysfs files: %d\n", ret);
return ret;
}
这个代码最好放在device的probe函数中。
原因么，在documentation/driver-model/Device.txt中有说明。
下面看看DEVICE_ATTR的定义：
\#define DEVICE_ATTR(_name, _mode, _show, _store) \
struct device_attribute dev_attr_\#\#_name = __ATTR(_name, _mode, _show, _store)
dev_attr_\#\#_name！！！！！
终于找到dev_attr_xxx定义的地方了！
\#define __ATTR(_name,_mode,_show,_store) { \
.attr = {.name = __stringify(_name), .mode = _mode }, \
.show = _show,     \
.store = _store,     \
}
device_attribute定义：
struct device_attribute {
struct attribute attr;
ssize_t (*show)(struct device *dev, struct device_attribute *attr,
char *buf);
ssize_t (*store)(struct device *dev, struct device_attribute *attr,
const char *buf, size_t count);
};
DEVICE_ATTR的功能就是定义一个device_attribute结构体对象。
device_create_file利用该对象在device下创建文件。
/**
* device_create_file - create sysfs attribute file for device.
* @dev: device.
* @attr: device attribute descriptor.
*/
int device_create_file(struct device *dev,
const struct device_attribute *attr)
{
int error = 0;
if (dev)
error = sysfs_create_file(&dev->kobj, &attr->attr);
return error;
}
/**
* sysfs_create_file - create an attribute file for an object.
* @kobj: object we're creating for.
* @attr: attribute descriptor.
*/
int sysfs_create_file(struct kobject * kobj, const struct attribute * attr)
{
BUG_ON(!kobj || !kobj->sd || !attr);
return sysfs_add_file(kobj->sd, attr, SYSFS_KOBJ_ATTR);
}
sd的类型为struct sysfs_dirent。
/*
* sysfs_dirent - the building block of sysfs hierarchy.  Each and
* every sysfs node is represented by single sysfs_dirent.
*
* As long as s_count reference is held, the sysfs_dirent itself is
* accessible.  Dereferencing s_elem or any other outer entity
* requires s_active reference.
*/
struct sysfs_dirent {
atomic_t  s_count;
atomic_t  s_active;
\#ifdef CONFIG_DEBUG_LOCK_ALLOC
struct lockdep_map dep_map;
\#endif
struct sysfs_dirent *s_parent;
struct sysfs_dirent *s_sibling;
const char  *s_name;
const void  *s_ns; /* namespace tag */
union {
struct sysfs_elem_dir  s_dir;
struct sysfs_elem_symlink s_symlink;
struct sysfs_elem_attr  s_attr;
struct sysfs_elem_bin_attr s_bin_attr;
};
unsigned int  s_flags;
unsigned short  s_mode;
ino_t   s_ino;
struct sysfs_inode_attrs *s_iattr;
};
int sysfs_add_file(struct sysfs_dirent *dir_sd, const struct attribute *attr,
int type)
{
return sysfs_add_file_mode(dir_sd, attr, type, attr->mode);
}
int sysfs_add_file_mode(struct sysfs_dirent *dir_sd,
const struct attribute *attr, int type, mode_t amode)
{
umode_t mode = (amode & S_IALLUGO) | S_IFREG;
struct sysfs_addrm_cxt acxt;
struct sysfs_dirent *sd;
int rc;
// 分配空间，并初始化部分成员。
sd = sysfs_new_dirent(attr->name, mode, type);
if (!sd)
return -ENOMEM;
sd->s_attr.attr = (void *)attr;
/*
* Initialize a lock instance's lock-class mapping info:
*/
sysfs_dirent_init_lockdep(sd);
/**
* sysfs_addrm_start - prepare for sysfs_dirent add/remove
* @acxt: pointer to sysfs_addrm_cxt to be used
* @parent_sd: parent sysfs_dirent
*
* This function is called when the caller is about to add or
* remove sysfs_dirent under @parent_sd.  This function acquires
* sysfs_mutex.  @acxt is used to keep and pass context to
* other addrm functions.
*
* LOCKING:
* Kernel thread context (may sleep).  sysfs_mutex is locked on
* return.
*/
sysfs_addrm_start(&acxt, dir_sd);
/**
* sysfs_add_one - add sysfs_dirent to parent
* @acxt: addrm context to use
* @sd: sysfs_dirent to be added
*
* Get @acxt->parent_sd and set sd->s_parent to it and increment
* nlink of parent inode if @sd is a directory and link into the
* children list of the parent.
*
* This function should be called between calls to
* sysfs_addrm_start() and sysfs_addrm_finish() and should be
* passed the same @acxt as passed to sysfs_addrm_start().
*
* LOCKING:
* Determined by sysfs_addrm_start().
*
* RETURNS:
* 0 on success, -EEXIST if entry with the given name already
* exists.
*/
rc = sysfs_add_one(&acxt, sd);
/**
* sysfs_addrm_finish - finish up sysfs_dirent add/remove
* @acxt: addrm context to finish up
*
* Finish up sysfs_dirent add/remove.  Resources acquired by
* sysfs_addrm_start() are released and removed sysfs_dirents are
* cleaned up.
*
* LOCKING:
* sysfs_mutex is released.
*/
sysfs_addrm_finish(&acxt);
if (rc)
sysfs_put(sd);
return rc;
}
基本上知道是怎么回事了。
暂时先到这，就不再深入了。


