# SQL Server 2016正式版安装（超多图） - xqhrs232的专栏 - CSDN博客
2017年10月17日 10:24:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6865
原文地址::[http://www.cnblogs.com/fishparadise/p/5655570.html](http://www.cnblogs.com/fishparadise/p/5655570.html)
相关文章
1、[SQL server 2016 安装步骤](http://www.cnblogs.com/ksguai/p/5869558.html)----[http://www.cnblogs.com/ksguai/p/5869558.html](http://www.cnblogs.com/ksguai/p/5869558.html)
2、SQL Server 2016 安装教程----[https://jingyan.baidu.com/article/851fbc3799e6143e1e15ab5f.html](https://jingyan.baidu.com/article/851fbc3799e6143e1e15ab5f.html)
3、SQL Server 2016 怎么用(真!!!)----[https://jingyan.baidu.com/article/86112f139c3f0c27379787b0.html](https://jingyan.baidu.com/article/86112f139c3f0c27379787b0.html)
微软数据库SQL Server 2016正式版在2016年6月就发布，由于近期工作忙，一直拖到现在才有时间把安装过程写到博客上，分享给大家。本人一直习惯使用英文版，所以版本和截图都是英文版的。废话少说，转入正题。
下载地址： [https://www.microsoft.com/en-us/server-cloud/products/sql-server-2016/](https://www.microsoft.com/en-us/server-cloud/products/sql-server-2016/)或者 [http://msdn.itellyou.cn/](http://msdn.itellyou.cn/)
开发版下载地址：[https://myprodscussu1.app.vssubscriptions.visualstudio.com/downloads](https://myprodscussu1.app.vssubscriptions.visualstudio.com/downloads)
KB2919442下载地址：[https://www.microsoft.com/en-us/download/details.aspx?id=42162](https://www.microsoft.com/en-us/download/details.aspx?id=42162)
KB2919355下载地址：[https://www.microsoft.com/en-us/download/details.aspx?id=42335](https://www.microsoft.com/en-us/download/details.aspx?id=42335)
KB3164398下载地址：[https://www.microsoft.com/en-us/download/details.aspx?id=52687](https://www.microsoft.com/en-us/download/details.aspx?id=52687)
SSMS下载地址：[https://msdn.microsoft.com/en-us/library/mt238290.aspx](https://msdn.microsoft.com/en-us/library/mt238290.aspx)
JRE下载地址：[http://java.com/en/](http://java.com/en/)
官方文档：[https://msdn.microsoft.com/en-us/library/ms130214.aspx](https://msdn.microsoft.com/en-us/library/ms130214.aspx)
软硬件要求：[https://msdn.microsoft.com/en-us/library/ms143506.aspx](https://msdn.microsoft.com/en-us/library/ms143506.aspx)
环境：Windows Server 2012 R2 DataCenter 英文版
SQL版本：64位英文企业版权
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115513186-350031373.png)
Figure-1: SQL Server 2016 官方页面
安装过程也是几经周折，不是很顺利。不过最终还好安装成功了。遇到的问题在安装的具体步骤中才说明。如果需要参考安装的童鞋可以先完整浏览一遍之后才开始安装。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115514499-1083542619.png)
Figure-2: 打开SQLServer2016-x64-ENU安装包
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115516686-160254434.png)
Figure-3
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115518202-1413783635.png)
Figure-4
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115520561-895605356.png)
Figure-5
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115523014-939323676.png)
Figure-6
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115524639-1980350882.png)
Figure-7
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115526780-998174036.png)
Figure-8
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115527952-270956084.png)
Figure-9
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115529889-636848414.png)
Figure-10
由于是安装测试，所以把全部功能都选择了。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115532624-729451899.png)
Figure-11
在验证时遇到两个错误，下图。本来以为像RC0版本那样（SQL 2016 RC0 安装过程请参考： [http://www.cnblogs.com/fishparadise/p/5288594.html](http://www.cnblogs.com/fishparadise/p/5288594.html)），安装了[JER7](http://java.com/en/)和K[B2919355](https://www.microsoft.com/en-us/download/details.aspx?id=42335)就可以了，却遇到安装KB2919355不成功的问题。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115534405-397195495.png)
Figure-12
安装KB2919355出错。后来详细阅读KB2919355的说明之后，才发现要先安装KB2919442然后再安装KB2919355，而且KB2919355下载页面还包含其他几个补丁，也必须安装说明的顺序进行安装，详细见[KB2919355](https://www.microsoft.com/en-us/download/details.aspx?id=42335)的说明。奇怪的是KB2919355说明的系统要求是Windows
 8.1，找了很久也不见有Windows Server 2012 版本的KB2919355。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115904014-220881284.png)
Figure-13：KB2919355安装出错
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115905139-252034226.png)
Figure-14：KB2919355的安装说明
成功安装相关补丁成功之后，继续。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115537717-938152310.png)
Figure-15
由于我的机器上已经有默认实例，所以只能使用命名实例了。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115538983-1726648274.png)
Figure-16
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115540327-384422869.png)
Figure-17
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115541921-265068443.png)
Figure-18
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115543546-925500316.png)
Figure-19
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115545311-388843387.png)
Figure-20
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115546889-1534730916.png)
Figure-21
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115548092-748782918.png)
Figure-22
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115549249-866138193.png)
Figure-23
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115550608-1962892188.png)
Figure-24
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115552483-49271914.png)
Figure-25
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115553889-781524962.png)
Figure-26
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115555624-41014328.png)
Figure-27
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115557374-1832675134.png)
Figure-28
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115559592-1755270907.png)
Figure-29
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115601327-665400546.png)
Figure-30
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115602749-1191184425.png)
Figure-31
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115604452-1687880447.png)
Figure-32
正式版也是没有默认安装SSMS的，需要在安装程序的界面（见Figure-4），点击Install SQL Server Management Tools会自动打开以下地址，选择自己的所需的语言版本后进行下载，安装。
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115610046-993531457.png)
Figure-33
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115611733-1910638698.png)
Figure-34
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115612967-1397751782.png)
Figure-35
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115615249-1174214013.png)
Figure-36
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115616780-765052247.png)
Figure-37
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115619186-1900618158.png)
Figure-38
```
Microsoft SQL Server **2016** (RTM) - **13.0**.**1601.5** (X64)   Apr **29** **2016** **23**:**23**:**58**   Copyright (c) Microsoft Corporation  Enterprise Evaluation Edition (**64**-bit) on Windows Server **2012** R2 Datacenter **6.3** <X64> (Build **9600**: ) (Hypervisor)
```
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115906249-783819569.png)
Figure-39
![image](http://images2015.cnblogs.com/blog/772143/201607/772143-20160709115907467-976049374.png)
Figure-40

