# Linux 下查找文件或文件夹 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月27日 15:29:21[左直拳](https://me.csdn.net/leftfist)阅读数：1710
有些在我看来比较实用的命令，在这里记一下，避免每次都搜索一轮。
1、查找文件和文件夹
$ find . -name "mongo*"
从当前路径开始，向子目录查找名字含有 "mongo"的文件和文件夹
2、仅查找文件夹：
$ find . -name "mongo*" -type d
3、查找文件内容中含有指定内容的文件或文件夹
$ find . -name "*.coffee"|xargs grep "mongo"
查找后缀名为".coffee"、且文件中含有"mongo"的文件
4、查找指定时间内修改的文件
$ find . -mmin -240
查找60 * 4 分钟以前修改的文件
$ find . -mtime 0
查找过去0 * 24 - 1 * 24小时内修改过的文件
$ find . -mtime 1
查找过去1*24 - 2 * 24 小时内修改过的文件
参考资料：
[http://blog.sina.com.cn/s/blog_62e7fe670101dg9d.html](http://blog.sina.com.cn/s/blog_62e7fe670101dg9d.html)
