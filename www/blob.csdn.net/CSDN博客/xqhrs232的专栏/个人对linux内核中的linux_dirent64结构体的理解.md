# 个人对linux内核中的linux_dirent64结构体的理解 - xqhrs232的专栏 - CSDN博客
2017年08月30日 15:46:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：649
原文地址::[http://blog.csdn.net/wfjrmars/article/details/50383589](http://blog.csdn.net/wfjrmars/article/details/50383589)
相关文章
1、dirent_百度百科------------[https://baike.baidu.com/item/dirent/2857085?fr=aladdin](https://baike.baidu.com/item/dirent/2857085?fr=aladdin)
结构定义：
struct linux_dirent64 {
     u64          d_ino;                   /* inode number 索引节点号 */
     s64          d_off;                     /* offset to this dirent 在目录文件中的偏移 */
     unsigned short     d_reclen;   
     unsigned char     d_type;        /* the type of d_name 文件类型 */
     char          d_name[0];             /* file name (null-terminated) 文件名，最长255字符 */
};
该结构体的指针用于传递给系统调用sys_getdents64，以获取一个目录中所有的目录项。
结构体初始大小为24字节，但是会随着文件名的增长而增大，而且不是连续增大，而是每次增大8字节。当文件名长度小于等于4字节时，结构大小为24字节；文件名长度达到5字节时，结构大小变为32字节；以后文件名长度每增加8字节，结构大小也增加8字节。
结构在内存中的布局好像有点奇怪，当直接使用指针引用成员时会出现获取的值与目标不相符的情况，因此我采用定位目标成员地址的方式获取目标成员的值。d_reclen在偏移8字节处，d_name在偏移19字节处。
d_name的声明比较特殊，类似于数组，但是数组元素数不可能为0。在sys_getdents64返回之后，d_name，或者说它的内存地址处会保存文件名。不能通过d_name直接访问文件名字符串，而需要一个字节一个字节读取。
sys_getdents获取的所有目录项最后会保存在一段连续内存中，类似于数组，但正如之前所说的，每个元素的大小可能是不一样的，而函数的返回值是所有元素的总字节数。作为参数传递的linux_dirent64结构体指针将指向第一个元素。
最后说说d_reclen这个成员，首先它的类型大小是2字节；然后它的值和文件名长度以及它在第几个元素中有关系。第一个元素中d_reclen的大小是12，从第二个元素开始，如果文件名长度在1~4之间d_reclen就增加12，如果文件名长度在5~8之间就就d_reclen增加16，依次类推。要把它和结构体大小变化规律区分开来，否则编程很容易出错。

