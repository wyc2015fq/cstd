
# SVN库迁移 - 小小情意 - 博客园






# [SVN库迁移](https://www.cnblogs.com/xiaoxiaoqingyi/p/7527624.html)
CentOS 环境下，由于某个磁盘下的空间快使用完了，需要将SVN迁移到另外一个拓展盘中（或其它目录）。
### 1、首先关闭目前的库，不让别人去操作这个库，避免在备份，迁移过程出错。
ps aux | grep svn
![](https://images2017.cnblogs.com/blog/804587/201709/804587-20170915174221235-160891231.png)
kill 10143 （关闭这个进程）
### 2、备份当前库，备份一般有两种方式，1： gzip 压缩备份方式。2：直接备份（默认方式）
我觉得选择默认的方式，不进行压缩，因为我用压缩备份，在恢复时出现乱码，恢复不了。
svnadmin dump /home/svn/svnrepos > /datadisk/repository-backup.svn
/home/svn/svnrepos ：当前库的路径
/datadisk/repository-backup.svn ： 目标路径（文件比较大，选定足够的磁盘）

### 3、备份好后，在你选定的目录下创建新的仓库 repository
svnadmin create /datadisk/svn/newrepos2

### 4、恢复备份的仓库
svnadmin load /datadisk/svn/newrepos2 < /datadisk/repository-backup.svn

### 5、恢复成功后，把 /home/svn/svnrepos/conf 下三个配置文件 覆盖到新创建的仓库里。

### 6、启动新的仓库
svnserve -d -r /datadisk/svn/newrepos2
可以通过 ps aux | grep svn 查询一下状态
**最后按照原来的链接方式访问SVN，完成！**
还有一个简单粗暴的方式： 先执行 1 步骤， 然后把 原库 迁移到 目标目录， 最后 执行 6步骤 开启仓库。






