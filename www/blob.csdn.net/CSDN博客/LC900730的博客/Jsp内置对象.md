# Jsp内置对象 - LC900730的博客 - CSDN博客
2017年07月26日 20:22:04[lc900730](https://me.csdn.net/LC900730)阅读数：197标签：[jsp](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 内置对象
在jsp开发中，需要经常用到HttpSession，ServletContext，ServletConfig，HttpServletRequest 
如Servlet中： 
    HttpServlet session=request.getSession(true); （需要开发者自己做） 
jsp： 
    tomcat服务器  HttpSession session=request.getSession (不需要开发者自己做） 
    开发者做的 session.getId()
## 九大内置对象
|内置对象名|类型|
|----|----|
|request|HttpServletRequest|
|response|HttpServletResponse|
|config|ServletConfig|
|application|ServletContext|
|session|HttpSession|
|exception|Throwable|
|page|Object(this)|
|out|jspWriter|
|pageContext|PageContext|
#### Out对象
```
out对象类型：JspWriter类，相当于带缓存的PrintWriter
```
PrintWriter： 
    writer(内容)：直接往浏览器写，不带缓冲
```
//剩余缓冲区大小
    1. out.getRemaining();
    //查看缓冲区大小
    2.out.getBufferSise();
```
Jsp页面：建议不要用PrintWriter，用JspWriter的writer(内容)
```
先放到JspWriter缓冲区，满足一定条件在往外写。
如1.缓冲区满；
2.刷新缓冲区；
3.关闭缓冲区；
4.执行完jsp页面
```
#### PageContext
Sun公司把8个经常使用的内置对象封装到PageContext对象中 
1.从pagecontext中获取其他8个内置对象 
如： 
    JspWriter out=pageContext.getOut(); 
    HttpServletRequest requset=pageContext.getRequest();
在自定义标签的时候，PageContext会频繁使用 
2.本身也是一个域对象，在不同 
3.使用 
pageContext.setAttribute(“message”,”itcast messae”);
pageContext.setAttribute(“message”,”request message”,PageContext.REQUEST_SCOPE)；保存到request域中 
等价于： 
request.setAttribute(“message”,”request message”);
原则上：在哪个域中存的在哪个域中取数据。
取数据： 
pageContext.getAttribute(“message”,PageContext.REQUEST_SCOPE); 
request.getAttribute(“message”)
pageContext.setAttribute(“message”,”request message”)；//默认page域
pageContext.setAttribute(“message”,”request message”,PageContext.REQUEST_SCOPE)；保存到request域中
pageContext.setAttribute(“message”,”request message”,PageContext.SESSION_SCOPE)；保存到session域中
pageContext.setAttribute(“message”,”request message”,PageContext.APPLICATION_SCOPE)；保存到application域中
findAttribute()://自动搜索 
先从page域开始–>session域(session设置后可能还会存成session对象30分钟)–>application域 
pageContext.findAttribute(“message”)；
page域：在哪个页面保存，就在哪个页面获取，跳页面获取不到； 
request域：转发的时候，request对象相同可以拿到。 
session域： 
context域：范围更大，服务器不关闭，则都可以拿到。
### Jsp最佳实践
```xml
//1.从xml中取出来数据
    ContactDao dao=new ContactDaoImpl();
    List<Contact> list=dao.findAll();
    //2.把结果保存到域对象中
    request.setAttribute("contact",list);
    //3.转发到jsp页面
    request.requestDispatcher("/listContact.jsp").forward(request,response);
listContact.jsp
//从request域中接收数据
<%
    List<contact> list=(List<Contact>)request.getAttribute("contact");
    for(contact c:list){
%>  
<tr>
    <td><%=c.getId() %></td>
    <td><%=c.getName() %></td>
    <td><%=c.getQq() %></td>
    <td><%=c.getEmail() %></td>
    <td><%=c.getAge() %></td>
    <td><%=c.getGender() %></td>
</tr>
<%
    }
%>
```
## EL表达式
向浏览器中输出域对象中的变量值或者表达式
1.输出基本数据类型 
    从4个域中获取 
${name}  2.      //在哪个域中保存，就在哪个域中获取      ${pageScope.name} ;//指定域中获取 
    ${pageScope.name}等价于 
    <%=pageContext.getAttribute(“name”,PageContext.PAGE_SCOPE) %>
3.输出对象的属性值
```
${student.name}    -${student.age}
        等价于
        (Student)pageContext.findAttribute("student").getName();
        并不是去访问对象的私有属性
```
因此对象如果没有getter方法，那么el会报错
4.输出集合对象 
    4.1List集合
```php
<%
     Student student=new Student("eric",20);
     pageContext.setAttribute("student",student);
     List<Student> list=new ArrayList<Student>();
     list.add(new Student("eric1",21));
     list.add(new Student("eric2",22));
     list.add(new Student("eric3",23));
     pageContext.setAttribute("list",list);
%>
    ${list[0]}
    //等价于(List).pageContext.findAttribute("list").get(0);
    ${list[0].name}
    ${list[0].age}
```
```
4.2Map集合
```
```
<%
     Map<String,Student> map=new HashMap<String,Student>();
     map.put("100",new Student("eric1",21));
     map.put("101",new Student("eric2",22));
     map.put("102",new Student("eric3",23));
     pageContext.setAttribute("map",map);
%>
//使用EL获取map对象
     ${map['100'].name}
     ${map['100'].age}
```
5.输出表达式 
        5.1逻辑 
    ${10>5}     //true 
        5.2判空 
    null或者空字符串
```
<%
        String name="eric";
        pageContext.setAttribute("name",name);
    %>
    判断null ：${name==null}
    判断空字符串：${name==""}
    两种都需要判断
        ${name==null||name==""}   <====>     ${empty name}
```
## jsp标签
jsp标签的作用：替换jsp脚本
```
1.流程判断(if for循环)
2.跳转页面(转发  重定向)
```
标签分类： 
    1.内置标签(动作标签)：不需要在jsp页面导入标签 
    2.jstl标签：需要在jsp页面中导入标签 
    3.自定义标签：开发者自己定义，也需要在jsp中导入标签
### 动作标签
转发标签：
```
request.getRequestDispatcher("/06hello.jsp").forward(request, response);
等价于
<jsp:forward page="/06hello.jsp"></jsp:forward>
```
传参数： 
方式1： 
方式2： 
```
接收页面：
    request.getParameter("name");
    request.getParameter("password);
包含标签：<jsp:include/>
```
原理：包含与被包含的页面各自翻译成java源文件，然后再运行的时候合并在一起(先翻译在合并)，动态包含 
    静态包含 vs 动态包含
#### 1.语法不同
```
静态包含语法：
        <%@include file="被包含的页面">
    动态包含语法：
        <jsp:include page="被包含的页面">
```
#### 2.传参方式不同
```
静态包含的话 
    file="XXX"是文件名字，不可能跟上？带参数(同一个页面无法传参)
    动态包含可以向被包含的页面传递参数
```
#### 3.原理不同
```
静态：先合并在翻译
动态：先翻译在合并
```
### Jstl标签
java standard tag libarary：java标准标签库
#### 核心标签库：C标签库
#### 国际化标签库：fmt标签库
#### xml标签库：x标签库
#### sql标签库：sql标签库
使用Jstl标签库的步骤 
1.导入
