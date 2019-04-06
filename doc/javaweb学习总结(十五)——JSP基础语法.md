## [javaweb学习总结(十五)——JSP基础语法](https://www.cnblogs.com/xdp-gacl/p/3776512.html)

　　任何语言都有自己的语法，JAVA中有，JSP虽然是在JAVA上的一种应用，但是依然有其自己扩充的语法，而且在JSP中，所有的JAVA语句都可以使用。

## 一、JSP模版元素

　　JSP页面中的HTML内容称之为JSP模版元素。 
　　JSP模版元素定义了网页的基本骨架，即定义了页面的结构和外观。

## 二、JSP表达式

　　JSP脚本表达式（expression）用于将程序数据输出到客户端
    　　语法：**<%= 变量或表达式 %>**
    　　**举例：输出当前系统时间:**

```
1 <%= new java.util.Date() %> 
```

　　JSP引擎在翻译脚本表达式时，会将程序数据转成字符串，然后在相应位置用out.print(…) 将数据输给客户端。
　　JSP脚本表达式中的变量或表达式后面**不能有分号（;）**。

## 三、JSP脚本片断

　　JSP脚本片断(scriptlet)用于在JSP页面中编写多行Java代码。语法：
　　　　**<%** 
        　　　　**多行java代码** 
　　　　**%>** 

　　**在<% %>中可以定义变量、编写语句，不能定义方法。**

范例：在Scriptlet中定义变量、编写语句

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 <%
2     int sum=0;//声明变量
3 
4     /*编写语句*/
5     for (int i=1;i<=100;i++){
6         sum+=i;
7     }
8     out.println("<h1>Sum="+sum+"</h1>");
9 %>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　注意事项：

- JSP脚本片断中只能出现java代码，不能出现其它模板元素， JSP引擎在翻译JSP页面中，会将JSP脚本片断中的Java代码将被原封不动地放到Servlet的_jspService方法中。
- JSP脚本片断中的Java代码必须严格遵循Java语法，例如，每执行语句后面必须用分号（;）结束。
- 在一个JSP页面中可以有多个脚本片断，在两个或多个脚本片断之间可以嵌入文本、HTML标记和其他JSP元素。

​    举例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 <%
2     int x = 10;
3     out.println(x);
4 %>
5 <p>这是JSP页面文本</p>
6 <%
7     int y = 20;
8     out.println(y);
9 %>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　多个脚本片断中的代码可以相互访问，犹如将所有的代码放在一对<%%>之中的情况。如：out.println(x);
　　单个脚本片断中的Java语句可以是不完整的，但是，多个脚本片断组合后的结果必须是完整的Java语句，例如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 <%
2     for (int i=1; i<5; i++) 
3     {
4 %>
5     <H1>http://localhost:8080/JavaWeb_Jsp_Study_20140603/</H1>
6 <%
7     }
8 %>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 四、JSP声明

　　**JSP页面中编写的所有代码，默认会翻译到servlet的service方法中， 而Jsp声明中的java代码被翻译到_jspService方法的外面。**语法：
　　　　**<%！** 
    　　　　**java代码**
　　　　**%>**
　　所以，**JSP声明可用于定义JSP页面转换成的Servlet程序的静态代码块、成员变量和方法** 。 
　　多个静态代码块、变量和函数可以定义在一个JSP声明中，也可以分别单独定义在多个JSP声明中。
　　JSP隐式对象的作用范围仅限于Servlet的_jspService方法，所以在JSP声明中不能使用这些隐式对象。

　　JSP声明案例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%!
 2 static { 
 3     System.out.println("loading Servlet!"); 
 4 }
 5 
 6 private int globalVar = 0;
 7 
 8 public void jspInit(){
 9     System.out.println("initializing jsp!");
10 }
11 %>
12 
13 <%!
14 public void jspDestroy(){
15     System.out.println("destroying jsp!");
16 }
17 %>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 五、JSP注释

在JSP中，注释有两大类：

   显式注释：直接使用**HTML**风格的注释：**<!- -** **注释内容****- ->**

   隐式注释：直接使用**JAVA**的注释：**//**、**/\*****……*****/**

　**JSP**自己的注释：**<%- -** **注释内容****- -%>**

**这三种注释的区别**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <!--这个注释可以看见-->
 2 
 3 <%
 4     //JAVA中的单行注释
 5 
 6     /*
 7         JAVA中的多行注释
 8     */
 9 %>
10 
11 <%--JSP自己的注释--%>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　HTML的注释在浏览器中查看源文件的时候是可以看得到的，而JAVA注释和JSP注释在浏览器中查看源文件时是看不到注释的内容的，这就是这三种注释的区别。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)