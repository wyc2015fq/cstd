# linux stat函数讲解 - xqhrs232的专栏 - CSDN博客
2016年03月23日 15:14:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：495
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://www.cnblogs.com/hnrainll/archive/2011/05/11/2043361.html](http://www.cnblogs.com/hnrainll/archive/2011/05/11/2043361.html)
相关文章
1、[fstat、stat和lstat 区别(转)](http://www.cnitblog.com/guopingleee/archive/2008/11/13/51411.html)----[http://www.cnitblog.com/guopingleee/archive/2008/11/13/51411.aspx](http://www.cnitblog.com/guopingleee/archive/2008/11/13/51411.aspx)
2、
## fstat函数及struct stat结构
 ----[http://blog.sina.com.cn/s/blog_803527e70100veqr.html](http://blog.sina.com.cn/s/blog_803527e70100veqr.html)
3、linux：文件和目录之stat、fstat和lstat函数！！？----[http://zhan.renren.com/lyitnews?gid=3602888498060354213&checked=true](http://zhan.renren.com/lyitnews?gid=3602888498060354213&checked=true)
stat函数讲解
表头文件:    #include <sys/stat.h>
             #include <unistd.h>
定义函数:    int stat(const char *file_name, struct stat *buf);
函数说明:    通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
返回值:      执行成功则返回0，失败返回-1，错误代码存于errno
错误代码:
    ENOENT         参数file_name指定的文件不存在
    ENOTDIR        路径中的目录存在但却非真正的目录
    ELOOP          欲打开的文件有过多符号连接问题，上限为16符号连接
    EFAULT         参数buf为无效指针，指向无法存在的内存空间
    EACCESS        存取文件时被拒绝
    ENOMEM         核心内存不足
    ENAMETOOLONG   参数file_name的路径名称太长
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
int main() {
    struct stat buf;
    stat("/etc/hosts", &buf);
    printf("/etc/hosts file size = %d\n", buf.st_size);
}
-----------------------------------------------------
struct stat {
    dev_t         st_dev;       //文件的设备编号
    ino_t         st_ino;       //节点
    mode_t        st_mode;      //文件的类型和存取的权限
    nlink_t       st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
    uid_t         st_uid;       //用户ID
    gid_t         st_gid;       //组ID
    dev_t         st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
    off_t         st_size;      //文件字节数(文件大小)
    unsigned long st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
    unsigned long st_blocks;    //块数
    time_t        st_atime;     //最后一次访问时间
    time_t        st_mtime;     //最后一次修改时间
    time_t        st_ctime;     //最后一次改变时间(指属性)
};
先前所描述的st_mode 则定义了下列数种情况：
    S_IFMT   0170000    文件类型的位遮罩
    S_IFSOCK 0140000    scoket
    S_IFLNK 0120000     符号连接
    S_IFREG 0100000     一般文件
    S_IFBLK 0060000     区块装置
    S_IFDIR 0040000     目录
    S_IFCHR 0020000     字符装置
    S_IFIFO 0010000     先进先出
    S_ISUID 04000     文件的(set user-id on execution)位
    S_ISGID 02000     文件的(set group-id on execution)位
    S_ISVTX 01000     文件的sticky位
    S_IRUSR(S_IREAD) 00400     文件所有者具可读取权限
    S_IWUSR(S_IWRITE)00200     文件所有者具可写入权限
    S_IXUSR(S_IEXEC) 00100     文件所有者具可执行权限
    S_IRGRP 00040             用户组具可读取权限
    S_IWGRP 00020             用户组具可写入权限
    S_IXGRP 00010             用户组具可执行权限
    S_IROTH 00004             其他用户具可读取权限
    S_IWOTH 00002             其他用户具可写入权限
    S_IXOTH 00001             其他用户具可执行权限
    上述的文件类型在POSIX中定义了检查这些类型的宏定义：
    S_ISLNK (st_mode)    判断是否为符号连接
    S_ISREG (st_mode)    是否为一般文件
    S_ISDIR (st_mode)    是否为目录
    S_ISCHR (st_mode)    是否为字符装置文件
    S_ISBLK (s3e)        是否为先进先出
    S_ISSOCK (st_mode)   是否为socket
    若一目录具有sticky位(S_ISVTX)，则表示在此目录下的文件只能被该文件所有者、此目录所有者或root来删除或改名。
-----------------------------------------------------
struct statfs {
    long    f_type;          //文件系统类型
    long    f_bsize;         //块大小
    long    f_blocks;        //块多少
    long    f_bfree;         //空闲的块
    long    f_bavail;        //可用块
    long    f_files;         //总文件节点
    long    f_ffree;         //空闲文件节点
    fsid_t f_fsid;           //文件系统id
    long    f_namelen;       //文件名的最大长度
    long    f_spare[6];      //spare for later
};
stat、fstat和lstat函数(UNIX)
#include<sys/types.h>
#include<sys/stat.h>
int stat(const char *restrict pathname, struct stat *restrict buf);
提供文件名字，获取文件对应属性。感觉一般是文件没有打开的时候这样操作。
int fstat(int filedes, struct stat *buf);
通过文件描述符获取文件对应的属性。文件打开后这样操作
int lstat(const char *restrict pathname, struct stat *restrict buf);
连接文件
三个函数的返回：若成功则为0，若出错则为-1
给定一个pathname，stat函数返回一个与此命名文件有关的信息结构，fstat函数获得已在描述符filedes上打开的文件的有关信息。lstat函数类似于stat，但是当命名的文件是一个符号连接时，lstat返回该符号连接的有关信息，而不是由该符号连接引用的文件的信息。
第二个参数是个指针，它指向一个我们应提供的结构。这些函数填写由buf指向的结构。该结构的实际定义可能随实现而有所不同，但其基本形式是：
struct stat{
mode_t st_mode;   /*file tpye &mode (permissions)*/
ino_t st_ino;     /*i=node number (serial number)*/
dev_t st_rdev;   /*device number for special files*/
nlink_t st_nlink; /*number of links*/
uid_t    st_uid; /*user id of owner*/
gid_t    st_gid; /*group ID of owner*/
off_t   st_size; /*size in bytes for regular files*/
time_t st_atime; /*time of last access*/
time_t st_mtime; /*time of last modification*/
time_t st_ctime; /*time of last file status change*/
long st_blksize; /*best I/O block size */
long st_blocks; /*number of 512-byte blocks allocated*/
};
   注意，除最后两个以外，其他各成员都为基本系统数据类型。我们将说明此结构的每个成员以了解文件属性。
使用stat函数最多的可能是ls-l命令，用其可以获得有关一个文件的所有信息。
1 函数都是获取文件（普通文件，目录，管道，socket，字符，块（）的属性。
函数原型
#include <sys/stat.h>
int stat(const char *restrict pathname, struct stat *restrict buf);
提供文件名字，获取文件对应属性。
int fstat(int filedes, struct stat *buf);
通过文件描述符获取文件对应的属性。
int lstat(const char *restrict pathname, struct stat *restrict buf);
连接文件描述命，获取文件属性。
2 文件对应的属性
struct stat {
        mode_t     st_mode;       //文件对应的模式，文件，目录等
        ino_t      st_ino;       //inode节点号
        dev_t      st_dev;        //设备号码
        dev_t      st_rdev;       //特殊设备号码
        nlink_t    st_nlink;      //文件的连接数
        uid_t      st_uid;        //文件所有者
        gid_t      st_gid;        //文件所有者对应的组
        off_t      st_size;       //普通文件，对应的文件字节数
        time_t     st_atime;      //文件最后被访问的时间
        time_t     st_mtime;      //文件内容最后被修改的时间
        time_t     st_ctime;      //文件状态改变时间
        blksize_t st_blksize;    //文件内容对应的块大小
        blkcnt_t   st_blocks;     //伟建内容对应的块数量
      };
可以通过上面提供的函数，返回一个结构体，保存着文件的信息。
