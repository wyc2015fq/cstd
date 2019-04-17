# ASP.NET Core 保存Session的方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月28日 15:39:02[boonya](https://me.csdn.net/boonya)阅读数：618标签：[ASP.NET Core																[Session](https://so.csdn.net/so/search/s.do?q=Session&t=blog)](https://so.csdn.net/so/search/s.do?q=ASP.NET Core&t=blog)
个人分类：[ASP.NET](https://blog.csdn.net/boonya/article/category/7665529)








原文地址：[https://www.cnblogs.com/wtujvk/p/8743035.html](https://www.cnblogs.com/wtujvk/p/8743035.html)

ASP.NET Core 保存Session的方法:a.内存方式，b.Redis，c.Redis集群，d.SqlServer

**1.使用Session（进程内）**

在startup中添加方法 


```
services.AddDistributedMemoryCache();

services.AddSession ();

.....

app.UseSession();
```



**2.使用Redis存储Session（单节点）**

```
services.AddDistributedRedisCache(option => {

 option.Configuration = “127.0.0.1：6379”;
 option.InstanceName = 30;
});
```

**3.使用Redis分布式存储 (分布式)**

**nuget:     Microsoft.AspNetCore.DataProtection**

```
services.AddDataProtection()
.SetApplicationName(Configuration["Redis:Session_application_name"])
.PersistKeysToRedis(ConnectionMultiplexer.Connect(redisconfig), "DataProtection-Keys");
```

示例：

![](https://images2018.cnblogs.com/blog/660972/201804/660972-20180408090713238-1097990564.png)

使用方式： 

```
HttpContext.Session.SetString("key", “value”);  //设置

HttpContext.Session.GetString("key");//获取session
```

**4.使用Sqlserver数据库存储Session**
`services.AddDistributedSqlServerCache`


参考：**ASP.NET Core中间件实现分布式 Session**： [https://www.cnblogs.com/vipyoumay/p/7771237.html](https://www.cnblogs.com/vipyoumay/p/7771237.html)




