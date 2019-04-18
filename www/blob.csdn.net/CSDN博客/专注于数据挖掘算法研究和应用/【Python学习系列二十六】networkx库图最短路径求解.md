# 【Python学习系列二十六】networkx库图最短路径求解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月24日 10:42:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1657
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：基于python库networkx来求解图最短路径，相关算法基础参考

http://blog.csdn.net/fjssharpsword/article/details/52931373


http://blog.csdn.net/fjssharpsword/article/details/52953640


可看networkx源码了解其内部算法用的原理。

代码：



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

#有向图构建
G=nx.DiGraph()
G.add_nodes_from(nodelist)
G.add_edges_from(edgelist)

#任意两点间最短路径
'''
try:
    n=nx.shortest_path_length(G,1,4)
    print n
except nx.NetworkXNoPath:
    print 'No path'
'''
path=nx.all_pairs_shortest_path(G)
#for item in path.items():
#    print item
print path.popitem()

#强联通和弱联通
for c in nx.weakly_connected_components(G):#弱联通
    print c
    
con = nx.strongly_connected_components(G)#强联通
print con
print type(con)
print list(con)

#有向图绘制
nx.draw_networkx(G, pos=None, arrows=True, with_labels=False)#with_labels=False不带节点名
#plt.savefig('D:\\tmp\\it.png')
plt.show()
```





