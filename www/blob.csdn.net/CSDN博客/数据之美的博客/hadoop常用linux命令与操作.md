# hadoop常用linux命令与操作 - 数据之美的博客 - CSDN博客
2018年01月17日 11:27:30[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：833
一、linux常用命令
 1.
 通过远程工具登陆到linux后，所在的位置是当前登录用户的家目录(home directory)。
 2.
 家目录的符号用~表示。
 3.
 linux的文件系统是一个树结构。
linux文件系统的树根成为根目录，使用符号"/"表示。
linux文件系统中使用符号".."表示上级目录。
 linux文件系统中使用符号"."表示当前目录。
linux文件系统中，如果文件使用“.”开头，就表示隐藏文件
linux的路径表示方式有两种，一种是绝对路径(以”/“开头的路径)，一种是相对路径
 4.
 命令cd(change directory)：跳转到其他目录
 5.
 命令ls  ：查看目录下包含什么东西
 ls  -l：显示详细信息
 ls  -a：显示隐藏文件
 6.
 命令pwd：显示当前所处位置的
 7.
 命令clear：清屏
 8.
 命令mkdir：创建文件夹
 mkdir
 -p ：递归创建上级文件夹
 9.
 命令touch：创建空白文件
 10.命令more：查看文件内容
 11.命令rm：删除文件
 命令rmdir：删除文件夹
 命令rm
 -rf:删除文件(夹)，不论层级多深，一概删除，并且不给出提示
 12.命令cp(copy)：复制文件，还有一个功能是创建新文件
 13.命令mv(move):
 移动文件，还有个功能是重命名
 14.VI操作：
 14.1
 当使用命令vi f1回车时，进入了vi编辑器内，处于只读状态(只能看不能写)；
 14.2
 按字母”i“(insert)或者”a“(append)就可以进入编辑状态；
 14.3
 按键盘上的”Esc“键，再按Shift+:，进入命令状态
 14.4
 如果输入命令wq，意味着保存退出；
 14.5
 如果输入命令q！，意味着不保存退出；
 15.linux的权限使用9位字符表示，rwx
 rwx rwx
 15.1
 这里面r表示读权限，w表示写权限，x表示执行权限，-表示没有权限
 15.2
 这9位字符，每3位一组，分为3组。
 15.3
 第一组表示创建者的权限；
 15.4
 第二组表示创建者所在组的权限；
 15.5
 第三组表示其他人的权限；
 15.6
 权限还可以使用数字表示。rwx用数字表示为4+2+1=7。
 15.7
 新创建文件的默认权限是644；新创建文件夹的默认权限是755.
 16.命令chmod：
 16.1
 修改权限，三个组分别使用u、g、o表示，赋权限使用“+”，去掉权限使用“-”。
 16.2
 chmod u+x f1  //表示文件f1的创建者增加执行权限
 16.3
 chmod o-r f1  //表示文件f1的其他人去掉读权限
二、RPC
 1.1
 RPC (remote procedure call)远程过程调用.
 远程过程指的是不是同一个进程。
 1.2
 RPC至少有两个过程。调用方(client)，被调用方(server)。
 1.3
 client主动发起请求，调用指定ip和port的server中的方法，把调用结果返回给client。
 1.4
 RPC是hadoop构建的基础。
 2.
 通过例子获得的认识？
 2.1
 RPC是一个远程过程调用。
 2.2
 客户端调用服务端的方法，意味着调用服务端的对象中的方法。
 2.3
 如果服务端的对象允许客户端调用，那么这个对象必须实现接口。
 2.4
 如果客户端能够调用到服务端对象的方法，那么这些方法一定位于对象的接口中。
三⒍詇dfs操作
 1.
 对hdfs操作的命令格式是hadoop fs
 1.1
 -ls  表示对hdfs下一级目录的查看
 1.2
 -lsr  表示对hdfs目录的递归查看
 1.3  -mkdir  创建目录
 1.4
 -put  从linux上传文件到hdfs
 1.5
 -get  从hdfs下载文件到linux
 1.6
 -text  查看文件内容
 1.7
 -rm  表示删除文件
 1.7
 -rmr  表示递归删除文件
 2.
 hdfs在对数据存储进行block划分时，如果文件大小超过block，那么按照block大小进行划分；不如block size的，划分为一个块，是实际数据大小。
四、MapReduce执行步骤
 1.Map处理
 1.1
 读取文件内容，解析成key、value对。对输入文件的每一行解析成key、value对，每一个键值对调用一次map函数。
 1.2
 写map函数自己的逻辑，对输入的key、value处理，转换成新的key、value输出。
 1.3
 对输出的key、value进行分区
 1.4
 对不同分区的数据。按照key进行排序、分组。相同的key的value放到一个集合中。
 1.5
 （可选）分组后的数据进行规约
 2.Reduce处理
 2.1
 对多个map任务的输出，按照不同的分区，通过网络copy到不同的Reduce节点。
 2.2
 对多个map任务的输出进行合并、排序。写reduce函数自己的逻辑，对key、value进行处理，转换成新的key、value输出
 2.3
 把reduce的输出保存到文件中。
五、MapReduce相关的问题
 1.
 问：在eclipse中的写的代码如何提交作业到JobTracker中的哪？
 答：
 1.1
 在eclipse中调用的job.waitForCompletion(true)实际上执行如下方法
 connect();
 info
 = jobClient.submitJobInternal(conf);
 1.2
 在connect()方法中，实际上创建了一个JobClient对象。
 在调用该对象的构造方法时，获得了JobTracker的客户端代理对象JobSubmissionProtocol。
 JobSubmissionProtocol的实现类是JobTracker。
 1.3
 在jobClient.submitJobInternal(conf)方法中，调用了
 JobSubmissionProtocol.submitJob(...),
 即执行的是JobTracker.submitJob(...)。
 2.
 Hadoop的数据类型要求必须实现Writable接口。
 3.
 java基本类型与Hadoop常见基本类型的对照
Long  LongWritable
Integer  IntWritable
Boolean  BooleanWritable
String  Text
 4.
 问：java类型如何转化为hadoop基本类型？
 答：调用hadoop类型的构造方法，或者调用set()方法。
 new
 LongWritable(123L);
 5.
 问：hadoop基本类型如何转化为java类型？
 答：对于Text，需要调用toString()方法，其他类型调用get()方法。
-------------------------------------
作业：
1.如果使用IdentityMapper、IdentityReducer作为自定义的MR类，
 如果输入文件是hello文件。问：输出内容是什么？
2.用真实数据计算手机流量。
四、HBase模型介绍
 1.
 HBase(NoSQL)的数据模型
 1.1
 表(table)，是存储管理数据的。
 1.2
 行键(row key)，类似于MySQL中的主键。
 行键是HBase表天然自带的。
 1.3
 列族(column family)，列的集合。
 HBase中列族是需要在定义表时指定的，列是在插入记录时动态增加的。
 HBase表中的数据，每个列族单独一个文件。
 1.4
 时间戳(timestamp)，列(也称作标签、修饰符)的一个属性。
 行键和列确定的单元格，可以存储多个数据，每个数据含有时间戳属性，数据具有版本特性。 
 如果不指定时间戳或者版本，默认取最新的数据。
 1.5
 存储的数据都是字节数组。
 1.6
 表中的数据是按照行键的顺序物理存储的。
 2.HBase的物理模型
 2.1
 HBase是适合海量数据(如20PB)的秒级简单查询的数据库。
 2.2
 HBase表中的记录，按照行键进行拆分， 拆分成一个个的region。
 许多个region存储在region
 server(单独的物理机器)中的。
 这样，对表的操作转化为对多台region
 server的并行查询。
 3.HBase的体系结构
 3.1
 HBase是主从式结构，HMaster、HRegionServer
