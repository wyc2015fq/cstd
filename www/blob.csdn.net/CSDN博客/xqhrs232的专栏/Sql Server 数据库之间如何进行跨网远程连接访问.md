# Sql Server 数据库之间如何进行跨网远程连接访问 - xqhrs232的专栏 - CSDN博客
2017年10月17日 13:48:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：751
原文地址::[http://www.cnblogs.com/xuliangxing/p/6005154.html](http://www.cnblogs.com/xuliangxing/p/6005154.html)
相关文章
1、如何远程连接数据库----[https://jingyan.baidu.com/article/4f7d57129c92511a201927e8.html](https://jingyan.baidu.com/article/4f7d57129c92511a201927e8.html)
2、求助：C语言如何连接远程数据库SQL SERVER ----[http://bbs.csdn.net/topics/391912446](http://bbs.csdn.net/topics/391912446)
# 场景说明
　　现在有一台A电脑和一台B电脑，两台电脑都安装了Sql Server数据库，两台电脑不在一个局域网(我们考虑的是不同网络的两台数据库连接)，比如A电脑在公司，B电脑在家里，现在我要在家里用B电脑连接到公司的A电脑里的数据库。我们需要进行如下操作
　　首先，我们要将设置A电脑的Sql Server端口，使用路由器的端口转发功能，能够外部访问的到
　　其次，B电脑使用花生壳绑定的域名进行远程访问，具体步骤如下：
　　SQL Server 2008 R2默认是不允许远程连接的，如果想要在本地用SSMS连接远程服务器上的SQL Server 2008，远程连接数据库。需要做两个部分的配置：
　　1、SQL Server Management Studio Express（简写SSMS）
　　2、SQL Server 配置管理器/SQL Server Configuration Manager（简写SSCM）
　　3、设置防火墙允许数据库端口开放
　　4、路由器配置端口转发功能
第一步：开启数据库的远程连接功能
　　1、这里我们以Sql Server 2008 R2为例，打开Sql Server 2008 R2使用windows身份登录
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232130481-1367673555.png)
2、登录后，右键选择【属性】。左侧选择【安全性】，选中右侧的【SQL Server 和 Windows 身份验证模式】以启用混合登录模式，如果已经是就跳过这步。
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232150325-1410593144.png)
打开Sql Serve
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232218434-160140818.png)
3、选择【连接】，勾选【允许远程连接此服务器】，然后点【确定】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232245122-1662267653.png)
4、展开【安全性】—>【登录名】—>【sa】，右键选择【属性】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232312075-338912868.png)
5、左侧选择【常规】，右侧选择【SQL Server 身份验证】，并设置密码，如果已经设置了，跳过这步
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232327325-1443675765.png)
6、右击数据库选择【方面】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232340622-1069267019.png)
7、在右侧的方面下拉框中选择“服务器配置”；将【RemoteAccessEnabled】属性设为“True”，点“确定”
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232353059-73339792.png)
第二步：SQL Server 配置管理器配置
1、打开sql server配置管理器
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232418653-454753580.png)
2、下面开始配置SSCM，选中左侧的【SQL Server服务】，确保右侧的【SQL Server】以及【SQL Server Browser】正在运行
3、在左则选择sql server网络配置节点下的sqlexpress的协议，在右侧的TCP/IP当中，右键启用或者双击打开设置面板将其修改为“是”，如果已经为“是”就不用修改了。
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232436809-1625459610.png)
4、选择【IP 地址】选项卡，设置TCP的端口为【1433】，如果已经为【1433】，跳过这步
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232453622-1916200197.png)
5、将"客户端协议"的【TCP/IP】也修改为启用
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232511075-498601582.png)
第三步：防火墙设置Sql Server端口允许外部访问
1、打开防火墙，选择【高级设置】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232526934-1809166762.png)
2、【入站规则】当中点击【新建规则】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232654575-744966013.png)
3、我们选择【端口】
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232718512-1923395501.png)
4、我们选择规则应用于【TCP】，在【特定本地端口】输入sql server的默认端口号1433，点击下一步
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232822919-511466051.png)
5、选择允许连接
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232740747-1265440856.png)
 6、给该规则任意取一个名字
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232843184-6660876.png)
第四步：路由器配置端口转发功能
进入A电脑的网关端口，进行端口转发配置，以TL-WVR308路由器为例，在【转发规则】当中的【虚拟服务器】新增转发端口，其中外部端口可以改成其他端口，也可以和内部端口号一致，而内部端口一定为1433(sql server的默认端口)，内部服务器IP地址就是A电脑的内网IP地址。
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232854403-990142830.png)
　　我们还需要绑定在路由器花生壳，这样就确保A电脑的外部地址一直能够通过花生壳解析的到。
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232904028-492026474.png)
至此，A电脑的Sql Server允许访问的配置全部完成，接下来是，B电脑连接A电脑的数据库了。最后，我们使用B电脑的Sql Server连接A电脑的Sql Server。如图所示
![](http://images2017.cnblogs.com/blog/506829/201709/506829-20170927232919465-1841852088.png)
这样就可以使用B电脑外部远程访问到A电脑的Sql server了~~
PS：如有问题，请留言，未经允许不得私自转载，转载请注明出处：[http://www.cnblogs.com/xuliangxing/p/6005154.html](http://www.cnblogs.com/xuliangxing/p/6005154.html)

