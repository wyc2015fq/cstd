# Scrapy之日志文件的产生 - lyx的专栏 - CSDN博客





2016年06月23日 22:36:32[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：8559








我们写爬虫的时候，也许会需要记录一些数据或者异常信息，通常这些数据我们会希望以日志的形式保存下来。

首先，我们在需要产生日志的python文件开头引入一个logging包。



```python
import logging
```
然后，我们可以用





```python
self.log("your log information")
```
这样的方式产生你的log文件。



当然，我们还需要一些设置，就是修改一些我们的setting文件。我们需要在setting文件里面加入LOG_FILE  =“file_name”这样一个变量。

爬虫运行后，就会生成这样一个文件，专门用来记录self.log函数产生的log信息了。




