# 接口自动化测试：用 JMeter 实测一个案例 - zhusongziye的博客 - CSDN博客





2018年11月09日 20:35:11[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：395








**Jmeter 介绍**

Jmeter 是一款使用Java开发的，开源免费的，测试工具， 主要用来做功能测试和性能测试（压力测试/负载测试）.

而且用Jmeter 来测试 Restful API, 非常好用。



**如何学好Jmeter**

如果你用Jmeter去对Web进行功能测试，或者性能测试。 你必须熟练HTTP协议，才能学好Jmeter。 否则你很难理解Jmeter中得概念。



**Jmeter 下载和运行**

官方网站：http://jmeter.apache.org/

解压后， 运行 “bin/jmeter.bat”

Jmeter 是支持中文的， 启动Jmeter 后， 点击 Options -> Choose Language 来选择语言



**实际测试的例子**

目标： 获取城市的天气数据：

**第一步：** 发送request 获取城市的城市代号

http://toy1.weather.com.cn/search?cityname=上海

从这个请求的response 中获取到上海的城市代码. 比如:

上海的地区代码是101020100

上海动物园的地区代码是: 10102010016A



**第二步：** 发送request 到： http://www.weather.com.cn/weather2d/101020100.shtml 可以得到该城市的天气数据

       第一步： 新建一个Thread Group

必须新建一个Thread Group, jmeter的所有任务都必须由线程处理，所有任务都必须在线程组下面创建。

![](https://img-blog.csdnimg.cn/20181109202544190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



      第二步：新建一个 HTTP Request



![](https://img-blog.csdnimg.cn/20181109202644721.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

比如要发送一个Get 方法的http 请求: http://toy1.weather.com.cn/search?cityname=上海

可以按照下图这么填

![](https://img-blog.csdnimg.cn/20181109202721280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



        第三步 添加HTTP Head Manager

选中上一步新建的HTTP request. 右键，新建一个Http Header manager. 添加一个header





![](https://img-blog.csdnimg.cn/20181109202752882.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181109202804929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

       第四步: 添加View Results Tree

View Results Tree 是用来看运行的结果的

![](https://img-blog.csdnimg.cn/20181109202822301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

       第五步：运行测试,查看结果



![](https://img-blog.csdnimg.cn/20181109202847304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181109202900762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

到这里。 我们已经成功运行起来了。

        第六步:添加Assertion和Assert Results

选择HTTP Request, 右键 Add-> Assertions -> Response Assertion.添加 Patterns To Test

![](https://img-blog.csdnimg.cn/20181109202950410.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



然后添加一个Assetion Results 用来查看Assertion执行的结果.

选中Thread Group 右键 Add -> Listener -> Assertion Results.

运行后， 如果HTTP Response中没有包含期待的字符串。 那么test 就会Fail.

![](https://img-blog.csdnimg.cn/2018110920300422.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



      第七步: 使用用户自定义变量

我们还可以在Jmeter中定义变量。 比如我定义一个变量叫 city. 使用它的时候用 ${city}

添加一个 User Defined Variables. 选中Thread Group: 右键 Add -> Config Element -> User Defined Variables.

我们添加一个变量: city

![](https://img-blog.csdnimg.cn/20181109203020419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

然后在Http Request中使用这个变量

![](https://img-blog.csdnimg.cn/20181109203036616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



       第八步：关联

所谓关联， 就是第二个Requst, 使用第一个Request中的数据

我们需要在第一个Http Requst 中新建一个正则表达式，把Response的值提取到变量中，提供给别的Http Request 使用

选择第一个Http Request, 右键 Add -> Post Processors -> Regular Expresstion Extractor

![](https://img-blog.csdnimg.cn/20181109203054280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



现在新建第二个Http Request, 发送到： http://www.weather.com.cn/weather2d/${citycode}.html

${citycode} 中的数据， 是从Regular Expression Extractor 中取来的

![](https://img-blog.csdnimg.cn/20181109203108571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



到这， 脚本就全部写好了， 运行下，看下最终结果

![](https://img-blog.csdnimg.cn/20181109203121916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



如何学习？学习没有资料？

jmeter基础和高级

链接：https://pan.baidu.com/s/1jJynzvS 

密码：cgrl



