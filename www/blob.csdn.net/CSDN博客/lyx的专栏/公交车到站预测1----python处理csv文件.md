# 公交车到站预测1----python处理csv文件 - lyx的专栏 - CSDN博客





2016年01月14日 22:03:56[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1453标签：[公交车预测																[csv																[数据																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/qtlyx/article/category/5969093)





之前处理csv一直用pandas，今天发现，pandas虽然强大，但是灵活度少了很多，有时候想实现一些简单的功能可能要花很大的代价。大道至简，返璞归真。就像这几天用excel一样，利用简单的规则就可以完成很多任务，不必要动不动就用vba去处理，吃力不讨好。有时候要跳出这样的逻辑陷阱，简单的或者就是最好的。

前两天把公交车的数据用excel处理了一下，最后希望能够把一段路的行驶状况变成一个记录，然后可以用聚类的方法归类，实现站点到站时间预测。但是数据量比较大，如果分成上千个记录用excel做的话，似乎会崩溃很多次，而且存储也显得麻烦，所以还是交给python了。

用透视图、筛选、上色等各种方法，算是把excel的功能挖掘光了。最后得到的数据大概就是这样。




a开头的单元格表示一条记录的开始。接下来就用python处理了，可以利用字典嵌套，一级key就是记录标号，如a1，a2.内部的key就是车站为key，到站间隔和到站速度为value。



```python
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 14 21:41:20 2016

@author: Luyixiao
"""
import csv#import the csv module

def csv2dict(path):
    reader = csv.reader(open(path,'rb'))
    csvList = []
    startNoList=[]
    stationKey = {}
    sectList = []
    li =[]
    tempKey = {}
    for line in reader:#change the csv file to a list
        csvList.append(line)

    for i in range(0,len(csvList)):#get the bit of the start record
        if csvList[i][0][0]=='a':
            startNoList.append(i)               
   
    for i in range(0,len(csvList)):#tranverse the csv list ,like going through the file
        if i in startNoList:
            if i == 0:#if this is a begining of the first record
                key = csvList[i][0]#mark the key 
            else:#if this is a begining of a record
                stationKey[key] = sectList#put the value to the key,the value is a list of the next data
                sectList = []#empty the sectList,which store the csv lines temporaryly
                key = csvList[i][0]
        if not i in startNoList:#this is the content of the record
            sectList.append(csvList[i])
   
    for k in stationKey:#tranverse the dict which store all the content as a record
       li = stationKey[k]#get a key's value
       for i in range(0,len(li)):
           tempKey[li[i][0]]=li[i][1:3]#create a dict as the value of this key
       stationKey[k]=tempKey
    return stationKey
```

之前花费了好多时间用pandas处理这类数据，往往吃力不讨好。有时候，简单最好，因为灵活性更好，不要强求高大上，强求工具，强求为了使用工具而去使用。最简单的就是最好的，当发现可以实现，但是重复劳动很多的时候再去考虑模块、考虑工具或者更好。



处理数据如此，生活亦是如此。](https://so.csdn.net/so/search/s.do?q=数据&t=blog)](https://so.csdn.net/so/search/s.do?q=csv&t=blog)](https://so.csdn.net/so/search/s.do?q=公交车预测&t=blog)




