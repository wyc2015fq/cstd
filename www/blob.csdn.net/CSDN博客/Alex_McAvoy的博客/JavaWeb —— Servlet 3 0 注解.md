# JavaWeb —— Servlet 3.0 注解 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：17








# 【概述】

在 Servlet 3.0 中，通过使用注解就无须在 web.xml 文件中对 Servlet/Filter/Listener 进行配置，注解定义在类之前，会在部署时被容器处理，容器根据具体的属性配置将相应的类部署为 Servlet/Filter/Listener 类，不同的注解包含不同的属性，通过对注解属性的设置，可以达到与 web.xml 文件中配置标签对相同的效果。

新增的注解有：
- @WebServlet：定义 Servlet 组件
- @WebFilter：定义 Filter 组件
- @WebListener：定义 Listener 组件
- @WebInitParam：

# 【@WebServlet】

@WebServlet 注解，用于定义 Servlet 组件，其属性有：
- name：String 类型，用于指定 Servlet 的 name 属性，等价于 <servlet-name>，若没有显示指定，默认取值为类的全限定名
- urlPattern：String[] 类型，用于指定一组 Servlet 的 URL 匹配模式，等价于 <url-pattern> 标签
- value：等价于 urlPattern 属性，且两属性不能同时使用
- loadOnStartup：int 类型，用于指定 Servlet 的加载顺序，等价于 <load-on-startup> 标签
- initParams：WebInitParam[] 类型，用于指定一组 Servlet 初始化参数，等价于 <init-param> 标签
- asyncSupported：boolean 类型，用于声明该 Servlet 是否支持异步操作模式，等价于 <async-supported> 标签
- description：String 类型，为当前 Servlet 的描述信息，等价于 <description> 标签
- displayName：String 类型，为当前 Servlet 的显示名，等价于 <display-name> 标签

例如：

```java
@WebServlet(name="Test",urlPatterns="/Test")
public class Test extends HttpServlet {
    ...
}
```

# 【@WebFilter】

@WebFilter 注解用于定义 Filter 组件，其属性有：
- filterName：String 类型，用于指定 Filter 的 name 属性，等价于 <filter-name>，若没有显示指定，默认取值为类的全限定名
- urlPattern：String[] 类型，用于指定一组 Servlet 的 URL 匹配模式，等价于 <url-pattern> 标签
- value：等价于 urlPattern 属性，且两属性不能同时使用
- servletNames：String[] 类型，用于指定过滤器将应用于那些 Servlet，是 @WebServlet 中的 name 属性的取值
- initParams：WebInitParam[] 类型，用于指定一组 Filter 初始化参数，等价于 <init-param> 标签
- asyncSupported：boolean 类型，用于声明该 Filter 是否支持异步操作模式，等价于 <async-supported> 标签
- description：String 类型，为当前 Filter 的描述信息，等价于 <description> 标签
- displayName：String 类型，为当前 Filter 的显示名，等价于 <display-name> 标签
- dispatcherTypes：DispatcherType 类型，用于指定 Filter 的转发模式，具体取值包括：ASYNC、ERROR、FORWARD、INCLUDE、REQUEST

例如：

```java
@WebFilter(filterName="Test",urlPatterns="/*")
public class Test implements Filter {
	...
}
```

# 【@WebListener】

@WebListener 注解用于声明监听器，使用该注解可以标注一个实现监听器的类，其只有一个属性 value，该属性为可选属性，用于描述当前监听器的类。

例如：

```java
@WebListener("This is a Listener")
public class Test implements HttpSessionBindingListener {
   ...
}
```

#  【@WebInitParam】

该注解用于为 Servlet/Filter 指定初始化参数，常与 @WebServlet、@WebFilter 配合使用，等价于 web.xml 中的 <servlet> 和 <filter> 的 <init-param> 子标签，其属性有：
- name：用于指定参数名字，等价于 <param-name>
- value：用于指定参数值，等价于 <param-value>
- description：关于参数的描述，等价于 <description>

例如：

```java
@WebServlet(name="Test",urlPatterns="/Test",
    initParam={@WebInitParam(name="username",value="test")}
)
public class Test extends HttpServlet {
    ...
}
```



