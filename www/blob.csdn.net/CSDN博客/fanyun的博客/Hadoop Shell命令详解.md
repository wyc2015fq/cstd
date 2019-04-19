# Hadoop Shell命令详解 - fanyun的博客 - CSDN博客
2016年03月21日 14:36:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2216
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
**     说明：**调用文件系统(FS)Shell命令应使用bin/hadoop fs <args>的形式。所有的的FS
 shell命令使用URI路径作为参数。
**1、cat**
说明：将路径指定文件的内容输出到stdout。
用法：hadoop fs -cat URI [URI …]
范例：
hadoop fs -cat hdfs://host1:port1/file1 hdfs://host2:port2/file2
hadoop fs -cat file:///file3/user/hadoop/file4
返回值：成功返回0，失败返回-1。
**2、chgrp**
说明：改变文件所属的组。使用-R将使改变在目录结构下递归进行。命令的使用者必须是文件的所有者或者超级用户。
用法：hadoop fs -chgrp [-R] GROUP URI [URI …]
范例：hadoop fs -chgrp -R hadoop /user/hadoop/
**3、chmod**
说明：改变文件的权限。使用-R将使改变在目录结构下递归进行。命令的使用者必须是文件的所有者或者超级用户。
用法：hadoop fs -chmod [-R] URI [URI …]
范例：hadoop fs -chmod -R 744 /user/hadoop/
**4、chown**
说明：改变文件的拥有者。使用-R将使改变在目录结构下递归进行。命令的使用者必须是超级用户。
用法：hadoop fs -chown [-R] [OWNER][:[GROUP]] URI [URI ]
范例：hadoop fs -chmod -R hadoop /user/hadoop/
**5、copyFromLocal(本地到hdfs)**
说明：除了限定源路径是一个本地文件外，和put命令相似。
用法：hadoop fs -copyFromLocal <localsrc> URI
**6、copyToLocal(hdfs到本地)**
说明：除了限定目标路径是一个本地文件外，和get命令类似。
用法：hadoop fs -copyToLocal [-ignorecrc] [-crc] URI <localdst>
**7、cp**
说明：将文件从源路径复制到目标路径。这个命令允许有多个源路径，此时目标路径必须是一个目录。
用法：hadoop fs -cp URI [URI …] <dest>
范例：
hadoop fs -cp /user/hadoop/file1 /user/hadoop/file2
hadoop fs -cp /user/hadoop/file1 /user/hadoop/file2 /user/hadoop/dir
返回值：成功返回0，失败返回-1。
**8、du**
说明：显示目录中所有文件的大小，或者当只指定一个文件时，显示此文件的大小。
用法：hadoop fs -du URI [URI …]
范例：
hadoop fs -du /user/hadoop/dir1 /user/hadoop/file1hdfs://host:port/user/hadoop/dir1
查看hbase所有文件的大小
hadoop fs -du hdfs://master:54310/hbase
返回值：成功返回0，失败返回-1。
**9、dus**
说明：显示文件的大小。
用法：hadoop fs -dus <args>
**10、expunge**
说明：清空回收站。
用法：hadoop fs -expunge
**11****、get(hdfs到本地)**
说明：复制文件到本地文件系统。可用-ignorecrc选项复制CRC校验失败的文件。使用-crc选项复制文件以及CRC信息。
用法：hadoop fs -get [-ignorecrc] [-crc] <src> <localdst>
范例：
hadoop fs -get /user/hadoop/file localfile
hadoop fs -get hdfs://host:port/user/hadoop/file localfile
返回值：成功返回0，失败返回-1。
**12、getmerge**
说明：接受一个源目录和一个目标文件作为输入，并且将源目录中所有的文件连接成本地目标文件。addnl是可选的，用于指定在每个文件结尾添加一个换行符。
用法：hadoop fs -getmerge <src> <localdst> [addnl]
**13、ls**
用法：hadoop fs -ls <args>
说明：
(1).如果是文件，则按照如下格式返回文件信息：
文件名 <副本数>
文件大小修改日期修改时间权限用户ID
组ID
(2).如果是目录，则返回它直接子文件的一个列表，就像在Unix中一样。目录返回列表的信息如下：
目录名 <dir> 修改日期修改时间权限用户ID
组ID
范例：
hadoop fs -ls /user/hadoop/file1 /user/hadoop/file2hdfs://host:port/user/hadoop/dir1 /nonexistentfile
返回值：成功返回0，失败返回-1。
**14、lsr**
用法：hadoop fs -lsr <args>
说明：ls命令的递归版本。类似于Unix中的ls -R。
**15、mkdir**
说明：接受路径指定的uri作为参数，创建这些目录。其行为类似于Unix的mkdir -p，它会创建路径中的各级父目录。
用法：hadoop fs -mkdir <paths>
范例：
hadoop fs -mkdir /user/hadoop/dir1 /user/hadoop/dir2
hadoop fs -mkdir hdfs://host1:port1/user/hadoop/dirhdfs://host2:port2/user/hadoop/dir
返回值：成功返回0，失败返回-1。
**16、movefromLocal**
说明：输出一个”not implemented“信息。
用法：dfs -moveFromLocal <src> <dst>
**17、mv**
说明：将文件从源路径移动到目标路径。这个命令允许有多个源路径，此时目标路径必须是一个目录。不允许在不同的文件系统间移动文件。
用法：hadoop fs -mv URI [URI …] <dest>
范例：
hadoop fs -mv /user/hadoop/file1 /user/hadoop/file2
hadoop fs -mv hdfs://host:port/file1 hdfs://host:port/file2hdfs://host:port/file3 hdfs://host:port/dir1
返回值：成功返回0，失败返回-1。
**18、put**
说明：从本地文件系统中复制单个或多个源路径到目标文件系统。也支持从标准输入中读取输入写入目标文件系统。
用法：hadoop fs -put <localsrc> … <dst>
范例：
hadoop fs -put localfile /user/hadoop/hadoopfile
hadoop fs -put localfile1 localfile2 /user/hadoop/hadoopdir
hadoop fs -put localfile hdfs://host:port/hadoop/hadoopfile
hadoop fs -put – hdfs://host:port/hadoop/hadoopfile
从标准输入中读取输入。
返回值：成功返回0，失败返回-1。
**19、rm**
说明：删除指定的文件。只删除非空目录和文件。请参考rmr命令了解递归删除。
用法：hadoop fs -rm URI [URI …]
范例：
hadoop fs -rm hdfs://host:port/file /user/hadoop/emptydir
返回值：成功返回0，失败返回-1。
**20、rmr**
说明：delete的递归版本。
用法：hadoop fs -rmr URI [URI …]
范例：
hadoop fs -rmr /user/hadoop/dir
hadoop fs -rmr hdfs://host:port/user/hadoop/dir
返回值：成功返回0，失败返回-1。
**21、setrep**
说明：改变一个文件的副本系数。-R选项用于递归改变目录下所有文件的副本系数。
用法：hadoop fs -setrep [-R] <path>
范例：
hadoop fs -setrep -w 3 -R /user/hadoop/dir1
返回值：成功返回0，失败返回-1。
**22、stat**
说明：返回指定路径的统计信息。
用法：hadoop fs -stat URI [URI …]
范例：
hadoop fs -stat path
返回值：成功返回0，失败返回-1。
**23、tail**
用法：将文件尾部1K字节的内容输出到stdout。支持-f选项，行为和Unix中一致。
用法：hadoop fs -tail [-f] URI
范例：
hadoop fs -tail pathname
返回值：成功返回0，失败返回-1。
**24、test**
用法：hadoop fs -test -[ezd] URI
选项：
-e 检查文件是否存在。如果存在则返回0。
-z 检查文件是否是0字节。如果是则返回0。
-d 如果路径是个目录，则返回1，否则返回0。
范例：
hadoop fs -test -e filename
**25、text**
说明：将源文件输出为文本格式。允许的格式是zip和TextRecordInputStream。
用法：hadoop fs -text <src>
**26、touchz**
说明：创建一个0字节的空文件。
用法：hadoop fs -touchz URI [URI …]
范例：
hadoop -touchz pathname
返回值：成功返回0，失败返回-1。
