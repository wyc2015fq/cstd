# 导航坐标系WGS-84和大地坐标系的转化程序 - 心纯净，行致远 - CSDN博客





2018年06月08日 22:53:40[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1979标签：[坐标系转换																[大地坐标系](https://so.csdn.net/so/search/s.do?q=大地坐标系&t=blog)](https://so.csdn.net/so/search/s.do?q=坐标系转换&t=blog)
个人分类：[【ADAS与ICV】](https://blog.csdn.net/zhanshen112/article/category/7655573)








****导**航坐标系和大地坐标系的转化程序**

主要目的：

实现了 站心坐标系和大地坐标系的转化（有什么用？为了实现由GPS坐标获得相对距离）

什么是站心坐标系呢？

以你为坐标原点，XYZ坐标系。

什么是大地坐标系呢？（WG-84坐标系）

GPS的输出坐标系，和地心坐标系不同。

本代码考虑了椭球修正，实测比较准确。

```
#encoding=UTF-8
'''
Created on 2016年9月26日
@author: sx
'''
import math
import numpy as np
PI= math.pi
a=6378137
b=6356755.00
e=0.016710219 
def deg(X):
    X=(X/180)*PI
    return X
def Translate(L,B,H):
    #L为经度，B为纬度，H为高度(补偿高度)
    L0=116
    B0=39.4877777
    H0=0
    print("GPS下经度、纬度、高度为",L,B,H)
    B=deg(B)
    L=deg(L)
    B0=deg(B0)
    L0=deg(L0)
    N=a/(math.sqrt(1-e*e*math.sin(B)*math.sin(B)))
    X=(N+H)*math.cos(B)*math.cos(L)
    Y=(N+H)*math.cos(B)*math.sin(L)
    Z=N*(1-e*e)*math.sin(B)
    N0=a/(math.sqrt(1-e*e*math.sin(B0)*math.sin(B0)))
    X0=(N0+H0)*math.cos(B0)*math.cos(L0)
    Y0=(N0+H0)*math.cos(B0)*math.sin(L0)
    Z0=N0*(1-e*e)*math.sin(B0)
    print("空间直角坐标系下X轴、Y轴、高度为",'%.3f'%X,'%.3f'%Y,'%.3f'%Z)
    mat=np.array([[-math.sin(L),math.cos(L),0],\
                 [-math.sin(B)*math.cos(L),-math.sin(B)*math.sin(L),math.cos(B)],\
                 [math.cos(B)*math.cos(L),math.cos(B)*math.sin(L),math.sin(B)]])
    arr=np.array([[X-X0],[Y-Y0],[Z-Z0]])
    res=np.dot(mat,arr)
    X2=res[0]
    Y2=res[1]
    print("站心坐标系下东偏向、北偏向",'%.3f'%X2,'%.3f'%Y2) 
Translate(116,41.08,0)
```
（不同坐标系及原理自行百度）
公式见 《大地测量学原理》一书

**转自：[https://blog.csdn.net/sinat_33829806/article/details/54429876](https://blog.csdn.net/sinat_33829806/article/details/54429876)**




