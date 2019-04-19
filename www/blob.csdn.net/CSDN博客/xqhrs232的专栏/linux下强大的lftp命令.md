# linux下强大的lftp命令 - xqhrs232的专栏 - CSDN博客
2016年10月18日 22:27:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1180
原文地址::[http://blog.csdn.net/chinalinuxzend/article/details/3318646](http://blog.csdn.net/chinalinuxzend/article/details/3318646)
相关文章
1、[linux lftp命令 详解](http://blog.chinaunix.net/uid-24993824-id-470961.html) ----[http://blog.chinaunix.net/uid-24993824-id-470961.html](http://blog.chinaunix.net/uid-24993824-id-470961.html)
原贴:http://www.php-oa.com/2008/04/17/lft.html
## [强大的lftp](http://www.php-oa.com/2008/04/17/lft.html)
[](http://www.php-oa.com/category/linux%E5%B8%B8%E7%94%A8%E6%8A%80%E5%B7%A7)[Linux](http://lib.csdn.net/base/linux)常用技巧4
 17th, 2008
原来一直认为wget很好用,很少用lftp.研究了一下.没想到lftp也这么强大
lftp很象shell: 有命令补全，历史记录，允许多个后台任务执行等功能，使用起来非常方便。这个可是超级好用的一点哦. 它还有书签、排队、镜像、断点续传、多进程下载等功能。
- #lftp --help
**语法介绍**
lftp [OPTS] <site>
`lftp' 是在 rc 文件执行后 lftp 执行的第一个命令
 -f <file>           执行文件中的命令后退出
 -c <cmd>            执行命令后退出
 --help              显示帮助信息后退出
 --version           显示 lftp 版本后退出
 其他的选项同 `open' 命令
 -e <cmd>            在选择后执行命令
 -u <user>[,<pass>]  使用指定的用户名/口令进行验证
 -p <port>           连接指定的端口
 <site>              主机名, URL 或书签的名字
例子
**1.下载**(镜象)
包括目录下的目录下的目录下的目录下的目录下的目录全拷过来。。。
- mirror .
--delete:  如果远程目录下已经没有某个文件，而本地有，则删除这个文件。
--only-newer:  只下载新的文件。
--exculde:  不包含某个目录。
--verbose:  详细输出。
**2.多线程下载**
- mirror -c -x b/ –parallel=3 .
上面的”-c“参数是说明支持断点续传；
“-x“参数是表明b目录不需要备份
“–parallel=3“是指同时下载3个文件
**3下载某种文件**
下传多个普通文件
- mget *
- mget  -c *.pdf
**4.上传**
- mirror -R 本地目录名
将本地目录以迭代（包括子目录）的方式反向上传到ftp site。
**5.编码设置**
远程ftp site用gbk编码，对应的要设置为utf8,只要替换gbk为utf8即可。有时会要用到.
代码:
- set file:charset utf8
**6.改变模式**
被动模式的问题需要在执行lftp命令以后，
set ftp:passive-mode off
