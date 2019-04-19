# 64位win7下安装SQL Server 2008（图文解说版）----本人备注 - xqhrs232的专栏 - CSDN博客
2017年10月18日 09:35:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6626
原文地址::[http://www.cnblogs.com/kayvanguo/p/5572415.html](http://www.cnblogs.com/kayvanguo/p/5572415.html)
相关文章
1、如何安装SQL Server 2008数据库（带完整图解）----[https://jingyan.baidu.com/article/948f592434b407d80ef5f97d.html](https://jingyan.baidu.com/article/948f592434b407d80ef5f97d.html)
2、怎样使用SQL SERVER新建立一个数据库----[https://jingyan.baidu.com/article/215817f7e68ab81eda14230c.html](https://jingyan.baidu.com/article/215817f7e68ab81eda14230c.html)
3、Microsoft® SQL Server® 2008 Express----[https://www.microsoft.com/zh-cn/download/details.aspx?id=1695](https://www.microsoft.com/zh-cn/download/details.aspx?id=1695)
4、Microsoft®
 SQL Server® 2008 R2 SP2 - Express Edition----[https://www.microsoft.com/zh-cn/download/details.aspx?id=30438](https://www.microsoft.com/zh-cn/download/details.aspx?id=30438)
运行sql安装
![clip_image016](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image016_thumb.jpg)
单击安装-全新的sql server独立安装,如果我们准备好了故障转移群集,那么我们就可以创建故障转移群集sql
![clip_image018](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image018_thumb.jpg)
常规检查
![clip_image020](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image020_thumb.jpg)
一笑而过
![clip_image022](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image022_thumb.jpg)
选择版本,或者输入密钥自动识别版本
![clip_image024](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image024_thumb.jpg)
授权协议
![clip_image026](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image026_thumb.jpg)
支持文件安装
![clip_image028](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image028_thumb.jpg)
安装完成开始检查自身
![clip_image030](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image030_thumb.jpg)
俩警告,一个是.net警告,说没网络会延迟,或者需要下载文件
![clip_image032](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image032_thumb.jpg)
一个数要打开端口,无视了,晚点再打开
![clip_image034](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image034_thumb.jpg)
选择安装的功能,sql[数据库](http://lib.csdn.net/base/mysql)和管理工具
![clip_image036](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image036_thumb.jpg)
选择实例
![clip_image038](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image038_thumb.jpg)
驱动器检查
![clip_image040](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image040_thumb.jpg)
选择服务账户,如图选择的是本地系统账户
![clip_image042](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image042_thumb.jpg)
验证模式:sql和本地模式两种,输入密码,另外添加管理员,可以添加本地组或者当前用户
![clip_image044](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image044_thumb.jpg)
选择汇报微软选项
![clip_image046](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image046_thumb.jpg)
运行检查
![clip_image048](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image048_thumb.jpg)
信息预览确认
![clip_image050](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image050_thumb.jpg)
开始正式安装
![clip_image052](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image052_thumb.jpg)
安装完成
![clip_image054](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image054_thumb.jpg)
单击关闭完成
![clip_image056](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image056_thumb.jpg)
开始菜单中的sql2008
![clip_image057](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image057_thumb.jpg)
打开smse管理工具（如果数据库连接不上报错”“在与sql server建立连接时出现与网络相关的或特定于实例的错误，服务器名称改为“localhost“）
注意请开启计算机服务。
![clip_image059](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image059_thumb.jpg)
打开管理工具如图
![clip_image061](http://images.cnblogs.com/cnblogs_com/ccfxny/WindowsLiveWriter/2008R2Win7SQL2008_10D0A/clip_image061_thumb.jpg)
//=====================================================================================================
备注::
1>在WIN7 X64下安装 SQL Server2008 R2  Express版本成功
