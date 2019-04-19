# [SQLServer2014]之附加数据库出错 - Jun5203 - CSDN博客
2018年05月13日 18:41:59[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：421
在SQL Server2014附加数据库时出错：
原因：权限的问题
解决方法：
1、找到存放数据库文件的文件夹，点击右键，选中属性；
2、点击安全→编辑→添加→高级→立即查找；
3、在搜索结果中，选中Everyone，点击确定→确定；
4、勾选完全控制，点击应用→确定![](https://note.youdao.com/yws/res/1389/WEBRESOURCE3c11f76eb2cc1549c2231036b186016b)
OK，这时候你再去数据库你就会发现畅通无阻
