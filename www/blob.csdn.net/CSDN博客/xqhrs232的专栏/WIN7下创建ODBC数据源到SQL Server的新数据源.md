# WIN7下创建ODBC数据源到SQL Server的新数据源 - xqhrs232的专栏 - CSDN博客
2018年07月25日 14:54:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：517
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[https://blog.csdn.net/u013007900/article/details/51169152](https://blog.csdn.net/u013007900/article/details/51169152)
相关文章
1、sqlserver2014连接ODBC----[https://blog.csdn.net/gdmj77zzh/article/details/70217402](https://blog.csdn.net/gdmj77zzh/article/details/70217402)
北邮计算机数据库实验四需求 
此方法是用JDBC-ODBC访问数据库
（1）单击【开始】——》【控制面板】——》【ODBC】
![这里写图片描述](https://img-blog.csdn.net/20160416171504726)
然后打开【ODBC数据源管理器】对话框
![这里写图片描述](https://img-blog.csdn.net/20160416171540241)
（2）在【ODBC数据源管理器】对话框中，单击【用户DNS】标签，单击【添加】按钮，弹出【创建新数据源】对话框。 
如下图所示
![这里写图片描述](https://img-blog.csdn.net/20160416171608790)
（3）在【创建新数据源】对话框中，在驱动程序序列表中选择【SQL Server】,单击【完成】按钮，弹出【创建到SQL Server的新数据源】对话框，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416171901982)
4）在【创建到SQL Server的新数据源】对话框中，在【名称(M)】文本框中输入数据源名称：SQLServer2008_ODBC(可自己自由命名，读取性好就行)，在【服务器(S)】文本框中选择服务器名为：XXX**（这个因人而异，通过下拉列表选择)**，单击【下一步】按钮，进入【创建到SQL Server的新数据源】对话框，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416172027343)
（5）在【创建到SQL Server的新数据源】对话框中，选择因人而异，需要按照自己创建**SQL Server时候的设置**而改动。默认是以Windows NT验证，但是建议按照图中的方式来修改：请务必注意，sa以及密码为可以登录SQL Server的账号，如果没有请自行百度[SQL Server修改sa密码](http://jingyan.baidu.com/article/3aed632e7a7d8b70108091c7.html)。单击【下一步】按钮，弹出【创建到SQL Server的新数据源】对话框，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416172205431)
（6）在【创建到SQL Server的新数据源】对话框中，勾选【更改默认的数据库为】复选框，默认的数据库为master，改为选择自己创建的数据库，单击【下一步】按钮，弹出【创建到SQL Server的新数据源】对话框，如图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416172357269)
（7）在【创建到SQL Server的新数据源】对话框中，采用默认设置。单击【完成】按钮，弹出【ODBC Microsoft SQL Server安装】对话框，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416172426948)
（8）在【ODBC Microsoft SQL Server安装】对话框中，单击【测试数据源】按钮，若弹出如下图所示的【SQL Server ODBC数据源测试】对话框，则表示数据源配置成功。
![这里写图片描述](https://img-blog.csdn.net/20160416172452083)
（9）在【SQL Server ODBC数据源测试】对话框中，单击【确定】按钮，完成ODBC数据源的配置，在【ODBC 数据源管理器】窗口中可以看到刚才创建的ODBC数据源:SQLServer2008_ODBC如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160416172514677)
至此ODBC数据源SQLServer2008_ODBC创建完成，接下来就可以使用ODBC数据源加载JDBC驱动进行数据库连接了。
