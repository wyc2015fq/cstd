# HDFS - Orisun - 博客园







# [HDFS](https://www.cnblogs.com/zhangchaoyang/articles/2647536.html)





HDFS适合做：
- 存储大文件。上G、T甚至P。
- 一次写入，多次读取。并且每次作业都要读取大部分的数据。
- 搭建在普通商业机群上就可以了。虽然会经常宕机，但HDFS有良好的容错机制。

HDFS不适合做：
- 实时数据获取。如果有这个需求可以用HBase。
- 很多小文件。因为namenode要存储HDFS的metadata（比如目录的树状结构，每个文件的文件名、ACL、长度、owner、文件内容存放的位置等等信息），所以HDFS上文件的数目受到namenode内存的限制。
- 并发环境下的写入和修改。

Block

一个磁盘的block通常是512B，内核一次读写磁盘不能少于这个数目。在HDFS上一个Block的默认大小是64M，HDFS block的大小可以通过ds.block.size来设置，很多工作站上把一个block调为128M。之所以要把block设置得这么大，是因为HDFS上的文件普遍都是大文件，如果block很小，那一个文件就要存放在很多block上，而这些位置信息都要被namenode所记录，一来浪费namenode的存储空间，二来检索一个文件的时候开销也比较高。

当一个文件的长度小于一个block size时，它会单独占用一个block，但它占用的磁盘空间仍然是其真实的长度。

Namenode和Datanode

namenode管理文件系统的namespace，而datanode负责存储和检索block。一般情况下一个block会存放在多个不同的datanode上，以提高容错性。datanode在读写HDFS文件时，都需要通过namenode来获知读写的具体位置。

你可以使用distcp命令在不同的datanode之间并行地复制大文件:

$ hadoop distcp hdfs://datanode1/foo hdfs://datanode2/bar

HDFS上的文件是使用URI来定位的，前缀都是hdfs://localhost:9000，你可以把这个前缀赋给属性fs.default.name（属性可以在配置文件中指定，也可以在代码中指定），这样你就不用每次都写这个前缀了，比如以下2个命令是等价的：

$ hadoop fs -ls /

$ hadoop fs -ls hsfs://localhost:9000/

本地文件系统的前缀是file://

```
orisun@zcypc:~$ hadoop fs -ls file:///
Found 22 items
drwxr-xr-x   - root root       4096 2012-08-02 19:17 /home
dr-xr-xr-x   - root root          0 2012-08-20 22:14 /proc
drwxr-xr-x   - root root       4096 2010-04-23 18:11 /mnt
drwx------   - root root       4096 2012-08-18 10:46 /root
drwxr-xr-x   - root root       4096 2012-08-18 10:40 /sbin
……
```

HDFS默认的文件备份数量是3，这个可以在dfs.replication属性中设置，在伪分布式模式中由于datanode只有一个，所以要把该值设为1。当你使用hadoop fs -ls命令时会得到形如：

drwxr-xr-x   　　- 　　orisun 　　supergroup        0 　　2012-08-20 14:23　　 /tmp

-rw------- 　　　1 　　orisun 　　supergroup 　　4 　　2012-08-20 14:23 　　/tmp/jobtracker.info

跟UNIX下的ls命令很像，其中第2列就是replication的数目，第5列是文件的长度，以B为单位（文件夹的长度是0,而在UNIX文件系统中目录的长度是512B的整倍数，因为目录所占的空间是以块为分配单位的，每块为512B）。

FSDataInputStream继承自Java的DataInputStream并支持随机读写。

```
public class FSDataInputStream extends DataInputStream implements Seekable, PositionedReadable {}
```

```
public interface Seekable {
　　void seek(long pos) throws IOException;
　　long getPos() throws IOException;
　　boolean seekToNewSource(long targetPos) throws IOException;
}
```

FSDataInputStream还可以从指定的位置读取文件的一部分。

```
public interface PositionedReadable {
    public int read(long position, byte[] buffer, int offset, int length) throws IOException;
    public void readFully(long position, byte[] buffer, int offset, int length) throws IOException;
    public void readFully(long position, byte[] buffer) throws IOException;
}
```

