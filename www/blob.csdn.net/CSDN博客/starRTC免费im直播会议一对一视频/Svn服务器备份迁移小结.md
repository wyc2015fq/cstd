# Svn服务器备份迁移小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年01月11日 14:26:58[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：474
注：svn备份千万不要采用打包压缩，然后解压文件的方式。
备份和还原之前先要关掉svn服务器。
svn备份一般采用三种方式：
- 1）svnadmin dump
- 2)svnadmin hotcopy
- 3)svnsync.
svn备份不宜采用普通的文件拷贝方式,如copy命令、rsync命令。
优缺点
- 第一种svnadmin dump是官方推荐的备份方式，优点是比较灵活，可以全量备份也可以增量备份，并提供了版本恢复机制。
缺点是：如果版本比较大，如版本数增长到数万、数十万，那么dump的过程将非常慢；备份耗时，恢复更耗时；不利于快速进行灾难恢复。
个人建议在版本数比较小的情况下使用这种备份方式。
- 第二种svnadmin hotcopy原设计目的估计不是用来备份的，只能进行全量拷贝，不能进行增量备份；
优点是：备份过程较快，灾难恢复也很快；如果备份机上已经搭建了svn服务，甚至不需要恢复，只需要进行简单配置即可切换到备份库上工作。
缺点是：比较耗费硬盘，需要有较大的硬盘支持。
- 第三种svnsync实际上是制作2个镜像库，当一个坏了的时候，可以迅速切换到另一个。不过，必须svn1.4版本以上才支持这个功能。
优点是：当制作成2个镜像库的时候起到双机实时备份的作用；
缺点是：当作为2个镜像库使用时，没办法做到“想完全抛弃今天的修改恢复到昨晚的样子”；而当作为普通备份机制每日备份时，操作又较前2种方法麻烦。
## 迁移
用到的命令：dump和load
这两个命令导出的是文本内容(肉眼可以读懂)。而且还有一次性导出和增量导出，单版本导出与多版本导出等多种选择操作。
在我们完成服务器端的版本迁移之后，必须通知客户端运行切换分支的命令，否则客户端的提交还是提交到旧的版本库去。SVN也为我们准备了这样的命令：svn switch。
### 导出
svnadmin dump oldRepository> dumpfile  //注，不能在根目录上dump多个Repository
svnadmin dump huanghe_projects >huanghe_projects.dump
### 创建新库
svnadmin create newRepostitory
### 导入
svnadmin load newRepository < dumpfile
一定要记得更新svnserve.conf这个文件。
### 启动
svnserve -d -r /data/svn --listen-port 3691
## 查看最新版本号
svnlook youngest devel/   (要进入到库的上一层目录，这个是在服务器上运行的)
svn switch命令用于更新工作副本的URL。意即指向新的版本库位置。首先要注意的一点是我们即可以将整个工作副本指向新的URL，也可将工作副本下的某个目录指向新的URL。
svn switch --relocate FROM TO 
如
先svn info查看下地址：Repository Root，替换这个就可以
svn switch --relocate svn://112.126.72.66/public/elesos svn://182.91.165.253:3691/public/elesos
windows可右键TortoiseSVN->Relocate，在里面填上新的url.
## 其它
过滤器svndumpfilter用于指定只包括那些项目，而不包括其它项目。
## 艺搜参考
[http://blog.csdn.net/hy6688_/article/details/12589869](http://blog.csdn.net/hy6688_/article/details/12589869)
[http://blog.csdn.net/shan9liang/article/details/7026287](http://blog.csdn.net/shan9liang/article/details/7026287)
[http://www.weiruoyu.cn/?p=597](http://www.weiruoyu.cn/?p=597)
[http://www.iitshare.com/linux-svn-migration.html](http://www.iitshare.com/linux-svn-migration.html)
[http://spiritfrog.iteye.com/blog/448578](http://spiritfrog.iteye.com/blog/448578)
