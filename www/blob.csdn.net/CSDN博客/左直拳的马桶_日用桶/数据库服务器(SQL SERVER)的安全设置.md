# 数据库服务器(SQL SERVER)的安全设置 - 左直拳的马桶_日用桶 - CSDN博客
2012年05月25日 18:44:02[左直拳](https://me.csdn.net/leftfist)阅读数：2487标签：[sql server																[数据库服务器																[microsoft																[服务器																[配置管理																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=配置管理&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
**一、开发注点意**
1、写好代码，运行拼接式的SQL语句要打醒12分精神，尽量用存储过程，或者是参数式的SQL语句，比如 
```
string sql = "SELECT * FROM table1 WHERE name=@name";
SqlParameter[] sqlParas = {
	new SqlParameter("@name", name)
};
SQLHelper.DBHelper.ExecuteNonQuery(CommandType.Text, sql, sqlParas);
```
2、采用最小权限。用户的权限够用就行，权限区分到select ,insert,update,delete，细分到字段。千万别为了图方便，随便就给映射一个owner的角色，甚至蠢到直接用sa！
3、登录名用复杂的密码
4、数据定期备份，异地保存。如果硬件许可，可以搞Raid0 + Raid1。
**二、数据库服务器尽量别暴露在互联网上。**
除非有必要，不给它外网IP，只通过同组的其他机器在局域网内对它进行访问。
**三、隐藏数据库实例**
![](https://img-my.csdn.net/uploads/201205/25/1337935684_5265.JPG)
**四、修改数据库端口**
SQL SERVER的默认端口是1433，修改成别的。
有2种方式：
1、对数据库服务器所有的IP采用一刀切，使用同一个端口（默认）。就是修改IPAll的端口。
![](https://img-my.csdn.net/uploads/201205/25/1337935980_1643.JPG)
默认是IPAll这里起作用的，这里改了，不管这个数据库服务器有多少个IP，采用哪个IP访问都要用IPAll这里指定的端口。
2、针对每个IP设置不同的端口
比如说，数据库服务器有以下IP
内网IP：192.168.0.1
外网IP：123.123.123.1
那我们想在内网访问的时候，采用默认端口1433，而外部访问，则用端口8888。
怎么设置呢？
1）首先要将TCP/IP协议里的全部侦听改为“否”
![](https://img-my.csdn.net/uploads/201205/25/1337936565_7778.png)
![](https://img-my.csdn.net/uploads/201205/25/1337936545_6642.JPG)
2）然后将每个要使用的IP的“活动”和“启用”都设为是，端口改为想要的端口。
值得一提的是，SQL SERVER配置管理器这里显示的IP，是安装SQL SERVER时的服务器IP。至于之后服务器的更改，在这里无从体现。帮助上说什么“
# 添加或删除 IP 地址
SQL Server 配置管理器显示安装 SQL Server 时可用的 IP 地址。如果发生以下情况，可用的 IP 地址也会随之改变：添加或删除网卡、动态分配的 IP 地址过期、重新配置网络结构或计算机的物理位置发生改变（例如便携式计算机在另一座大楼连接到网络）。若要更改 IP 地址，可以编辑**“IP 地址”**框，然后重新启动 SQL Server。
”
狗屁！没有这回事，你就算将服务器重启到烂为止，它都纹丝不动。
怎么办呢？
只好自己去改注册表了。
HKEY_LOCAL_MACHINE \SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10.<InstanceID>\MSSQLServer\SuperSocketNetLib
新增一个IP完全没有问题的。就是太繁琐了，要一个键值一个键值地新增、修改，还没有复制粘贴，狗日的。
值得一提的是，对于新增的IP,Active键值，一定要在注册表里改，在SQL SERVER配置改没有效果，不知怎么回事。
**五、设置IP安全策略**
![](https://img-my.csdn.net/uploads/201205/25/1337941853_7471.png)
根据需要，限定只有某些机器才能访问本数据库，或限定只有某些机器才能远程桌面本服务器。
![](https://img-my.csdn.net/uploads/201205/25/1337942619_8340.png)
值得一提的是，WINDOWS的安全策略里有允许和阻止两类，它们并没有什么优先级之分。谁先建立谁先起作用。比如说，我新建了一个策略，允许IP192。168。0。2访问本服务器，然后又新建了一个策略阻止它访问本服务器，结果就是它可以访问；假如建立的顺序不同，结果就刚好相反。
这个是对WINDOWS2003 server 而言。如果是WIN7，就是配置防火墙。
**六、WINDOWS2003 安全设置**
详见拙作：[WINDOWS2003 安全设置](http://blog.csdn.net/leftfist/article/details/7398241)
