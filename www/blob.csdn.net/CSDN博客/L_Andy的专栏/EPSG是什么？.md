# EPSG是什么？ - L_Andy的专栏 - CSDN博客

2018年04月10日 16:14:04[卡哥](https://me.csdn.net/L_Andy)阅读数：5758



EPSP的英文全称是European Petroleum Survey Group，中文名称为欧洲石油调查组织。这个组织成立于1986年，2005年并入IOGP(InternationalAssociation of Oil & Gas Producers)，中文名称为国际油气生产者协会。

EPSG对世界的每一个地方都制定了地图，但是由于座标系不同，所以地图也各不相同。

**中国地图**

比如对于中国来讲，以地球的几何球心为中心的地图就是EPSG:4479，以地球的椭球焦点为中心就是EPSG:4480，此外还有EPSG:4490，因为选择不同的座标系对于油气勘探的成本至关重要，所以有不同的座标系。

**世界地图**

**EPSG:4326**

在世界地图方面，EPSG:4326是比较著名的一个，因为由美国主导的GPS系统就是在用它，它还有一个名气更大的别名叫作WGS84，WGS(World Geodetic System)是世界大地测量系统的意思，由于是1984年定义的，所以叫WGS84，之前的版本还有WGS72、WGS66、WGS60。

**EPSG:3857**

另一个比较知名的编码是EPSG:3857，这也是一张世界地图，目前主要是各大互联网地图公司以它为基准，例如Google地图，Microsoft地图都在用它。

所有这些EPSG编码的具体含义都可以在[这个网站](https://link.zhihu.com/?target=https%3A//epsg.io/)上查到。


