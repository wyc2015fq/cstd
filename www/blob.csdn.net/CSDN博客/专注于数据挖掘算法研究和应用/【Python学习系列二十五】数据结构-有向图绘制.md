# 【Python学习系列二十五】数据结构-有向图绘制 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月21日 12:46:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3484
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、场景：从文件中读取节点、有向边，然后绘制。

2、参考代码：



```python
# -*- coding: utf-8 -*-

import networkx as nx
import matplotlib.pyplot as plt

#读取文件，获取节点和边
f = open("D:\\tmp\\gy_contest_link_top.txt", "r")  
nodelist=[]
edgelist=[]
while True:  
    line = f.readline()  
    if line:  
        pass    # do something here 
        line=line.strip()
        node=line.split(';')[0]#获取图节点
        nodelist.append(node)
        in_nodes=line.split(';')[1].split('#')#获取图边，该节点是终点
        for ins in range( len(in_nodes) ) :
            if in_nodes[ins].strip() !='': 
                in_edge=(in_nodes[ins],node)
                if in_edge not in edgelist:
                    edgelist.append(in_edge)
        out_nodes=line.split(';')[2].split('#')#获取图边，该节点是起点  
        for ins in range( len(out_nodes) ) :
            if out_nodes[ins].strip() !='': 
                out_edge=(node,out_nodes[ins])
                if out_edge not in edgelist:
                    edgelist.append(out_edge)
    else:  
        break
f.close()
del nodelist[0] #删除表头生成的节点
del edgelist[0]
del edgelist[0] #删除表头生成的边
#print len(nodelist) #图节点
#print len(edgelist) #边数

#有向图绘制
G=nx.DiGraph()
G.add_nodes_from(nodelist)
G.add_edges_from(edgelist)
nx.draw_networkx(G, pos=None, arrows=True, with_labels=True)
#plt.savefig('D:\\tmp\\it.png')
plt.show()
```


效果图：



![](https://img-blog.csdn.net/20170721124615915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


笔者没有对节点名做简易处理，所以看起来有点乱。






