
# Python反转序列的方法实例分析 - jiahaowanhao的博客 - CSDN博客


2018年06月22日 13:55:50[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：48


[Python反转序列的方法实例分析](http://cda.pinggu.org/view/25894.html)
本文实例讲述了Python反转序列的方法。分享给大家供大家参考，具体如下：
序列是python中最基本的数据结构，序列中每个元素都有一个跟位置相关的序号，也称为索引。对于一个有N个元素的序列来说，
从左到右索引：0，1,2，……N-1
从右到左索引：-1，-2，-3……-N
1》列表反转
>>> l=[1,2,3,4]
>>> ll=l[::-1]
>>> l
[1, 2, 3, 4]
>>> ll
[4, 3, 2, 1]
>>> l=[4,5,6,7]
>>> ll=reversed(l)
>>> l
[4, 5, 6, 7]
>>> ll
<listreverseiterator object at 0x06A07F70>
>>> list(ll)
[7, 6, 5, 4]
2》元组反转
>>> t=(2,3,4,5)
>>> tt=t[::-1]
>>> t
(2, 3, 4, 5)
>>> tt
(5, 4, 3, 2)
>>> t=(4,5,6,7)
>>> tt=reversed(t)
>>> t
(4, 5, 6, 7)
>>> tt
<reversed object at 0x06A07E90>
>>> tuple(tt)
(7, 6, 5, 4)
3》反转字符串
>>> s='python'
>>> ss=s[::-1]
>>> s
'python'
>>> ss
'nohtyp'
>>> s='nohtyp'
>>> ss=''.join(reversed(s))
>>> s
'nohtyp'
>>> ss
'python'

