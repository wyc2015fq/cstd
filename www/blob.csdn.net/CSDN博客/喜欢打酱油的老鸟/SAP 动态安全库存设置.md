
# SAP 动态安全库存设置 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月14日 16:35:21[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：541


[https://blog.csdn.net/chbvb4302/article/details/5393465?locationNum=1&fps=1](https://blog.csdn.net/chbvb4302/article/details/5393465?locationNum=1&fps=1)
1。动态安全库存是基于历史的消耗需求来实现未来的动态库存；
如图所示
![](http://hi.csdn.net/attachment/201003/18/84609_1268902707aU3a.jpg)
min:当库存低于MIN值时，系统建立采购3*平均需求（也就是说这段期间安全库存为3*平均需求）
例如：上图表示A物料 如果第一天的需求为200 第二天需求为 300 第三天的需求为 400
则 （400+200+300）/3 = 300 表示平均需求。
2。具体生效期间在下面设置：
![](http://hi.csdn.net/attachment/201003/18/84609_12689033080070.jpg)
3。然后在物料主数据中-mrp2-供货天数参数文件-选择参数文件
4。在物料主数据中有相关的消耗数据：路径为附加数据-消耗 中可以查出消耗情况

