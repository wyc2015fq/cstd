# pmap - weixin_33985507的博客 - CSDN博客
2016年06月13日 13:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
```
1、[root@localhost security]# pmap -d 4993
    4993: -bash
    Address Kbytes Mode Offset Device Mapping
    08047000 596 r-x-- 0000000000000000 003:00006 bash bash的代码段
    080dc000 28 rwx-- 0000000000094000 003:00006 bash bash的数据段
    080e3000 280 rwx-- 00000000080e3000 000:00000 [ anon ] bash的堆
    4d575000 84 r-x-- 0000000000000000 003:00006 ld-2.3.4.so 共享库的代码段
    4d58a000 4 r-x-- 0000000000015000 003:00006 ld-2.3.4.so 共享库的数据段
    4d58b000 4 rwx-- 0000000000016000 003:00006 ld-2.3.4.so 共享库的堆
    4d58e000 1164 r-x-- 0000000000000000 003:00006 libc-2.3.4.so
    4d6b1000 4 r-x-- 0000000000123000 003:00006 libc-2.3.4.so
    4d6b2000 12 rwx-- 0000000000124000 003:00006 libc-2.3.4.so
    4d6b5000 8 rwx-- 000000004d6b5000 000:00000 [ anon ] 匿名物理内存，
    4d6de000 8 r-x-- 0000000000000000 003:00006 libdl-2.3.4.so
    4d6e0000 8 rwx-- 0000000000001000 003:00006 libdl-2.3.4.so
    4d807000 12 r-x-- 0000000000000000 003:00006 libtermcap.so.2.0.8
    4d80a000 4 rwx-- 0000000000002000 003:00006 libtermcap.so.2.0.8
    b7bc2000 176 r-x-- 0000000000000000 003:00006 GB18030.so
    b7bee000 8 rwx-- 000000000002b000 003:00006 GB18030.so
    b7bf0000 24 r-xs- 0000000000000000 003:00006 gconv-modules.cache
    b7bf6000 1060 r-x-- 0000000002197000 003:00006 locale-archive
    b7cff000 2048 r-x-- 0000000000000000 003:00006 locale-archive
    b7eff000 36 r-x-- 0000000000000000 003:00006 libnss_files-2.3.4.so
    b7f08000 8 rwx-- 0000000000008000 003:00006 libnss_files-2.3.4.so
    b7f0a000 8 rwx-- 00000000b7f0a000 000:00000 [ anon ]
    b7f20000 8 rwx-- 00000000b7f20000 000:00000 [ anon ]
    bff0d000 84 rw--- 00000000bff0d000 000:00000 [ stack ]
    ffffe000 4 ----- 0000000000000000 000:00000 [ anon ]
    mapped: 5680K writeable/private: 460K shared: 24K
2、[root@localhost security]# pmap -x 4993
    4993: -bash
    Address Kbytes RSS Anon Locked Mode Mapping
    08047000 596 - - - r-x-- bash
    080dc000 28 - - - rwx-- bash
    080e3000 280 - - - rwx-- [ anon ]
    4d575000 84 - - - r-x-- ld-2.3.4.so
    4d58a000 4 - - - r-x-- ld-2.3.4.so
    4d58b000 4 - - - rwx-- ld-2.3.4.so
    4d58e000 1164 - - - r-x-- libc-2.3.4.so
    4d6b1000 4 - - - r-x-- libc-2.3.4.so
    4d6b2000 12 - - - rwx-- libc-2.3.4.so
    4d6b5000 8 - - - rwx-- [ anon ]
    4d6de000 8 - - - r-x-- libdl-2.3.4.so
    4d6e0000 8 - - - rwx-- libdl-2.3.4.so
    4d807000 12 - - - r-x-- libtermcap.so.2.0.8
    4d80a000 4 - - - rwx-- libtermcap.so.2.0.8
    b7bc2000 176 - - - r-x-- GB18030.so
    b7bee000 8 - - - rwx-- GB18030.so
    b7bf0000 24 - - - r-xs- gconv-modules.cache
    b7bf6000 1060 - - - r-x-- locale-archive
    b7cff000 2048 - - - r-x-- locale-archive
    b7eff000 36 - - - r-x-- libnss_files-2.3.4.so
    b7f08000 8 - - - rwx-- libnss_files-2.3.4.so
    b7f0a000 8 - - - rwx-- [ anon ]
    b7f20000 8 - - - rwx-- [ anon ]
    bff0d000 84 - - - rw--- [ stack ]
    ffffe000 4 - - - ----- [ anon ]
    -------- ------- ------- ------- -------
    total kB 5680 - - -
3、查看进程1的设备格式，不显示头尾行
    pmap -d -q 1
4、循环显示进程3066的设备格式的最后1行，间隔2秒
     while true; do pmap -d  3066 | tail -1; sleep 2; done
每列的含义如下：
参数 解释
Address:00378000-0038d000 进程所占的地址空间
Kbytes 该虚拟段的大小
RSS 设备号（主设备：次设备）
Anon 设备的节点号，0表示没有节点与内存相对应
Locked 是否允许swapped
Mode 权限：r=read, w=write, x=execute, s=shared, p=private(copy on write)
Mapping: bash 对应的映像文件名
```
