
# 【python数据挖掘课程】十七.社交网络Networkx库分析人物关系（初识篇） - 杨秀璋的专栏 - CSDN博客

2017年11月05日 21:22:59[Eastmount](https://me.csdn.net/Eastmount)阅读数：5787所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



这是《Python数据挖掘课程》系列文章，也是我大数据金融学院上课的部分内容。本章主要讲述复杂网络或社交网络基础知识，通过Networkx扩展包绘制人物关系，并分析了班级学生的关系学院信息。本篇文章为初始篇，基础文章希望对你有所帮助，如果文章中存在错误或不足支持，还请海涵~
自己真的太忙了，只能挤午休的时间学习新知识，与天斗其乐无穷。

![](https://img-blog.csdn.net/20171105213730990)

前文推荐：
[【Python数据挖掘课程】一.安装Python及爬虫入门介绍](http://blog.csdn.net/eastmount/article/details/52577215)
[【Python数据挖掘课程】二.Kmeans聚类数据分析及Anaconda介绍](http://blog.csdn.net/eastmount/article/details/52777308)
[【Python数据挖掘课程】三.Kmeans聚类代码实现、作业及优化](http://blog.csdn.net/eastmount/article/details/52793549)
[【Python数据挖掘课程】四.决策树DTC数据分析及鸢尾数据集分析](http://blog.csdn.net/eastmount/article/details/52820400)
[【Python数据挖掘课程】五.线性回归知识及预测糖尿病实例](http://blog.csdn.net/eastmount/article/details/52929765)
[【Python数据挖掘课程】六.Numpy、Pandas和Matplotlib包基础知识](http://blog.csdn.net/eastmount/article/details/53144633)
[【Python数据挖掘课程】七.PCA降维操作及subplot子图绘制](http://blog.csdn.net/eastmount/article/details/53285192)
[【Python数据挖掘课程】八.关联规则挖掘及Apriori实现购物推荐](http://blog.csdn.net/eastmount/article/details/53368440)
[【Python数据挖掘课程】九.回归模型LinearRegression简单分析氧化物数据](http://blog.csdn.net/eastmount/article/details/60468818)
[【python数据挖掘课程】十.Pandas、Matplotlib、PCA绘图实用代码补充](http://blog.csdn.net/eastmount/article/details/60675865)
[【python数据挖掘课程】十一.Pandas、Matplotlib结合SQL语句可视化分析](http://blog.csdn.net/eastmount/article/details/62489186)
[【python数据挖掘课程】十二.Pandas、Matplotlib结合SQL语句对比图分析](http://blog.csdn.net/eastmount/article/details/64127445)
[【python数据挖掘课程】十三.WordCloud词云配置过程及词频分析](http://blog.csdn.net/eastmount/article/details/64438407)
[【python数据挖掘课程】十四.Scipy调用curve_fit实现曲线拟合](http://blog.csdn.net/eastmount/article/details/71308373)
[【python数据挖掘课程】十五.Matplotlib调用imshow()函数绘制热图](http://blog.csdn.net/eastmount/article/details/73392106)
【python数据挖掘课程】十六.逻辑回归LogisticRegression分析鸢尾花数据


## 一. 社交网络基础
近年来，随着社交网络的兴起，比如Facebook、微博微信、搜索引擎、知识图谱等领域都涉及到了社交网络技术。网络采用一种关系为中心的世界观，通过利用人与人间关系的现有数据结构（社交网络），可以利用聚类等技术发现社群，洞察图中重要成员的作用，甚至通过关系推断来进行行为预测。
网络与图论密切相关，图论起源于1735年欧拉对七桥问题的研究。一个图是由一组顶点（结点）和它们间的连接（关系或边）构成的网络的负号表示。图定义为G={V, E}，一个由有限多结点V形成的集合和不分顺序的二元组数对{u, v}形成的边E。如下图所示：

![](https://img-blog.csdn.net/20171105214708137)

图可以是有方向的或无方向的，通常可以用邻接矩阵表示。推荐大家简单复习下图论知识，这里作者主要通过Python的Networkx扩展包讲解基础用法。

## 二. Networkx基础知识
Networkx是一个用来创建、操作、研究复杂网络结构的Python扩展包。它支持图的快速创建，同时常用的图算法。同时Networkx扩展包完善了Python的科学计算工具集，如Scipy、Numpy等。
**1.安装过程**
安装主要调用PIP命令，作者这里使用Anaconda软件已经集成了该扩展包。

```python
pip install networkx  --安装包
pip install --upgrade networkx  --更新升级包
pip uninstall networkx  --卸载包
```
**2.基础代码**
下面首先给大家看一个Networkx调用draw(G)绘图的代码。
```python
# -*- coding: utf-8 -*-
import networkx as nx
import matplotlib.pyplot as plt
#定义有向图
DG = nx.DiGraph() 
#添加五个节点(列表)
DG.add_nodes_from(['A', 'B', 'C', 'D', 'E'])
print DG.nodes()
#添加边(列表)
DG.add_edges_from([('A', 'B'), ('A', 'C'), ('A', 'D'), ('D','A'),('E','A'),('E','D')])
print DG.edges()
#绘制图形 设置节点名显示\节点大小\节点颜色
colors = ['red', 'green', 'blue', 'yellow']
nx.draw(DG,with_labels=True, node_size=900, node_color = colors)
plt.show()
```
运行代码如下图所示：
![](https://img-blog.csdn.net/20171105220801208)
**3.Networkx详细介绍****(1) 导入扩展包创建无多重边有向图**
import networkx as nx
DG = nx.DiGraph()
图对象主要包括点和边，Networkx创建图包括四类：Graph无多重边无向图，DiGraph无多重边有向图，MultiGraph有多重边无向图，MultiDiGraph有多重边有向图。
**(2) 增加点，采用序列增加五个点**
DG.add_nodes_from(['A', 'B', 'C', 'D', 'E'])
增加点可以通过G.add_node(1)、G.add_node("first_node")函数增加一个点，也可以调用DG.add_nodes_from([1,2,3])、DG.add_nodes_from(D)函数批量增加多个点。删除点调用DG.remove_node(1)或DG.remove_nodes_from([1,2,3])实现。
**(3) 增加边，采用序列增加多条边**
DG.add_edges_from([('A', 'B'), ('A', 'C'), ('A', 'D'), ('D','A')])
增加一条边可以调用DG.add_edge(1,2)函数，表示在1和2之间增加一个点，从1指向2；也可以定义 e=(1,2) 边，在调用DG.add_edge(*e)函数实现，注意*用来获取元组(1,2)中的元素。增加多条表则使用DG.add_edges+from([(1,2), (2,3)])函数实现。
同理，删除边采用remove_edge(1,2)函数或remove_edges_from(list)实现。

**(4) 访问点和边**
DG.nodes()  \#访问点，返回结果：['A', 'C', 'B', 'E', 'D']
DG.edges()  \#访问边，返回结果：[('A', 'C'), ('A', 'B'), .... , ('D', 'A')]
DG.node['A']         \#返回包含点和边的列表
DG.edge['A']['B']   \#f返回包含两个key之间的边
**(5) 查看点和边的数量**
DG.number_of_nodes()  \#查看点的数量，返回结果：5
DG.number_of_edges()  \#查看边的数量，返回结果：6
DG.neighbors('A')          \#所有与A连通的点，返回结果：['C', 'B', 'D']
DG['A']  \#所有与A相连边的信息，{'C': {}, 'B': {}, 'D': {}}，未设置属性
**(6) 设置属性**
可以给图、点、边赋予各种属性，其中权值属性最为常见，如权重、频率等。
DG.add_node('A', time='5s')
DG.add_nodes_from([1,2,3],time='5s')
DG.add_nodes_from([(1,{'time':'5s'}), (2,{'time':'4s'})])  \#元组列表
DG.node['A']  \#访问
DG.add_edges_from([(1,2),(3,4)], color='red')

**4.draw绘图**
绘制图只要调用draw(G)函数，比如：
nx.draw(DG,with_labels=True, node_size=900, node_color = colors)
参数pos表示布局，包括spring_layout、random_layout、circular_layout、shell_layout四种类型，如pos=nx.random_layout(G)；参数node_color='b'设置节点颜色；edge_color='r'设置边颜色；with_labels显示节点；font_size设置大小；node_size=20设置节点大小。
circular_layout：节点在一个圆环上均匀分布
random_layout：节点随机分布
shell_layout：节点在同心圆上分布
spring_layout： 用Fruchterman-Reingold算法排列节点
同时，补充一个更好的绘制图形函数：
```python
pos=nx.random_layout(G)
nx.draw_networkx_nodes(G, pos, alpha=0.2,node_size=1200,node_color=colors)
nx.draw_networkx_edges(G, pos, node_color='r', alpha=0.3)
nx.draw_networkx_labels(G, pos, font_family='sans-serif', alpha=0.5)
```


## 三. 分析人物关系网络
写到这里正式开始我们的人物关系分析，首先统计了我这门课程的105个学生信息，如下表所示，主要包括姓名、性别、学院、班级、宿舍等内容，这里主要根据学院信息简历人物关系。

![](https://img-blog.csdn.net/20171105230244796)

步骤如下：
1.调用Pandas库读取data.csv文件，并获取学生姓名，姓名存储在一个数组里；
2.计算各个学生的共现矩阵，比如A和B都是同一个学院的，则共现依次，权重加1。注意，这里的学生来自各个学院，分析他们学院之间的关系；
3.将共现矩阵存储至word_node.txt文件中，格式为"学生A 学生B 共现词频"；
4.读取word_node.txt文件，采用空格分割，绘制对应关系图，如果学生A和学生B共同出现，则建立一条边，表示存在关系。类似的，如果做小说或电视人物关系分析，如果人物某一章同时出现，则认为存在关系建立一条边。
5.调用Networkx库绘制图形。
完整代码如下：

```python
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 02 10:33:58 2017
@author: eastmount CSDN 杨秀璋
"""
import pandas as pd
import numpy as np
import codecs
import networkx as nx
import matplotlib.pyplot as plt

""" 第一步:读取数据并获取姓名 """
data = pd.read_csv("data.csv",encoding ="gb2312") #中文乱码
print data[:4]
print data[u'姓名'] #获取某一列数据
print type(data[u'姓名'])
name = []
for n in data[u'姓名']:
    name.append(n)
print name[0]

""" 第二步:计算共现矩阵 定义函数实现 """
a = np.zeros([2,3])
print a
print len(name)
word_vector = np.zeros([len(name),len(name)]) #共现矩阵
#1.计算学院共线矩阵
i = 0
while i<len(name):  #len(name)
    academy1 = data[u'学院'][i]
    j = i + 1
    while j<len(name):
        academy2 = data[u'学院'][j]
        if academy1==academy2: #学院相同
            word_vector[i][j] += 1
            word_vector[j][i] += 1
        j = j + 1   
    i = i + 1
print word_vector
np_data = np.array(word_vector)  #矩阵写入文件
pd_data = pd.DataFrame(np_data)
pd_data.to_csv('result.csv')
#2.计算大数据金融班级共线矩阵
#3.计算性别共线矩阵
#4.计算宿舍楼层共线矩阵
"""
i = 0
while i<len(name):  #len(name)
    academy1 = data[u'宿舍楼层'][i]
    j = i + 1
    while j<len(name):
        academy2 = data[u'宿舍楼层'][j]
        if academy1==academy2: #相同
            word_vector[i][j] += 1
            word_vector[j][i] += 1
        j = j + 1   
    i = i + 1
print word_vector
"""

""" 第三步:共现矩阵计算(学生1 学生2 共现词频)文件 """
words = codecs.open("word_node.txt", "a+", "utf-8")
i = 0
while i<len(name):  #len(name)
    student1 = name[i]
    j = i + 1
    while j<len(name):
        student2 = name[j]
        #判断学生是否共现 共现词频不为0则加入
        if word_vector[i][j]>0:
            words.write(student1 + " " + student2 + " " 
                + str(word_vector[i][j]) + "\r\n")
        j = j + 1
    i = i + 1
words.close()

""" 第四步:图形生成 """
a = []
f = codecs.open('word_node.txt','r','utf-8')
line = f.readline()
print line
i = 0
A = []
B = []
while line!="":
    a.append(line.split())   #保存文件是以空格分离的
    print a[i][0],a[i][1]
    A.append(a[i][0])
    B.append(a[i][1])
    i = i + 1
    line = f.readline()
elem_dic = tuple(zip(A,B)) 
print type(elem_dic)
print list(elem_dic)
f.close()
import matplotlib
matplotlib.rcParams['font.sans-serif'] = ['SimHei']   
matplotlib.rcParams['font.family']='sans-serif'
colors = ["red","green","blue","yellow"]
G = nx.Graph()
G.add_edges_from(list(elem_dic))
#nx.draw(G,with_labels=True,pos=nx.random_layout(G),font_size=12,node_size=2000,node_color=colors) #alpha=0.3
#pos=nx.spring_layout(G,iterations=50)
pos=nx.random_layout(G)
nx.draw_networkx_nodes(G, pos, alpha=0.2,node_size=1200,node_color=colors)
nx.draw_networkx_edges(G, pos, node_color='r', alpha=0.3) #style='dashed'
nx.draw_networkx_labels(G, pos, font_family='sans-serif', alpha=0.5) #font_size=5
plt.show()
```
其中 word_node.txt 文件输出如下所示：
```python
杨宜波 陈杰 1.0
杨宜波  包承菊 1.0
杨宜波 刘天华 1.0
杨宜波 刘紫杰 1.0
杨宜波 欧守杰 1.0
杨宜波 涂书发 1.0
杨宜波 雷琳 1.0
杨宜波 杨美 1.0
张涛 胡垠 1.0
张涛 班敬凯 1.0
张涛 李蓝 1.0
张涛 曾伟 1.0
张涛 杨杰 1.0
张涛 穆佑湖 1.0
张涛 吴贵禹 1.0
张涛 邓迪 1.0
张涛 张军 1.0
```
spring_layout输出图形如下所示，可以看到聚集在一起的为同一个学院学生。
![](https://img-blog.csdn.net/20171105232417941)
random_layout输出图形如下所示：
![](https://img-blog.csdn.net/20171105232339223)
circular_layout输出图形如下所示，将人物围城一圈：
![](https://img-blog.csdn.net/20171105232644894)
数据集建议读者自己去建立，学生数据保密，同时可以根据各种关系建立人物关系，共现矩阵也可以有不同的计算方法，推荐读者下来研究。最后作者也是刚刚学习这类知识，一起进步，希望文章对你有所帮助，尤其是我的学生，如果文章中存在错误或不足之处，还请海涵。给绿幺准备惊喜中~

参考文献：
《数据科学实战手册（R+Python）》 郝智恒 - 第8章
[python networkx 包绘制复杂网络关系图 - 简书（Forget_ever）](http://www.jianshu.com/p/e543dc63454f)
[python复杂网络分析库NetworkX - 博客园（jihite）](http://www.cnblogs.com/kaituorensheng/p/5423131.html)
[[Python]networkx入门 - CSDN（ice110956）](http://blog.csdn.net/ice110956/article/details/38796529)
[基于共现发现人物关系的python实现 - 知乎（邓旭东）](https://zhuanlan.zhihu.com/p/24767513)
[文本分析之制作网络关系图邓旭东HIT - 知乎（邓旭东大神）](https://zhuanlan.zhihu.com/p/26531225?utm_medium=social&utm_source=weibo)
[python简单实战项目：《冰与火之歌1-5》角色关系图谱构建——人物关系可视化](http://blog.csdn.net/AlanConstantineLau/article/details/72146213)
[python学习笔记(2)--Networkx画图 - CSDN（Leige_Smart）](http://blog.csdn.net/leige_smart/article/details/53064932)

(By:Eastmount 2017-11-5 深夜12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))




