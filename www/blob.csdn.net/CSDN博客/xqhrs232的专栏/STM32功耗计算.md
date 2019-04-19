# STM32功耗计算 - xqhrs232的专栏 - CSDN博客
2019年04月19日 11:17:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：51
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://wenku.baidu.com/view/ee43956fbed5b9f3f90f1ccd.html](https://wenku.baidu.com/view/ee43956fbed5b9f3f90f1ccd.html)
相关文章
1、STM32F1功耗或工作电流大概是多少----[https://zhidao.baidu.com/question/181440841664108484.html](https://zhidao.baidu.com/question/181440841664108484.html)
2、单片机STM32 功耗的单位是mA怎么理解？----[https://zhidao.baidu.com/question/398002553.html](https://zhidao.baidu.com/question/398002553.html)
但就芯片的功耗跟芯片的工作模式或工作状态密切相关，
你可以看看芯片的数据手册的Electrical characteristics下面的Current characteristics
部分。针对不同状态或模式下的功耗介绍得很清楚。以运行模式下为例，基于不同的工作频率
芯片功耗从几个ma到几十个ma不等。
功率公式是:p=U*I
一般我们单片机都是工作一定电压下,电流的大小就反映了公耗的大小
比如5V单片机正常工作36mA ,就是P=5*36=180mW,功耗很大
待机时下降到2μA P=5*2/1000=0.01mW ,功耗很小了.
在这个行业中,一般将电流的耗损定为功耗
