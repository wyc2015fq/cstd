# JavaWeb学习总结(五十三)——Web应用中使用JavaMail发送邮件 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(五十三)——Web应用中使用JavaMail发送邮件](https://www.cnblogs.com/xdp-gacl/p/4220190.html)



　　现在很多的网站都提供有用户注册功能， 通常我们注册成功之后就会收到一封来自注册网站的邮件。邮件里面的内容可能包含了我们的注册的用户名和密码以及一个激活账户的超链接等信息。今天我们也来实现一个这样的功能，用户注册成功之后，就将用户的注册信息以Email的形式发送到用户的注册邮箱当中，实现发送邮件功能就得借助于JavaMail了。

## 一、搭建开发环境

### 1.1、创建Web项目

![](https://images0.cnblogs.com/blog/289233/201501/122321383394010.png)

### 1.2、用户注册的Jsp页面

register.jsp

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>注册页面</title>
 6   </head>
 7   
 8   <body>
 9     <form action="${pageContext.request.contextPath}/servlet/RegisterServlet" method="post">
10         用户名：<input type="text" name="username"><br/>
11         密码：<input type="password" name="password"><br/>
12         邮箱：<input type="text" name="email"><br/>
13         <input type="submit" value="注册">
14     </form>
15   </body>
16 </html>
```

### 1.3、消息提示页面

message.jsp

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>消息提示页面</title>
 6   </head>
 7   
 8   <body>
 9        ${message}
10   </body>
11 </html>
```

## 二、编写处理用户注册处理程序

### 2.1、开发封装用户注册信息的domain

User.java

```
1 package me.gacl.domain;
 2 
 3 public class User {
 4     private String username;
 5     private String password;
 6     private String email;
 7     public String getUsername() {
 8         return username;
 9     }
10     public void setUsername(String username) {
11         this.username = username;
12     }
13     public String getPassword() {
14         return password;
15     }
16     public void setPassword(String password) {
17         this.password = password;
18     }
19     public String getEmail() {
20         return email;
21     }
22     public void setEmail(String email) {
23         this.email = email;
24     }
25 }
```

### 2.2、编写邮件发送功能

　　发送邮件是一件非常耗时的事情，因此这里设计一个线程类来发送邮件

```
1 package me.gacl.web.controller;
 2 
 3 import java.util.Properties;
 4 import javax.mail.Message;
 5 import javax.mail.Session;
 6 import javax.mail.Transport;
 7 import javax.mail.internet.InternetAddress;
 8 import javax.mail.internet.MimeMessage;
 9 import me.gacl.domain.User;
10 
11 /**
12 * @ClassName: Sendmail
13 * @Description: Sendmail类继承Thread，因此Sendmail就是一个线程类，这个线程类用于给指定的用户发送Email
14 * @author: 孤傲苍狼
15 * @date: 2015-1-12 下午10:43:48
16 *
17 */ 
18 public class Sendmail extends Thread {
19         //用于给用户发送邮件的邮箱
20     private String from = "gacl@sohu.com";
21         //邮箱的用户名
22     private String username = "gacl";
23         //邮箱的密码
24     private String password = "邮箱密码";
25         //发送邮件的服务器地址
26     private String host = "smtp.sohu.com";
27     
28     private User user;
29     public Sendmail(User user){
30         this.user = user;
31     }
32     
33     /* 重写run方法的实现，在run方法中发送邮件给指定的用户
34      * @see java.lang.Thread#run()
35      */
36     @Override
37     public void run() {
38         try{
39             Properties prop = new Properties();
40             prop.setProperty("mail.host", host);
41             prop.setProperty("mail.transport.protocol", "smtp");
42             prop.setProperty("mail.smtp.auth", "true");
43             Session session = Session.getInstance(prop);
44             session.setDebug(true);
45             Transport ts = session.getTransport();
46             ts.connect(host, username, password);
47             Message message = createEmail(session,user);
48             ts.sendMessage(message, message.getAllRecipients());
49             ts.close();
50         }catch (Exception e) {
51             throw new RuntimeException(e);
52         }
53     }
54     
55     /**
56     * @Method: createEmail
57     * @Description: 创建要发送的邮件
58     * @Anthor:孤傲苍狼
59     *
60     * @param session
61     * @param user
62     * @return
63     * @throws Exception
64     */ 
65     public Message createEmail(Session session,User user) throws Exception{
66         
67         MimeMessage message = new MimeMessage(session);
68         message.setFrom(new InternetAddress(from));
69         message.setRecipient(Message.RecipientType.TO, new InternetAddress(user.getEmail()));
70         message.setSubject("用户注册邮件");
71         
72         String info = "恭喜您注册成功，您的用户名：" + user.getUsername() + ",您的密码：" + user.getPassword() + "，请妥善保管，如有问题请联系网站客服!!";
73         message.setContent(info, "text/html;charset=UTF-8");
74         message.saveChanges();
75         return message;
76     }
77 }
```

### 2.3、编写处理用户注册的Servlet

```
1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.ServletException;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.servlet.http.HttpServletRequest;
 7 import javax.servlet.http.HttpServletResponse;
 8 import me.gacl.domain.User;
 9 import me.gacl.service.UserService;
10 
11 public class RegisterServlet extends HttpServlet {
12 
13     public void doGet(HttpServletRequest request, HttpServletResponse response)
14             throws ServletException, IOException {
15         try{
16             String username = request.getParameter("username");
17             String password = request.getParameter("password");
18             String email = request.getParameter("email");
19             User user = new User();
20             user.setEmail(email);
21             user.setPassword(password);
22             user.setUsername(username);
23             
24             System.out.println("把用户信息注册到数据库中");
25             //用户注册成功之后就使用用户注册时的邮箱给用户发送一封Email
26             //发送邮件是一件非常耗时的事情，因此这里开辟了另一个线程来专门发送邮件
27             Sendmail send = new Sendmail(user);
28             //启动线程，线程启动之后就会执行run方法来发送邮件
29             send.start();
30             
31             //注册用户
32             //new UserService().registerUser(user);
33             request.setAttribute("message", "恭喜您，注册成功，我们已经发了一封带了注册信息的电子邮件，请查收，如果没有收到，可能是网络原因，过一会儿就收到了！！");
34             request.getRequestDispatcher("/message.jsp").forward(request, response);
35         }catch (Exception e) {
36             e.printStackTrace();
37             request.setAttribute("message", "注册失败！！");
38             request.getRequestDispatcher("/message.jsp").forward(request, response);
39         }
40     }
41 
42     public void doPost(HttpServletRequest request, HttpServletResponse response)
43             throws ServletException, IOException {
44         doGet(request, response);
45     }
46 }
```

　　程序运行效果如下：

![](https://images0.cnblogs.com/blog/289233/201501/122341169485604.gif)

　　现在很多网站都有这样的功能，用户注册完成之后，网站根据我们注册时填写的邮箱给我们一封Email，然后点击Email中的超链接去激活我们的用户。这种功能就是这样实现的。

　　在总结使用JavaMail发送邮件时发现，将邮件发送到sina或者sohu的邮箱时，不一定能够马上收取得到邮件，总是有延迟，有时甚至会延迟很长的时间，甚至会被当成垃圾邮件来处理掉，或者干脆就拒绝接收，有时候为了看到邮件发送成功的效果，要等半天，实属无奈啊。









