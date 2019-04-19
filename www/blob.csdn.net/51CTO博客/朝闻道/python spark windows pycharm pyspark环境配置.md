# python spark windows pycharm pyspark环境配置-朝闻道-51CTO博客
1、下载如下
![python spark windows pycharm pyspark环境配置](https://s1.51cto.com/images/blog/201808/09/9d6ceb624d75791e2831d07c4a1dbced.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
放在D盘
添加 SPARK_HOME = D:\spark-2.3.0-bin-hadoop2.7。 
- 并将 %SPARK_HOME%/bin 添加至环境变量PATH。 
- 然后进入命令行，输入pyspark命令。若成功执行。则成功设置环境变量
![python spark windows pycharm pyspark环境配置](https://s1.51cto.com/images/blog/201808/09/8bc802b07189f8acf4b4fa1fef8bb58b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
找到pycharm sitepackage目录 
![python spark windows pycharm pyspark环境配置](https://s1.51cto.com/images/blog/201808/09/50b76a7cd1c4f828b03d8d438525b448.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
右键点击即可进入目录，将上面D:\spark-2.3.0-bin-hadoop2.7里面有个/python/pyspark目录拷贝到上面的 sitepackage目录 
![python spark windows pycharm pyspark环境配置](https://s1.51cto.com/images/blog/201808/09/7eeb7de8ae984b96fcf50ee4b584a8f1.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
安装 py4j
![python spark windows pycharm pyspark环境配置](https://s1.51cto.com/images/blog/201808/09/10db793ed138624f8b99a3c4da085e11.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
试验如下代码：
```
from __future__ import print_function
import sys
from operator import add
import os
# Path for spark source folder
os.environ['SPARK_HOME'] = "D:\spark-2.3.0-bin-hadoop2.7"
# Append pyspark to Python Path
sys.path.append("D:\spark-2.3.0-bin-hadoop2.7\python")
sys.path.append("D:\spark-2.3.0-bin-hadoop2.7\python\lib\py4j-0.9-src.zip")
from pyspark import SparkContext
from pyspark import SparkConf
if __name__ == '__main__':
    inputFile = "D:\Harry.txt"
    outputFile = "D:\Harry1.txt"
    sc = SparkContext()
    text_file = sc.textFile(inputFile)
    counts = text_file.flatMap(lambda line: line.split(' ')).map(lambda word: (word, 1)).reduceByKey(lambda a, b: a + b)
    counts.saveAsTextFile(outputFile)
```
计算成功即可
