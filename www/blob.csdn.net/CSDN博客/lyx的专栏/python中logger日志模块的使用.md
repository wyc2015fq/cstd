# python中logger日志模块的使用 - lyx的专栏 - CSDN博客





2017年07月24日 20:39:11[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3874
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









        一般，我们做一些简单的状态输出都会用print，但是这是最简单的情况下使用的工具。当我们的程序比较复杂的时候，我们会使用日志文件，特别是程序运行的时间特别久，中间可能存在一些问题，需要后面来看的时候。

        所以，python自带了一个很有用的库，logger，也就是日志记录。

        使用起来还是很方便的。



```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
import logging
# create a log file
logger = logging.getLogger('atp_log')
logger.setLevel(logging.DEBUG)
# create a handler, write the log info into it
fh = logging.FileHandler('atp.log')
fh.setLevel(logging.DEBUG)
# create another handler output the log though console
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
# 定义handler的输出格式
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)
# 给logger添加handler
logger.addHandler(fh)
logger.addHandler(ch)
# 记录一条日志
logger.info('foorbar')
logger.error('foorbar')
```
        之后，我们队logger输入info warning或者error都可以，而且会被记录在日志文件里面。当然，上面的代码中，我们在设置
```python
fh = logging.FileHandler('atp.log')
```

        这个文件的logger也创建了一个从console的日志显示的地方。

```python

```
ch = logging.StreamHandler()


        之后，我们就可以看到在文件和console中都会有相应的信息出现。

![](https://img-blog.csdn.net/20170724203828576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




