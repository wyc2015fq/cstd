# JavaWeb —— EL 表达式 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27








# 【概述】

EL（Expression Language）是为了让 JSP 写起来更加简单而出现的一种表达式语言，在 EL 未出现前，常在 JSP 页面中嵌入大量的 Java 代码，使得页面看起来十分繁杂，EL 表达式提供了在 JSP 中简化的方法，让 JSP 的代码更加简洁。

其主要有 4 个用法：
- 访问数据：替换 JSP 页面中的脚本表达式，从各种类型的 web 域中检索 Java 对象获取数据
- 简单运算：在 JSP 页面中执行一些基本的关系运算、逻辑运算和算术运算
- 获取常用 Web 对象：利用 EL 表达式定义的隐式对象，可以获得对 Web 常用对象的引用，从而获得对象中的数据
- 定义、使用 EL 函数：EL 表达式允许用户自定义 EL 函数，以在 JSP 页面中通过 EL 表达式调用 Java 的方法

# 【基本语法】

## 1.格式
- **${expression}：均以 ${ 开头，｝结尾**

expression：合法的表达式，可以是字符串，可以是由 EL 运算符组成的表达式

例如：将保存在 session 范围内的变量 username 输出到 JSP 页面中

```
<!-- 使用Java代码片段实现 -->
<%
   if(session.getAttribute("username")!=null)
       out.println(session.getAttribute("username").toString());
%>

<!-- 使用EL表达式实现 -->
${username}
```

## 2.保留关键字

同所有的程序设计语言一样，EL 也有属于自己的保留关键字，在使用 EL 表达式时，应避免使用这些保留关键字
|and|or|not|
|----|----|----|
|eq|ne|lt|
|ge|le|ge|
|instanceof|true|false|
|empty|mod|div|

若在 EL 中使用了保留关键字，那么在 IDE 中将会给出错误提示。

## 3.运算符与优先级

EL 可以访问数据运算符、算术运算符、关系运算符、逻辑运算符、条件运算符、empty 运算符等，运算符的优先级决定了在多个运算符同时存在时的求值顺序，对于同级运算符，采用从左到右计算原则进行计算，灵活使用括号可以使表达式更易阅读且避免出错，各运算符优先级如下：
|等级(由高到低)|优先级|
|----|----|
|1|[]、.|
|2|0|
|3|-(负)、not、!、empty|
|4|*、/、div、%、mod|
|5|+、-(减)|
|6|<、>、<=、>=、lt、gt、le、ge|
|7|==、!-、eq、ne|
|8|&&、and|
|9|||、or|
|10|?:|

# 【访问数据】

通过 EL 提供的 " [ ] "、" . " 运算符可以访问数据，通常情况下，两者是等价的，可以互相代替。

但当对象属性名中包含一些特殊符号时，就只能使用 " [ ] " 来访问数据，" [ ] " 还可以获取数组与 List 集合的指定元素。

```
<!-- 对象含有特殊符号 -->
${sessionScope["com.javabean.test"].Test}

<!-- 访问JavaBean对象userInfo的id属性 -->
${userInfo.id}
${userInfo[id]}

<!-- 获取request范围中的数组arr中的第一个元素 -->
${arr[0]}
```

实例：向session域中保存一个 List 集合对象，通过 EL 输出的全部元素

```
<% List<String> list=new ArrayList<String>();
   list.add("abc");
   list.add("xyz");
   list.add("ABC");
   session.setAttribute("letters",list);  //将list保存到session对象中
%>
<% List<String> list1=(List<String>)session.getAttribute("letter");  //获取保存在session范围内的变量
   for(int i=0;i<arr1.length;i++)  //将循环变量i保存到request范围内的变量中
       request.setAttribute("num",i);
%>
${letters[num]}<br>  //输出list中第i个元素
```

# 【简单运算】

## 1.算术运算

同 Java 一样，EL 提供了加、减、乘、除、求模五种运算，各运算符及用法如下：
- 加：+
- 减：-
- 乘：*
- 除：/、div
- 求模：%、mod

要注意的是，EL 中的 + 与 Java 中的 + 不同，其不可实现两个字符串间的连接，若使用该运算符连接两个不可转换为数值型的字符串，将抛出异常。此外，在进行除法运算时，若 0 作为除数，不会抛出异常，而是返回无穷大 Infinity。而进行求模运算时，若 0 作为模数，则会抛出异常。

## 2.逻辑关系运算

### 1）关系运算符

关系运算符用于实现对两个表达式的比较，进行比较的表达式可以是数值型也可以是字符串型，其结果是 Boolean 型。
- 等于：==、eq
- 不等于：!=、ne
- 小于：<、lt
- 大于：>、gt
- 小于等于：<=、le
- 大于等于：>=、ge

### 2）逻辑运算符

逻辑运算符条件表达式的值必须是 Boolean 型或可以转换成 Boolean 型的字符串("false"、"true")，其常与关系运算符一起使用。
- 与：&&、and
- 或：||、or
- 非：！、not

在进行逻辑运算时，在表达式的值确定时停止。例如：在 A and B and C 中，若 A 为 true，B 为 false，则计算到 A and B 即停止，结果返回 false

## 3.条件运算

在 EL 中进行条件运算，与 Java 中用法完全一致。通常情况下，条件运算符可以使用 JSTL 中的条件标签 <c:if> 或 <c:choose> 替代。

条件运算格式：${条件表达式：表达式 1 ？表达式 2}，条件表达式为真，返回表达式 1，否则返回表达式 2

## 4.**判断对象是否为空**

使用 empty 运算符，将其置于操作数前方即可判断一个对象或变量是否为 null 或空。其也可与 not 运算符结合使用，判断一个对象或变量是否为非空。

一个对象或变量为 null 或空的意义是不同的，null 表示这个变量没有指明任何对象，而空表示这个变量所属的对象的内容为空。

```
<% request.setAttribute("user1",""); %>
<% request.setAttribute("user2",null); %>
<% request.setAttribute("user3","abc"); %>

${empty user1}      //返回值为true
${empty user2}      //返回值为true
${not empty user3}  //返回值为false
```

# 【获取常用 Web 对象】 

EL 提供了 11 个隐式对象，类似于 JSP 的内置对象，可以通过对象名直接操作，除 pageContext 是 JavaBean 对象，对应于 javax.servlet.jsp.PageContext 类型，其余的对象均对应于 java.util.Map 类型。

## 1.页面上下文对象

页面上下文对象即 pageContext 对象，用于访问 JSP 的部分内置对象和 ServletContext。
- 访问 request 对象：${pageContext.request}
- 访问 response 对象：${pageContext.response}
- 访问 out 对象：${pageContext.out}
- 访问 session 对象：${pageContext.session}
- 访问 exception 对象：${pageContext.exception}
- 访问 page 对象：${pageContext.page}
- 访问 servletContext 对象：${pageContext.ServletContext}

在获取到这些对象后，即可获取其属性值，这些属性与对象的 getXXX() 方法相对应，在使用时，去掉方法名中的 get 并将首字母改为小写即可。

例如：
- 获取响应类型：${pageContext.response.contentType}
- 获取上下文路径：${pageContext.ServletContext.contextPath}

## 2.访问作用域范围内的隐式对象

用于访问作用域范围的隐式对象有 4 个：
- 访问 page 对象：pageScope
- 访问 request 对象：requestScope
- 访问 session 对象：sessionScope
- 访问 application 对象：applicationScope

他们返回各自范围内的属性值的集合，返回值均为 java.util.Map 对象，与 JSP 中相应的内置对象类似，但只能用来取得指定范围内的属性值。

例如：
- 获取 request 范围内的 user 变量：${requestScope.user}
- 获取 session 范围内的 manager 变量：${sessionScope.manager}

## 3.访问环境信息的隐式对象

EL 中，提供了 6 个访问环境信息的隐式对象：
- 获取单个请求参数的单个值：${param.参数名}，返回结果为字符串
- 获取单个请求参数的多个值：${paramValues.参数名}，返回结果为数组
- 获取 HTTP 请求的只有一个 header 的值：${header.属性名}
- 获取 HTTP 请求的拥有多个不同值的 header 的值：${header["属性名"]}
- 获取 Web 应用初始化参数值：${initParam.参数名}
- 获取 cookie 对象：${cookie.对象名}（若想获得某个 cookie 对象的值，可使用 ${cookie.对象名.value}）

# 【定义与使用 EL 函数】

EL 函数的定义与使用分 3 个步骤：
- 编写一个 Java 类，在该类中编写公用的静态方法，用于实现自定义 EL 函数的具体功能
- 编写标签库描述文件，对函数进行声明，文件的扩展名为 .tld，保存至 Web 应用的 WEB-INF 文件夹中
- 在 JSP 页面中引用标签库，并调用定义的 EL 函数，实现相应功能

 TLD 文件中对 EL 函数的描述

```
<!-- 描述tld文件 -->
<description>描述</description>
<display-name>名称</display-name>
<tlib-version>版本</tlib-version>
 
<!-- 建议在JSP页面上使用的标签前缀 -->
<short-name>test</short-name>
<!-- 作为tld文件的id，用于唯一标识当前tld文件 -->
<uri>http://www.test.com/myel/core</uri>
 
<!-- 描述自定义的EL函数 -->
<function>
    <!-- 函数名 -->
    <function-class>函数名</function-class>
    <!-- 函数所在类 -->
    <function-class>com.el.Test</function-class>
    <!-- 函数的方法 -->
    <function-signature>返回类型 函数名(参数类型)</function-signature>
</function>
```

例如：拼接字符串的 EL 函数

1.编写静态方法

```java
public class Test{
    public static String concat(String str1,String str2){
        return str1+str2;
    }
}
```

2.在标签库中表述

```
<!-- 建议在JSP页面上使用的标签前缀 -->
<short-name>test</short-name>
<!-- 作为tld文件的id，用于唯一标识当前tld文件 -->
<uri>http://www.test.com/myel/core</uri>

<!-- 描述自定义的EL函数 -->
<function>
    <!-- 函数名 -->
    <function-name>test</function-name>
    <!-- 函数所在类 -->
    <function-class>com.el.Test</function-class>
    <!-- 函数的方法 -->
    <function-signature>java.lang.String test(java.lang.String, java.lang.String)</function-signature>
</function>
```

3.在 JSP 页面中使用

```
<!-- 引入标签库 -->
<%@ taglib prefix="fn" uri="http://www.test.com/myel/core" %>
<!-- 使用自定义EL函数 -->
${fn:test(param.name1,param.name2) }
```



