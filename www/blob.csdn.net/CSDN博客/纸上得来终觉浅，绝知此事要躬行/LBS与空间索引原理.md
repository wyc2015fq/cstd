# LBS与空间索引原理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年04月15日 12:58:01[boonya](https://me.csdn.net/boonya)阅读数：11标签：[LBS																[GEO																[空间索引原理](https://so.csdn.net/so/search/s.do?q=空间索引原理&t=blog)
个人分类：[LBS位置服务](https://blog.csdn.net/boonya/article/category/6358231)





LBS与 空间索引原理。看过很多这方面的文章，发现作者还是比较深入地研究了这方面的东西，向作者学习：[https://home.cnblogs.com/u/LBSer/](https://home.cnblogs.com/u/LBSer/)

**目录**

[LBS](#LBS)

[地图匹配实践](#%E5%9C%B0%E5%9B%BE%E5%8C%B9%E9%85%8D%E5%AE%9E%E8%B7%B5)

[如何设计实现一个地址反解析服务？](#%E5%A6%82%E4%BD%95%E8%AE%BE%E8%AE%A1%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E5%9C%B0%E5%9D%80%E5%8F%8D%E8%A7%A3%E6%9E%90%E6%9C%8D%E5%8A%A1%EF%BC%9F)

[地理围栏算法解析（Geo-fencing）](#%E5%9C%B0%E7%90%86%E5%9B%B4%E6%A0%8F%E7%AE%97%E6%B3%95%E8%A7%A3%E6%9E%90%EF%BC%88Geo-fencing%EF%BC%89)

[利用OpenStreetMap（OSM）数据搭建一个地图服务](#%E5%88%A9%E7%94%A8OpenStreetMap%EF%BC%88OSM%EF%BC%89%E6%95%B0%E6%8D%AE%E6%90%AD%E5%BB%BA%E4%B8%80%E4%B8%AA%E5%9C%B0%E5%9B%BE%E6%9C%8D%E5%8A%A1)

[地图点聚合优化方案](#%E5%9C%B0%E5%9B%BE%E7%82%B9%E8%81%9A%E5%90%88%E4%BC%98%E5%8C%96%E6%96%B9%E6%A1%88)

[lucene索引文件大小优化小结](#lucene%E7%B4%A2%E5%BC%95%E6%96%87%E4%BB%B6%E5%A4%A7%E5%B0%8F%E4%BC%98%E5%8C%96%E5%B0%8F%E7%BB%93)

[基于朴素贝叶斯的定位算法](#%E5%9F%BA%E4%BA%8E%E6%9C%B4%E7%B4%A0%E8%B4%9D%E5%8F%B6%E6%96%AF%E7%9A%84%E5%AE%9A%E4%BD%8D%E7%AE%97%E6%B3%95)

[地理空间距离计算优化](#%E5%9C%B0%E7%90%86%E7%A9%BA%E9%97%B4%E8%B7%9D%E7%A6%BB%E8%AE%A1%E7%AE%97%E4%BC%98%E5%8C%96)

[深入浅出空间索引：2](#%E6%B7%B1%E5%85%A5%E6%B5%85%E5%87%BA%E7%A9%BA%E9%97%B4%E7%B4%A2%E5%BC%95%EF%BC%9A2)

[LBS核心技术解析（引子）](#LBS%E6%A0%B8%E5%BF%83%E6%8A%80%E6%9C%AF%E8%A7%A3%E6%9E%90%EF%BC%88%E5%BC%95%E5%AD%90%EF%BC%89)

[空间索引原理](#%E7%A9%BA%E9%97%B4%E7%B4%A2%E5%BC%95%E5%8E%9F%E7%90%86)

[地理围栏算法解析（Geo-fencing）](#%E5%9C%B0%E7%90%86%E5%9B%B4%E6%A0%8F%E7%AE%97%E6%B3%95%E8%A7%A3%E6%9E%90%EF%BC%88Geo-fencing%EF%BC%89)

[地图点聚合优化方案](#%E5%9C%B0%E5%9B%BE%E7%82%B9%E8%81%9A%E5%90%88%E4%BC%98%E5%8C%96%E6%96%B9%E6%A1%88)

[深入浅出空间索引：2](#%E6%B7%B1%E5%85%A5%E6%B5%85%E5%87%BA%E7%A9%BA%E9%97%B4%E7%B4%A2%E5%BC%95%EF%BC%9A2)

[深入浅出空间索引：为什么需要空间索引](#%E6%B7%B1%E5%85%A5%E6%B5%85%E5%87%BA%E7%A9%BA%E9%97%B4%E7%B4%A2%E5%BC%95%EF%BC%9A%E4%B8%BA%E4%BB%80%E4%B9%88%E9%9C%80%E8%A6%81%E7%A9%BA%E9%97%B4%E7%B4%A2%E5%BC%95)

[位图索引:原理（BitMap index）](#%E4%BD%8D%E5%9B%BE%E7%B4%A2%E5%BC%95%3A%E5%8E%9F%E7%90%86%EF%BC%88BitMap%20index%EF%BC%89)

[GeoHash核心原理解析](#GeoHash%E6%A0%B8%E5%BF%83%E5%8E%9F%E7%90%86%E8%A7%A3%E6%9E%90)

[Geohash距离估算](#Geohash%E8%B7%9D%E7%A6%BB%E4%BC%B0%E7%AE%97)

## LBS

[https://www.cnblogs.com/LBSer/category/475421.html](https://www.cnblogs.com/LBSer/category/475421.html)

LBS GPS 定位 地理信息

### [地图匹配实践](https://www.cnblogs.com/LBSer/p/4612031.html)

摘要: http://www.cnblogs.com/LBSer/p/4612031.html1 背景 如下图所示，1、2、3这三个点是汽车的GPS定位结果，尽管汽车是在道路上，但定位结果与道路存在偏差。地图匹配（Map Matching）是指将行车轨迹的经纬度采样序列与数字地图路网匹配的过程，其本质上...[阅读全文](https://www.cnblogs.com/LBSer/p/4612031.html)



### [如何设计实现一个地址反解析服务？](https://www.cnblogs.com/LBSer/p/4507829.html)

摘要: http://www.cnblogs.com/LBSer/p/4507829.html一、什么是地址反解析 我们都知道手机定位服务，其本质是汇总各种信号得出一个经纬度坐标（x,y）（具体定位原理可以参考：LBS定位技术、基于朴素贝叶斯的定位算法），然而定位服务并未提供该坐标对应的实体地理信息，比...[阅读全文](https://www.cnblogs.com/LBSer/p/4507829.html)



### [地理围栏算法解析（Geo-fencing）](https://www.cnblogs.com/LBSer/p/4471742.html)

摘要: 地理围栏算法解析 http://www.cnblogs.com/LBSer/p/4471742.html 地理围栏（Geo-fencing）是LBS的一种应用，就是用一个虚拟的栅栏围出一个虚拟地理边界，当手机进入、离开某个特定地理区域，或在该区域内活动时，手机可以接收自动通知和警告。如下图...[阅读全文](https://www.cnblogs.com/LBSer/p/4471742.html)



### [利用OpenStreetMap（OSM）数据搭建一个地图服务](https://www.cnblogs.com/LBSer/p/4451471.html)

摘要: http://www.cnblogs.com/LBSer/p/4451471.html图 利用OSM数据简单发布的北京地图服务一、OSM是什么 开放街道图（OpenStreetMap，简称OSM）是一个网上地图协作计划，目标是创造一个内容自由且能让所有人编辑的世界地图（wiki：http://w...[阅读全文](https://www.cnblogs.com/LBSer/p/4451471.html)



### [地图点聚合优化方案](https://www.cnblogs.com/LBSer/p/4417127.html)

摘要: http://www.cnblogs.com/LBSer/p/4417127.html一、为什么需要点聚合 在地图上查询结果通常以标记点的形式展现，但是如果标记点较多，不仅会大大增加客户端的渲染时间，让客户端变得很卡，而且会让人产生密集恐惧症（图1）。为了解决这一问题，我们需要一种手段能在用户有...[阅读全文](https://www.cnblogs.com/LBSer/p/4417127.html)



### [lucene索引文件大小优化小结](https://www.cnblogs.com/LBSer/p/4068864.html)

摘要: http://www.cnblogs.com/LBSer/p/4068864.html 随着业务快速发展，基于lucene的索引文件zip压缩后也接近了GB量级，而保持索引文件大小为一个可以接受的范围非常有必要，不仅可以提高索引传输、读取速度，还能提高索引cache效率（lucene打开索引文件...[阅读全文](https://www.cnblogs.com/LBSer/p/4068864.html)



### [基于朴素贝叶斯的定位算法](https://www.cnblogs.com/LBSer/p/4020370.html)

摘要: http://www.cnblogs.com/LBSer/p/4020370.html1 定位背景介绍 一说到定位大家都会想到gps，然而gps定位有首次定位缓慢（具体可以参考之前的博文《LBS定位技术》）、室内不能使用、耗电等缺陷，这些缺陷大大限制了gps的使用。在大多数移动互联网应用例如go...[阅读全文](https://www.cnblogs.com/LBSer/p/4020370.html)



### [地理空间距离计算优化](https://www.cnblogs.com/LBSer/p/4020306.html)

摘要: http://www.cnblogs.com/LBSer/p/4020306.html 在美团技术博客发表《地理空间距离计算优化》一文，有相似场景的朋友可以参考。 https://tech.meituan.com/2014/09/05/lucene-distance.html 打开美团app，不管是[阅读全文](https://www.cnblogs.com/LBSer/p/4020306.html)



### [深入浅出空间索引：2](https://www.cnblogs.com/LBSer/p/3403933.html)

摘要: http://www.cnblogs.com/LBSer/p/3403933.html深入浅出空间索引2 第一篇讲到了传统的索引如B树不能很好的支持空间数据，比如点（POI等）、线（道路、河流等）、面（行政边界、住宅区等）。本篇将对空间索引进行简单分类，然后介绍网格索引。（深入浅出空间索引1：ht...[阅读全文](https://www.cnblogs.com/LBSer/p/3403933.html)



### [LBS核心技术解析（引子）](https://www.cnblogs.com/LBSer/archive/2013/04/25/3048754.html)

摘要: http://www.cnblogs.com/LBSer/archive/2013/04/25/3048754.html引子：人们常用“上知天文，下知地理”来形容一个人的博学，人们总是用三要素论“什么时间、什么地点，发生或干了什么事情”来描述一件事情,人们也常常借用“天时、地利、人和”之说来总结事件...[阅读全文](https://www.cnblogs.com/LBSer/archive/2013/04/25/3048754.html)

## 空间索引原理

geohash、R树、四叉树、网格、kd树

[https://www.cnblogs.com/LBSer/category/575692.html](https://www.cnblogs.com/LBSer/category/575692.html)

### [地理围栏算法解析（Geo-fencing）](https://www.cnblogs.com/LBSer/p/4471742.html)

摘要: 地理围栏算法解析 http://www.cnblogs.com/LBSer/p/4471742.html 地理围栏（Geo-fencing）是LBS的一种应用，就是用一个虚拟的栅栏围出一个虚拟地理边界，当手机进入、离开某个特定地理区域，或在该区域内活动时，手机可以接收自动通知和警告。如下图...[阅读全文](https://www.cnblogs.com/LBSer/p/4471742.html)



### [地图点聚合优化方案](https://www.cnblogs.com/LBSer/p/4417127.html)

摘要: http://www.cnblogs.com/LBSer/p/4417127.html一、为什么需要点聚合 在地图上查询结果通常以标记点的形式展现，但是如果标记点较多，不仅会大大增加客户端的渲染时间，让客户端变得很卡，而且会让人产生密集恐惧症（图1）。为了解决这一问题，我们需要一种手段能在用户有...[阅读全文](https://www.cnblogs.com/LBSer/p/4417127.html)



### [深入浅出空间索引：2](https://www.cnblogs.com/LBSer/p/3403933.html)

摘要: http://www.cnblogs.com/LBSer/p/3403933.html深入浅出空间索引2 第一篇讲到了传统的索引如B树不能很好的支持空间数据，比如点（POI等）、线（道路、河流等）、面（行政边界、住宅区等）。本篇将对空间索引进行简单分类，然后介绍网格索引。（深入浅出空间索引1：ht...[阅读全文](https://www.cnblogs.com/LBSer/p/3403933.html)



### [深入浅出空间索引：为什么需要空间索引](https://www.cnblogs.com/LBSer/p/3392491.html)

摘要: http://www.cnblogs.com/LBSer/p/3392491.html一、问题 先思考个常见的问题：如何根据自己所在位置查询来查询附近50米的POI（point of interest，比如商家、景点等）呢（图1a）？ 每个POI都有经纬度信息，我用图1b的SQL语句在mySQL...[阅读全文](https://www.cnblogs.com/LBSer/p/3392491.html)



### [位图索引:原理（BitMap index）](https://www.cnblogs.com/LBSer/p/3322630.html)

摘要: http://www.cnblogs.com/LBSer/p/3322630.html位图（BitMap）索引 前段时间听同事分享，偶尔讲起Oracle数据库的位图索引，顿时大感兴趣。说来惭愧，在这之前对位图索引一无所知，因此趁此机会写篇博文介绍下位图索引。1. 案例 有张表名为table的表，...[阅读全文](https://www.cnblogs.com/LBSer/p/3322630.html)



### [GeoHash核心原理解析](https://www.cnblogs.com/LBSer/p/3310455.html)

摘要: http://www.cnblogs.com/LBSer/p/3310455.html引子 机机是个好动又好学的孩子，平日里就喜欢拿着手机地图点点按按来查询一些好玩的东西。某一天机机到北海公园游玩，肚肚饿了，于是乎打开手机地图，搜索北海公园附近的餐馆，并选了其中一家用餐。 饭饱之后机机开始反思了...[阅读全文](https://www.cnblogs.com/LBSer/p/3310455.html)



### [Geohash距离估算](https://www.cnblogs.com/LBSer/p/3298057.html)

摘要: http://www.cnblogs.com/LBSer/p/3298057.html一、经纬度距离换算a）在纬度相等的情况下：经度每隔0.00001度，距离相差约1米；每隔0.0001度，距离相差约10米；每隔0.001度，距离相差约100米；每隔0.01度，距离相差约1000米；每隔0.1度，距...[阅读全文](https://www.cnblogs.com/LBSer/p/3298057.html)](https://so.csdn.net/so/search/s.do?q=GEO&t=blog)](https://so.csdn.net/so/search/s.do?q=LBS&t=blog)




