# Hadoop MapReduce入门 - wsp_1138886114的博客 - CSDN博客





2018年07月23日 19:03:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：73










- - - [一 入门简介](#一-入门简介)- - - [1.1  计算过程分为两个阶段](#11-计算过程分为两个阶段)
- [1.2 Linux常见命令：](#12-linux常见命令)



- [二 Hadoop测试代码](#二-hadoop测试代码)- - - [2.1 wordcount_mapper.py代码](#21-wordcountmapperpy代码)
- [2.2  wordcount_reducer.py 代码](#22-wordcountreducerpy-代码)
- [2.3 集成测试](#23-集成测试)









### 一 入门简介

Apache Hadoop 官网：[http://hadoop.apache.org](http://hadoop.apache.org)

企业版：[http://www.cloudera.com/content/support/en/downloads/quickstart_vms.html](http://www.cloudera.com/content/support/en/downloads/quickstart_vms.html)

虚拟机说明：用户名和密码：cloudera. 

主目录：/home/cloudera 
![这里写图片描述](https://img-blog.csdn.net/20180723184600181?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
###### 1.1  计算过程分为两个阶段

```
Map 和 Reduce
    Map 阶段并行处理输入数据
    Reduce阶段对Map结果进行汇总 

一个简单的 MapReduce 程序只需要指定 
     map()     reduce()   
     input()   output() 
     剩下的由框架完成。
```

###### 1.2 Linux常见命令：

```
- 读取文件（文本文件，在Windows下使用记事本打开的文件）
    - more 文件名称
    - less 文件名称
    - head 文件名称 
        默认是显示前10行数据
    - tail 文件名称
        默认是显示后10行数据
    - cat 文件名称 
        当且仅当，查看文件内容较少的时候
- 显示当前目录（print work derictory)
    pwd
- 创建目录
    mkdir 目录名称
    mkdir -p bigdata/ai0404 
        -p 表示创建目录的父目录不存在的话，一并创建
- 两个特殊的目录
    .. -> 表示的是 上一级目录，比如: cd ..
    .  -> 表示的是 当前目录
- 创建文件
    touch 文件名称 
- 编辑文件
    vim aa.txt 进入以后：
        i  ->  显示插入模式Insert
        Esc + : 、x   -> 保存文件内容
        Esc、:、q!  -> 退出编辑，不保存文件
- 拷贝文件
    cp 源文件 目标文件
- 删除文件
    rm -rf 文件名称/目录名称 

linux 上传下载文件软件安装：
    sudo yum install -y lrzsz  

给文件执行权限：
    chmod u+x file_name.py
```

### 二 Hadoop测试代码

###### 2.1 wordcount_mapper.py代码

```
#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

# 从标准输入读取数据
for line in sys.stdin:
    # hadoop spark hadoop spark spark 
    # 去除读取每条数据前后空格, 并按照空格分割
    for word in line.strip().split():
        print "%s\t%d" %(word, 1)
```

###### 2.2  wordcount_reducer.py 代码

```
#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

# 定义变量存储 单词 和 词频
current_word = None 
current_count = 1

for line in sys.stdin:
    # 读取mapper函数输出的结果
    word, count = line.strip().split("\t")

    # 判断当前是否存单词
    if current_word:
        if word == current_word:
            current_count += int(count)
        else:
            print "%s\t%d" %(current_word, current_count)
            current_count = 1
    # 赋值当前单词
    current_word = word

# 处理读物最后一行数据
if current_count >= 1:
    print "%s\t%d" %(current_word, current_count)
```

###### 2.3 集成测试

```
hadoop jar \
/usr/lib/hadoop-mapreduce/hadoop-streaming-2.6.0-cdh5.12.0.jar \      
-files /home/cloudera/word_count/wordcount_mapper.py,\    # wordcount_mapper.py文件目录位置
/home/cloudera/word_count/wordcount_reducer.py \          # wordcount_reducer.py文件目录位置
-mapper "python wordcount_mapper.py" \
-reducer "python wordcount_reducer.py" \
-input /user/cloudera/wordcount/input.wc \                # Hadoop测试文件输入目录
-output /user/cloudera/wordcount/output                   # Hadoop测试文件输出目录
```




