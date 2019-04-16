# 猪与蟒（pig and python) - 我相信...... - CSDN博客





2014年12月21日 14:45:42[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2622
个人分类：[云计算																[python](https://blog.csdn.net/wireless_com/article/category/5586333)](https://blog.csdn.net/wireless_com/article/category/1292302)

所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)








Python 真是无处不在呀.

pig 0.9以后将python作为嵌入式支持语音，使用Jython解释器利用python2.5的功能，这个接口的最上层是org.apache.pig.scripting.Pig
首先python脚本会对一段Pig Latin脚本进行编译，然后将在Python中定义的变量传递给它，最后执行它。

1） Pig.compile 或compilefromFile 对代码进行预编译
2）Bind 方法将控制流中的变量绑定到Pig Latin脚本中的变量，会返回一个BoundScript对象
3）针对BoundScript对象，可以调用runSingle方法来执行他，返回一个pigStat对象，如果在绑定过程中pig对象绑定到了一组包含参数的map，就要调用run方法了，同样返回一个pigStats对象。

用户编写的UDF的一个单独的实例会被构建并运行在每个map或reduce的任务中，构造器参数是一种传递信息给用户UDF的方式。
python与pig的类型对应

int number
long number
float number
double number
chararray string
bytearray string
map dictionary
tuple tuple
bag list oftuples 




pig 的加载函数是基于hadoop的InputFormat创建的，基类是loadFunc，LoadFunc 的默认实现是针对HDFS的，pig 提供了prepareToRead 方法为加载函数提供了初始化自己的一个途径。一旦用户的加载函数实现了getSchema 方法，那么Load语句就不再需要定义他们的模式了。

同理，存储函数式基于Hadoop的OutoutFormat构建的。接受的是Pig的Tuple，然后根据输出个好事创建健-值对后写入存储中，基类是storeFunc,Pig会在每个map或reduce 任务中调用存储函数的prepareToWrite方法，PutNext是存储函数的核心方法。            



