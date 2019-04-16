# python与BeautifulSouop计算SUMO仿真的到达速率 - lyx的专栏 - CSDN博客





2015年12月03日 15:05:17[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1529











## 1.到达速率


到达速率就是外部车辆进入优化路网在单位时间内的流量，每个进入的lane有不同的到达速率。

## 2.到达速率的获取
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 02 17:16:44 2015

@author: Luyixiao
"""

import bs4
soup = bs4.BeautifulSoup(open('E:/arrivalRate.xml'))

soup = soup.body.routes
list = []

for child in soup.children:#遍历子节点，并存在List中，用来排序  
    print cnt  
    list.append(child)  
    cnt = cnt+1
    
kk = list[0]
for i in range (0, len(list)):#删除冗余的节点信息  
    if list[i] == kk:  
        print i  
        list.pop(i)
        
list.pop(0)
list.pop(0)
list.pop(0)#前期处理，删除不必要的记录

inputLane = ['-47150988#4','-160896555','47157274#0','24474520#0',\
'39960687#0','73897135','40113020#0','-24474520#4','226657316#0',\
'24576919','11624985#0','178493002#0','-46975399#1','178441747#0',\
'-47150994#1']#建立进入路网的lane链表
cnt = 0
listTemp = []
for cnt in range(len(inputLane)):
    listTemp.append(0)
    
aVec = {}

aVec=dict(zip(inputLane,listTemp))#利用链表建立字典（Dict），lane为key，对应一小时车流次数为value
        
for item in list:#累加，计算数率。由于记录为一小时，所以累加即为速度
    if item.route['edges'].split(' ')[0] in aVec:
        aVec[item.route['edges'].split(' ')[0]] = aVec[item.route['edges'].split(' ')[0]]+1

aVec.items()
最后的字典的键就是进入的lane，值就是速率






## 1.到达速率


到达速率就是外部车辆进入优化路网在单位时间内的流量，每个进入的lane有不同的到达速率。

## 2.到达速率的获取
# -*- coding: utf-8 -*-
"""
Created on Wed Dec 02 17:16:44 2015

@author: Luyixiao
"""

import bs4
soup = bs4.BeautifulSoup(open('E:/arrivalRate.xml'))

soup = soup.body.routes
list = []

for child in soup.children:#遍历子节点，并存在List中，用来排序  
    print cnt  
    list.append(child)  
    cnt = cnt+1
    
kk = list[0]
for i in range (0, len(list)):#删除冗余的节点信息  
    if list[i] == kk:  
        print i  
        list.pop(i)
        
list.pop(0)
list.pop(0)
list.pop(0)#前期处理，删除不必要的记录

inputLane = ['-47150988#4','-160896555','47157274#0','24474520#0',\
'39960687#0','73897135','40113020#0','-24474520#4','226657316#0',\
'24576919','11624985#0','178493002#0','-46975399#1','178441747#0',\
'-47150994#1']#建立进入路网的lane链表
cnt = 0
listTemp = []
for cnt in range(len(inputLane)):
    listTemp.append(0)
    
aVec = {}

aVec=dict(zip(inputLane,listTemp))#利用链表建立字典（Dict），lane为key，对应一小时车流次数为value
        
for item in list:#累加，计算数率。由于记录为一小时，所以累加即为速度
    if item.route['edges'].split(' ')[0] in aVec:
        aVec[item.route['edges'].split(' ')[0]] = aVec[item.route['edges'].split(' ')[0]]+1

aVec.items()
最后的字典的键就是进入的lane，值就是速率







