# 【实现&调用】实现可被简单调用的python函数 - CD's Coding - CSDN博客





2016年10月25日 19:34:05[糖果天王](https://me.csdn.net/okcd00)阅读数：833








0x00 前言

本篇破事水

@ZoeCUR 在学习WEB安全课程上，有一个对于用户密码的数据分析处理问题，懒惰的孩纸们表示数据处理还是挺麻烦的，吾辈夸下海口10分钟之内搞定

然后就有了这段主体部分25行的破事水——

需求：读入文件，去重；查询重复条目数；有的文件包含序号前缀，需要去掉；文件多为百兆或GB级别，复杂度要较低。




0x01 实现



```python
#coding=utf8
# ========================================================
#   Copyright (C) 2016 All rights reserved.
#   
#   filename : CDUnique.py
#   author   : okcd00 / chendian@baidu.com
#   date     : 2016-10-25
#   desc     : Unique and output custom attributes
# ======================================================== 
"""
    usage:
    from CDUnique import *
    ret = CDUnique( <path>, <mode>, <repeat> )
    ret = CDUnique( path, [all/no-head], [False, True],)
"""
    
import os,sys

def uniqueList(inlist): # Unstable for MarkLR, 'inlist' is stableList
    dict = {}
    for each in inlist:
        dict.setdefault(each,0)
        dict[each] += 1
    #return list(set(inlist))
    #outlist.sort(key=lambda x: len(x))
    return dict.keys()

def CDUnique(path, mode='all', repeat=False):
    # Check Existance
    if not os.path.exists(path):
        print "ReadFile ERROR: File not found"
        return None
    
    # Get InputList
    if mode == 'no-head': inpList = [':'.join(l.strip().split(':')[1:]) for l in file(path)]
    else: inpList = [line.strip() for line in file(path)]
    
    # Output Attribute
    if repeat: 
        uniqList = list(set(inpList))
        return inpList.__len__() - uniqList.__len__()
    else: 
        uniqList = uniqueList(inpList)
        return uniqList
```




0x02 调用方法

```python
from CDUnique import *

ret = CDUnique('in.txt', 'no-head', False)
print ret
#for e in ret: print e
```




0x03 要点？

这段代码其实几乎没用到任何高大上的东西，主要是想体现一个熟练工（伪）的速敲情怀。

一般来说如果需要进行功能实现的话，要求高点的话封装成类，要求低点的话就做成一个单文件可以被import直接调用，无论是在大型的工程项目还是简单的课堂作业上，都是一个用者省心，写者易改的感觉。









