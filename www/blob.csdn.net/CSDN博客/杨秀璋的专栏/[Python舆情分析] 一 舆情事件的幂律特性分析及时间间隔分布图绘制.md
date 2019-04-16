# [Python舆情分析] 一.舆情事件的幂律特性分析及时间间隔分布图绘制 - 杨秀璋的专栏 - CSDN博客





2019年01月22日 14:19:35[Eastmount](https://me.csdn.net/Eastmount)阅读数：610
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









本文主要是作者学习舆情分析、情感分析、人类行为动力学分析的在线笔记，主要包括两方面内容，一是结合参考文献介绍人类行为动力学分析，二是讲解Python绘制幂律特性及时间间隔分布图的方法。基础性文章，希望对您有所帮助。

PS：最近参加CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 一.舆情分析基础知识

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
梁晓敏等老师提出了如下图所示的舆情事件中评论对象的情感及关系分析模型，并分析了“魏则西事件”的负向情感指数演化趋势及关系网络。
![](https://img-blog.csdnimg.cn/20190122135804595.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190122135815398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190122135830854.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二.时间间隔分布图绘制

为解释人类个体为何具有高概率进行长时间停留的特征，通常会对个体在统计时间段内的日常活动事件序列进行分析。下图是一个典型的个体在一周内活动的情况，图中空白区域表示个体在某地点的停留，黑色竖线表示在不同地点的出行。
![](https://img-blog.csdnimg.cn/20190122140348511.png)
幂律特性分析通常会得到如下图所示的图形，而它如何通过Python进行绘制呢？
![](https://img-blog.csdnimg.cn/20190122140724664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**1.数据集**

假设作者定义网络爬虫抓取了天涯社区某一个事件的992条回复信息，包括id、主题用户、评论点赞数、评论被追评数、评论内容、评论时间、积极情绪分数。现在需要绘制时间间隔分布图。
![](https://img-blog.csdnimg.cn/20190122140834403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.SQL语句**

接着通过Python调用本地的MySQL数据库，并获取每个评论时间的时间，然后绘制柱状图，其中SQL语句核心代码如下：

```
select zhutishijian,jijiqingxu from yq_ml order by zhutishijian;
```

包括评论时间和积极情绪分数，输出结果如下所示：
![](https://img-blog.csdnimg.cn/20190122141210646.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
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

**3.时间间隔分布图完整代码**

完整代码如下：

```python
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 19 13:35:17 2018

@author: yxz yq
"""

import matplotlib.pyplot as plt
import MySQLdb as msd
class lianjie(object):
    def __init__(self):
        pass
    def connect(self,hostaddress):
        self.conn = msd.connect(host = hostaddress,user = "root",passwd = "123456",db = "20181228db",charset = 'utf8')
        #cur = self.conn.cursor()
        return self.conn
    def guanbi(self):
        return self.conn.close()
    
    
if __name__ == '__main__':
    lj = lianjie()
    ############################################调用数据##########################
    conn = lj.connect("localhost")
    cur=conn.cursor()
    sql = "select zhutishijian,jijiqingxu from yq_ml order by zhutishijian;"
    cur.execute(sql)
    data = cur.fetchall()
    
    print (data)
    
    time =[]
    score = []
    l1=[]
    for i in range (0,len(data)):
        time.append(data[i][0])
        score.append(data[i][1])
        l1.append(1)
    plt.subplot(711)
    plt.bar(time,l1,width=0.005,color='black')
    plt.yticks([])
    plt.xticks([])
    plt.show()
```

代码中time用于统计时间，l1用于绘制竖线，生成图形如下，图中空白区域表示个体回复在某个时刻的停留时间，黑色竖线表示在不同时刻出现了回复行为，黑色竖线越多，该时刻的回复事件越活跃。
![](https://img-blog.csdnimg.cn/20190122141613420.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
这是2019年的基础性文章，希望对大家有所帮助，不喜勿喷，感谢Y与我一起付出。同时，寒假已开始了自己奋斗学习之路，希望一个月的时间能坚持把英语、专业课巩固上来。考博之路很艰辛，且努力且珍惜。你我一起加油，也希望读者给我投一票吧。我是59号，Eastmount，杨秀璋。

投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)

(By:Eastmount 2019-01-22 下午2点 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )



