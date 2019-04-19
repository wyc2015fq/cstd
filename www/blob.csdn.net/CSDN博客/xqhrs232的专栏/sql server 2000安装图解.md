# sql server 2000安装图解 - xqhrs232的专栏 - CSDN博客
2017年10月17日 10:33:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：390
原文地址::[https://jingyan.baidu.com/article/22fe7ced67dd443002617f88.html](https://jingyan.baidu.com/article/22fe7ced67dd443002617f88.html)
相关文章
1、win7系统安装SQLServer2000的详细步骤(图文)----[http://www.jb51.net/article/52885.htm](http://www.jb51.net/article/52885.htm)
- 
1
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=36407d96ad345982c5dfb3d063840c94/8c1001e93901213f1993a73e54e736d12e2e95b4.jpg)2
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=67fbc3e2e924b899de692f7a017620ac/10dfa9ec8a1363272d132bd1918fa0ec09fac752.jpg)3
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=b7037b3dce1b9d168a92cc239cae89b6/bd315c6034a85edf4c7417c149540923dc54755c.jpg)4
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=701b2ad1738da9774e7ad069df21c520/8601a18b87d6277fa4a4dbda28381f30e824fc9a.jpg)5
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=8f800dc173f082022dc7c77d248bc6d4/b219ebc4b74543a9a0ed132e1e178a82b8011459.jpg)6
- 
![sql server 2000安装图解](https://imgsa.baidu.com/exp/whcrop=92,69/sign=a2bd08f82834349b745338c7a69a28f7/c75c10385343fbf235814fc1b07eca8064388fa7.jpg)7
[分步阅读](http://jingyan.baidu.com/album/22fe7ced67dd443002617f88.html)
MS Sql Server 2000包含两个部分：服务器组件和客户端工具，其中服务器组建是以Windows服务的方式运行的，有四种服务分别是：MSSqlServer、Distributed Transaction Coordinator、SQLServerAgent、Search Service。MSSqlServer 是最常用的服务，一般的数据库功能都是由它提供的，例如文件管理、查询处理、数据存储等；DTC是分布式事务协调器，支持跨越两个或多个服务器的更新操作来保证事务的完整性；SQLServerAgent负责SQL
 Server 自动化工作，如果需要SQL Server 在指定时间执行某一个存储过程，就需要用到这个服务了；Search Service是全文查询服务，负责全文检索方面的工作。
## 方法/步骤
- 
1
首先运行安装包
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=e5987d5987d6277fe912323818391f63/472309f7905298221097d021d7ca7bcb0b46d4a8.jpg)
- 
2
选择本地计算机，点击下一步
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=75b7f4e6349b033b2c88fcda25cf3620/8c1001e93901213f1993a73e54e736d12e2e95b4.jpg)
- 
3
选第一个，创建ＳＱＬ　SEVSER实例，点击下一步
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=803fc2c58a82b9013dadc333438fa97e/10dfa9ec8a1363272d132bd1918fa0ec09fac752.jpg)
- 
4
输入姓名和公司，随便写，点击下一步
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=f068173d76094b36db921bed93ce7c00/bd315c6034a85edf4c7417c149540923dc54755c.jpg)
- 
之后提示软件安装协议，点击是
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=ebc87199d11373f0f53f6f9f940e4b8b/8601a18b87d6277fa4a4dbda28381f30e824fc9a.jpg)
- 
选择第二个，安装服务端和客户端
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=f0d0d2261bd5ad6eaaf964eab1c939a3/b219ebc4b74543a9a0ed132e1e178a82b8011459.jpg)
- 
实例名选择默认就好，点击下一步
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=eab2939820a446237ecaa562a8237246/c75c10385343fbf235814fc1b07eca8064388fa7.jpg)
- 
点击自定义，然后选择安装目录，最好不要在系统盘
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=eab1939820a446237ecaa562a8237246/c75c10385343fbf235824fc1b07eca8064388fa2.jpg)
- 
之后会让你选择安装的组建，按图选择即可
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=7203d6a902087bf47dec57e9c2d2575e/6a600c338744ebf875da8dddd9f9d72a6159a7ad.jpg)
- 
在这里如果单纯的个人学习就选第一个，如果是想做服务器，那么选第二个，用户名填sa
密码自定义
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=9b285a96ebf81a4c2632ecc9e72b6029/0df431adcbef7609dc2d717d2edda3cc7cd99e2a.jpg)
- 
建议选第二个，开发型的登录验证，
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=3a049f75d933c895a67e987be1127397/4bed2e738bd4b31cb2217c5987d6277f9e2ff837.jpg)
- 
这里默认就行
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=a0658667c9ea15ce41eee00986013a25/203fb80e7bec54e7958ff2fcb9389b504ec26af5.jpg)
- 
在这可以配置端口号，一般默认就可以
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=b544e97a5edf8db1bc2e7c643922dddb/d000baa1cd11728bae3464c0c8fcc3cec3fd2c33.jpg)
- 
软件开始安装，等待安装结束
![sql server 2000安装图解](https://imgsa.baidu.com/exp/w=500/sign=3c942ae4afc379317d688629dbc5b784/4d086e061d950a7b466ca6260ad162d9f3d3c9f3.jpg)步骤阅读
