# JavaWeb学习总结(五十二)——使用JavaMail创建邮件和发送邮件 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(五十二)——使用JavaMail创建邮件和发送邮件](https://www.cnblogs.com/xdp-gacl/p/4216311.html)



## 一、RFC882文档简单说明

　　RFC882文档规定了如何编写一封简单的邮件(纯文本邮件)，一封简单的邮件包含邮件头和邮件体两个部分，邮件头和邮件体之间使用**空行**分隔。

　　邮件头包含的内容有：
- **from**字段 　　--用于指明发件人
- **to**字段 　　    --用于指明收件人
- **subject**字段  --用于说明邮件主题
- **cc**字段 　　   -- 抄送，将邮件发送给收件人的同时抄送给另一个收件人，收件人可以看到邮件抄送给了谁
- **bcc**字段 　　 -- 密送，将邮件发送给收件人的同时将邮件秘密发送给另一个收件人，收件人无法看到邮件密送给了谁

　　邮件体指的就是邮件的具体内容。

## 二、MIME协议简单介绍

　　在我们的实际开发当中，一封邮件既可能包含图片，又可能包含有附件，在这样的情况下，RFC882文档规定的邮件格式就无法满足要求了。

**MIME协议是对RFC822文档的升级和补充**，它描述了如何生产一封复杂的邮件。通常我们把**MIME协议描述的邮件称之为MIME邮件**。**MIME协议描述的数据称之为MIME消息。**
　　对于一封复杂邮件，如果包含了多个不同的数据，MIME协议规定了要使用分隔线对多段数据进行分隔，并使用Content-Type头字段对数据的类型、以及多个数据之间的关系进行描述。

## 三、使用JavaMail创建邮件和发送邮件

　　JavaMail创建的邮件是基于MIME协议的。因此可以使用JavaMail创建出包含图片，包含附件的复杂邮件。

### 3.1、JavaMail API的简单介绍

