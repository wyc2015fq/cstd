# 与猪(pig)共舞 - 我相信...... - CSDN博客





2014年12月20日 17:26:30[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1527
所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)









猪在吃东西的时候稍有浪费,pig 也有类似的情况.

pig执行时消耗的内存，一般对没有压缩的数据，将会占用4倍于本地磁盘空间的内存空间。Pig中的NULL与SQL中的NULL概念一样，类型转换的语法与Java中的语法是一样的，是强类型的。

## 过程函数

Pig Latin 是一种数据流语言，每个处理步骤都产生一个新的数据集，或产生一个新的关系。无论关系还是字段都是以字母字符开头，大小写敏感，注释和java相同。

任何一种数据流处理的第一步都是指定输入，pig通过load语句完成。默认加载函数是PigStorage，加载存放在HDFS中且以制表符分割的文件，可以使用相对路径或绝对路径。 更多情况是应用Using 语句指定指定加载函数，例如 Using HBaseStorage() 是从HBase中加载数据。 进一步，使用as语句确定加载的数据指定模式。同时，PigStorage和TextLoader这两个内置的加载函数是支持正则匹配的。 存储时使用Store 语句，是Load的镜像语句。标准输出使用Dump语句。

## 关系操作

foreach 语句接受一组表达式，在数据管道中将它们应用到每条记录中，可以使用自定义函数UDF，每一条处理结果是一条新的tuple。 Filter 可以选择将哪些数据保留到用户的数据流中，可以使用正则表达式，不可以在foreach中使用。 Group：将包含了特定的健所对应的值的所有记录封装到一个bag中，group中的字段是包含每个键值的tuple。 order by 按照一个或多个健对数据进行排序，总是会引起数据流通过一个reduce阶段，因此将所有相等的记录收集在一起是必要的。 distinct 对这个记录去重，会出发一个reduce处理过程。 jion 将一个输入的数据和另一个输入的数据放在一起，当键值相等的时候，数据会被连接在一起，没有匹配到的数据会被去掉，同样会触发一个新的reduce过程。 limit 获得指定的数据条数 sample 通过百分比获得抽样数据。 parallel 附在以上的操作符后，只设置reduce阶段的并行执行。

UDF可以java或python实现，PiggyBank包含了用户贡献的UDF集合。在脚本中通过register 命令完成这个注册过程，define命令来简化路径，invokeForxxx 方法来实现静态java函数的调用。

面对输入数据的倾斜问题，可以采用skew join的方法，但是使用skew join，可能会损失5%的性能。如果在下一步的处理中要求具有相同健的所有记录需要在同一个文件片中，就不能使用skew join了。

cogroup 是基于一个键收集多个输入的记录，结果是一条包含一个键和每个输入的一个数据包bag的记录。（和json类似）

union只是将两个数据集合并到一起，不会像Join那样去重。

cross 相当于叉乘运算，GFCross是一个内置的UDF。

Stream 可以在数据流中插入一个个性化的可执行任务，而mapreduce命令可在数据流中直接添加MapReduce任务,可以使用包含mapreduce 任务的jar文件。

pig对join，union，cross或cogroup 不会使用multiquery，只有对hadoop组合器才会采用。

set 设置执行MapReduce任务的环境变量，同时可以设置Partitioner 在shuffle阶段对记录进行划分，然后传给reducer。可以有宏定义和include其他pig latin 脚本。




**pig的开发工具**

 Eclipse 查件 http://code.google.com/p/pig-eclipse 

Emacs http://github.com/cloudera/piglatin-mode 

TextMate http://www.github.com/kevinweil/pig.tmbundle

 Vim http://www.vim.org/scripts/script.php?script_id=2186



