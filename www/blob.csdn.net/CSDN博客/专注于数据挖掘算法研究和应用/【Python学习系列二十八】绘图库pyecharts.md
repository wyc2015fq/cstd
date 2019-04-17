# 【Python学习系列二十八】绘图库pyecharts - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年08月04日 14:31:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9371
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









网址：https://github.com/chenjiandongx/pyecharts

安装：pip install pyecharts

参考代码：



```python
# -*- coding: utf-8 -*-
import sys
defaultencoding = 'utf-8'
if sys.getdefaultencoding() != defaultencoding:
    reload(sys)
    sys.setdefaultencoding(defaultencoding)
    
import pandas as pd
import numpy as np
from pyecharts import Graph 

'''
adj = pd.read_csv("D:\\tmp\\gy_contest_link_top.txt", delimiter=';',dtype={'in_links': np.str, 'out_links': str})
adj = adj.fillna('')
adj=adj['link_ID'].astype(str)
info = pd.read_csv("D:\\tmp\\gy_contest_link_info.txt", delimiter=';')
info=info.astype(str)

nodes = [{"name": n, "symbolSize": info[info.link_ID == n]['width'].values[0]} for n in adj.link_ID.values]

links = []
for (link, inl, outl) in adj.values:
    for ol in outl.split("#"):
        links.append({"source": link, "target": ol, "value": info[info.link_ID==link]['length'].values[0]+(0 if ol == '' else info[info.link_ID==ol]['length'].values[0])})
    for il in inl.split("#"):
        links.append({"source": il, "target": link, "value": info[info.link_ID==link]['length'].values[0]+(0 if il == '' else info[info.link_ID==il]['length'].values[0])})
'''
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

graph = Graph("道路图", width=1600, height=800)
graph.add("", nodelist, edgelist, is_label_show=False, repulsion=1000, label_text_color=None, gravity=0.001)
graph.show_config()
# plt.show()
graph.render('D:\\tmp\\out.html')
```







