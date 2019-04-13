
# 使用python实现MapReduce的wordcount实例 - lpty的博客 - CSDN博客

2017年01月14日 18:15:11[lpty](https://me.csdn.net/sinat_33741547)阅读数：4681标签：[mapreduce																](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[hadoop																](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)[wordcount																](https://so.csdn.net/so/search/s.do?q=wordcount&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[杂七杂八																](https://blog.csdn.net/sinat_33741547/article/category/7513215)
[
																								](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)

Hadopp的基本框架是用java实现的，而各类书籍基本也是以java为例实现mapreduce，但笔者日常工作都是用python，故此找了一些资料来用python实现mapreduce实例。
一、环境
1、[Hadoop-2.7.3完全分布式搭建](http://blog.csdn.net/sinat_33741547/article/details/54427113)
2、python3.5
二、基本思想介绍
使用python实现mapreduce调用的是Hadoop Stream，主要利用STDIN(标准输入)，STDOUT（标准输出）来实现在map函数和reduce函数之间的数据传递。
我们需要做的是利用python的sys.stdin读取输入数据，并把输入传递到sys.stdout，其他的工作Hadoop的流API会为我们处理。
三、实例
以下是在hadoop官网下载的python版本mapper函数和reducer函数，文件位置默认在/usr/local/working中，
1、mapper.py
(1)代码

```python
import sys  
#输入为标准输入stdin  
for line in sys.stdin:  
    #删除开头和结果的空格  
    line = line.strip( )  
    #以默认空格分隔行单词到words列表  
    words = line.split( )  
    for word in words:  
        #输出所有单词，格式为“单词，1”以便作为reduce的输入  
        print('%s\t%s' % (word,1))
```
(2)可对代码进行检验
echo "aa bb cc dd aa cc" | python mapper.py
2、reducer.py
(1)代码

```python
import sys  
  
current_word = None  
current_count = 0  
word = None  
  
#获取标准输入，即mapper.py的输出  
for line in sys.stdin:  
    line = line.strip()  
    #解析mapper.py输出作为程序的输入，以tab作为分隔符  
    word,count = line.split('\t',1)  
    #转换count从字符型成整型  
    try:  
        count = int(count)  
    except ValueError:  
        #非字符时忽略此行  
        continue  
    #要求mapper.py的输出做排序（sort）操作，以便对连续的word做判断  
    if current_word == word:  
        current_count +=count  
    else:  
        if current_word:  
            #输出当前word统计结果到标准输出  
            print('%s\t%s' % (current_word,current_count))  
        current_count =count  
        current_word =word  
  
#输出最后一个word统计  
if current_word ==word:  
    print('%s\t%s' % (current_word,current_count))
```
(2)对代码进行检验
echo "aa aa bb cc dd dd" | python mapper.py | python reducer.py
3、确保已经搭建好完全分布式的hadoop环境，在HDFS中新建文件夹
bin/hdfs dfs -mkdir /temp/
bin/hdfs dfs -mkdir /temp/hdin
4、将hadoop目录中的LICENSE.txt文件上传到HDFS中
bin/hdfs dfs -copyFromLocal LICENSE.txt /temp/hdin
5、执行文件work.sh
(1)代码

```python
#!/bin/bash  
#mapper函数和reducer函数文件地址
export CURRENT=/usr/local/working
#先删除输出目录  
$HADOOP_HOME/bin/hdfs dfs -rm -r /temp/hdout   
$HADOOP_HOME/bin/hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-2.7.3.jar \
-input "/temp/hdin/*" \
-output "/temp/hdout" \
-mapper "python mapper.py" \
-reducer "python reducer.py" \
-file "$CURRENT/mapper.py" \
-file "$CURRENT/reducer.py"
```
(2)执行代码
sh work.sh
6、查看结果
bin/hdfs dfs -cat /temp/hdout/*

```python
"AS	16
"COPYRIGHTS	1
"Contribution"	2
"Contributor"	2
"Derivative	1
"Legal	1
"License"	1
"License");	1
"Licensed	1
"Licensor"	1
"Losses")	1
"NOTICE"	1
"Not	1
...
```




