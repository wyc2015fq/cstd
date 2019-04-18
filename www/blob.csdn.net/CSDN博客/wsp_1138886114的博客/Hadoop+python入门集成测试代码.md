# Hadoop+python入门集成测试代码 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:24:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：97标签：[Hadoop入门数据测试](https://so.csdn.net/so/search/s.do?q=Hadoop入门数据测试&t=blog)
个人分类：[大数据](https://blog.csdn.net/wsp_1138886114/article/category/7828918)












### 文章目录
- [一 词频统计WordCount（类似TF）](#_WordCountTF_1)
- [1.1、准备数据](#11_5)
- [1.2、使用Python编码，实现wordcount统计](#12Pythonwordcount_27)
- [1.3、运行Python编写的WordCountt在YARN](#13PythonWordCounttYARN_47)
- [二、IBM股票价格数据：](#IBM_69)
- [2.1开发程序：](#21_84)




### 一 词频统计WordCount（类似TF）

属于大数据框架中 最经典的案例：

统计文件中每个单词出现的个数

##### 1.1、准备数据

```python
```python
# 创建目录
$ hdfs dfs -mkdir -p /user/cloudera/wordcount/input 

#安装上传模块
$ sudo yum install -y lrzsz 

#创建文件，并编辑
$ touch wordcount_mapper.py  
$ touch wordcount_reducer.py
$ vim wordcount_mapper.py
$ vim wordcount_reducer.py

#上传测试模块（文件）
rz  wordcount_mapper.py
rz  wordcount_reducer.py 

# 上传数据文件
$ hdfs dfs -put /home/cloudera/bigdata/wc.data /user/cloudera/wordcount/input
```
```

##### 1.2、使用Python编码，实现wordcount统计

```python
```python
- input 
        要读取处理分析数据的路径 
        默认情况下，一行一行的读取文件中的数据 
    - mapper
        函数 ，进行分析处理
    - reducer
        函数，合并map函数输出的结果 
    
    当使用Python开发完成mapper.py和reduce.py脚本以后，在linux系统上要执行的话，需要给予执行权限，命令如何：
    $ chmod u+x wordcount_mapper.py
    $ chmod u+x wordcount_reducer.py

    本地测试：
    - wordcount_mapper.py
        $ echo "hadoop mapreduce mapreduce python" | python wordcount_mapper.py
    - wordcount_reducer.py
        $ echo "hadoop mapreduce mapreduce python" | python  wordcount_mapper.py | python wordcount_reducer.py
```
```

##### 1.3、运行Python编写的WordCountt在YARN

```python
```python
hadoop jar \
/usr/lib/hadoop-mapreduce/hadoop-streaming-2.6.0-cdh5.12.0.jar \
-files /home/cloudera/word_count/wordcount_mapper.py,\
/home/cloudera/word_count/wordcount_reducer.py \
-mapper "python wordcount_mapper.py" \
-reducer "python wordcount_reducer.py" \
-input /user/cloudera/wordcount/input.wc \
-output /user/cloudera/wordcount/output
----------------------------------------------------------------
    -a. 第一点：
        提交运行Hadoop 中MapReduce运行在YARN上
        hadoop jar / yarn jar 
    -b. 第二点：
        -files 参数 将Python编写脚本文件上传到集群上，以便集群中各个集群下载使用
        要求集群中各个机器上必须按照同一版本、同一目录的Python
    -c. 第三点：
        指定 input、output、mapper和reducer各个参数的值
```
```

![这里写图片描述](https://img-blog.csdn.net/20180724093232249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 二、IBM股票价格数据：

历史股票价格数据：[https://finance.yahoo.com/quote/IBM/history?p=IBM](https://finance.yahoo.com/quote/IBM/history?p=IBM)

```python
```python
1962-01-02,7.713333,7.713333,7.626667,7.626667,0.689273,387200

    日期，开盘价，最高价，最低价，收盘价，调整的收盘价和交易量
    Date,Open,High,Low,Close,Adj Close,Volume

	统计每日变化百分比总数
	    每日变化百分比 = （开盘价-收盘价）/ 开盘价
	
	1. 统计每日变化百分比  
	    0.23%   0.24%  -0.13%
```
```

##### 2.1开发程序：

```python
```python
-a. 创建目录stock，拷贝wordcount_mapper.py和wordcount_reducer.py文件至stock目录
    重命名文件：
    $ mv wordcount_mapper.py stock_mapper.py
    $ mv wordcount_reducer.py stock_reducer.py
-b. 开发代码
-c. 本地测试：
    $ more stock-ibm.csv | python stock_mapper.py | sort -k1| python stock_reducer.py > stock-ibm.output
-d. 集群测试
    读取HDFS上的数据，将程序提交运行在YARN上。 

------------------------------------------------------------------------------
#/user/bin/python
#encoding:utf-8

import sys

for line in  sys.stdin:
	row = line.split(","):
	open_price = float(row[1])
	close_price = float(row[-3])
	change = (open_price-close_price)/open_price*100
	change_text = str(round(change,1))+"%"
		print "%s\t%d" % (change_text,1)
```
```

```python
```python
#!/user/bin/python
# encoding:utf-8

import sys
current_word = None
current_count = 1
	for line in sys.stdin:
		word,count = line.strip().split('\t')
		#判断当前是否存在单词
		if current_word:
			if word == current_word:
				current_count += int(count)
			else:
				print "%s\t%d"%(current_word,current_count)
				current_count =1
				
		current_word = word    #赋值当前单词
	
	if current_count >=1:    #处理读出最后一行数据
		print "%s\t%d"%(current_word,current_count)
```
```



