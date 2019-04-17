# JavaWeb —— Servlet 接口与配置 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:26:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40








# 【Servlet 接口】

javax.servlet.Servlet 接口是最基础的 Servlet 接口，在 Servlet 开发中，任何 Servlet 都要直接或间接的实现该接口。

## 1.接口方法
- void init(ServletConfig config)：Servlet 容器调用该方法完成 Servlet 实例化后的初始化工作。
- void destroy()：Servlet 对象从 Servlet 容器移除后，容器调用该方法释放资源。
- void service(ServletRequest request,ServletResponse response)：处理客户端请求。
- ServletConfig getServletConfig()：获取 Servlet 对象配置信息。
- String getServletInfo()：返回有关 Servlet 的信息，如作者、版本等。

## 2.与 Servlet 生命周期有关的方法

在 Servlet 接口中，与 Servlet 生命周期有关的方法共有以下 4 个，且均由 Servlet 容器调用。
- 构造器：只被调用一次，只有在第一次请求 Servlet 时，创建 Servlet 实例，调用构造器。
- init()：只被调用一次，在创建好实例后立即被调用，用于初始化当前 Servlet。
- service()：可被多次调用，每次请求都会调用该方法，实际用于响应请求。
- destroy()： 只被调用一次，在当前 Servlet 所在的 Web 应用被卸载时调用，用于释放当前 Servlet 所占的资源。

## 3.Servlet 容器响应客户请求过程

① Servlet 引擎检查是否已装载并创建了该 Servlet 实例对象，若是，则执行第 ④ 步，否则执行第 ② 步。

② 装载并创建该 Servlet 的一个实例对象，并调用该 Servlet 的构造器。

③ 调用 Servlet 实例对象的 init() 方法。

④ 创建一个用于封装请求的 ServletRequest 对象和一个代表响应消息的 ServletResponse 对象，然后调用 service() 方法，并将请求和响应对象作为参数传递进去。

⑤ Web 应用被停止或重新启动前，Servlet 引擎将卸载 Servlet ，并在卸载前调用 Servlet 的 destroy() 方法。

## 4.架构图
- 第一个到达服务器的 HTTP 请求被委派到 Servlet 容器。
- Servlet 容器在调用 service() 方法之前加载 Servlet。
- 然后 Servlet 容器处理由多个线程产生的多个请求，每个线程执行一个单一的 Servlet 实例的 service() 方法。

![](https://img-blog.csdnimg.cn/20190112170619227.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# 【配置】

要使 Servlet 对象正常运行，需要进行适当的配置，以告知 Web 容器哪一个请求调用哪一个 Servlet 对象处理，对 Servlet 起到一个注册的作用。

Servlet 配置包含在 web.xml 文件中，主要分为两个步骤：声明 Servlet 对象、创建 Servlet 映射。

## 1.声明 Servlet 对象

在 web.xml 文件中，通过 <servlet> 标签声明一个 Servlet 对象，在此标签下包含两个核心子元素：<servlet-name> 用于指定 Servlet 的名称，可以自定义；<servlet-class> 用于指定 Servlet 的完整位置

<init-param> 标签用于配置 Servlet 初始化参数，其子标签 <param-name> 用于指定参数名，<param-value> 用于指定参数值

<load-on-startup> 标签可以指定 Servlet 创建的时机：若为负数，则在第一次请求时被创建；若为 0 或正数，则在当前 Web 应用被 Servlet 容器加载时创建，且数字越小越早被创建。

例如：

```
<servlet>
  <servlet-name>test</servlet-name>
  <servlet-class>servlet.Test</servlet-class>

  <init-param>
    <param-name>username</param-name>
    <param-value>root</param-value>
  </init-param>

  <init-param>
    <param-name>password</param-name>
    <param-value>123456</param-value>
  </init-param>

  <load-on-startup>1</load-on-startup>
</servlet>
```

## 2.映射 Servlet

在 web.xml 中声明了 Servlet 对象后，需要映射访问 Servlet 的 URL，该操作使用 <servlet-mapping> 标签进行配置。

<servlet-mapping> 标签包含两核心个子元素：<servlet-name> 与 <servlet> 标签中的 <servlet-name> 元素相对应，不可随意命名；<url-pattern> 用于映射访问 URL，在 Servlet 映射到的 URL 中，可以使用通配符 *，但只有两种格式：*.扩展名、/*

同一 Servlet 可以被映射到多个 URL 上，即多个 <servler-mapping> 元素的 <servlet-name> 子元素的设置值可以是同一个 Servlet 的注册名。

例如：

```
<servlet-mapping>
  <servlet-name>test</servlet-name>
  <url-pattern>/servlet/Test</url-pattern>
</servlet-mapping>

<servlet-mapping>
  <servlet-name>test</servlet-name>
  <url-pattern>/*</url-pattern>
</servlet-mapping>
```





