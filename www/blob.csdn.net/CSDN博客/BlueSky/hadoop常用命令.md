# hadoop常用命令 - BlueSky - CSDN博客
2015年11月17日 15:23:54[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：255标签：[hadoop																[shell																[hdfs](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
**官网手册很详细**
网址：[http://hadoop.apache.org/docs/r1.0.4/cn/hdfs_shell.html](http://hadoop.apache.org/docs/r1.0.4/cn/hdfs_shell.html)
FS Shell 
调用文件系统(FS)Shell命令应使用 bin/hadoop fs 的形式。 所有的的FS shell命令使用URI路径作为参数。URI格式是scheme://authority/path。对HDFS文件系统，scheme是hdfs，对本地文件系统，scheme是file。其中scheme和authority参数都是可选的，如果未加指定，就会使用配置中指定的默认scheme。一个HDFS文件或目录比如/parent/child可以表示成hdfs://namenode:namenodeport/parent/child，或者更简单的/parent/child（假设你配置文件中的默认值是namenode:namenodeport）。大多数FS Shell命令的行为和对应的Unix Shell命令类似，不同之处会在下面介绍各命令使用详情时指出。出错信息会输出到stderr，其他信息输出到stdout。
cat 
使用方法：hadoop fs -cat URI [URI …]
将路径指定文件的内容输出到stdout。
示例：
hadoop fs -cat hdfs://host1:port1/file1 hdfs://host2:port2/file2 
hadoop fs -cat file:///file3 /user/hadoop/file4 
返回值： 
成功返回0，失败返回-1。
chgrp 
使用方法：hadoop fs -chgrp [-R] GROUP URI [URI …] Change group association of files. With -R, make the change recursively through the directory structure. The user must be the owner of files, or else a super-user. Additional information is in the Permissions User Guide. –>
改变文件所属的组。使用-R将使改变在目录结构下递归进行。命令的使用者必须是文件的所有者或者超级用户。更多的信息请参见HDFS权限用户指南。
chmod 
使用方法：hadoop fs -chmod [-R] 
