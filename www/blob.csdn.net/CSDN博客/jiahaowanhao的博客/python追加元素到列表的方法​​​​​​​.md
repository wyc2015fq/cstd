
# python追加元素到列表的方法​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月20日 21:01:47[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：85


**[python追加元素到列表的方法](http://cda.pinggu.org/view/26422.html)**
这篇文章主要介绍了python追加元素到列表的方法,涉及Python列表操作中append方法追加元素的使用技巧,具有一定参考借鉴价值,需要的朋友可以参考下
scores = ["1","2","3"]
\# add a score
score = int(raw_input("What score did you get?: "))
scores.append(score)
\# list high-score table
for score in scores:
print score
运行结果如下：
What score did you get?: 5
1
2
3
5
希望本文所述对大家的Python程序设计有所帮助。

