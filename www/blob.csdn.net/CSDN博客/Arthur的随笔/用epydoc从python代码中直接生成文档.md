# 用epydoc从python代码中直接生成文档 - Arthur的随笔 - CSDN博客
2011年11月08日 23:17:55[largetalk](https://me.csdn.net/largetalk)阅读数：7218
在一个大的项目里，或在接手别人项目时才知道文档是多么重要，以后写代码时也要多写写文档，实在没有文档，注释也要多写写。今天晚上本来是想看看pynotify的，没文档，需要自己生成，这一下就让我想到，如果我也这么干，以后写注释就是写文档了。他得文档生成工具是epydoc， 用了一下还是很简单，非常不错，学习之。
先要安装epydoc和graphviz ，arch的源上都有，相信ubuntu也肯定有。
然后代码里加上注释， 我写了个demo，文件名是tepydoc.py
```python
#!/usr/bin/env python
#coding: utf-8
'''
@author: Arthur
@license: ***
@contact: ****@*****
@see: http://wenku.baidu.com/view/1d2ac17fa26925c52cc5bf92.html
@version: 0.0.1
@todo[0.0.2]: a new story
@note: a comment
@attention: please attention
@bug: a exist bug
@warning: warnings
'''
import sys
#@var lst: a list variable
lst = [0,1,2,3]
#@type vt: a new type
vt = int
class ottool:
    '''
    Ottool 主类
      -组织其它小工具，完成任务....
    @todo: we want do is ...
    '''
    def __init__(self):
        '''
        初始化函数, 调研其他模块
          -函数很简单，；)
        '''
        self.lst = lst
```
再写个Makefile:
```
EPYDOC=epydoc
DSTDOC=docstrings
doc: clean-doc
	$(EPYDOC) --html --graph=all -v -o $(DSTDOC) tepydoc.py
clean-doc:
	rm -rf $(DSTDOC)
clean: clean-doc
	find . \( -name '*~' -or \
	     -name '*.pyc' -or \
	     -name '*.pyo' \) \
	     -print -exec rm {} \;
```
make 一下就可以了，去当前目录docstrings目录下就可以看到生成好的html了。
源码有多个文件也是一样得，只有把代码文件名加上去就可以了， 如：
```
$(EPYDOC) --html --graph=all -v -o $(DSTDOC) tepydoc.py tepydoc2.py \
		ttepy/tepydoc3.py
```
有些细节可以参考这里： [http://wenku.baidu.com/view/1d2ac17fa26925c52cc5bf92.html](http://wenku.baidu.com/view/1d2ac17fa26925c52cc5bf92.html)
epydoc对于一个可以自说明的项目是够的，可以让人很容易了解模块有什么类，有什么函数，大概用处是什么，对于更大或者更复杂得项目就可能无能为力了，django用的是sphinx-build, 用空再看看
