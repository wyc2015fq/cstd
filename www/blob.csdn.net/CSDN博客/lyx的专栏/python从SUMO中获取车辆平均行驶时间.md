# python从SUMO中获取车辆平均行驶时间 - lyx的专栏 - CSDN博客





2015年12月03日 15:36:32[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2479








## 1.SUMO的trip information文件

在SUMO的帮助文档中的simulation output中有如下说明。SUMO提供用来记录车辆trip信息的文件。




文件的格式如下



```
<tripinfos>
    <tripinfo id="<VEHICLE_ID>" \
            depart="<DEPARTURE_TIME>" departLane="<DEPARTURE_LANE_ID>" \
            departPos="<DEPARTURE_POSITION>" departSpeed="<DEPARTURE_SPEED>" \
            departDelay="<DEPARTURE_DELAY>" \
            arrival="<ARRIVAL_TIME>" arrivalLane="<DEPARTURE_LANE_ID>" \
            arrivalPos="<ARRIVAL_POSITION>" arrivalSpeed="<ARRIVAL_SPEED>" \
            duration="<TRAVEL_TIME>" routeLength="<ROUTE_LENGTH>" \
            waitSteps="<STEPS_WITH_HALTS>" rerouteNo="<REROUTE_NUMBER>" \
            devices="<DEVICE_LIST>" vtype="<VEHICLE_TYPE_ID>"/>

   ... information about further vehicles ...

</tripinfos>
```
为了计算车辆的平均行驶时间，其实就是提取出每一个车辆记录中的duration属性，然后相加取平均。



## 2.python代码


```python
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 03 18:50:50 2015

@author: Luyixiao
"""

import bs4
soup = bs4.BeautifulSoup(open('E:/averageTime.xml'))
soup = soup.body.tripinfos

list = []
for child in soup.children:
    list.append(child)

kk = list[0]
for i in range(0,len(list)):#delete the blank record
    if list[i]==kk:
        list.pop(i)
        
sum = 0
for t in list:#count the amount duration
    sum= sum+float(t['duration'])

average = sum/len(list)#calculate the average time for per vehicle
```
很简单的一个处理就可以获取车辆平均滞留时间，这一指标可以用来衡量区域交通的通畅情况。








