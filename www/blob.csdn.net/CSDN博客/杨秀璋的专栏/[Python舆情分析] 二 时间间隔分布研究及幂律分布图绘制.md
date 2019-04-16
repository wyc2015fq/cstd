# [Python舆情分析] 二.时间间隔分布研究及幂律分布图绘制 - 杨秀璋的专栏 - CSDN博客





2019年01月24日 19:19:46[Eastmount](https://me.csdn.net/Eastmount)阅读数：415标签：[舆情分析																[Python																[幂律分布																[时间间隔分析																[power-low](https://so.csdn.net/so/search/s.do?q=power-low&t=blog)
个人分类：[Python舆情分析																[Python数据挖掘课程																[数据挖掘																[机器学习](https://blog.csdn.net/Eastmount/article/category/2812423)
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)](https://blog.csdn.net/Eastmount/article/category/2613003)




本文主要是作者学习舆情分析、情感分析、人类行为动力学分析的在线笔记，主要包括两方面内容，一是幂律特性，二是讲解时间间隔分布，三是Python绘制基于时间间隔分布的幂律特性图，四提供了另一种方法。基础性文章，希望对您有所帮助。




### 文章目录
- [一.幂律分布](#_16)
- [二.舆情分析基础知识](#_50)
- [三.Python时间间隔分布图绘制](#Python_74)
- [四.SQL语句获取时间间隔](#SQL_205)




参考前文：
[[Python舆情分析] 一.舆情事件的幂律特性分析及时间间隔分布图绘制](https://blog.csdn.net/Eastmount/article/details/86593092)
[[python数据分析] 简述幂率定律及绘制Power-law函数](https://blog.csdn.net/Eastmount/article/details/65443025)
PS：最近参加CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 一.幂律分布

在我们的日常生活中Power Law（幂次分布，Power-law Distributions）是常见的一个数学模型，如二八原则。这个世界上是20%的人掌握80%的人的金钱去经营，20％的人口拥有80％的财富，20％的上市公司创造了80％的价值，80％的收入来自20％的商品，80％的利润来自20％的顾客等等。

下图表示人类的财富幂率分布图，极少数人拥有微弱优势的人却拥有天文级别的财富。
![](https://img-blog.csdnimg.cn/20190124181817858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
为什么会有这样的差别呢？

这是因为时间的乘积效应，智力上的微弱优势，乘以时间，就会得到价值（财富）几何级的增长。经济学财富分布满足Pareto Power law tail分布，语言中有词频的幂律分布，城市规模和数量满足幂律分布，音乐中有f分之1噪音（幂律分布）。通常人们理解幂律分布就是所谓的马太效应，二八原则，即少数人聚集了大量的财富，而大多数人的财富数量都很小，因为胜者通吃的原则。

f(x)表示某一数量指标x的发生次数，即幂率分布公式。
![](https://img-blog.csdnimg.cn/2019012418195528.png)
幂率公式推导参考kevinelstri大神的 [博客](https://blog.csdn.net/kevinelstri/article/details/52685934)，如下图所示：
![](https://img-blog.csdnimg.cn/20190124182019212.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
再如钱学森通信反应间隔分布：
![](https://img-blog.csdnimg.cn/20190124182041352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二.舆情分析基础知识

该部分主要借鉴引用如下论文，强烈推荐读者学习，尤其是研究舆情分析、图书情报、人类行为动力学的同学。
- 闫小勇. 人类个体出行行为的统计特征. 电子科技大学学报, 2011.
- 刘海鸥, 等. 在线社交活动中的用户画像及其信息传播行为研究. 情报科学, 2018.
- 梁晓敏, 等. 舆情时间中评论对象的情感分析及其关系网络研究. 情报科学, 2018.
- 郭宇, 等. 基于情感分析的社会网络用户影响力模型研究. 情报科学, 2018.
- Barabasi A-L. The origin of bursts and heavy tails in human dynamics. Natrue（London），2005,435:207-211.
- 王澎. 人类在线行为的实证和建模. 安徽：中国科学技术大学，2010.


随着互联网迅速发展，社交网络发展为民众了解社会现象、舆情事件的重要平台，带来便利的同时，部分网民也会在互联网上宣泄情感，引发公共舆情事件。为了更好地进行舆情监控和情感预警，把握网民的情感趋向，基于人类行为动力学的舆情事件分析，探究舆情对象的情感变化和关系演化是非常必要的。

人类个体行为是隐藏在许多复杂社会经济现象背后的驱动力，定量理解人类行为是现代科学的一个重要研究课题。2005年，Barabási研究显示人类行为间隔规律是高度非均匀的，称之服从幂律分布，并在《自然》发表了一篇文章，开创了“人类行为动力学”的新研究方向。目前，科学家通过大量的实证统计发现了一些人类行为，如邮件通讯、短信通讯、网页浏览、电影点播、微博事件等的时间间隔近似服从幂律分布，这种幂律分布特性无论在群体水平还是个体水平上都可以得到证实。除了发现人类行为的时间间隔分布中广泛存在的幂律现象外，近年来证实研究发现在人类的空间运动行为中也存在幂律分布特性，如停留时间分布和出行距离分布。

常见的人类行为动力学分析包括：时间间隔分布、活跃性分析、时间间隔分布宽度、时间间隔重标度、交互周期与热度分析、交互的阵发性和记忆性分析等。刘海鸥等老师研究发现微博、QQ群、天涯论坛、人人网服从幂律分布如下图所示，表明在线社交活动少数人处于活跃状态，积极频繁地发布消息，而大部分成员活跃性较低，处于静默状态。
![](https://img-blog.csdnimg.cn/20190122135355693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.Python时间间隔分布图绘制

为解释人类个体为何具有高概率进行长时间停留的特征，通常会对个体在统计时间段内的日常活动事件序列进行分析。幂律特性分析通常会得到如下图所示的图形，而它如何通过Python进行绘制呢？
![](https://img-blog.csdnimg.cn/20190122140724664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**1.数据集**

假设作者定义网络爬虫抓取了天涯社区某一个事件（如“嫦娥四号”事件）的992条回复信息，包括id、主题用户、评论点赞数、评论被追评数、评论内容、评论时间、积极情绪分数。现在需要绘制时间间隔分布图。
![](https://img-blog.csdnimg.cn/20190122140834403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.SQL语句**

接着通过Python调用本地的MySQL数据库，并获取每个评论的时间，然后绘制胖尾图，其中SQL语句核心代码如下：

```
select zhutishijian from yq_ml;
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/201901241826398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
Python访问MySQL数据库的核心代码如下，本地数据库名为“20181228db”。

```python
class lianjie(object):
    def __init__(self):
        pass
    def connect(self,hostaddress):
        self.conn = msd.connect(host = hostaddress,user = "root",passwd = "123456",db = "20181228db",charset = 'utf8')
        #cur = self.conn.cursor()
        return self.conn
    def guanbi(self):
        return self.conn.close()
```

**3.时间间隔分布胖尾图**

核心步骤如下：
- 通过SQL语句获取每条评论的时间
- 对评论时间数组进行排序，然后依次获取两两评论时间的时间间隔
- 通过函数计算myset内容的无重复项，并统计每个时间间隔出现的频次
- 最后绘制Pow-low幂律分布图

完整代码如下：

```python
# -*- coding: utf-8 -*-
from pylab import *
import MySQLdb as msd
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

#绘图显示中文字体和负号
plt.rcParams['font.sans-serif'] = ['SimHei'] 
myfont = matplotlib.font_manager.FontProperties(fname='C:/Windows/Fonts/msyh.ttf')  
plt.rcParams['axes.unicode_minus'] = False 
font1 = {'family' : 'Times New Roman', 'weight' : 'normal', 'size'   : 26}

#链接本地数据库20181228db
class lianjie(object):
    def __init__(self):
        pass
    def connect(self,hostaddress):
        self.conn = msd.connect(host = hostaddress,user = "root",passwd = "123456",
                                db = "20181228db",charset = 'utf8')
        return self.conn
    def guanbi(self):
        return self.conn.close()

#时间间隔 幂律分布
if __name__ == '__main__':
    #调用数据
    lj = lianjie()
    conn = lj.connect("localhost")
    cur=conn.cursor()  
    #查询评论时间
    sql = "select zhutishijian from yq_ml;"
    cur.execute(sql)
    times = cur.fetchall()
    PLTimeList = [] #评论时间列表
    Period= [] #时间间隔
    PeriodSeconds = []

    #获取时间
    for i in times:
        PLTimeList.append(datetime.strptime(str(i[0]),"%Y-%m-%d %H:%M:%S"))

    PLTimeList.sort() #时间排序
    PLTimeList.reverse() #列表中元素反向

    #获取时间间隔再赋值给列表
    for i in range(0, len(PLTimeList)-1):    
       cnt = (PLTimeList[i]-PLTimeList[i+1]) 
       Period.append(cnt)
    #获取秒
    for i in Period:
        PeriodSeconds.append(i.seconds)
    print (PeriodSeconds)

    #myset是另外一个列表,里面的内容是mylist里面的无重复项
    x = []
    y = []
    myset = set(PeriodSeconds)  
    for item in myset:
        #print("the %d has found %d" %(item, PeriodSeconds.count(item)))
        x.append(item)
        y.append(PeriodSeconds.count(item)) #出现数量

    #绘图
    plt.subplot(111)
    plt.plot(x, y,'ko')
    plt.yscale('log')
    plt.ylabel('P', font1)
    plt.xlabel('timespan', font1)
    plt.xscale('log')
    plt.ylim(0.5,20)
    #plt.xlim(0.001,)
    plt.show()
```

绘制最终的结果如下所示：
![](https://img-blog.csdnimg.cn/20190124182912433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 四.SQL语句获取时间间隔

由于作者比较喜欢SQL语句解决一些问题（虽然后台语言积极更好），但这里也提供这种SQL语句计算时间间隔的方法。因为感觉语言之间相互转化并解决问题挺有意思的，希望读者也试试。

**1.时间间隔排序，ord_num为序号，输出下标从2开始，命名为A**

```
select a.id, a.zhutishijian, (@i := @i + 1) as ord_num 
from yq_ml a,(select @i := 1) d 
order by zhutishijian
```
![](https://img-blog.csdnimg.cn/20190124185415377.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.时间间隔排序，ord_num为序号，下标从1开始，命名为A。**
![](https://img-blog.csdnimg.cn/2019012418573677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.利用TIMESTAMPDIFF获取A和B的时间间隔，该SQL语句较为复杂。**

```
select TIMESTAMPDIFF(SECOND, A.zhutishijian, B.zhutishijian) sub_seconds, count(A.id)
from 
(select a.id, a.zhutishijian, (@i := @i + 1) as ord_num from yq_ml a,(select @i := 1) d 
 order by zhutishijian) as A 
LEFT JOIN 
(select a.id, a.zhutishijian, (@j := @j + 1) as ord_num from yq_ml a,( select @j := 0) c 
 order by zhutishijian) as B 
on A.ord_num=B.ord_num GROUP BY sub_seconds;
```

输出结果是各时间差出现的频次。
![](https://img-blog.csdnimg.cn/20190124190636132.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**4.Python完整代码。**

```python
# -*- coding: utf-8 -*-
from pylab import *
import MySQLdb as msd
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

#绘图显示中文字体和负号
plt.rcParams['font.sans-serif'] = ['SimHei'] 
myfont = matplotlib.font_manager.FontProperties(fname='C:/Windows/Fonts/msyh.ttf')  
plt.rcParams['axes.unicode_minus'] = False 
font1 = {'family' : 'Times New Roman', 'weight' : 'normal', 'size'   : 26}

#链接本地数据库20181228db
class lianjie(object):
    def __init__(self):
        pass
    def connect(self,hostaddress):
        self.conn = msd.connect(host = hostaddress,user = "root",passwd = "123456",
                                db = "20181228db",charset = 'utf8')
        return self.conn
    def guanbi(self):
        return self.conn.close()

#时间间隔 幂律分布
if __name__ == '__main__':
    #调用数据
    lj = lianjie()
    conn = lj.connect("localhost")
    cur=conn.cursor()  
    #查询评论时间
    sql = """select TIMESTAMPDIFF(SECOND, A.zhutishijian, B.zhutishijian) sub_seconds, count(A.id)
        from 
        (select a.id, a.zhutishijian, (@i := @i + 1) as ord_num from yq_ml a,(select @i := 1) d 
         order by zhutishijian) as A 
        LEFT JOIN 
        (select a.id, a.zhutishijian, (@j := @j + 1) as ord_num from yq_ml a,( select @j := 0) c 
         order by zhutishijian) as B 
        on A.ord_num=B.ord_num GROUP BY sub_seconds;
        """
    cur.execute(sql)
    result = cur.fetchall()

    #时间间隔 单位秒
    time1 = [n[0] for n in result]
    del time1[0] #第一个值为空
    print len(time1)
    print time1
    
    #出现频次
    num1 = [n[1] for n in result]
    del num1[0] 
    print len(num1),type(num1)
    print num1.index(max(num1)) #获取最大值的序列
    
    #绘图
    plt.subplot(111)
    plt.plot(time1, num1, 'ko')
    plt.yscale('log')
    plt.ylabel('P', font1)
    plt.xlabel('timespan', font1)
    plt.xscale('log')
    plt.ylim(0.5,20)
    #plt.xlim(0.001,)
    plt.show()
```

输出结果如下图所示，
![](https://img-blog.csdnimg.cn/20190124191624534.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
这是2019年的基础性文章，希望对大家有所帮助，不喜勿喷。同时，寒假已开始了自己奋斗学习之路，希望一个月的时间能坚持把英语、专业课巩固上来。考博之路很艰辛，且努力且珍惜。娜女神和我一起加油，也希望读者给我投一票吧。我是59号，Eastmount，杨秀璋。

投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)

(By:Eastmount 2019-01-24 下午2点 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )](https://blog.csdn.net/Eastmount/article/category/6423551)](https://blog.csdn.net/Eastmount/article/category/8632362)](https://so.csdn.net/so/search/s.do?q=时间间隔分析&t=blog)](https://so.csdn.net/so/search/s.do?q=幂律分布&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)](https://so.csdn.net/so/search/s.do?q=舆情分析&t=blog)




