## [JavaWeb学习总结(十三)——使用Session防止表单重复提交](https://www.cnblogs.com/xdp-gacl/p/3859416.html)

　　在平时开发中，如果网速比较慢的情况下，用户提交表单后，发现服务器半天都没有响应，那么用户可能会以为是自己没有提交表单，就会再点击提交按钮重复提交表单，我们在开发中必须防止表单重复提交。

## 一、表单重复提交的常见应用场景

有如下的form.jsp页面

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>Form表单</title>
 6   </head>
 7   
 8   <body>
 9       <form action="${pageContext.request.contextPath}/servlet/DoFormServlet" method="post">
10         用户名：<input type="text" name="username">
11         <input type="submit" value="提交" id="submit">
12     </form>
13   </body>
14 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　form表单提交到DoFormServlet进行处理

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package xdp.gacl.session;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.ServletException;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.servlet.http.HttpServletRequest;
 7 import javax.servlet.http.HttpServletResponse;
 8 
 9 public class DoFormServlet extends HttpServlet {
10 
11     public void doGet(HttpServletRequest request, HttpServletResponse response)
12             throws ServletException, IOException {
13         //客户端是以UTF-8编码传输数据到服务器端的，所以需要设置服务器端以UTF-8的编码进行接收，否则对于中文数据就会产生乱码
14         request.setCharacterEncoding("UTF-8");
15         String userName = request.getParameter("username");
16         try {
17             //让当前的线程睡眠3秒钟，模拟网络延迟而导致表单重复提交的现象
18             Thread.sleep(3*1000);
19         } catch (InterruptedException e) {
20             e.printStackTrace();
21         }
22         System.out.println("向数据库中插入数据："+userName);
23     }
24 
25     public void doPost(HttpServletRequest request, HttpServletResponse response)
26             throws ServletException, IOException {
27         doGet(request, response);
28     }
29 
30 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　如果没有进行form表单重复提交处理，那么在网络延迟的情况下下面的操作将会导致form表单重复提交多次

### **1.1、场景一：在网络延迟的情况下让用户有时间点击多次submit按钮导致表单重复提交**

　　演示动画如下所示：

![img](https://images0.cnblogs.com/i/289233/201407/212301006198192.gif)

###  1.2、场景二：表单提交后用户点击【刷新】按钮导致表单重复提交

演示动画如下所示：

![img](https://images0.cnblogs.com/i/289233/201407/212310387296176.gif)

　　点击浏览器的刷新按钮，就是把浏览器上次做的事情再做一次，因为这样也会导致表单重复提交。

### 1.3、场景三：用户提交表单后，点击浏览器的【后退】按钮回退到表单页面后进行再次提交

演示动画如下所示：

![img](https://images0.cnblogs.com/i/289233/201407/212318370414550.gif)

## 二、利用JavaScript防止表单重复提交

　　既然存在上述所说的表单重复提交问题，那么我们就要想办法解决，比较常用的方法是采用JavaScript来防止表单重复提交，具体做法如下：

修改form.jsp页面，添加如下的JavaScript代码来防止表单重复提交

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>Form表单</title>
 6         <script type="text/javascript">
 7         var isCommitted = false;//表单是否已经提交标识，默认为false
 8         function dosubmit(){
 9             if(isCommitted==false){
10                 isCommitted = true;//提交表单后，将表单是否已经提交标识设置为true
11                 return true;//返回true让表单正常提交
12             }else{
13                 return false;//返回false那么表单将不提交
14             }
15         }
16     </script>
17   </head>
18   
19   <body>
20       <form action="${pageContext.request.contextPath}/servlet/DoFormServlet" onsubmit="return dosubmit()" method="post">
21         用户名：<input type="text" name="username">
22         <input type="submit" value="提交" id="submit">
23     </form>
24   </body>
25 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　我们看看使用了JavaScript来防止表单提交重复是否可以成功，运行效果如下：

![img](https://images0.cnblogs.com/i/289233/201407/212330499471376.gif)

　　可以看到，针对"**在网络延迟的情况下让用户有时间点击多次submit按钮导致表单重复提交**"这个应用场景，使用JavaScript是可以解决这个问题的，**解决的做法就是**"**用JavaScript控制Form表单只能提交一次**"。

　　除了用这种方式之外，经常见的另一种方式就是表单提交之后，将提交按钮设置为不可用，让用户没有机会点击第二次提交按钮，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 function dosubmit(){
2     //获取表单提交按钮
3     var btnSubmit = document.getElementById("submit");
4     //将表单提交按钮设置为不可用，这样就可以避免用户再次点击提交按钮
5     btnSubmit.disabled= "disabled";
6     //返回true让表单可以正常提交
7     return true;
8 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行效果如下：

![img](https://images0.cnblogs.com/i/289233/201407/212340214327750.gif)

 　　另外还有一种做法就是提交表单后，将提交按钮隐藏起来，这种做法和将提交按钮设置为不可用是差不多的，个人觉得将提交按钮隐藏影响到页面布局的美观，并且可能会让用户误以为是bug(怎么我一点击按钮，按钮就不见了呢？用户可能会有这样的疑问)，我个人在开发中用得比较多的是表单提交后，将提交按钮设置为不可用，反正使用JavaScript防止表单重复提交的做法都是差不多的，目的都是让表单只能提交一次，这样就可以做到表单不重复提交了。

　　使用JavaScript防止表单重复提交的做法只对上述提交到导致表单重复提交的三种场景中的【场景一】有效，而对于【场景二】和【场景三】是没有用，依然无法解决表单重复提交问题。

## 三、利用Session防止表单重复提交

　　对于【场景二】和【场景三】导致表单重复提交的问题，既然客户端无法解决，那么就在服务器端解决，在服务器端解决就需要用到session了。

　　具体的做法：**在服务器端生成一个唯一的随机标识号，专业术语称为Token(令牌)，同时在当前用户的Session域中保存这个Token。然后将Token发送到客户端的Form表单中，在Form表单中使用隐藏域来存储这个Token，表单提交的时候连同这个Token一起提交到服务器端，然后在服务器端判断客户端提交上来的Token与服务器端生成的Token是否一致，如果不一致，那就是重复提交了，此时服务器端就可以不处理重复提交的表单。如果相同则处理表单提交，处理完后清除当前用户的Session域中存储的标识号。**
　　在下列情况下，服务器程序将拒绝处理用户提交的表单请求：

1. **存储Session域中的Token(令牌)与表单提交的Token(令牌)不同。**
2. **当前用户的Session中不存在Token(令牌)。**
3. **用户提交的表单数据中没有Token(令牌)。**

看具体的范例：

　　1.创建FormServlet，用于生成**Token(令牌)**和跳转到form.jsp页面

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package xdp.gacl.session;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.ServletException;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.servlet.http.HttpServletRequest;
 7 import javax.servlet.http.HttpServletResponse;
 8 
 9 public class FormServlet extends HttpServlet {
10     private static final long serialVersionUID = -884689940866074733L;
11 
12     public void doGet(HttpServletRequest request, HttpServletResponse response)
13             throws ServletException, IOException {
14 
15         String token = TokenProccessor.getInstance().makeToken();//创建令牌
16         System.out.println("在FormServlet中生成的token："+token);
17         request.getSession().setAttribute("token", token);  //在服务器使用session保存token(令牌)
18         request.getRequestDispatcher("/form.jsp").forward(request, response);//跳转到form.jsp页面
19     }
20 
21     public void doPost(HttpServletRequest request, HttpServletResponse response)
22             throws ServletException, IOException {
23         doGet(request, response);
24     }
25 
26 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　2.在form.jsp中使用隐藏域来存储**Token(令牌)**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
 3 <html>
 4 <head>
 5 <title>form表单</title>
 6 </head>
 7 
 8 <body>
 9     <form action="${pageContext.request.contextPath}/servlet/DoFormServlet" method="post">
10         <%--使用隐藏域存储生成的token--%>
11         <%--
12             <input type="hidden" name="token" value="<%=session.getAttribute("token") %>">
13         --%>
14         <%--使用EL表达式取出存储在session中的token--%>
15         <input type="hidden" name="token" value="${token}"/> 
16         用户名：<input type="text" name="username"> 
17         <input type="submit" value="提交">
18     </form>
19 </body>
20 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　3.DoFormServlet处理表单提交

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package xdp.gacl.session;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.ServletException;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.servlet.http.HttpServletRequest;
 7 import javax.servlet.http.HttpServletResponse;
 8 
 9 public class DoFormServlet extends HttpServlet {
10 
11     public void doGet(HttpServletRequest request, HttpServletResponse response)
12                 throws ServletException, IOException {
13 
14             boolean b = isRepeatSubmit(request);//判断用户是否是重复提交
15             if(b==true){
16                 System.out.println("请不要重复提交");
17                 return;
18             }
19             request.getSession().removeAttribute("token");//移除session中的token
20             System.out.println("处理用户提交请求！！");
21         }
22         
23         /**
24          * 判断客户端提交上来的令牌和服务器端生成的令牌是否一致
25          * @param request
26          * @return 
27          *         true 用户重复提交了表单 
28          *         false 用户没有重复提交表单
29          */
30         private boolean isRepeatSubmit(HttpServletRequest request) {
31             String client_token = request.getParameter("token");
32             //1、如果用户提交的表单数据中没有token，则用户是重复提交了表单
33             if(client_token==null){
34                 return true;
35             }
36             //取出存储在Session中的token
37             String server_token = (String) request.getSession().getAttribute("token");
38             //2、如果当前用户的Session中不存在Token(令牌)，则用户是重复提交了表单
39             if(server_token==null){
40                 return true;
41             }
42             //3、存储在Session中的Token(令牌)与表单提交的Token(令牌)不同，则用户是重复提交了表单
43             if(!client_token.equals(server_token)){
44                 return true;
45             }
46             
47             return false;
48         }
49 
50     public void doPost(HttpServletRequest request, HttpServletResponse response)
51             throws ServletException, IOException {
52         doGet(request, response);
53     }
54 
55 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　生成Token的工具类TokenProccessor

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package xdp.gacl.session;
 2 
 3 import java.security.MessageDigest;
 4 import java.security.NoSuchAlgorithmException;
 5 import java.util.Random;
 6 import sun.misc.BASE64Encoder;
 7 
 8 public class TokenProccessor {
 9 
10     /*
11      *单例设计模式（保证类的对象在内存中只有一个）
12      *1、把类的构造函数私有
13      *2、自己创建一个类的对象
14      *3、对外提供一个公共的方法，返回类的对象
15      */
16     private TokenProccessor(){}
17     
18     private static final TokenProccessor instance = new TokenProccessor();
19     
20     /**
21      * 返回类的对象
22      * @return
23      */
24     public static TokenProccessor getInstance(){
25         return instance;
26     }
27     
28     /**
29      * 生成Token
30      * Token：Nv6RRuGEVvmGjB+jimI/gw==
31      * @return
32      */
33     public String makeToken(){  //checkException
34         //  7346734837483  834u938493493849384  43434384
35         String token = (System.currentTimeMillis() + new Random().nextInt(999999999)) + "";
36         //数据指纹   128位长   16个字节  md5
37         try {
38             MessageDigest md = MessageDigest.getInstance("md5");
39             byte md5[] =  md.digest(token.getBytes());
40             //base64编码--任意二进制编码明文字符   adfsdfsdfsf
41             BASE64Encoder encoder = new BASE64Encoder();
42             return encoder.encode(md5);
43         } catch (NoSuchAlgorithmException e) {
44             throw new RuntimeException(e);
45         }
46     }
47 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　首先访问FormServlet，在FormServlet中生成Token之后再重定向到form.jsp页面，这次是在服务器端处理表单重复提交的，运行效果如下：

![img](https://images0.cnblogs.com/i/289233/201407/222101334003765.gif)

　　从运行效果中可以看到，通过这种方式处理表单重复提交，可以解决上述的场景二和场景三中出现的表单重复提交问题。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)