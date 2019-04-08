# JavaWeb开发模式

## Web开发模式的变迁

```
1.servlet开发：繁琐，不利于分工
2.JSP开发：前端把页面写好转化了JSP交给后台就可以；
    缺点：1.页面缺乏可读性；
         2.没有将页面表现和业务逻辑实现分离，维护难度增加；
```

## JSP开发模式1

```
JSP+JavaBean开发模式
1.主要采用JSP和JavaBean技术
2.将页面显示与业务逻辑处理分开
3.JSP负责页面的显示，而业务逻辑则由JavaBean处理；
（没有三层架构，只有JSP和JavaBean）
好处：适用于简单项目，业务逻辑不复杂的，减少代码的层数，能够提高程序效率；

分工：JSP负责接收用户请求，处理请求逻辑，显示数据；
     JavaBean：传输数据，处理业务逻辑；
```

## JSP开发模式2

```
JSP+Servlet+JavaBean
1.Servlet处理请求(控制器Controller)
2.JavaBean处理业务逻辑(模型Model)
3.JSP负责显示(视图View)

借鉴了MVC的架构模式
```

## MVC

```
是一种架构模式，本身不引入任何的功能，只是帮助我们开发变得更加合理，使展示和模型分离，逻辑控制与业务逻辑分开；

Model(模型)：提供要展示的数据，处理业务逻辑；
View(视图)：负责进行模型数据的展示，主要用于显示；
Controller(控制器):接收用户请求，委托模型进行处理，处理完毕后再将数据交给视图进行显示；实际上就是一个调度员；

MVC开发也会遇到问题，业务逻辑太复杂的项目不适用，所以采用MVC+三层架构的模式；
```

## Web MVC

```
Model--JavaBean（被称为JavaBean组件）(三层架构里成为域模型)
View--JSP
Controller--Servlet
```

## MVC+三层架构

```
在三层架构中，JavaBean称为域模型；
在MVC中，JavaBean称为JavaBean组件；
JavaBean组件=业务层+持久层+域模型；
结构为：
1.表示层：Servlet+JSP
2.业务层
3.持久层
4.贯穿三层的JavaBean域模型
```







[好文要顶](javascript:void(0);) [关注我](javascript:void(0);) [收藏该文](javascript:void(0);) [![img](https://common.cnblogs.com/images/icon_weibo_24.png)](javascript:void(0);) [![img](https://common.cnblogs.com/images/wechat.png)](javascript:void(0);)