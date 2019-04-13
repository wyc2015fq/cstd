
# Python for循环生成列表的实例​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年07月30日 21:08:55[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：177


**[Python for循环生成列表的实例](http://cda.pinggu.org/view/26237.html)**
一般Python for语句前不加语句，但我在机器学习实战中看到了这两条语句：
featList = [example[i] for example in dataSet]
classList = [example[-1] for example in dataSet]
多方研究和询问，得到如下解释：
语句featList = [example[i] for example in dataSet]作用为：
将dataSet中的数据按行依次放入example中，然后取得example中的example[i]元素，放入列表featList中
语句classList = [example[-1] for example in dataSet]作用为：
将dataSet中的数据按行依次放入example中，然后取得example中的example[-1]元素，放入列表classList中
总而言之，类似上述两种for循环形式可以很方便地用来创建列表，如下例：
list_0 = [x*x for x in range(5)]
print(list_0)
\#输出：
\#[0, 1, 4, 9, 16]
以上这篇Python for循环生成列表的实例就是小编分享给大家的全部内容了

