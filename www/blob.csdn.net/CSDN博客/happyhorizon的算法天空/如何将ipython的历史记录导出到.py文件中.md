# 如何将ipython的历史记录导出到.py文件中? - happyhorizon的算法天空 - CSDN博客
2017年10月16日 10:32:32[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：4627
python绝对是生产力工具,真的太好用了!!! 
python jupyter提供了非常好的交互编程方式, 最棒的就是在数据分析过程中,可以把想法和代码实现放在一起,大大加速了分析过程,也使得代码的可读性更好.  
回到上面的问题, 两种办法解决： 
1. 用%hist保存后把%开头的删掉再执行。 
`Input[1]:%hist -f filename.py`
2. 用%logstart和%logstop。它会把你所用的%命令对应的的python代码（如下面的magic…）。
```python
In [7]: %logstart /tmp/test_log.py
Activating auto-logging. Current session state plus future input saved.
Filename       : /tmp/test_log.py
Mode           : backup
Output logging : False
Raw input log  : False
Timestamping   : False
State          : active
 In [8]: a = 10
 In [9]: b = a*a
 In [10]: %who
a        b       
 In [11]: %logstop
 In [12]: !cat /tmp/test_log.py
# IPython log file
357x46
357*46
54*32
53*42
52*43
532*4
get_ipython().magic(u'logstart /tmp/test_log.py')
a = 10
b = a*a
get_ipython().magic(u'who')
get_ipython().magic(u'logstop')
```