![](https://images0.cnblogs.com/blog/289233/201501/122128504487771.png)

![](https://images0.cnblogs.com/blog/289233/201501/122130317295595.png)

![](https://images0.cnblogs.com/blog/289233/201501/122131389484760.png)

### 3.2、创建邮件发送测试项目

![](https://images0.cnblogs.com/blog/289233/201501/122200071519160.png)

### 3.3、发送一封只包含文本的简单邮件

```
1 package me.gacl.main;
 2 
 3 import java.util.Properties;
 4 import javax.mail.Message;
 5 import javax.mail.Session;
 6 import javax.mail.Transport;
 7 import javax.mail.internet.InternetAddress;
 8 import javax.mail.internet.MimeMessage;
 9 
10 /**
11 * @ClassName: Sendmail
12 * @Description: 发送Email
13 * @author: 孤傲苍狼
14 * @date: 2015-1-12 下午9:42:56
15 *
16 */ 
17 public class Sendmail {
18 
19     /**
20      * @param args
21      * @throws Exception 
22      */
23     public static void main(String[] args) throws Exception {
24         
25         Properties prop = new Properties();
26         prop.setProperty("mail.host", "smtp.sohu.com");
27         prop.setProperty("mail.transport.protocol", "smtp");
28         prop.setProperty("mail.smtp.auth", "true");
29         //使用JavaMail发送邮件的5个步骤
30         //1、创建session
31         Session session = Session.getInstance(prop);
32         //开启Session的debug模式，这样就可以查看到程序发送Email的运行状态
33         session.setDebug(true);
34         //2、通过session得到transport对象
35         Transport ts = session.getTransport();
36         //3、使用邮箱的用户名和密码连上邮件服务器，发送邮件时，发件人需要提交邮箱的用户名和密码给smtp服务器，用户名和密码都通过验证之后才能够正常发送邮件给收件人。
37         ts.connect("smtp.sohu.com", "gacl", "邮箱密码");
38         //4、创建邮件
39         Message message = createSimpleMail(session);
40         //5、发送邮件
41         ts.sendMessage(message, message.getAllRecipients());
42         ts.close();
43     }
44     
45     /**
46     * @Method: createSimpleMail
47     * @Description: 创建一封只包含文本的邮件
48     * @Anthor:孤傲苍狼
49     *
50     * @param session
51     * @return
52     * @throws Exception
53     */ 
54     public static MimeMessage createSimpleMail(Session session)
55             throws Exception {
56         //创建邮件对象
57         MimeMessage message = new MimeMessage(session);
58         //指明邮件的发件人
59         message.setFrom(new InternetAddress("gacl@sohu.com"));
60         //指明邮件的收件人，现在发件人和收件人是一样的，那就是自己给自己发
61         message.setRecipient(Message.RecipientType.TO, new InternetAddress("gacl@sohu.com"));
62         //邮件的标题
63         message.setSubject("只包含文本的简单邮件");
64         //邮件的文本内容
65         message.setContent("你好啊！", "text/html;charset=UTF-8");
66         //返回创建好的邮件对象
67         return message;
68     }
69 }
```

### 3.4、发送包含内嵌图片的邮件

```
1 package me.gacl.main;
 2 
 3 import java.io.FileOutputStream;
 4 import java.util.Properties;
 5 
 6 import javax.activation.DataHandler;
 7 import javax.activation.FileDataSource;
 8 import javax.mail.Message;
 9 import javax.mail.Session;
10 import javax.mail.Transport;
11 import javax.mail.internet.InternetAddress;
12 import javax.mail.internet.MimeBodyPart;
13 import javax.mail.internet.MimeMessage;
14 import javax.mail.internet.MimeMultipart;
15 
16 /**
17 * @ClassName: Sendmail
18 * @Description: 发送Email
19 * @author: 孤傲苍狼
20 * @date: 2015-1-12 下午9:42:56
21 *
22 */ 
23 public class Sendmail {
24 
25     /**
26      * @param args
27      * @throws Exception 
28      */
29     public static void main(String[] args) throws Exception {
30         
31         Properties prop = new Properties();
32         prop.setProperty("mail.host", "smtp.sohu.com");
33         prop.setProperty("mail.transport.protocol", "smtp");
34         prop.setProperty("mail.smtp.auth", "true");
35         //使用JavaMail发送邮件的5个步骤
36         //1、创建session
37         Session session = Session.getInstance(prop);
38         //开启Session的debug模式，这样就可以查看到程序发送Email的运行状态
39         session.setDebug(true);
40         //2、通过session得到transport对象
41         Transport ts = session.getTransport();
42         //3、连上邮件服务器，需要发件人提供邮箱的用户名和密码进行验证
43         ts.connect("smtp.sohu.com", "gacl", "邮箱密码");
44         //4、创建邮件
45         Message message = createImageMail(session);
46         //5、发送邮件
47         ts.sendMessage(message, message.getAllRecipients());
48         ts.close();
49     }
50     
51     /**
52     * @Method: createImageMail
53     * @Description: 生成一封邮件正文带图片的邮件
54     * @Anthor:孤傲苍狼
55     *
56     * @param session
57     * @return
58     * @throws Exception
59     */ 
60     public static MimeMessage createImageMail(Session session) throws Exception {
61         //创建邮件
62         MimeMessage message = new MimeMessage(session);
63         // 设置邮件的基本信息
64         //发件人
65         message.setFrom(new InternetAddress("gacl@sohu.com"));
66         //收件人
67         message.setRecipient(Message.RecipientType.TO, new InternetAddress("xdp_gacl@sina.cn"));
68         //邮件标题
69         message.setSubject("带图片的邮件");
70 
71         // 准备邮件数据
72         // 准备邮件正文数据
73         MimeBodyPart text = new MimeBodyPart();
74         text.setContent("这是一封邮件正文带图片<img src='cid:xxx.jpg'>的邮件", "text/html;charset=UTF-8");
75         // 准备图片数据
76         MimeBodyPart image = new MimeBodyPart();
77         DataHandler dh = new DataHandler(new FileDataSource("src\\1.jpg"));
78         image.setDataHandler(dh);
79         image.setContentID("xxx.jpg");
80         // 描述数据关系
81         MimeMultipart mm = new MimeMultipart();
82         mm.addBodyPart(text);
83         mm.addBodyPart(image);
84         mm.setSubType("related");
85 
86         message.setContent(mm);
87         message.saveChanges();
88         //将创建好的邮件写入到E盘以文件的形式进行保存
89         message.writeTo(new FileOutputStream("E:\\ImageMail.eml"));
90         //返回创建好的邮件
91         return message;
92     }
93 }
```

### 3.5、发送包含附件的邮件

```
1 package me.gacl.main;
 2 
 3 import java.io.FileOutputStream;
 4 import java.util.Properties;
 5 
 6 import javax.activation.DataHandler;
 7 import javax.activation.FileDataSource;
 8 import javax.mail.Message;
 9 import javax.mail.Session;
10 import javax.mail.Transport;
11 import javax.mail.internet.InternetAddress;
12 import javax.mail.internet.MimeBodyPart;
13 import javax.mail.internet.MimeMessage;
14 import javax.mail.internet.MimeMultipart;
15 
16 /**
17 * @ClassName: Sendmail
18 * @Description: 发送Email
19 * @author: 孤傲苍狼
20 * @date: 2015-1-12 下午9:42:56
21 *
22 */ 
23 public class Sendmail {
24 
25     /**
26      * @param args
27      * @throws Exception 
28      */
29     public static void main(String[] args) throws Exception {
30         
31         Properties prop = new Properties();
32         prop.setProperty("mail.host", "smtp.sohu.com");
33         prop.setProperty("mail.transport.protocol", "smtp");
34         prop.setProperty("mail.smtp.auth", "true");
35         //使用JavaMail发送邮件的5个步骤
36         //1、创建session
37         Session session = Session.getInstance(prop);
38         //开启Session的debug模式，这样就可以查看到程序发送Email的运行状态
39         session.setDebug(true);
40         //2、通过session得到transport对象
41         Transport ts = session.getTransport();
42         //3、连上邮件服务器
43         ts.connect("smtp.sohu.com", "gacl", "邮箱密码");
44         //4、创建邮件
45         Message message = createAttachMail(session);
46         //5、发送邮件
47         ts.sendMessage(message, message.getAllRecipients());
48         ts.close();
49     }
50     
51     /**
52     * @Method: createAttachMail
53     * @Description: 创建一封带附件的邮件
54     * @Anthor:孤傲苍狼
55     *
56     * @param session
57     * @return
58     * @throws Exception
59     */ 
60     public static MimeMessage createAttachMail(Session session) throws Exception{
61         MimeMessage message = new MimeMessage(session);
62         
63         //设置邮件的基本信息
64         //发件人
65         message.setFrom(new InternetAddress("gacl@sohu.com"));
66         //收件人
67         message.setRecipient(Message.RecipientType.TO, new InternetAddress("xdp_gacl@sina.cn"));
68         //邮件标题
69         message.setSubject("JavaMail邮件发送测试");
70         
71         //创建邮件正文，为了避免邮件正文中文乱码问题，需要使用charset=UTF-8指明字符编码
72         MimeBodyPart text = new MimeBodyPart();
73         text.setContent("使用JavaMail创建的带附件的邮件", "text/html;charset=UTF-8");
74         
75         //创建邮件附件
76         MimeBodyPart attach = new MimeBodyPart();
77         DataHandler dh = new DataHandler(new FileDataSource("src\\2.jpg"));
78         attach.setDataHandler(dh);
79         attach.setFileName(dh.getName());  //
80         
81         //创建容器描述数据关系
82         MimeMultipart mp = new MimeMultipart();
83         mp.addBodyPart(text);
84         mp.addBodyPart(attach);
85         mp.setSubType("mixed");
86         
87         message.setContent(mp);
88         message.saveChanges();
89         //将创建的Email写入到E盘存储
90         message.writeTo(new FileOutputStream("E:\\attachMail.eml"));
91         //返回生成的邮件
92         return message;
93     }
94 }
```

### 3.6、发送包含内嵌图片和附件的复杂邮件

```
1 package me.gacl.main;
  2 
  3 import java.io.FileOutputStream;
  4 import java.util.Properties;
  5 import javax.activation.DataHandler;
  6 import javax.activation.FileDataSource;
  7 import javax.mail.Message;
  8 import javax.mail.Session;
  9 import javax.mail.Transport;
 10 import javax.mail.internet.InternetAddress;
 11 import javax.mail.internet.MimeBodyPart;
 12 import javax.mail.internet.MimeMessage;
 13 import javax.mail.internet.MimeMultipart;
 14 import javax.mail.internet.MimeUtility;
 15 
 16 /**
 17 * @ClassName: Sendmail
 18 * @Description: 发送Email
 19 * @author: 孤傲苍狼
 20 * @date: 2015-1-12 下午9:42:56
 21 *
 22 */ 
 23 public class Sendmail {
 24 
 25     /**
 26      * @param args
 27      * @throws Exception 
 28      */
 29     public static void main(String[] args) throws Exception {
 30         
 31         Properties prop = new Properties();
 32         prop.setProperty("mail.host", "smtp.sohu.com");
 33         prop.setProperty("mail.transport.protocol", "smtp");
 34         prop.setProperty("mail.smtp.auth", "true");
 35         //使用JavaMail发送邮件的5个步骤
 36         //1、创建session
 37         Session session = Session.getInstance(prop);
 38         //开启Session的debug模式，这样就可以查看到程序发送Email的运行状态
 39         session.setDebug(true);
 40         //2、通过session得到transport对象
 41         Transport ts = session.getTransport();
 42         //3、连上邮件服务器
 43         ts.connect("smtp.sohu.com", "gacl", "邮箱密码");
 44         //4、创建邮件
 45         Message message = createMixedMail(session);
 46         //5、发送邮件
 47         ts.sendMessage(message, message.getAllRecipients());
 48         ts.close();
 49     }
 50     
 51     /**
 52     * @Method: createMixedMail
 53     * @Description: 生成一封带附件和带图片的邮件
 54     * @Anthor:孤傲苍狼
 55     *
 56     * @param session
 57     * @return
 58     * @throws Exception
 59     */ 
 60     public static MimeMessage createMixedMail(Session session) throws Exception {
 61         //创建邮件
 62         MimeMessage message = new MimeMessage(session);
 63         
 64         //设置邮件的基本信息
 65         message.setFrom(new InternetAddress("gacl@sohu.com"));
 66         message.setRecipient(Message.RecipientType.TO, new InternetAddress("xdp_gacl@sina.cn"));
 67         message.setSubject("带附件和带图片的的邮件");
 68         
 69         //正文
 70         MimeBodyPart text = new MimeBodyPart();
 71         text.setContent("xxx这是女的xxxx<br/><img src='cid:aaa.jpg'>","text/html;charset=UTF-8");
 72         
 73         //图片
 74         MimeBodyPart image = new MimeBodyPart();
 75         image.setDataHandler(new DataHandler(new FileDataSource("src\\3.jpg")));
 76         image.setContentID("aaa.jpg");
 77         
 78         //附件1
 79         MimeBodyPart attach = new MimeBodyPart();
 80         DataHandler dh = new DataHandler(new FileDataSource("src\\4.zip"));
 81         attach.setDataHandler(dh);
 82         attach.setFileName(dh.getName());
 83         
 84         //附件2
 85         MimeBodyPart attach2 = new MimeBodyPart();
 86         DataHandler dh2 = new DataHandler(new FileDataSource("src\\波子.zip"));
 87         attach2.setDataHandler(dh2);
 88         attach2.setFileName(MimeUtility.encodeText(dh2.getName()));
 89         
 90         //描述关系:正文和图片
 91         MimeMultipart mp1 = new MimeMultipart();
 92         mp1.addBodyPart(text);
 93         mp1.addBodyPart(image);
 94         mp1.setSubType("related");
 95         
 96         //描述关系:正文和附件
 97         MimeMultipart mp2 = new MimeMultipart();
 98         mp2.addBodyPart(attach);
 99         mp2.addBodyPart(attach2);
100         
101         //代表正文的bodypart
102         MimeBodyPart content = new MimeBodyPart();
103         content.setContent(mp1);
104         mp2.addBodyPart(content);
105         mp2.setSubType("mixed");
106         
107         message.setContent(mp2);
108         message.saveChanges();
109         
110         message.writeTo(new FileOutputStream("E:\\MixedMail.eml"));
111         //返回创建好的的邮件
112         return message;
113     }
114 }
```

　　以上就是使用JavaMail的API创建邮件和发送邮件的全部内容。









