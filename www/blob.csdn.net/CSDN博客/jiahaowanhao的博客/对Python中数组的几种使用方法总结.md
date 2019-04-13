
# 对Python中数组的几种使用方法总结 - jiahaowanhao的博客 - CSDN博客


2018年07月04日 21:40:36[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：187


[对Python中数组的几种使用方法总结](http://cda.pinggu.org/view/25999.html)
今天小编就为大家分享一篇对Python中数组的几种使用方法总结，具有很好的参考价值，希望对大家有所帮助。一起跟随小编过来看看吧
二维数组的初始化
matirx_done = [[0 for i in range(0, len(matirx))]for j in range(0, len(matirx[0]))]
就将其初始化为一个与matrix相同大小的元素全为 0 的矩阵
数组的多级排序
在数组 idea_collect = [[3, 1, 2], [3, 2, 1], [3, 2, 2], [3, 1, 1]] 中, 先按照第二项排列, 再按照第三项倒序排列 可写为:
idea_collect.sort(key=lambda x: (x[1], -x[2]))
其中, x[1] 代表第二项正序排列, -x[2] 代表第三项倒序排列
排列结果为 [[3, 1, 2], [3, 1, 1], [3, 2, 2], [3, 2, 1]]
在一个 class 中多个函数不传参使用同一个数组
如例所示:
class Partition:
def __init__(self):
self.num_complete = []
def partition(self, num, start, end):
self.num_compelete = num
def partition_core(self):
del self.num_compelete[0]
其中,self.num_compelete就是 class 中两个函数同时可以直接调用的数组, 不过最好先在def __init__中声明这个数组
以上这篇对Python中数组的几种使用方法总结就是小编分享给大家的全部内容了

