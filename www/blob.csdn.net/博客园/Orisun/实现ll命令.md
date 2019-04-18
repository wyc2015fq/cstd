# 实现ll命令 - Orisun - 博客园







# [实现ll命令](https://www.cnblogs.com/zhangchaoyang/articles/2295193.html)





$ ll wget
total 544
drwxr-xr-x 3 orisun orisun 4096 2011-12-15 09:48 ./
drwxr-xr-x 65 orisun orisun 4096 2011-12-20 19:45 ../
drwxr-xr-x 11 orisun orisun 4096 2011-08-09 21:55 wget-1.13/
-rw-r--r-- 1 orisun orisun 540756 2011-12-15 09:47 wget.pdf

目录总是不为空，它至少包含两项：.代表当前路径，..代表父路径。

ll命令用于列出一个目录下的文件的详细信息：
- 文件模式
这是个16位的2进制。最高4位表示文件类型，接下来3位依次是suid、sgid、sticky，后面的9位依次是user、group、other的读写执行标志。
文件类型：
1000　　　　-　　　　普通文件
0100　　　　d　　　　目录
0110　　　　b　　　　块设备
0010　　　　c　　　　字符设备
0001　　　　p　　　　命名管道
1010　　　　l　　　　符号链接
1100　　　　s　　　　套接字
文件可读意味着可以用cat命令查看，可写可以用vi打开。文件夹可执行则可以使用cd命令。
suid代表set-user-id，设置用户ID，即执行该文件的用户就像本文件的所有者一样。比如任何用户都可以通过password命令来修改自己的密码，但是存储密码的文件/etc/passwd的拥有者是root，其他用户是没有写权限的。而可执行文件/usr/bin/passwd的拥有者也是root，同时它的suid位上是1。则普通用户在运行/usr/bin/passwd的时候就成了root身份，既然是root身份自然对/etc/passwd拥有了写权限。那么passwd命令能不能修改其他用户的密码呢？不能！因为passwd命令会通过系统调用getuid来获得当前用户的ID，它只能修改这个ID用户的密码。如果文件拥有者本来具备写权限，同时suid位上是1，则相就位上的x会变成s；如果文件本来不是可执行文件，则相应位上会变成S。
sgid与suid类同。
sticky对文件和文件夹有不同的功用。sticky文件告诉系统要把文件放在swap区域。从交换空间加载程序比从普通磁盘区域加载要快，因为程序在硬盘上可能被分为好几块存放在许多不同的地方，而在交换空间上文件是不分块的。所以常用的程序如编辑器、编译器会放在交换空间。不过现在交换技术已经没那么重要了，取而代之的是虚拟内存，因为虚拟内存可以以更小的单位（如页page）进行交换。sticky使得一个目录里的文件只能被创建者删除，在/tmp目录里，谁都可以进行创建/删除文件。其他用户的x被替换这t，代表sticky被设置。
- 第二列显示链接数（硬链接），即指向该文件的引用次数。
- 第三列是文件所有者。/etc/passwd文件中包含用户列表，但它并没有包含所有的用户，因为在网络系统中，一台机器可以被很多用户登录，此时所有的用户信息都存储在一台叫作NIS的服务器上，所有的主机通过NIS进行身份验证。当然本地的/etc/passwd也存储了部分用户列表（这是NIS上用户列表的一个子集），以备离线操作。每个用户都有一个ID，该用户创建的文件有一个属性值就是此ID，这里就有一个问题，当用户被删除后，他创建的文件还在，如果后来又到了一个新用户，他刚好又被赋予了被删除的那个用户的ID，那么原先用户创建的文件归新用户所有。
- 第四列是文件拥有者所在的主组。不错，一个用户可以属于多个组。/etc/group里存储组列表。
- 第五列是文件大小。令我们好奇的是为什么上面三个文件夹的大小都是4096？这是因为目录所占的空间是以块为分配单位的，每块为512B。对于一般文件，则列出实际字节数。
- 第六列是文件的最后修改时间。
- 第七列是文件名。

下面讨论如何来编程实现ll命令。

文件的很多基本信息都可以通过系统调用lstat来获取，它返回一个结构体：

```
struct stat {
               dev_t     st_dev;     /* ID of device containing file */
               ino_t     st_ino;     /* inode number */
               mode_t    st_mode;    /* protection */
               nlink_t   st_nlink;   /* number of hard links */
               uid_t     st_uid;     /* user ID of owner */
               gid_t     st_gid;     /* group ID of owner */
               dev_t     st_rdev;    /* device ID (if special file) */
               off_t     st_size;    /* total size, in bytes */
               blksize_t st_blksize; /* blocksize for file system I/O */
               blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
               time_t    st_atime;   /* time of last access */
               time_t    st_mtime;   /* time of last modification */
               time_t    st_ctime;   /* time of last status change */
 };
```

一个struct stat里面似乎包含了我们编写ll命令所需的所有文件信息，但有些表现形式上还不对。
- 文件模式我们希望以“-rw-rw-r--"的形式显示，但st_mode是个十进制数。这里可以使用掩码来获取相应的位标识。
- 用户和组提供的是ID，而不是名称。通过UID来获取用户名可以用getpwuid(uid)->p_name，根据GID来获取组名可以用getgrgid(gid)->gr_name。getpwuid()先从本地/etc/passwd上查找用户，如果没有，再从NIS中获取。
- st_mtime是time_t类型的，我们可以通过系统调用localtime把它转换成struct tm类型：

