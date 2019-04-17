# 【Python学习系列四】Python程序通过hadoop-streaming提交到Hadoop集群执行MapReduce - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月07日 15:55:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3975
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









                
场景：将Python程序通过hadoop-streaming提交到Hadoop集群执行。

参考：http://www.michael-noll.com/tutorials/writing-an-hadoop-mapreduce-program-in-python/



1、Python编写Mapper

   业务逻辑是从会从标准输入（stdin）读取数据，默认以空格分割单词，然后按行输出单词机器出现频率到标准输出（stdout），不过整个Map处理过程并不会统计每个单词出现的总次数，而是直接输出“word,1”，以便作为Reduce的输入进行统计。

   代码如下：



```python
#coding:utf-8

'''
Created on 2017年6月7日 
@author: fjs
'''

#!/usr/bin/env python
import sys

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        # write the results to STDOUT (standard output);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        print '%s\t%s' % (word, 1)
```
2、Python编写Reducer

   Reduce代码，它会从标准输入（stdin）读取mapper.py的结果，然后统计每个单词出现的总次数并输出到标准输出（stdout）。

   代码如下：



```python
#coding:utf-8

'''
Created on 2017年6月7日 
@author: fjs
'''

#!/usr/bin/env python

from operator import itemgetter
import sys

current_word = None
current_count = 0
word = None

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parse the input we got from mapper.py
    word, count = line.split('\t', 1)

    # convert count (currently a string) to int
    try:
        count = int(count)
    except ValueError:
        # count was not a number, so silently
        # ignore/discard this line
        continue

    # this IF-switch only works because Hadoop sorts map output
    # by key (here: word) before it is passed to the reducer
    if current_word == word:
        current_count += count
    else:
        if current_word:
            # write result to STDOUT
            print '%s\t%s' % (current_word, current_count)
        current_count = count
        current_word = word

# do not forget to output the last word if needed!
if current_word == word:
    print '%s\t%s' % (current_word, current_count)
```


3、文件准备

   1)将python程序文件上传到Hadoop集群客户机，为文件赋予执行权限

   #chmod +x /data/etlcj/python/mapper.py

   #chmod +x /data/etlcj/python/reducer.py

   2)上传测试文件到集群

   #vi /data/etlcj/python/wcin.txt   加入：


`foo foo quux labs foo bar quux abc bar see you by test welcome test abc labs foo me python hadoop ab ac bc bec python`上传到集群

   #hadoop fs -put /data/etlcj/python/wcin.txt  /apps/etlcj/python/




4、基于hadoop-streaming执行MapReduce任务：


     执行语句：


`#hadoop jar /usr/hdp/2.5.3.0-37/hadoop-mapreduce/hadoop-streaming-2.7.3.2.5.3.0-37.jar -files '/data/etlcj/python/mapper.py,/data/etlcj/python/reducer.py' -input /apps/etlcj/python/wcin.txt -output /apps/etlcj/python/out/ -mapper ./mapper.py -reducer ./reducer.py` 执行过程中提示：

```
Error: java.lang.RuntimeException: PipeMapRed.waitOutputThreads(): subprocess failed with code 126
        at org.apache.hadoop.streaming.PipeMapRed.waitOutputThreads(PipeMapRed.java:322)
        at org.apache.hadoop.streaming.PipeMapRed.mapRedFinished(PipeMapRed.java:535)
        at org.apache.hadoop.streaming.PipeMapper.close(PipeMapper.java:130)
        at org.apache.hadoop.mapred.MapRunner.run(MapRunner.java:61)
        at org.apache.hadoop.streaming.PipeMapRunner.run(PipeMapRunner.java:34)
        at org.apache.hadoop.mapred.MapTask.runOldMapper(MapTask.java:453)
        at org.apache.hadoop.mapred.MapTask.run(MapTask.java:343)
        at org.apache.hadoop.mapred.YarnChild$2.run(YarnChild.java:168)
        at java.security.AccessController.doPrivileged(Native Method)
        at javax.security.auth.Subject.doAs(Subject.java:415)
        at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1724)
        at org.apache.hadoop.mapred.YarnChild.main(YarnChild.java:162)
```


怀疑是py脚本代码问题或版本环境不匹配问题，对python语法不熟悉，暂无法深入，但python提交到hadoop集群的方法可以。 



5、hadoop-streaming参数参考：

  Usage:hadoop jar $Haoop_Home$/hadoop-streaming-*.jar 

   -input <输入目录> \ # 可以指定多个输入路径，例如：-input '/user/foo/dir1' -input '/user/foo/dir2'

   -inputformat <输入格式 JavaClassName> 

   -output <输出目录> 

   -outputformat <输出格式 JavaClassName> 

   -mapper <mapper executable or JavaClassName> 

   -reducer <reducer executable or JavaClassName> 

   -combiner <combiner executable or JavaClassName> 

   -partitioner <JavaClassName> \

   -cmdenv <name=value> \ # 可以传递环境变量，可以当作参数传入到任务中，可以配置多个

   -file <依赖的文件> \ # 配置文件，字典等依赖

   -D <name=value> \ # 作业的属性配置






