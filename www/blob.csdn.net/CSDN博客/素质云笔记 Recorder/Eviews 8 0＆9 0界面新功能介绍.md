# Eviews 8.0＆9.0界面新功能介绍 - 素质云笔记/Recorder... - CSDN博客





2016年03月05日 22:33:34[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7883











Eviews 8.0＆9.0界面新功能介绍





本文其中一些是自己的整理，也有一些是经管之家论坛中一位热心、好学坛友的整理，其中只是简单介绍一下这两个新版本的部分特性，分享出来，有兴趣的看客可以一起学习、进步。









# Eviews 8




## 一、变量显示

![](https://img-blog.csdn.net/20160305220452264)




估计参数界面得到加强，打入头几个词，会有相关关联字段的出现，方便写入变量。




## **二、工作表的详细介绍**



workfile界面，点击”details”，就会出现变量或者工作表的形式，并且有“description”介绍。
![](https://img-blog.csdn.net/20160305220500754)










# Eviews 9




## **一、用户界面方面（Interface）**



用户交互界面下的命令捕捉（Command Capture）——与MATLAB有些相似，这个功能对于学习编程或写脚本很有帮助；
![](https://img-blog.csdn.net/20160305220835238)![](https://img-blog.csdn.net/20160305220844427)






命令窗口和捕捉窗口的停靠功能（Docking）——便于用户界面布置。实际上在老版本中，命令窗口我很少用；

数据对象和工作文件对象的预览功能（Preview）——这个功能也不错。右键点击对象，选择Preview就会出现下面的预览界面：


![](https://img-blog.csdn.net/20160305220848427)






## **二、数据处理方面（Data Handling） **




### 1、加强输入与数据连接



可以直接导入其他workfile格式数据,txt,excel,ODBC数据库。所有载入的数据，eviews都有预先存储数据，这样可以脱机操作，并且在需要链接的时候就会链接原来的数据来源。




![](https://img-blog.csdn.net/20160305222243745)





Manage Links and Forumulae可以让你更好的了解你所用数据的信息（来源）。




![](https://img-blog.csdn.net/20160305222248804)








### 2、数据输入接口与链接——增加了数据的兼容性——FREDX






![](https://img-blog.csdn.net/20160305222251901)





Eviews 9 提供了一个数据接入口（来自圣路易斯联邦储备银行）。这一接口提供了一个新的浏览页面，并且可以知道数据来源。




![](https://img-blog.csdn.net/20160305220851958)![](https://img-blog.csdn.net/20160305220855192)

功能强大的美联储（Federal Reserve Bank）数据接口

云盘存储数据的直接读写

自定义时间序列格式（还未发现这个功能在哪儿用）
![](https://img-blog.csdn.net/20160305220858598)







### 3、新的数据重采样（抽样与内抽）方法
![](https://img-blog.csdn.net/20160305220902193)




### 4、支持云驱动



可以直接读入你的BOX、DROPBOX、google drive等上面的数据。
![](https://img-blog.csdn.net/20160305222255695)




### 5、支持过时数据表



通过设置模板和数据，处理过时的一些数据表，快速汇总过时的数据表格。


![](https://img-blog.csdn.net/20160305222303792)





### 6、新的变频方式




提供了新的从低到高的变频方式。三类方式：Point, Denton, Chow-Lin and Litterman。每一种方式都支持一定数量的不同匹配的点，第一个、最后一个、汇总数、平均（如何匹配不同时期的时间点）。现有的线性、多项式的方式允许在第一、最后使用。

![](https://img-blog.csdn.net/20160305222308820)








## **三、图形、数据表与筒对象（Graphs, Tables and Spools） **




### 1、混合图的增强
![](https://img-blog.csdn.net/20160305220905817)


![](https://img-blog.csdn.net/20160305220908817)



### 2、细节的放大显示(Zoom功能的增强)







![](https://img-blog.csdn.net/20160305220913068)





### 3、多图的侧边栏显示




![](https://img-blog.csdn.net/20160305220916287)
![](https://img-blog.csdn.net/20160305220920724)

除箭头外，图形中又增加了画椭圆和长方形的功能，便于强调某部分细节
![](https://img-blog.csdn.net/20160305220927646)


### 4、图形中箭头、椭圆、长方形的精确控制




![](https://img-blog.csdn.net/20160305220930974)



### 5、数据表、图形和筒对象的LaTeX格式支持


![](https://img-blog.csdn.net/20160305220935490)






## **四、计量经济学与统计（Econometrics and Statistics）**



预报（Forecasting）



增加了序列的ARIMA自动建模预测
![](https://img-blog.csdn.net/20160305220938412)![](https://img-blog.csdn.net/20160305220941693)


预测的评估与综合测试，见我接下来的博客，会有分门别类的详解。






