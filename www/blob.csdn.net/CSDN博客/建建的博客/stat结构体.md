# stat结构体 - 建建的博客 - CSDN博客
2017年08月12日 13:01:53[纪建](https://me.csdn.net/u013898698)阅读数：92
个人分类：[Darwin](https://blog.csdn.net/u013898698/article/category/7009805)
**stat结构体：**
struct stat {
    dev_t         st_dev;       //文件的设备编号
    ino_t          st_ino;       //节点
    mode_t      st_mode;      //文件的类型和存取的权限
    nlink_t        st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
    uid_t          st_uid;       //用户ID
    gid_t          st_gid;       //组ID
    dev_t         st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
    off_t          st_size;      //文件字节数(文件大小)
    unsigned long st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
    unsigned long st_blocks;    //块数
    time_t        st_atime;     //最后一次访问时间
    time_t        st_mtime;     //最后一次修改时间
    time_t        st_ctime;     //最后一次改变时间(指属性)
};
