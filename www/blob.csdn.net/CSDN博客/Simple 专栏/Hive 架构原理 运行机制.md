# Hive  架构原理  运行机制 - Simple 专栏 - CSDN博客
2018年10月09日 20:28:56[Simple_Zz](https://me.csdn.net/love284969214)阅读数：504

![](https://img-blog.csdn.net/20181009202037785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)Hive 架构图
如图中所示，Hive通过给用户提供的一系列交互接口，接收到用户的指令(SQL)，使用自己的Driver，结合元数据(MetaStore)，将这些指令翻译成MapReduce，提交到Hadoop中执行，最后，将执行返回的结果输出到用户交互接口。
1．用户接口：Client
- CLI（command-line interface）、JDBC/ODBC(jdbc访问hive)、WEBUI（浏览器访问hive）
2．元数据：Metastore
- 元数据包括：表名、表所属的数据库（默认是default）、表的拥有者、列/分区字段、表的类型（是否是外部表）、表的数据所在目录等。
- 默认存储在自带的derby数据库中，推荐使用MySQL存储Metastore。
3．Hadoop
- 使用HDFS进行存储，使用MapReduce进行计算。
4．驱动器：Driver
- 解析器（SQL Parser）：将SQL字符串转换成抽象语法树AST，这一步一般都用第三方工具库完成，比如antlr；对AST进行语法分析，比如表是否存在、字段是否存在、SQL语义是否有误。
- 编译器（Physical Plan）：将AST编译生成逻辑执行计划。
- 优化器（Query Optimizer）：对逻辑执行计划进行优化。
- 执行器（Execution）：把逻辑执行计划转换成可以运行的物理计划。对于Hive来说，就是MR/Spark。
![](https://img-blog.csdn.net/20181009202828537?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)Hive 运行机制
