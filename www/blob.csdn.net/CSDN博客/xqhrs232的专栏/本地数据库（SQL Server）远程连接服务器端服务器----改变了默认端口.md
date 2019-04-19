# 本地数据库（SQL Server）远程连接服务器端服务器----改变了默认端口 - xqhrs232的专栏 - CSDN博客
2017年10月19日 12:07:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：871
原文地址::[http://www.cnblogs.com/yougmi/p/4616273.html](http://www.cnblogs.com/yougmi/p/4616273.html)
相关文章
1、实例tcp/ip端口改变后如何连接到数据库?----[https://zhidao.baidu.com/question/1369548859264685779.html](https://zhidao.baidu.com/question/1369548859264685779.html)
2、各数据库、服务器默认端口----[http://blog.csdn.net/superit401/article/details/51612865](http://blog.csdn.net/superit401/article/details/51612865)
本地数据库（SQL Server 2012） 连接外网服务器的数据库，外网的服务器端需要做如下配置：
1. 首先是要打开 数据的配置管理工具
![](http://images0.cnblogs.com/blog2015/611553/201507/021518341374220.png)
2. 配置相关的客户端协议，开启TCP/IP
![](http://images0.cnblogs.com/blog2015/611553/201507/021520437711882.png)
3. 数据库默认的远程端口是 1433，但是不是安全的，可以手动更改端口，更待客户端协议的IP
![](http://images0.cnblogs.com/blog2015/611553/201507/021523011087493.png)
这里需要找到最下边的 IPAll,然后修改端口，修改之后，重启数据库服务----------------------------------------------------------------------------
![](http://images0.cnblogs.com/blog2015/611553/201507/021525096084370.png)
4.这里的配置做好之后，然后本地登录数据库，然后在数据库服务右键选择属性，然后选择【链接】，勾选允许远程
![](http://images0.cnblogs.com/blog2015/611553/201507/021527153795975.png)
5. 在这之后，继续在数据库服务右键，选择【注册】，注册一个数据库服务
![](http://images0.cnblogs.com/blog2015/611553/201507/021528507693034.png)
![](http://images0.cnblogs.com/blog2015/611553/201507/021529078378620.png)
可以先测试，然后保存，测试成功，证明可以使用该账号远程登录了-------------------------
6. 以上步骤完成之后，就需要修改防火墙的设置了，当然，如果是直接关闭了服务器的，就不需要配置了，但是一般来说，安全起见，服务器还是不要关闭防火墙。
（1）. 首先是给防火墙添加端口（这里是你设置的服务器端口号，默认1433）
打开 控制面板--》Windows 防火墙-->高级设置-->入栈规则-->右键，新建规则-->选择端口-->设置特定本地端口号-->允许链接-->勾选所有的项-->设置名称-->完成
![](http://images0.cnblogs.com/blog2015/611553/201507/021537515167431.png)
![](http://images0.cnblogs.com/blog2015/611553/201507/021539094258030.png)
![](http://images0.cnblogs.com/blog2015/611553/201507/021539194468836.png)
![](http://images0.cnblogs.com/blog2015/611553/201507/021539321161399.png)
![](http://images0.cnblogs.com/blog2015/611553/201507/021539462131960.png)
.......
(2).第一步完后，防火墙继续新建入栈规则，这次选择添加程序，添加 D:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Binn\sqlservr.exe
这里的 sqlservr.exe  是在你安装数据库的文件夹里
　　添加步骤和上面一样
那么，通过以上的配置，我们就可以再本地访问外网的数据库了，只需输入相应的IP地址和端口号，登录名，密码就行
这里需要注意的是 数据库服务器格式 ： IP地址，端口号
![](http://images0.cnblogs.com/blog2015/611553/201507/021546460998148.png)
IP地址和端口号之间使用的 ',' 号分隔

