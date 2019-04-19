# HDFS  常用命令 - Simple 专栏 - CSDN博客
2015年03月05日 12:42:41[Simple_Zz](https://me.csdn.net/love284969214)阅读数：2975
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
## 文件操作
|查看  列出HDFS下的文件|# bin/hadoop dfs -ls|
|----|----|
|查看  列出HDFS文件下名为 in 的文档中的文件|# bin/hadoop dfs -ls in|
|上传  将hadoop目录下的test1文件上传到HDFS上并重命名为test|# bin/hadoop dfs -put test1 test|
|下载  将HDFS中的in文件复制到本地系统并命名为getin|# bin/hadoop dfs -get in getin|
|删除  文件或文件夹|# bin/hadoop fs -rm /user/atguigu/test/jinlian2.txt|
|删除  HDFS下名为out的文档|# bin/hadoop dfs -rmr out|
|查看  HDFS下in文件中的内容|# bin/hadoop dfs -cat in/*|
|创建  目录|# bin/hadoop dfs -mkdir /user/hadoop/examples|
|创建  多级目录|# bin/hadoop dfs -mkdir -p /user/hadoop/examples/a/b/c|
|复制  文件|# bin/hadoop dfs -copyFromLocal 源路径 路径|
|合并（ 注：合并后的文件位于当前目录，不在hdfs中，是本地文件）|# hdfs dfs -getmerge hdfs://Master:9000/data/abc.txt  all.txt|
## 管理与更新
|查看  HDFS的基本统计信息|# bin/hadoop dfsadmin -report|
|----|----|
|退出安全模式|# bin/hadoop dfsadmin -safemode leave|
|进入安全模式|# bin/hadoop dfsadmin -safemode enter|
|进入等待状态（安全模式退出后执行）|# bin/hadoop dfsadmin -safemode wait|
|获取状态信息|# bin/hadoop dfsadmin -safemode get|
|添加新节点|# bin/start-all.sh|
|负载均衡|# bin/start-balancer.sh|