```
struct tm {
int tm_sec;         /* seconds */
int tm_min;         /* minutes */
int tm_hour;        /* hours */
int tm_mday;        /* day of the month */
int tm_mon;         /* month */
int tm_year;        /* year */
int tm_wday;        /* day of the week */
int tm_yday;        /* day in the year */
int tm_isdst;       /* daylight saving time */
};
```

需要注意的是，这里的tm_year是减去了1990的，tm_mon是从0开始计算的。

下面给出完整代码。

```
#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
void do_ls(char[]);
void dostat(char *);
void show_file_info(char *,struct stat *);
void mode_to_letters(int,char[]);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

void main(int argc,char *argv[]){
    if(argc==1)
        do_ls(".");
    else
        printf("输入命令./ls即可，不要带任何参数。\n");
}

void do_ls(char dirname[]){
    DIR *dir_ptr;   //路径变量
    struct dirent *direntp;     //存储路径下一个子项信息的结构体
    if((dir_ptr=opendir(dirname))==0)
        fprintf(stderr,"ls:cannot open %s\n",dirname);
    else{
        while((direntp=readdir(dir_ptr))!=0)
            dostat(direntp->d_name);
        closedir(dir_ptr);
    }
}

void dostat(char *filename){
    struct stat info;
    if(lstat(filename,&info)==-1)
        perror("lstat");
    else
        show_file_info(filename,&info);
}

void show_file_info(char *filename,struct stat *info_p){
    char modestr[11];
    mode_to_letters(info_p->st_mode,modestr);
    printf("%-12s",modestr);
    printf("%-4d",(int)info_p->st_nlink);
    printf("%-8s",uid_to_name(info_p->st_uid));
    printf("%-8s",gid_to_name(info_p->st_gid));
    printf("%-8ld",(long)info_p->st_size);
    time_t timelong=info_p->st_mtime;
    struct tm *htime=localtime(&timelong);
    printf("%-4d-%02d-%02d %02d:%02d",htime->tm_year+1990,htime->tm_mon+1,htime->tm_mday,htime->tm_hour,htime->tm_min);
    printf(" %s\n",filename);
}

/*这个函数写得不够全面，首先文件类型不全，其次没有考虑suid,sgid,sticky*/
void mode_to_letters(int mode,char str[]){
    strcpy(str,"----------");
    if(S_ISDIR(mode))   str[0]='d';
    if(S_ISCHR(mode))   str[0]='c';
    if(S_ISBLK(mode))   str[0]='b';

    if(mode & S_IRUSR)  str[1]='r';
    if(mode & S_IWUSR)  str[2]='w';
    if(mode & S_IXUSR)  str[3]='x';

    if(mode & S_IRGRP)  str[4]='r';
    if(mode & S_IWGRP)  str[5]='w';
    if(mode & S_IXGRP)  str[6]='x';

    if(mode & S_IROTH)  str[7]='r';
    if(mode & S_IWOTH)  str[8]='w';
    if(mode & S_IXOTH)  str[9]='x';
}

#include<pwd.h>     //#include可以出现在代码中的任何位置
char * uid_to_name(uid_t uid){
    struct passwd *pw_str;
    static char numstr[10];
    if((pw_str=getpwuid(uid))==NULL){
        sprintf(numstr,"%d",uid);       //如果没有获得用户名，则直接把uid当作用户名
        return numstr;
    }
    else
        return pw_str->pw_name;
}

#include<grp.h>
char * gid_to_name(gid_t gid){
    struct group *grp_ptr;
    static char numstr[10];
    if((grp_ptr=getgrgid(gid))==NULL){
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}
```

![](https://pic002.cnblogs.com/images/2011/103496/2011122110481092.png)

### 硬链接和软链接

![](https://pic002.cnblogs.com/images/2012/103496/2012090409240518.png)

本来只存在file1，现在我们把file2软链接到file1。file1和file2是两个文件，拥有不同的Inode，我们看到file1的Inode为267519，而file2的Inode为262693。只是file2的文件内容指针指向了存放file1文件内容的磁盘块，所以我们看到file1的size为23，而file2的size只有5。软链接不会使"Links"的值增加，所以到目前为止file1和file2的links都是1。

然后又把file3硬链接到file2，file3和file2拥有相同的Inode，它们的Inode编号都是262693，即file3和file2是同一个文件的两个不同别名而已，用stat查看两文件的信息是完全一样的。这时候"Links"值要加1。

解释一下Access Time、Modify Time和Change Time的区别。Modify Time是文件内容最后一次被修改的时间，Change Time是Inode最后一次被修改的时间，Inode中存放着文件的元信息（即你使用stat系统调用看到的信息）。当给文件创建一个硬链接时，文件内容没有被更改，但Links数改变了，所以Change Time被更新，而Modify Time还是原先的值；当文件内容被更改时，即使文件的size没变，Modify Time也要变，而Modify Time也属于文件元信息的一部分，所以Change Time也要变。所以Change Time不会早于Modify Time，而Access Time则不会早于前两者。对文件施行cat和more时不会影响Access Time，对文件施行vim时（不论你是否改变了文件内容），Access Time都会得到更新。

在Linux系统编程中create会创建一个文件，unlink会将链接减1，在子进程中打开同一个文件时链接数会加1。当链接数减为0时系统并不会立即删除文件，此时文件仍然是可写（不可读）的，当程序退出后才会真正删除文件。












