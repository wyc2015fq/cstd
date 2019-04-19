# linux下fstat、stat和lstat 区别(转) - xqhrs232的专栏 - CSDN博客
2016年10月27日 22:41:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2721
原文地址::[http://www.cnitblog.com/guopingleee/archive/2008/11/13/51411.aspx](http://www.cnitblog.com/guopingleee/archive/2008/11/13/51411.aspx)
相关文章
1、[C语言中fstat、stat和lstat 区别](http://blog.csdn.net/qq_21792169/article/details/50454475)----[http://blog.csdn.net/qq_21792169/article/details/50454475](http://blog.csdn.net/qq_21792169/article/details/50454475)
2、linux：文件和目录之stat、fstat和lstat函数！！？----[http://zhan.renren.com/lyitnews?gid=3602888498060354213&checked=true](http://zhan.renren.com/lyitnews?gid=3602888498060354213&checked=true)
3、fstat__百度百科----[http://baike.baidu.com/link?url=aJ_RgnP8qcesGzfO9qfwF8Nu0yzXwh9vme9Fwqhsx1urF46z1gLmdtDeFtMCbfzRP_BESY9twW_roqdP9JFsv_](http://baike.baidu.com/link?url=aJ_RgnP8qcesGzfO9qfwF8Nu0yzXwh9vme9Fwqhsx1urF46z1gLmdtDeFtMCbfzRP_BESY9twW_roqdP9JFsv_)
stat系统调用系列包括了fstat、stat和lstat，它们都是用来返回“相关文件状态信息”的，三者的不同之处在于设定源文件的方式不同。
**1**
首先隆重介绍的是一个非常重要的”VIP”人物，他是fstat, stat和lstat三者都要用到的一个结构体类型，名字叫做struct stat。可以说，没有这个struct stat的支持，上述三个系统调用将寸步难行。
这个struct stat结构体在不同的UNIX/Linux系统中的定义是有小的区别的，但你完全不用担心，这并不会影响我们的使用。
在struct stat结构体中我们常用的且各个平台都一定有的域是：
**st_mode 文件权限和文件类型信息 （记住这个黑体橘红色）**
st_ino   与该文件关联的inode
st_dev   保存文件的设备
st_uid   文件属主的UID号
st_gid   文件属主的GID号
st_atime 文件上一次被访问的时间
st_ctime 文件的权限、属主、组或内容上一次被修改的时间
st_mtime 文件的内容上一次被修改的时间。（和st_ctime的不同之处显而易见）
st_nlink  该文件上硬连接的个数
我分别提取了solaris（UNIX）和fedora（Linux）的struct stat结构体的原始定义：大家可以自己比对一下便可以发现两者确实有所不同，但主要的域是完全相同的。
**solaris的struct stat定义：**
struct stat {
dev_t           st_dev;
ino_t           st_ino;
mode_t          st_mode;
nlink_t         st_nlink;
uid_t           st_uid;
gid_t           st_gid;
dev_t           st_rdev;
off_t           st_size;
timestruc_t     st_atim;
timestruc_t     st_mtim;
timestruc_t     st_ctim;
blksize_t       st_blksize;
blkcnt_t        st_blocks;
char            st_fstype[_ST_FSTYPSZ];
};
**fedora的struct stat定义：**
struct stat
{
__dev_t st_dev;                     /* Device.  */
unsigned short int __pad1;
__ino_t st_ino;                     /* File serial number.  */
__mode_t st_mode;                   /* File mode.  */
__nlink_t st_nlink;                 /* Link count.  */
__uid_t st_uid;                     /* User ID of the file’s owner. */
__gid_t st_gid;                     /* Group ID of the file’s group.*/
__dev_t st_rdev;                    /* Device number, if device.  */
unsigned short int __pad2;
__off_t st_size;                    /* Size of file, in bytes.  */
__blksize_t st_blksize;             /* Optimal block size for I/O.  */
__blkcnt_t st_blocks;               /* Number 512-byte blocks allocated. */
struct timespec st_atim;            /* Time of last access.  */
struct timespec st_mtim;            /* Time of last modification.  */
struct timespec st_ctim;            /* Time of last status change.  */
unsigned long int __unused4;
unsigned long int __unused5;
};
**2**
大家一定注意到了，在上面列举域的时候，我在st_mode处使用了**黑体橘红色**标识，原因在于这个域不像其他域那么容易使用，其他的域的值显而易见，而st_mode域是需要一些宏予以配合才能使用的。其实，通俗说，这些宏就是一些特定位置为1的二进制数的外号，我们使用它们和st_mode进行”&”操作，从而就可以得到某些特定的信息。
**文件类型标志包括：**
S_IFBLK：文件是一个特殊的块设备
S_IFDIR：文件是一个目录
S_IFCHR：文件是一个特殊的字符设备
S_IFIFO：文件是一个FIFO设备
S_IFREG：文件是一个普通文件（REG即使regular啦）
S_IFLNK：文件是一个符号链接
**其他模式标志包括：**
S_ISUID：文件设置了SUID位
S_ISGID：文件设置了SGID位
S_ISVTX：文件设置了sticky位
**用于解释st_mode标志的掩码包括：**
S_IFMT：文件类型
S_IRWXU：属主的读/写/执行权限，可以分成S_IXUSR, S_IRUSR, S_IWUSR
S_IRWXG：属组的读/写/执行权限，可以分成S_IXGRP, S_IRGRP, S_IWGRP
S_IRWXO：其他用户的读/写/执行权限，可以分为S_IXOTH, S_IROTH, S_IWOTH
**还有一些用于帮助确定文件类型的宏定义，这些和上面的宏不一样，这些是带有参数的宏，类似与函数的使用方法：**
S_ISBLK：测试是否是特殊的块设备文件
S_ISCHR：测试是否是特殊的字符设备文件
S_ISDIR：测试是否是目录（我估计find . -type d的源代码实现中就用到了这个宏）
S_ISFIFO：测试是否是FIFO设备
S_ISREG：测试是否是普通文件
S_ISLNK：测试是否是符号链接
S_ISSOCK：测试是否是socket
**3**
我们已经学习完了struct stat和各种st_mode相关宏，现在就可以拿它们和stat系统调用相互配合工作了！
int fstat(int filedes, struct stat *buf);
int stat(const char *path, struct stat *buf);
int lstat(const char *path, struct stat *buf);
聪明人一眼就能看出来fstat的第一个参数是和另外两个不一样的，对！fstat区别于另外两个系统调用的地方在于，fstat系统调用接受的是 一个“文件描述符”，而另外两个则直接接受“文件全路径”。文件描述符是需要我们用open系统调用后才能得到的，而文件全路经直接写就可以了。
stat和lstat的区别：当文件是一个符号链接时，lstat返回的是该符号链接本身的信息；而stat返回的是该链接指向的文件的信息。（似乎有些晕吧，这样记，lstat比stat多了一个l，因此它是有本事处理符号链接文件的，因此当遇到符号链接文件时，lstat当然不会放过。而 stat系统调用没有这个本事，它只能对符号链接文件睁一只眼闭一只眼，直接去处理链接所指文件喽）
