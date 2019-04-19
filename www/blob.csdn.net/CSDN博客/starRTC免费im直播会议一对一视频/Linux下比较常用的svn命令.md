# Linux下比较常用的svn命令 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月06日 13:49:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：275
svn: command not found
yum install -y subversion
以下是一些常用命令的使用方法，希望对大家有所帮助。
1，check out(co)签出代码
**svn co** path
签出特定版本
svn co path -r 79
签到本地指定目录下
 svn co path [本地目录全路径]
2，添加文件并提交
先往版本库中添加新文件svn add file，如：
**svn add *******.php  **//**添加当前目录下所有的php文件
再提交commit(ci),如：
**svn ci **-m “add test.php **file**”    **//**-m后面的引号中为注释注意必须要先add才行
3，更新update(up)
**svn up****//**将当前目录以及子目录下的所有文件都更新到最新版本
 
**svn up**-r200 test.php   **//**将版本库中的文件test.php还原到版本200
使用 svn 的话，可以 svn update + 文件名，就可以只更新某个文件
4,删除文件delete(del)
先删除
**svn delete** test.php
再提交
**svn ci **-m “delete test.php **file**”
5,查看提交日志
svn log -r HEAD  //查看最新日志
6，SVN 帮助
**svn help****svn help** ci
# 获取svn的URL地址
**svn info****svn info****|****grep**'URL'
但是敲大写的URL还需要按住shift键，可怜啊。咋办呢？为grep添加个-i参数吧，忽略大小写，搞定。
**svn info****|****grep**-i'url'
提交linux下新生成的文件
svn stat或 svn st 
  “?” 未纳入版本控制
svn: warning: 'apps/autocomplete' is already under version control
svn ignore 的用法（忽略文件及目录）
若已经创建了文件夹，并加入了版本控制，现在想忽略这个文件夹，但要保持文件夹的内容：
$ svn export spool spool-tmp 
$ svn rm spool 
$ svn ci -m 'Removing inadvertently added directory "spool".' 
$ mv spool-tmp spool 
$ svn propset svn:ignore 'spool' . 
$ svn ci -m 'Ignoring a directory called "spool".'

参考;
[http://blog.csdn.net/samxx8/article/details/51244265](http://blog.csdn.net/samxx8/article/details/51244265)