如果你想在HDFS上新建一文件可以使用

```
public FSDataOutputStream create(Path f) throws IOException
```

使用create()函数时注意2点：文件必须之前不存在；它可附带地创建任意多级的父目录。

有时候你可能会需要用append()，在文件不存在时会创建它。

```
public FSDataOutputStream append(Path f) throws IOException
```

重命名文件

public void rename(String oldName,String newName)

当然你也可以用mkdir来创建目录

```
public boolean mkdirs(Path f) throws IOException
```

由于create()可以附带地创建任意多级的父目录，所以mkdir你并不会常用。

FileSystem的getFileStatus()方法可以获取文件和目录的FileStatus。

```
Path file = new Path("/dir/file");
FileStatus stat = fs.getFileStatus(file);
```

然后你就可以访问：

```
stat.getPath()
stat.getLen()
stat.isLen()
stat.getMogificationTime()
stat.getReplication()
stat.getBlockSize()
stat.getOwner()
stat.getReplication()
stat.getBlockSize()
stat.getGroup()
stat.getPermission()
```

实际上上述信息都存储在namenode中。

你还可以获取一个目录下所有文件的FileStatus。

```
public FileStatus[] listStatus(Path f) throws IOException
public FileStatus[] listStatus(Path f, PathFilter filter) throws IOException
public FileStatus[] listStatus(Path[] files) throws IOException
public FileStatus[] listStatus(Path[] files, PathFilter filter) throws IOException
```

在指定文件时，hadoop同样支持globbing，它支持的wildcard有：

*　　0个或多个任意字符

?　　任意单个字符

[ab]　　[^ab]　　[a-b]　　[^a-b]

{exp1,exp2}　　　　匹配exp1或exp2

\c　　转义

```
fs.listStatus(new Path("/2007/*/*"), new RegexExcludeFilter("^.*/2007/12/31$"))
```

将匹配2007年的所有文件，但是2007-12-31的文件将被filter掉。

```
public boolean delete(Path f, boolean recursive) throws IOException
```

删除目录时可以选择是否启用递归模式。

 上面已经提到大量的小文件会极大消耗namenode的内存，所以在这种情况下我们需要使用Hadoop Archives（HAR）把文件归档为一个大文件。

$ hadoop archive -archiveName orisun.har -p /user/orisun /user

把/user/orisun下的所有文件打包成orisun.tar放在/user目录下。

你还可以查看一个har文件中包含哪些文件：

```
orisun@zcypc:~$ hadoop fs -lsr har:///user/orisun.har
drwxr-xr-x   - orisun supergroup          0 2012-08-20 16:49 /user/orisun.har/mse
-rw-r--r--   1 orisun supergroup          0 2012-08-20 16:49 /user/orisun.har/mse/list
-rw-r--r--   1 orisun supergroup          0 2012-08-20 16:49 /user/orisun.har/book
orisun@zcypc:~$ hadoop fs -ls har:///user/orisun.har/mse
Found 1 items
-rw-r--r--   1 orisun supergroup          0 2012-08-20 16:49 /user/orisun.har/mse/list
```

HAR也是一个文件系统，一个Har URI的完整模式是har://<scheme>-<host>/<path>

```
orisun@zcypc:~$ hadoop fs -lsr har://hdfs-localhost:9000/user/orisun.har/mse
-rw-r--r--   1 orisun supergroup          0 2012-08-20 16:49 /user/orisun.har/mse/list
```

删除har文件必须使用rmr命令，用rm是不行的。

$ hadoop fs -rmr /user/orisun.har

 使用HAR的一些限制：
- 会产生原始文件的完整备份，占用磁盘空间。当然你可以以在建好har文件后把原文件删掉。
- HAR只是把多个文件打包成一个文件并没有采用任何的压缩策略。
- HAR文件是不可变，如何你想增加或从har中删除一个文件，你只能重新归档。
- InputFormat不理会har的存在，这意味着har文件对于MapReduce来说仍然会产生多个InputSlit，不会提高效率。要解决“小文件很多导致map task很多”的问题，可以采用CombineFileInputFormat。












