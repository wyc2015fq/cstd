# mysql 一些排错技巧 - Arthur的随笔 - CSDN博客
2012年01月29日 11:20:55[largetalk](https://me.csdn.net/largetalk)阅读数：3155
今天开机之后mysql启动不了了，搞了半天原来是ip换了导致的， 原来在my.cnf中我的bind-address是绑定到特定ip的，其实如果想让别的机器访问到只要把这一行注释掉就行了。
虽然花费了些时间，不过也学到了一些mysql查错的技巧
1. 到/var/log/mysql/下， tail -f error.log
2. 调试用mysqld --console， 必须要以root用户
我这里用调试显示的信息是unable to lock 'ibdata1', 在网上找都说是磁盘满了，可我磁盘还没满，被误导了，想这个想了很久
后来还是error.log中这两条错误提示提醒了我
120129 10:56:07 [ERROR] Can't start server: Bind on TCP/IP port: Cannot assign requested address
120129 10:56:07 [ERROR] Do you already have another mysqld server running on port: 3306 ?
然后ifconfig一下才恍然大悟。
还有一开始也被error.log中[Note] Plugin 'FEDERATED' is disabled. 给迷惑了
网上都说是innodb的什么什么错误，而且我也刚好用到了innodb
下面来自网络:
```
InnoDB数据存储结构
1.MySQL将所有数据都逻辑地存放在ib_data1文件中，我们称之为表空间。当然，你也可以一个表对应一个物理文件,将innodb_file_per_table设置成ON即可。
2.表空间又划为成段，有数据段(leaf node segment)，索引段(none-leaf node segment)，回滚段(rollback segment)。表空间由这些段和页组成，比如32页碎片页。
3.每段又划为成区，InnoDB每次最多可以申请4个区，即4M的存储空间。
4.每个区又划为成页，一个区划分成64页，每个页的大小是16KB，大小不能够改，这也固定了一个区的大小为4M。页是MySQL操作的最小逻辑单位。
5.InnoDB是面向行的，这就意味着数据行存放在页中，每页最多能记录7992行数据。
6.MySQL定义了不同作用的页类型，比如B-Tree Page, Undo Log Page等，我们最关心的是B-Tree Page(数据页)。实际数据就以这样的页逻辑实体存在于表空间,总是以B+树结构索引组织的。
7.换句话就说，实际数据一行一行地存放在B-Tree页中，这些页都放在数据段leaf node segment中。B-Tree Page是B+树的叶子节点。
8.一个B-Tree树，由7部分构成
8-1.File Header，这里记录了页在表空间的一些信息，比如上一页，下一页，属于哪个表空间等等
8-2.Page Header, 这里记录了页本身的一些存储信息。比如第一个记录的位置，记录数，最后插入记录行的位置，该页的索引ID等等
8-3.Infimum & Supermum Records, MySQL虚拟的二个行记录，用来界定记录的边界。分别代表此页中任何pk值还小的值和任何pk值还大的值。
8-4.user records, 实际存储的行记录。
8-5.free space,空闲空间，同样是链表结构。当一个数据记录删除后，就会加入到空闲链表中
8-6.page directory, 存放了记录的相对位置。注:聚集索引本身找不到具体的一条记录。而是通过 聚集索引找到该记录所在的页，然后再通过Page Directory进行二分查找找到具体数据。
8-7.File Trailer, MySQL InnoDB利用它来保证页完整地写入磁盘。
```
有时mysql链接速度非常慢时，不妨在my.cnf 添加 skip-name-resolve 这行配置看看
