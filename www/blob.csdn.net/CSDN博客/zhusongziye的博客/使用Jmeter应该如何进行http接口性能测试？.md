# 使用Jmeter应该如何进行http接口性能测试？ - zhusongziye的博客 - CSDN博客





2018年11月20日 21:41:47[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：126








在进行网页或应用程序后台接口开发时，一般要及时测试开发的接口能否正确接收和返回数据，对于单次测试，Postman插件是个不错的Http请求模拟工具。 

![](https://img-blog.csdnimg.cn/2018112021375126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

但是Postman只能模拟单客户端的单次请求，而对于模拟多用户并发等性能测试，就必须借助其他的工具了，这里推荐功能强大的JMeter自动化测试工具



Apache JMeter - Apache JMeter™



Apache JMeter是Apache组织开发的基于Java的压力测试工具。用于对软件做压力测试，它最初被设计用于Web应用测试但后来扩展到其他测试领域。 它可以用于测试静态和动态资源例如静态文件、Java 小服务程序、CGI 脚本、Java 对象、数据库， FTP 服务器， 等等。JMeter 可以用于对服务器、网络或对象模拟巨大的负载，来自不同压力类别下测试它们的强度和分析整体性能。

下面就以上图的Restful类型的接口为例简单介绍使用Jmeter进行接口测试的方法。

获取某个具体网关设备的基本信息。

请求

方法：GET

URI：/api/gateway/<gateway_id>

参数：

在HTTP请求（http request）的头部（header）添加Userid和Token字段作为用户验证字段

![](https://img-blog.csdnimg.cn/20181120213824575.png)



响应

返回值：JSON格式的网关信息

参数：

![](https://img-blog.csdnimg.cn/20181120213837463.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**1.启动jmeter：在bin下以管理员身份运行jmeter.bat，启动jmeter**

![](https://img-blog.csdnimg.cn/20181120213849295.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**2. 创建测试计划：**

默认启动jmeter时会加载一个测试技术模板，保存测试计划：修改名称为Apitest,点击保存，选择保存路径,后面的步骤，每次添加或修改了了一些选项，软件并不会自动保存到jmx文件中，所以进行测试后，如果需要保存本次测试选项，要手动到“文件”菜单中保存一下。

![](https://img-blog.csdnimg.cn/20181120213859978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**3. 添加线程组**

右键左边树中的测试计划“Apitest”节点，“添 加”→”Threads”→”线程组”

![](https://img-blog.csdnimg.cn/20181120213910338.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

添加成功后，“Apitest”节点下多了“线程组” 节点,当然这个线程组的名称是可以自己设定的。

![](https://img-blog.csdnimg.cn/20181120213921552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**4. 添加http默认请求：（用来配置公共参数,不是http请求）**

右键线程组，选择“添加”→ “配置元件”→“HTTP请求默认值”，点击“HTTP请求默认值”后

添加成功后，线程组”节点下多了“HTTP请求默认值”节点

![](https://img-blog.csdnimg.cn/20181120213935314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这里可以设置主机地址等一下公共参数，比如我们的例子中请求路径前面都是主机地址+index.php,就可以统一在"http请求默认值"里设置



填写默认请求名、服务器、默认请求路径，保存测试计划。



**5.添加http请求信息头**

这一项并不是必须的，只不过我们的例子中使用了Userid和Token放在HTTP请求头中用作用户验证

右键“Apitest”，选择“添加”→ “配置元件”→“HTTP信息头管理器”

![](https://img-blog.csdnimg.cn/20181120213950768.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**6. 添加http请求**

右键“Apitest”，选择“添加”→ “Sampler”→“HTTP请求”

![](https://img-blog.csdnimg.cn/20181120213959769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

添加成功后，出现新的节点“HTTP请求”，就可以填写具体的请求参数了。

![](https://img-blog.csdnimg.cn/20181120214010985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

填写完成，保存测试计划



**7. 添加监听器：**

右键线程组，选择“添加”→“监听器”→“XXXXXXXXX”

可以添加的监听器有很多种，可以添加多个监听器，这里我们添加几个常用的“图形结果”、“察看结果树”、“聚合报告”

![](https://img-blog.csdnimg.cn/2018112021402325.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

添加成功后，“线程组”下增加了几个节点

![](https://img-blog.csdnimg.cn/20181120214033468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**8. 试运行**

点击执行

![](https://img-blog.csdnimg.cn/20181120214045450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以查看各个“监听器”的结果

![](https://img-blog.csdnimg.cn/20181120214055151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

Label：每个 JMeter 的 element（例如 HTTP Request）都有一个 Name 属性，这里显示的就是 Name 属性的值



#Samples：表示你这次测试中一共发出了多少个请求，如果模拟10个用户，每个用户迭代10次，那么这里显示100



Average：平均响应时间——默认情况下是单个 Request 的平均响应时间，当使用了 Transaction Controller 时，也可以以Transaction 为单位显示平均响应时间



Median：中位数，也就是 50％ 用户的响应时间



90% Line：90％ 用户的响应时间



Note：关于 50％ 和 90％ 并发用户数的含义，请参考下文



http://www.cnblogs.com/jackei/archive/2006/11/11/557972.html



Min：最小响应时间



Max：最大响应时间



Error%：本次测试中出现错误的请求的数量/请求的总数



Throughput：吞吐量——默认情况下表示每秒完成的请求数（Request per Second），当使用了 Transaction Controller 时，也可以表示类似 LoadRunner 的 Transaction per Second 数



KB/Sec：每秒从服务器端接收到的数据量，相当于LoadRunner中的Throughput/Sec



**9. 修改线程组的线程数等参数，用于压力测试**

点击左侧树形导航中的“线程组”

![](https://img-blog.csdnimg.cn/20181120214106156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



设置上面几个参数，模拟的总的请求数是：线程数*循环次数。执行一下，用“图形结果”监听器查看一下

![](https://img-blog.csdnimg.cn/20181120214114555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

样本数目：总共发送到服务器的请求数.

最新样本：代表时间的数字,是服务器响应最后一个请求的时间.

吞吐量：服务器每分钟处理的请求数.

平均值：总运行时间除以发送到服务器的请求数.

中间值：时间的数字,有一半的服务器响应时间低于该值而另一半高于该值.

偏离：服务器响应时间变化、离散程度测量值的大小,或者,换句话说,就是数据的分布。



