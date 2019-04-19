# Jmeter登录后Session自动共享与多线程组并行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月05日 18:24:51[boonya](https://me.csdn.net/boonya)阅读数：169
在接口测试中，出于安全考虑接口是需要session才能访问。另外在此基础上，我们还可能模拟不同的客户端登录，需要并行运行移动端线程组。
## 实现session共享
1）修改jmeter安装目录bin下的jmeter.properties，CookieManager.save.cookies=true的值修改为true，打开#注释
![](https://img-blog.csdnimg.cn/20190305181059388.png)
2）重启Jmeter
![](https://img-blog.csdnimg.cn/20190305181115719.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
3）在登录接口下新增一个HTTP Cookie管理器，内容为空。
![](https://img-blog.csdnimg.cn/20190305181158592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
4）为登录验证接口添加正则表达式提取
![](https://img-blog.csdnimg.cn/20190305181258683.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
## 多线程组并行
并行：独立运行线程组即可。
![](https://img-blog.csdnimg.cn/20190305182301385.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
更多阅读：
[常用性能测试用具Jemeter](http://www.51testing.com/zhuanti/jmeter.html)
[Jmeter性能测试入门](https://www.cnblogs.com/TankXiao/p/4045439.html)
