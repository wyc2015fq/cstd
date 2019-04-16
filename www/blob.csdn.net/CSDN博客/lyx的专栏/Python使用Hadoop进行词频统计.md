# Python使用Hadoop进行词频统计 - lyx的专栏 - CSDN博客





2016年12月25日 20:05:06[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2279








今天，我们利用python编写一个MapReduce程序，程序的目的还是百年不变的计算单词个数，也就是WordCunt。

所谓mapreduce其实就是先分散计算后综合处理计算结果。

首先我们来看一下map部分的代码。



```python
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
mapper其实只做了一个功能，就是读取每行文字，然后分割成一个一个的单词
reduce部分代码：



```python
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
这里，其实是对上面的map步骤的输出做处理。

然后，我们把一本英语小说放到HDFS上，也就是bin/hdfs dfs下的命令。

接下来，调用如下命令：

bin/hadoop jar share/hadoop/tools/lib/hadoop-streaming-2.2.0.jar -files ./mapper.py,./reducer.py -mapper ./mapper.py -reducer ./reducer.py -input /book/test.txt -output book-out


也就是说，我们是用streaming来实现python编写和运行mapreduce的。这里-input 后是hdfs中我们放置文件的路径，也就是英语小说的路径，-output 则是输出结果的路径。

bin/hdfs dfs -cat /user/hadoops2/book-out/part-00000 | sort -nk 2 | tail


最后，我们可以用这个命令查看运行结果的尾部几条。







