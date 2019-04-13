
# linux tar  使用 - 嵌入式Linux - CSDN博客

2015年11月02日 15:31:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：389个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)[Shell																](https://blog.csdn.net/weiqifa0/article/category/2102961)[
							](https://blog.csdn.net/weiqifa0/article/category/1388863)



#### tar
`tar`主要用于创建归档文件，和解压归档文件，其本身是没有压缩功能的，但可以调用`gzip`、`bzip2`进行压缩处理。
参数解释：
-c创建归档
-x解压归档
-v显示处理过程
-f目标文件，其后必须紧跟 目标文件
-j调用bzip2进行解压缩
-z调用gzip进行解压缩
-t列出归档中的文件
$tar-cvf filename.tar.\#\#\# 将当前目录所有文件归档，但不压缩，注意后面有个 ’.‘ ，不可省略，代表当前目录的意思
$tar-xvf filename.tar\#\#\# 解压 filename.tar 到当前文件夹
$tar-cvjf filename.tar.bz2.\#\#\# 使用 bzip2 压缩
$tar-xvjf  filename.tar.bz2\#\#\# 解压 filename.tar.bz2 到当前文件夹
$tar-cvzf filename.tar.gz\#\#\# 使用 gzip  压缩
$tar-xvzf filename.tar.gz\#\#\# 解压 filename.tar.gz 到当前文件夹
$tar-tf   filename\#\#\# 只查看 filename 归档中的文件，不解压


