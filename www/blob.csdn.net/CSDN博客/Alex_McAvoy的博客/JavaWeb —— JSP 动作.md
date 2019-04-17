# JavaWeb —— JSP 动作 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37
所属专栏：[JavaWeb 学习笔记](https://blog.csdn.net/column/details/34361.html)









# 【概述】

JSP 提供了一种称为 Action 的元素，在 JSP 页面中使用 Action 元素可以完成各种通用的 JSP 页面功能，也可实现处理复杂业务逻辑的专用功能。

Action 元素采用 XML 元素的语法格式，即每个 Action 元素在 JSP 页面中均以 XML 标签的形式出现，这些元素的标签名都以 jsp 作为前缀，且全部采用小写

所有的动作要素都有两个属性：id 属性和 scope 属性，其中 id 属性是动作元素的唯一标识，可以在 JSP 页面中引用，scope 属性用于识别动作元素的声明周期， scope 属性有四个可能的值： page、request、session、application，两属性间有直接关系，scope 属性定义了相关联 id 对象的生存期。

# 【<jsp:include> 标签】

<jsp:include> 标签用于将另一个资源的输出内容插入进当前 JSP 页面的输出内容中，其不像 include 指令**静态引入**来生成一个 Servlet 源文件，而是生成两个 Servlet 源文件，然后通过一个方法来将目标页面包含进来，这种引入方式称为**动态引入**。

无论是 <jsp:include> 标签还是 include 指令，都可以将一个页面的内容分成多个组件来生成，不必将页眉和页脚部分相同的 HTML 代码复制到每个 JSP 文件中，但是都应注意最终输出结果内容应遵循 HTML 语法结构，例如：当前 JSP 页面产生了 <html>、<body> 等标记，那么被引入的文件就不能再输出 <html>、<body> 等标记

语法：**<jsp:include page="relativeURL | <%=expression%> " flush="true|false" />**
- page 属性用于指定被引入资源的相对路径，其也可通过执行一个表达式来获得
- flush 属性指定在插入其他资源的输出内容时，是否先将当前 JSP 页面的已输出内容刷新到客户端，默认为 false

与 include 指令的区别

<jsp:include> 标签
- 是在当前 JSP 页面的执行期间插入被引入资源的输出内容，被动态引入的资源必须是一个能独立被 WEB 容器调用和执行的资源
- 对 JSP 引擎翻译 JSP 页面的过程不起作用，其是在 JSP 页面的执行期间被调用，不会影响两个页面的编译

include 指令
- 只能引入遵循 JSP 格式的文件，被引入文件与当前 JSP 文件共同被编译成一个 Servlet 源文件
- 是在 JSP 引擎翻译 JSP 页面过程中处理的，其对 JSP 引擎翻译 JSP 页面的过程起作用，若多个 JSP 页面中都需要用到某个共同的声明，那么就可将这些声明语句放进一个单独的文件中编写，然后在每个 JSP 页面中使用 include 指令将文件包含进来
`<jsp:include page="index.jsp" />`
# 【<jsp:forward> 标签】

<jsp:forward> 标签用于将请求转发给另外一个资源，相当于 request.getRequestDispatcher(path).forward(request,response)，但不同是，其可以使用 <jsp:param> 子标签来传递参数

语法：**<jsp:forward page="relativeURL | <%=expression%>" />**
- page 属性用于指定请求转发到的资源的相对路径，其也可以通过执行一个表达式来获得
`<jsp:forward page="welcome.jsp" />`
# 【<jsp:param> 标签】

<jsp:param> 是子标签，其在 <jsp:include>、<jsp:forward> 标签使用时传递参数

```
//可在welcome.jsp通过request.getParamter("test")获取到传入的请求参数
<jsp:forward page="welcome.jsp">
  <jsp:param name="test" value="123"/>
</jsp:forward>
```

# 【<jsp:useBean> 标签】

该标签与 JavaBean 相关，用于加载一个将在 JSP 页面中使用的 JavaBean

语法：**<jsp:useBean id="name" class="package.class" type="Object" scope="page|request|session|application"/>**
- id：通过标签实例化对象的名称
- class：指定 Bean 的完整包名
- type：指定将引用该对象变量的类型
- scope：设置 Bean 的作用域，取值为 page、request、session、application，默认情况下为 page

在载入后，可以通过 <jsp:getProperty> 和 <jsp:setProperty> 动作来检索和修改 bean 的属性。

通过 <jsp:useBean> 实例化的 JavaBean 对象，若在 JSP 页面中使用 Java 代码调用 JavaBean 对象中的属性或方法，所使用的 JavaBean 对象的变量名为 <jsp:useBean> 标签中的 id 属性
`<jsp:useBean id="produce" class="javabean.Produce" />`
# 【<jsp:getProperty> 标签】

<jsp:getProperty> 标签与 JavaBean 有关，用于获取指定 Bean 属性的值，转换成字符串，然后输出，其要与 <jsp:useBean> 配合使用。

语法：**<jsp:getProperty name="className" property="someProperty" />**
- name：要检索的 Bean 的类名，要求 Bean 必须已定义。
- property：要提取的 Bean 属性名称。

```
<jsp:useBean id="produce" class="javabean.Produce">
    Name：<jsp:getProperty property="name" name="produce"/>
</jsp:useBean>
```

# 【 <jsp:setProperty> 标签】

<jsp:setProperty> 标签与 JavaBean 有关，用于设置已实例化的 Bean 对象的属性，其要与 <jsp:useBean> 配合使用。

语法格式：**<jsp:setProperty name="className" property="someProperty" value|param="someValue" />**
- name：要检索的 Bean 的类名，要求 Bean 必须已定义。
- property：要设置的 Bean 属性名。
- value：用来指定 Bean 属性的值，将自定义属性作为值注入到该 property 中。
- param：用来指定 Bean 属性的值，param 是将请求参数（前端表单数据等）作为值注入到该 property 中。

```
<jsp:useBean id="produce" class="javabean.Produce">
    Name：<jsp:setProperty name="produce" property="name" value="123" />
</jsp:useBean>
```





