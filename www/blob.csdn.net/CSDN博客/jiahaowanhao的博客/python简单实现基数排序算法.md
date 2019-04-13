
# python简单实现基数排序算法 - jiahaowanhao的博客 - CSDN博客


2018年03月29日 20:45:47[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：259


[python简单实现基数排序算法](http://cda.pinggu.org/view/25128.html)
这篇文章主要介绍了python简单实现基数排序算法,仅用4行代码即可实现基数排序算法,非常简单实用,分享给大家供大家参考。
具体实现方法如下：
from random import randint
def main():
A = [randint(1, 99999999) for _ in xrange(9999)]
for k in xrange(8):
S = [ [] for _ in xrange(10)]
for j in A:
S[j / (10 ** k) % 10].append(j)
A = [a for b in S for a in b]
for i in A:
print i
main()
希望本文所述对大家的Python程序设计有所帮助。

