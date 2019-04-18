# VC++笔记九 - L_Andy的专栏 - CSDN博客

2014年04月26日 16:26:32[卡哥](https://me.csdn.net/L_Andy)阅读数：1667


                
一、   .\与..\的区别的区别

.\   表示项目文件所在目录之下的目录。

..\ 表示项目文件所在目录向上一级目录下的目录。

..\..\表示项目文件所在目录向上二级目录之下的目录。

都是针对项目文件所在目录而言，用于定位其它文件的路径位置。

//////////////////////////////////////////////////////////

二、DDV_MaxChars() 函数是做什么？

DDX_Text(pDX, IDC_USERNAME, m_username);

DDV_MaxChars(pDX, m_username, 8);

从字面意思就可以看出控制字符串长度的

这个语句DDV_MaxChars(pDX, m_username, 8);

就是说m_username最长只有8个字符

//////////////////////////////////////////////////

为什么VC编辑框的最大字符数不起作用？DDV_MaxChars(pDX, m_strComputerID, 1);可是还能输入好多

解决方案：

一、有没有做DDX_Text关联你的String变量和控件。

二、PostMessage/SendMessage(HWND_PARANET, WM_COMMAND, MAKEWPARAM(IDC_EDIT1, EN_MAXTEXT), (LPARAM)HWND_EDIT);

CEdit::SetLimitText();

///////////////////////////////////////////////

LIB文件和DLL文件有什么不同？

(1)lib是编译时需要的，dll是运行时需要的。

如果要完成源代码的编译，有lib就够了。

如果也使动态连接的程序运行起来，有dll就够了。

在开发和调试阶段，当然最好都有。

(2)一般的动态库程序有lib文件和dll文件。lib文件是必须在编译期就连接到应用程序中的，而dll文件是运行期才会被调用的。

如果有dll文件，那么对应的lib文件一般是一些索引信息，具体的实现在dll文件中。如果只有lib文件，那么这个lib文件是静态编译出来的，

索引和实现都在其中。静态编译的lib文件有好处：给用户安装时就不需要再挂动态库了。但也有缺点，就是导致应用程序比较大，

而且失去了动态库的灵活性，在版本升级时，同时要发布新的应用程序才行。

(3)在动态库的情况下，有两个文件，一个是引入库（.LIB）文件，一个是DLL文件，引入库文件包含被DLL导出的函数的名称和位置，

DLL包含实际的函数和数据，应用程序使用LIB文件链接到所需要使用的DLL文件，库中的函数和数据并不复制到可执行文件中，

因此在应用程序的可执行文件中，存放的不是被调用的函数代码，而是DLL中所要调用的函数的内存地址，

这样当一个或多个应用程序运行是再把程序代码和被调用的函数代码链接起来，从而节省了内存资源。

从上面的说明可以看出，DLL和.LIB文件必须随应用程序一起发行，否则应用程序将会产生错误。

/////////////////////////////////////////////////////

批处理，%~d0 cd %~dp0 代表什么意思

%~dp0 “d”为Drive的缩写，即为驱动器，磁盘、“p”为Path缩写，即为路径，目录

cd是转到这个目录，不过我觉得cd /d %~dp0 还好些

选项语法:

    ~0         - 删除任何引号(")，扩充 %0

    %~f0        - 将 %0 扩充到一个完全合格的路径名(“f”是file，即文件)

    %~d0        - 仅将 %0 扩充到一个驱动器号

    %~p0        - 仅将 %0 扩充到一个路径

    %~n0        - 仅将 %0 扩充到一个文件名(“n”是name 文件名)

    %~x0        - 仅将 %0 扩充到一个文件扩展名

    %~s0        - 扩充的路径只含有短名(“s”为Short，短的)

    %~a0        - 将 %0 扩充到文件的文件属性(“a”为attribute，即属性)

    %~t0        - 将 %0 扩充到文件的日期/时间(“t”time)

    %~z0        - 将 %0 扩充到文件的大小(Size 大小)

    %~$PATH:0   - 查找列在路径环境变量的目录，并将 %0 扩充

                  到找到的第一个完全合格的名称。如果环境变量名

                  未被定义，或者没有找到文件，此组合键会扩充到

                  空字符串

可以组合修饰符来得到多重结果:

    %~dp0       - 仅将 %0 扩充到一个驱动器号和路径

    %~nx0       - 仅将 %0 扩充到一个文件名和扩展名

    %~fs0       - 仅将 %0 扩充到一个带有短名的完整路径名

    %~dp$PATH:0 - 查找列在路径环境变量的目录，并将 %I 扩充

                  到找到的第一个驱动器号和路径。

    %~ftza0     - 将 %0 扩充到类似输出线路的 DIR

%0为当前批处理文件

如果0换成1为第一个文件，2为第2个

/////////////////////////////////////////////////////

stat()获取文件的状态

头文件：#include <sys/stat.h>   #include <unistd.h>

定义函数：int stat(const char * file_name, struct stat *buf);

函数说明：stat()用来将参数file_name 所指的文件状态, 复制到参数buf 所指的结构中。

下面是struct stat 内各参数的说明：

struct stat

{

    dev_t st_dev; //device 文件的设备编号

    ino_t st_ino; //inode 文件的i-node

    mode_t st_mode; //protection 文件的类型和存取的权限

    nlink_t st_nlink; //number of hard links 连到该文件的硬连接数目, 刚建立的文件值为1.

    uid_t st_uid; //user ID of owner 文件所有者的用户识别码

    gid_t st_gid; //group ID of owner 文件所有者的组识别码

    dev_t st_rdev; //device type 若此文件为装置设备文件, 则为其设备编号

    off_t st_size; //total size, in bytes 文件大小, 以字节计算

    unsigned long st_blksize; //blocksize for filesystem I/O 文件系统的I/O 缓冲区大小.

    unsigned long st_blocks; //number of blocks allocated 占用文件区块的个数, 每一区块大小为512 个字节.

    time_t st_atime; //time of lastaccess 文件最近一次被存取或被执行的时间, 一般只有在用mknod、utime、read、write 与tructate 时改变.

    time_t st_mtime; //time of last modification 文件最后一次被修改的时间, 一般只有在用mknod、utime 和write 时才会改变

    time_t st_ctime; //time of last change i-node 最近一次被更改的时间, 此参数会在文件所有者、组、权限被更改时更新

};

先前所描述的st_mode 则定义了下列数种情况：

1、S_IFMT 0170000 文件类型的位遮罩

2、S_IFSOCK 0140000 scoket

3、S_IFLNK 0120000 符号连接

4、S_IFREG 0100000 一般文件

5、S_IFBLK 0060000 区块装置

6、S_IFDIR 0040000 目录

7、S_IFCHR 0020000 字符装置

8、S_IFIFO 0010000 先进先出

9、S_ISUID 04000 文件的 (set user-id on execution)位

10、S_ISGID 02000 文件的 (set group-id on execution)位

11、S_ISVTX 01000 文件的sticky 位

12、S_IRUSR (S_IREAD) 00400 文件所有者具可读取权限

13、S_IWUSR (S_IWRITE)00200 文件所有者具可写入权限

14、S_IXUSR (S_IEXEC) 00100 文件所有者具可执行权限

15、S_IRGRP 00040 用户组具可读取权限

16、S_IWGRP 00020 用户组具可写入权限

17、S_IXGRP 00010 用户组具可执行权限

18、S_IROTH 00004 其他用户具可读取权限

19、S_IWOTH 00002 其他用户具可写入权限

20、S_IXOTH 00001 其他用户具可执行权限上述的文件类型在 POSIX 中定义了检查这些类型的宏定义

21、S_ISLNK (st_mode) 判断是否为符号连接

22、S_ISREG (st_mode) 是否为一般文件

23、S_ISDIR (st_mode) 是否为目录

24、S_ISCHR (st_mode) 是否为字符装置文件

25、S_ISBLK (s3e) 是否为先进先出

26、S_ISSOCK (st_mode) 是否为socket 若一目录具有sticky 位 (S_ISVTX), 则表示在此目录下的文件只能被该文件所有者、此目录所有者或root 来删除或改名.

返回值：执行成功则返回0，失败返回-1，错误代码存于errno。

错误代码：

1、ENOENT 参数file_name 指定的文件不存在

2、ENOTDIR 路径中的目录存在但却非真正的目录

3、ELOOP 欲打开的文件有过多符号连接问题, 上限为16 符号连接

4、EFAULT 参数buf 为无效指针, 指向无法存在的内存空间

5、EACCESS 存取文件时被拒绝

6、ENOMEM 核心内存不足

7、ENAMETOOLONG 参数file_name 的路径名称太长

范例

#include <sys/stat.h>

#include <unistd.h>

main()

{

    struct stat buf;

    stat("/etc/passwd", &buf);

    printf("/etc/passwd file size = %d \n", buf.st_size);

}
            

