# C语言中构造数据类型的初始化 - xiahouzuoxin - CSDN博客





2013年12月14日 12:58:21[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2062
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)









## 1. 数组的静态初始化

我们都知道，一维数组的静态初始化可以写成



```cpp
int A[] = {1,2,3,4};
```
其实，可以指定数组元素进行初始化，比如

```cpp
int a[] = {
    [0] = 1, 
    [1] = 2, 
    [2] = 3
};
```
这种技巧在某些情况下其实很有用：（1）只想初始化数组中一些非连续的元素，其它元素都为0；（2）需要根据一组#define作为数组下标来保持某种映射关系的时候，就此我们举个例子。


比如要定义一组错误码和错误码提示的字符串，我们可以用#define来定义错误码，将错误码作为错误提示字符串数组的下标，



```cpp
/* 错误码定义 */
#define EINVAL  1
#define ENOMEM  2
#define EFAULT  3

/* 错误提示字符串 */
char *err_str[] = {
         [0] = "Success",
    [EINVAL] = "Invalid arguments",
    [ENOMEM] = "Not enough memory", 
    [EFAULT] = "Bad address"
};
```

对于这种情况，我们改变#define的值，都可以对err_str可以方便地不用做任何修改地使用索引。






## 2. 结构体的静态初始化

我们可以用字段名称对结构体进行初始化，这种方法在Linux内核源代码中使用非常普遍，比如

```cpp
static const struct file_operations mycdev_fops = {
	.owner = THIS_MODULE,
	.read = mycdev_read,
	.write = mycdev_write,
	.open = mycdev_open,
	.release = mycdev_release,
	.llseek = mycdev_llseek,
	.unlocked_ioctl = mycdev_unlocked_ioctl,
	.mmap = mycdev_mmap,
};
```
这段代码常用于字符设备驱动程序的设计中，owner、read等都是struct file_operations的成员变量（数据类型是函数指针），这里直接使用字段名称完成函数指针类型的结构体成员的初始化。



再给个简单的例子，定义坐标，



```cpp
struct point {
    int x;
    int y;
    int z;
}
```
我们可以这样初始化struct point，





```cpp
struct point p = {.x = 3, .y = 4, .z = 5};
```

这种使用字段名进行初始化的方法同样适用于union类型。

















