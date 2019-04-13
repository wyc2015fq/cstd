
# [python数据分析] 简述幂率定律及绘制Power-law函数 - 杨秀璋的专栏 - CSDN博客

2017年03月23日 23:57:57[Eastmount](https://me.csdn.net/Eastmount)阅读数：7707所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



这篇文章主要是最近研究人类行为应用的内容，主要简单叙述下复杂网络的幂率分布以及绘制Power-law函数一些知识，同时是一篇在线笔记。希望对您有所帮助，如果文章中存在不足或错误的地方，还请海涵~
1.幂率分布
2.Zipf定律
3.Scale free
4.Python绘制幂率分布
这篇文章基础知识转载了别人的内容，这里强烈推荐大家阅读原文。此篇文章主要是讲述代码部分，定律我也还在学习中，和大家共勉。

## 一. 幂率分布
在我们的日常生活中Power Law（幂次分布，Power-law Distributions）是常见的一个数学模型，如二八原则。这个世界上是20%的人掌握80%的人的金钱去经营，20％的人口拥有80％的财富，20％的上市公司创造了80％的价值，80％的收入来自20％的商品，80％的利润来自20％的顾客等等。
下图表示人类的财富幂率分布图，极少数人拥有微弱优势的人却拥有天文级别的财富。
![](https://img-blog.csdn.net/20170323221554784)
为什么会有这样的差别呢？
这是因为时间的乘积效应，智力上的微弱优势，乘以时间，就会得到价值（财富）几何级的增长。
经济学财富分布满足Pareto Power law tail分布，语言中有词频的幂律分布，城市规模和数量满足幂律分布，音乐中有f分之1噪音（幂律分布）。通常人们理解幂律分布就是所谓的马太效应，二八原则，即少数人聚集了大量的财富，而大多数人的财富数量都很小，因为胜者通吃的原则。
f(x)表示某一数量指标x的发生次数，即幂率分布公式。

![](https://img-blog.csdn.net/20170323214653066)

幂率公式推导参考kevinelstri大神的博客，如下图所示：

![](https://img-blog.csdn.net/20170323224751986)
再如钱学森通信反应间隔分布：
![](https://img-blog.csdn.net/20170323230410775)
![](https://img-blog.csdn.net/20170323230516233)

参考资料：
[关于幂律分布的一个笔记 - 哈克](http://blog.sina.com.cn/s/blog_55954cfb0100ps89.html)
[复杂网络的一些相关概念:power law(幂率分布)以及 scale free（无标度） - 刘永轩](http://blog.sina.com.cn/s/blog_a5527bf301016qi1.html)
[Python数据可视化：幂律分布 - kevinelstri](http://blog.csdn.net/kevinelstri/article/details/52685934)

## 二. Zipf定律
PS：这部分内容引用哈克老师的博客 "[关于幂律分布的一个笔记 ](http://blog.sina.com.cn/s/blog_55954cfb0100ps89.html)"，推荐给大家。
Zipf是哈佛大学的语言学家，他在1932年研究英文单词的出现频率时，发现如果把单词频率从高到低排列，每个单词的出现频率和它的排名之间存在简单的反比关系：

![](https://img-blog.csdn.net/20170323232224878)
在对其取对数变换后成为：

![](https://img-blog.csdn.net/20170323232341228)
则在双对数坐标系下，该分布呈现为一条斜率为负幂指数的直线。这里r表示一个单词出现频率的排名，P(r)表示排名为r的单词的出现频率。在单词分布中，C约等于0.1，α约等于1。
总结规律：**只有少数英文单词才会被经常使用，大部分的单词很少被使用。**
这个规律在其他领域的数据分布里也得到了验证。另一个类似的定律是意大利经济学家Pareto提出的80/20法则，即20%的人口占据了80%的社会财富。个人收入X不小于某个值x的概率与x的常数次幂存在简单的反比关系：
![](https://img-blog.csdn.net/20170323232803467)
称为Pareto定律。

## 三. Scale free
PS：这部分内容引用哈克老师的博客 "[关于幂律分布的一个笔记 ](http://blog.sina.com.cn/s/blog_55954cfb0100ps89.html)"，推荐给大家。
一个网络的度分布如果服从power law，即有P(k)∼k−γ，（直观的看，就是少部分节点度极大，与许多节点相连，而大部分节点度都比较小），那么，这个网络就叫做无标度网络（scale free network）。
许多现实中的网络包括WWW，社交网络，性伴侣网络，PPI网络等都被认为是无标度网络，并且大部分实际网络中power law的指数γ一般都在2和3之间，并且网络直径极小(d~lnN,N为节点数)。
对应的，随机网络的度分布是泊松分布，也就是钟形分布的，节点的度值一般不会比平均值高出很多或低很多。
复杂网络的定义：具有自组织，自相似，吸引子，小世界，无标度中部分或全部性质的网络称为复杂网络。
现实网络的无标度特性源于众多网络所共有的两种生成机制（Albert-Laszlo Barabasi ,Reka Albert，1999）:
(i) 网络通过增添新节点而连续扩张
(ii) 新节点择优连接到具有大量连接的节点上，第二点即新节点连接到已有节点的概率与该节点的度数成正比例如：
![](https://img-blog.csdn.net/20170323233407555)

这样就可以生成度分布服从power law的网络。
关于复杂网络的统计学机制可以参考Reka Albert的博士论文:
[Statistic Mechanics of complex networks](https://wenku.baidu.com/view/ba15df1b10a6f524cdbf8506.html)

## 四. Python绘制幂率分布
推荐文章：[Python数据可视化：幂律分布 - kevinelstri](http://blog.csdn.net/kevinelstri/article/details/52685934)
[【python数据挖掘课程】十二.Pandas结合SQL语句对比图分析](http://blog.csdn.net/eastmount/article/details/64127445)
下面是我前面文章关于博客数量的绘制图形，X轴为发表博客年份，Y轴为博客数量。如下图所示：

![](https://img-blog.csdn.net/20170323233658916)
下图X轴表示发表博客的评论数，Y轴表示博客那个评论的总文章数量。

![](https://img-blog.csdn.net/20170323234201094)
然后分别对X、Y坐标取log10对数，如下图所示，一个完美的幂率分布图。

![](https://img-blog.csdn.net/20170323234330575)

具体代码如下所示：（因文章未发表，占不公开数据，抱歉）

```python
# coding=utf-8
'''
' 这篇代码主要讲述获取MySQL中数据，再进行简单的统计
' 统计采用SQL语句进行 By：Eastmount CSDN 杨秀璋
'''
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd
import numpy as np
import MySQLdb
from pandas import *
import math
# 根据SQL语句输出24小时的柱状图
try:
    conn = MySQLdb.connect(host='localhost',user='root',
                         passwd='123456',port=3306, db='test01')
    cur = conn.cursor() #数据库游标
    #防止报错:UnicodeEncodeError: 'latin-1' codec can't encode character
    conn.set_character_set('utf8')
    cur.execute('SET NAMES utf8;')
    cur.execute('SET CHARACTER SET utf8;')
    cur.execute('SET character_set_connection=utf8;')
    #sql = '''select DATE_FORMAT(FBTime,'%Y'), Count(*) from csdn_blog group by DATE_FORMAT(FBTime,'%Y');'''
    sql = '''select PLNum, count(*) from csdn_blog group by  PLNum;'''
    cur.execute(sql)
    result = cur.fetchall()        
    PLNum = [n[0] for n in result] #评论数
    Num = [n[1] for n in result]   #数量
    print len(PLNum), type(Num)
    #两边起对数log10
    PLNumlog = range(len(PLNum))
    Numlog = range(len(PLNum))
    i = 1
    while i < len(PLNum):
        print PLNum[i], Num[i]
        print math.log10(PLNum[i]), math.log10(Num[i])
        PLNumlog[i] = math.log10(PLNum[i])
        Numlog[i] = math.log10(Num[i])
        i = i + 1
    else:
        print 'end log10'
    #print PLNumlog
    #print Numlog
    
    matplotlib.style.use('ggplot')
    #df=DataFrame(Num[1:100], index=PLNum[1:100],columns=['Nums'])
    
    plt.scatter(PLNumlog[1:], Numlog[1:], marker='s')
    plt.title('Number of users published')
    plt.xlabel('The number of comment')
    plt.ylabel('The number of blog')
    plt.savefig('06csdn.png',dpi=400)
    plt.show()

#异常处理
except MySQLdb.Error,e:
    print "Mysql Error %d: %s" % (e.args[0], e.args[1])
finally:
    cur.close()
    conn.commit()  
    conn.close()
```
国足今天真的不负英雄之名。
第一次听到给韩国的时间不多了，爽！
第一次因为国足把虎扑服务器搞崩了，哈哈。
第一次见国足赢了还秀恩爱的，对不起大家，我激动啦，别屏蔽我。
晚上躲在办公室看球，代码也不写了，学生敲门也不开了，假老师一个。
感谢东方神秘的力量，奶死韩国，6666。
![](https://img-blog.csdn.net/20170323235253769)

同时今天给学生讲了一节数据挖掘课，站在讲台前还是那么享受，回来教书真的不亏，而且一节课就赢得学生的好感，多为学生考虑真的挺好，为了学生的一切。终于12点前在办公室写完这篇博客，回寝睡觉了，还有好多事情等着我啦，加油，秀璋，最感谢的是娜娜的一路同行，一路理解。
娜娜值得我去爱，怎能负了她的选择。
她的气质是藏在她的经历和读过的书中。
(By:Eastmount 2017-03-23 晚上12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



