# JavaWeb —— MVC 设计模式 - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:43:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42








# 【概念】

MVC 是 Model-View-Controller 的简称，即：模型-视图-控制器

其是一种设计模式，将应用程序分为三个核心模块：模型、视图、控制器，每个模块都各自处理自己的业务

模型：是应用程序的主体部分，表示业务数据与业务逻辑，一个模型能为多个视图提供数据，由于应用模型的代码只需写一次就可被多个视图重用，因此其提供了代码的可重用性

视图：视图是用户看到并与之交互的页面，其主要有三个作用：向用户显示相关数据、接受用户输入、不进行任何实际的业务处理

控制器：接受用户的输入并调用模型与视图完成用户的需求，其接收请求并决定调用哪个模型组件去处理请求，然后决定调用哪个视图来显示模型处理返回的数据

# 【实现】

Model：DAO

View：在 JSP 页面上通过 Java 代码实现

Controller：
- Servlet 受理请求
- 获取请求参数
- 调用 DAO 方法
- 可能会将 DAO 方法的返回值放入 request 中
- 转发/重定向页面：当目标的响应界面不需要从request中读取任何值，使用重定向（还可防止表单的重复提交）

![](https://img-blog.csdnimg.cn/2019022616185279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)



