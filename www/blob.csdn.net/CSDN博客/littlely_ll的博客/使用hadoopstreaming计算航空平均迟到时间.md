# 使用hadoopstreaming计算航空平均迟到时间 - littlely_ll的博客 - CSDN博客





2017年05月28日 18:03:16[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：382








下属内容为本人的学习笔记：

```python
#!/usr/bin/env python
import sys
import csv
SEP = "\t"

class Mapper(object):
    def __init__(self, stream, sep=SEP):
        self.stream = stream
        self.sep = sep

    def emit(self, key, value):
        sys.stdout.write("{}{}{}\n".format(key,self.sep, value))

    def map(self):
        for row in self:
            self.emit(row[3], row[6])

    def __iter__(self):
        reader = csv.reader(self.stream)
        for row in reader:
            yield row

if __name__ == "__main__":
    mapper = Mapper(sys.stdin)
    mapper.map()
```

```python
#!/usr/bin/env python
import sys

from itertools import groupby
from operator import itemgetter

SEP = '\t'

class Reducer(object):
    def __init__(self, stream, sep=SEP):
        self.stream = stream
        self.sep = sep

    def emit(self, key, value):
        sys.stdout.write("{}{}{}\n".format(key,self.sep,value))

    def reduce(self):
        for current, group in groupby(self, itemgetter(0)):
            total = 0
            count = 0
            for item in group:
                total += item[1]
                count += 1
            self.emit(current, float(total) / float(count))

    def __iter__(self):
        for line in self.stream:
            try:
                parts = line.split(self.sep)
                yield parts[0], float(parts[1])
            except:
                continue

if __name__ == "__main__":
    reducer = Reducer(sys.stdin)
    reducer.reduce()
```

运行代码时需注意，在每个mapper和reducer最上面应该加有`#!/usr/bin/env python`，这样，即使Python没有安装到默认路径中也能运行，经过测试，除了这样，还可以指定路径，比如我的Python在Linux中的安装路径为`.../usr/local/bin/python`，所以可以设置为`#!/usr/local/bin/python`，不过这样的话，可移植性就比较差了。

运行代码中，另一个问题就是在windows的pycharm编辑好代码后，如果直接运行的话，会出现`-bash: ./reducer.py: /usr/local/bin/python^M: bad interpreter: No such file or directory`，经过网上搜索，出现这样的原因是不同系统的编码格式引起的。所以，为了解决这个问题，可以在Linux中修改文件的格式。比如对mapper.py的修改，可以使用：

```
vim mapper.py
:set fileformat=unix
```

这样就修改了mapper.py的编码格式，对于reducer.py同理。 

最后可以在hadoop中直接运行了：

```
hadoop jar $HADOOP_HOME/share/hadoop/tools/lib/hadoop-streaming-*.jar \
 -input /test/shakespeare.txt\ 
  -output /test/count \
   -mapper mapper.py \
    -reducer reducer.py \
    -file mapper.py \
    -file reducer.py
```

其实，也可以在Linux中直接运行代码：
`cat flights.csv | ./mapper.py | sort | ./reducer.py`
运行结果和hadoop运行结果一样。下面是运行的部分结果： 
![这里写图片描述](https://img-blog.csdn.net/20170528180202759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



