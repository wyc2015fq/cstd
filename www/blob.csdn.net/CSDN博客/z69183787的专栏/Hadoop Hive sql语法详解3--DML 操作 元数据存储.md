# Hadoop Hive sql语法详解3--DML 操作:元数据存储 - z69183787的专栏 - CSDN博客
2016年10月26日 11:43:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：393
个人分类：[大数据-Hive](https://blog.csdn.net/z69183787/article/category/5568487)
hive增删改查与传统数据的区别是什么？
hive不支持用insert语句一条一条的进行插入操作，也不支持update操作。数据是以load的方式加载到建立好的表中。数据一旦导入就不可以修改。
DML包括：INSERT插入、UPDATE更新、DELETE删除
•向数据表内加载文件
•将查询结果插入到Hive表中
•0.8新特性 insert into
向数据表内加载文件
•LOAD DATA [LOCAL] INPATH 'filepath' [OVERWRITE] INTO TABLE tablename [PARTITION (partcol1=val1, partcol2=val2 ...)]
•Load 操作只是单纯的复制/移动操作，将数据文件移动到 Hive 表对应的位置。
•filepath
•相对路径，例如：project/data1
•绝对路径，例如： /user/hive/project/data1
•包含模式的完整 URI，例如：hdfs://namenode:9000/user/hive/project/data1
例如：
hive> LOAD DATA LOCAL INPATH './examples/files/kv1.txt' OVERWRITE INTO TABLE pokes;
加载本地数据，同时给定分区信息
•加载的目标可以是一个表或者分区。如果表包含分区，必须指定每一个分区的分区名
•filepath 可以引用一个文件（这种情况下，Hive 会将文件移动到表所对应的目录中）或者是一个目录（在这种情况下，Hive 会将目录中的所有文件移动至表所对应的目录中）
LOCAL关键字
•指定了LOCAL，即本地
•load 命令会去查找本地文件系统中的 filepath。如果发现是相对路径，则路径会被解释为相对于当前用户的当前路径。用户也可以为本地文件指定一个完整的 URI，比如：file:///user/hive/project/data1.
•load 命令会将 filepath 中的文件复制到目标文件系统中。目标文件系统由表的位置属性决定。被复制的数据文件移动到表的数据对应的位置
例如：加载本地数据，同时给定分区信息：
hive> LOAD DATA LOCAL INPATH './examples/files/kv2.txt' OVERWRITE INTO TABLE invites PARTITION (ds='2008-08-15');
• 没有指定LOCAL
         如果 filepath 指向的是一个完整的 URI，hive 会直接使用这个 URI。 否则
•如果没有指定 schema 或者 authority，Hive 会使用在 hadoop 配置文件中定义的 schema 和 authority，fs.default.name 指定了 Namenode 的 URI
•如果路径不是绝对的，Hive 相对于 /user/ 进行解释。 Hive 会将 filepath 中指定的文件内容移动到 table （或者 partition）所指定的路径中
加载DFS数据 ，同时给定分区信息：
hive> LOAD DATA INPATH '/user/myname/kv2.txt' OVERWRITE INTO TABLE invites PARTITION (ds='2008-08-15');
The above command will load data from an HDFS file/directory to the table. Note that loading data from HDFS will result in moving the file/directory. As a result, the operation is almost instantaneous.
OVERWRITE
•指定了OVERWRITE
•目标表（或者分区）中的内容（如果有）会被删除，然后再将 filepath 指向的文件/目录中的内容添加到表/分区中。
•如果目标表（分区）已经有一个文件，并且文件名和 filepath 中的文件名冲突，那么现有的文件会被新文件所替代。
将查询结果插入Hive表
•将查询结果插入Hive表
•将查询结果写入HDFS文件系统
•基本模式
     INSERT OVERWRITE TABLE tablename1 [PARTITION (partcol1=val1, partcol2=val2 ...)] select_statement1 FROM from_statement
•多插入模式
FROM from_statement
INSERT OVERWRITE TABLE tablename1 [PARTITION (partcol1=val1, partcol2=val2 ...)] select_statement1
[INSERT OVERWRITE TABLE tablename2 [PARTITION ...] select_statement2] ...
•自动分区模式
INSERT OVERWRITE TABLE tablename PARTITION (partcol1[=val1], partcol2[=val2] ...) select_statement FROM from_statement
将查询结果写入HDFS文件系统
•INSERT OVERWRITE [LOCAL] DIRECTORY directory1 SELECT ... FROM ...
        FROM from_statement
        INSERT OVERWRITE [LOCAL] DIRECTORY directory1 select_statement1
     [INSERT OVERWRITE [LOCAL] DIRECTORY directory2 select_statement2]
•
•数据写入文件系统时进行文本序列化，且每列用^A 来区分，\n换行
INSERT INTO
•INSERT INTO  TABLE tablename1 [PARTITION (partcol1=val1, partcol2=val2 ...)] select_statement1 FROM from_statement
