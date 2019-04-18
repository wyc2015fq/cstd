# AEB E-NCAP测试项目及发展趋势 - 心纯净，行致远 - CSDN博客





2018年12月28日 10:11:31[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：513








序 言

AEB作为一项重要的主动安全功能，目前已普遍纳入全球主要汽车市场的碰撞安全评分体系，其中E-NCAP率先于2014年将AEB纳入评分体系，并于2016年将AEB VRU纳入评分体系。



本文将主要介绍2018版E-NCP中AEB相关测试项目、2020版E-NCAP与AEB 的可能变化及后续发展规划。

**一、2018版E-NCAP测试项目**

E-NCAP中AEB的测试主要包括AEB CCR（追尾）和AEB VRU（弱势道路使用者）两部分，以下分别介绍。

**1、AEB CCR**

E-NCAP中定义的AEB CCR包括CCRs（前程静止）、CCRm（前车运动）和CCRb（前车制动）三大测试场景，具体见下表1：

表 1  E-NCAP（Version 2.0 March 2017）_AEB CCR测试场景

![](https://img-blog.csdnimg.cn/20181228100451259.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

续表1：

![](https://img-blog.csdnimg.cn/20181228100508163.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

*注：1）CCRs和CCRb中车速以10km/h为变化步长，横向重叠率以25%为变化步长；2）CCRb中GVT需要在1s内达到期望减速度，且误差不超过±0.25 m/s2。



上表中的横向重叠率指VUT（测试车）与GVT（全尺寸目标车）在横向的重叠率，当VUT与GVT的X向轴线完全重合时，定义横向重叠率为100%，其它-50%、-75%、75%、50%横向重叠率定义见下表2：



表 2  VUT与GVT横向重叠率

![](https://img-blog.csdnimg.cn/20181228100532783.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)



**2、AEB VRU**

2018版C-NCAP中AEB VRU仅考虑成年人的情况，而E-NCAP中AEB VRU除了考虑成年人外，还会考虑儿童和骑自行车的人。

E-NCAP（Version 2.0 March 2017）中定义的AEBVRU_Pedestrian和AEB VRU_Bicyclist测试场景定义分别见表3、表4。

表3  E-NCAP（Version 2.0 March 2017）_AEB VRU_Pedestrian测试场景

![](https://img-blog.csdnimg.cn/20181228100601462.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

续表3：

![](https://img-blog.csdnimg.cn/20181228100657831.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181228100745357.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

续表3：

![](https://img-blog.csdnimg.cn/20181228100803537.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

*注：上表中所有测试车速均以5km/h为变化步长。

表 4  E-NCAP（Version 2.0 March 2017）_AEB VRU_Bicyclist测试场景

![](https://img-blog.csdnimg.cn/2018122810083137.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181228100913284.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

续表4：
||**CBNA****-50**|**C****BLA-50**|**C****BLA-25**|
|----|----|----|----|
|**A****EB/FCW**|AEB|FCW| |
|**VUT车速**|（20~60）km/h|（25~60）km/h|（50~80）km/h|
|**目标速度**|15 km/h|20 km/h| |
|**碰撞位置**|50%|25%| |

*注：上表中所有测试车速均以5km/h为变化步长。

3、全尺寸目标车

E-NCAP中要求采用全尺寸目标车（GVT），其与C-NCAP中要求的非全尺寸目标车（VT）对比见下表5：

表 5  E-NCAP和C-NCAP对于目标车辆要求对比

![](https://img-blog.csdnimg.cn/20181228100938783.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)



**二、2020版E-NCAP可能的变化**

根据目前了解的信息，2020版E-NCAP中对于AEB VRU的测试将会更加严格，将会引入新的测试场景，其中AEB VRU_Bicyclist和AEB VRU_Pedestrian的测试场景见下表6及下表7。

表 6  2020版E-NCAP AEB VRU_Bicyclist测试场景
||**C****VNB**|**C****VNBO**|**C****VFB**|**C****VLB**| |
|----|----|----|----|----|----|
|**V****UT****车速**|（10~60）km/h|（10~60）km/h|（10~60）km/h|（20~60）km/h|（50~80）km/h|
|**自行车速度**|15 km/h|10 km/h|20 km/h|15 km/h|20 km/h|
|**碰撞点**|（0~50）%|（0~50）%|（0~50）%|（0~50）%|（0~50）%|

续表6：

![](https://img-blog.csdnimg.cn/20181228101006422.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

表 7  2020版E-NCAP AEB VRU_Pedestrian测试场景

![](https://img-blog.csdnimg.cn/20181228101019593.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**三、E-NCAP AEB测试发展规划**

根据目前公布的E-NCAP发展规划，在未来几年，将会持续引入AEB VRU_Pedestrian-Back over（后向行人保护）、AEB - Junction-Crossing（交叉路口AEB）、AEB – Head-on（迎面）等全新测试场景。

其中AEB VRU_Pedestrian-Backover的功能为系统检测到车辆后方人的存在，并自动启动制动或防止加速，能很大程度上防止车辆倒车时和行人发生碰撞事故。AEB - Junction-Crossing主要针对车对车、车对行人、车对PTW（机动两轮车）的交叉路口及转弯场景的AEB测试。AEB – Head-on是模拟车辆正面头碰头的场景。

![](https://img-blog.csdnimg.cn/20181228101044191.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

图 E-NCAP发展规划

转载自：[知荐 | AEB E-NCAP测试项目及发展趋势](https://mp.weixin.qq.com/s/8_zsGp8MnpCy6JpFiLSOwg)



