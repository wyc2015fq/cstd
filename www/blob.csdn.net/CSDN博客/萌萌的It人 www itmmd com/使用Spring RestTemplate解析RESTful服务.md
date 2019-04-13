
# 使用Spring RestTemplate解析RESTful服务 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月19日 17:13:30[Jlins](https://me.csdn.net/dyllove98)阅读数：30584


在上一篇“使用Spring MVC 4构建Restful服务”博文中，我们谈到了如何使用Spring MVC 4搭建RESTful服务，本文以上篇文章为基础，继续讲解如何解析Spring RESTful服务。上篇文章中，我们用了jacson包进行Java对象的序列化，本文仍然使用此包进行Java对象的反序列化。

构建RESTful服务

1.我们继续在 FavRestfulController 类中，新增 buildRestUser() 方法，将FavUser对象序列化成JSON对象输出，代码如下：

|1
|2
|3
|4
|5
|6
|7
|8
|9
|@RequestMapping|(value=|"buildRestUser"|)
|public|FavUser buildRestUser(){
|FavUser favUser =|new|FavUser();
|favUser.setUserId(|"mm"|);
|favUser.setUserName(|"美眉"|);
|favUser.setUserAge(|18|);
|favUser.setCreateDate(|new|Date());
|return|favUser;
|}
|


2.启动favspringmvcrestful工程，使用Firefox的RESTClient插件进行测试，在URL中输入[http://localhost](http://localhost/):8080/favspringmvcrestful/buildRestUser，输出如下图所示的结果，RESTful服务搭建完成。

![wKiom1RrP5bQXK_kAAIBhwZkpb8047.jpg](http://s3.51cto.com/wyfs02/M01/53/94/wKiom1RrP5bQXK_kAAIBhwZkpb8047.jpg)


解析RESTful服务

1. 新建“ConsumeRESTful” Java Project，在Build Path中导入工程所需的Jar。

![wKioL1RrQDjwbP5dAAQOgt7G__s789.jpg](http://s3.51cto.com/wyfs02/M02/53/92/wKioL1RrQDjwbP5dAAQOgt7G__s789.jpg)

2. 导入FavUser.Java对象，注意：该对象中的日期类型应该与构建RESTful服务中的FavUser对象的日期类型一致，即对createDate的Getter方法进行格式化，否则在反序列化过程中会报错。

3.新建 ConsumeSpringRestful.java ，在Main()方法中，测试RESTful服务。

|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|package|com.favccxx.favsoft.main;
|import|org.springframework.web.client.RestTemplate;
|import|com.favccxx.favsoft.pojo.FavUser;
|public|class|ConsumeSpringRestful {
|public|static|void|main(String[] args) {
|RestTemplate restTemplate =|new|RestTemplate();
|FavUser favUser = restTemplate.getForObject(|"http://localhost:8080/favspringmvcrestful/buildRestUser"|, FavUser.|class|);
|System.out.println(|"userId:    "|+ favUser.getUserId());
|System.out.println(|"userName:    "|+ favUser.getUserName());
|System.out.println(|"userAge:    "|+ favUser.getUserAge());
|System.out.println(|"createDate:    "|+ favUser.getCreateDate());
|}
|}
|


4. 运行工程，输入内容如下，正说明解析RESTful服务成功。


![wKiom1RrQDKCw7P4AAMYPskEUqo810.jpg](http://s3.51cto.com/wyfs02/M00/53/94/wKiom1RrQDKCw7P4AAMYPskEUqo810.jpg)


本例中采用的就是jacson框架解析JSON对象，当然也可以使用GSON，FastJSON，Jersey等框架进行解析，但通常的做法是反序列化JSON的工具类与序列化的工具类相同，防止因类型不匹配等原因而造成的数据映射错误。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

