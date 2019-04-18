# HDFS中的命令行 - bigfacesafdasgfewgf - CSDN博客





2014年12月15日 10:00:24[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1409标签：[hdfs																[分布式																[数据库																[命令行																[存储机制](https://so.csdn.net/so/search/s.do?q=存储机制&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/puqutogether/article/category/2633077)





**HDFS中的命令行**

    本文介绍了HDFS以命令行运行的时候，几个常用的命令行的作用和如何使用~





# 1. fs



> 
fs是启动命令行动作,该命令用于提供一系列子命令。

使用形式为hadoop fs –cmd <args>

其中，cmd是子命令，args是具体的命令操作。

例如hadoop fs –help

或者说fs是其余子命令的父亲，其余都是在“-cmd”的模式下的！





# 2. –cat 输出



> 
hadoop fs –cat URI

将路径指定的文件输出到屏幕





# 3. –copyFromLocal 将本地文件复制到HDFS中



> 
hadoop fs -copyFromLocal <localsrc> URI

hadoop fs –copyFromLocal /usr/localFile.txt sample.txt

可以实现本地文件上传到HDFS中，而且目标路径是简化过的





# 4. -cp 复制



> 
hadoop fs –cp /usr/file1 /usr/files /user/dir 

源路径的文件复制到目标路径中，源路径可以有多个，但是目标路径只能有一个





# 5. –du 显示目录中所有文件的大小，或者一个文件的大小



> 
hadoop fs –du /usr/dir1





# 6. –dus 显示文件大小




# 7. –expunge 清空回收站



> 
hadoop fs –expunge





# 8. –get 复制本件到本地系统



> 
使用方法为：hadoop fs –get <locaodst>

hadoop fs –get /usr/file localfile





# 9. –put 本地文件复制到目标文件系统



> 
使用方式：hadoop fs –put <localsrc> <dst>

hadoop fs –put localfile hadfs://host:port/usr/file





# 10. –ls 浏览本地文件



> 
使用方法：hadoop fs –ls <arg>

返回信息：

文件名<副本数> 文件大小 修改日期 权限 用户 ID/组 ID





# 11. –lsr 递归地查阅文件内容




# 12. –mkdir 创建对应的文件目录



> 
hadoop fs –mkdir /usr/dir1/dir2/dir3/file





# 13. –mv 移动



> 
将源文件移动到目标路径，目标路径可以有多个

hadoop fs –mv /usr/file1 /usr/file2





# 14. –rm 删除



> 
hadoop fs –rm URI 





# 15. –rmr 递归删除




# 16. –Setrep 改变一个副本的复制份数



> 
hadoop fs –setrep –w 3 –R /usr/file





# 17. –Test 检查文件



> 
使用ezd对文件进行检查

-e：检查文件是否存在，若存在返回0；

-z：检查文件是否为0字节，如果是返回0；

-d：检查路径是否为目录，如果是返回1；




# 18. –text 将源文件输出为文本格式



> 
hadoop fs –text <src> 运行的格式是zip以及Text类](https://so.csdn.net/so/search/s.do?q=命令行&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=分布式&t=blog)](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)




